#include "stdafx.h"
#include <array>
#include <emmintrin.h>	// (th4) SSE2 in TownWater_Render steps 2-3
#include <intrin.h>		// (th4) _BitScanForward, same place
#include <unordered_map>	// (th4) SNOW GROUND: identical sets of votes stored once (SnowDecShared)
#include <thread>			// (th4) TREE SCAN THREAD
#include <atomic>
#include <mutex>
#include <condition_variable>

// ============================================================================================
// (new by th4) TOWN RIVER WATER -- ripples and hero reflections on the river in Tristram.
// ============================================================================================
//
// WHICH SYSTEM THIS IS. TH4 has two "water" effects:
//   - WaterShimmer on cave water maps (dx_utility.cpp HeatHaze_BuildMask / PostFx.hlsl). There
//     the water is palette entries 1..31, which nothing else on those maps uses, so the mask is
//     a pure colour test.
//   - THIS file, for the town river. Town has no water-only palette range (AnimateTownPalette,
//     palette.cpp, blends 1..127 for day/night and nothing else), and the river's blues are
//     shared with rocks and grass speckle. So the water is found in two steps:
//       1. WHERE: the floor diamonds of map cells whose piece (FineMap value) belongs to a river
//          tile of Town.TIL (kRiverTiles below);
//       2. WHICH PIXELS: inside those diamonds, the palette entries in kWaterEntries -- where the
//          frame still shows the entry the cell's OWN FLOOR ART has there (RIVER ART: a rock, a tree
//          or a sprite drawn over the diamond later is never water, whatever its colour). They are
//          entry NUMBERS from the tile art, so they are the same under every town palette
//          (normal, Classic's orig_pals\town.pal, the random multiplayer L0_N.PAL, Ice Age's
//          iceage\0town_P.pal -- palette.cpp SelectRandomLevelPalette) and on every screen.
//          Why a fixed list: an RGB rule fails (every blue-ish town entry is shared with grass,
//          rocks and shadows, all under 25% inside the river), and learning the entries at run
//          time from what is on screen fails too -- the water entries sit only 54..68% inside
//          river cells, so a session that learned while little river was on screen accepted
//          nothing and had no water at all. The list is what that learning found in a session
//          that saw a lot of river (TH4_TOWNWATER_DEBUG's share log reproduces it).
//     The shader side is shared: this file only fills a mask buffer that HeatHaze_BuildMask
//     turns into the 170 = "water" code PostFx.hlsl already ripples. PostFx.hlsl is unchanged.
//   - PUDDLES (WET GROUND, further down) also live here: rain puddles on the town's walkable ground
//     during a storm night and the day after it. They have their OWN mask (PSurf), never Surf, so
//     they never get the river's flowing ripple; the river reflection code (DrawReflection) is shared.
//   - Not water, but recorded the same way and so kept here: TREE SWAY, SUN GROUND and GRASS (further
//     down) -- which pixels are crowns, sunlit ground, and grass that grows blades (Grass.hlsl).
//   - RUNNING RIVER (further down, BuildRiverFlow): the town river FLOWS -- its texture slides downstream and
//     its ripples travel with it (dx_utility.cpp RIVER FLOW + River.hlsl). This file gives that pass the
//     direction of the stream at every map cell and how far downstream it lies (the FLOW MAP), and the hero
//     reflections' pixels, which are kept out of the slide (they would be dragged downstream). It also adds the
//     river water of tiles outside kRiverTiles -- the Hellfire abyss gate (WATER THAT JOINS THE RIVER).
//     Ravenholm's water does NOT flow: it is stagnant and keeps the old ripple (RAVENHOLM'S RIVER).
//
// PER FRAME, in order (every call site carries a one-line pointer back here):
//   Draw.cpp  DrawIsoLand start ........ TownWater_BeginFrame()   forget last frame's notes
//   TownMap.cpp DrawSceneTownTop ....... TownWater_NoteCell()     river cells drawn this frame
//   TownMap.cpp DrawDynamicObjectsTown . TownWater_NotePlayer()   hero sprites drawn this frame
//   Draw.cpp  after DrawIsoLand ........ TownWater_Render()       mask + reflections (post-zoom)
//   Draw.cpp  Perspective_WarpWorld .... warps TownWater_Surface() with the other world layers
//                                        (and TownWater_MirrorSurface(), the Ice Age mirror's classes)
//   dx_utility.cpp HeatHaze_BuildMask .. TownWater_MaskRows()     water texels for the ripple
//   dx_utility.cpp IceRiver_BuildMask .. TownWater_MirrorRows()   ICE RIVER: the Ice Age mirror's water (made solid
//                                        here, see ICE RIVER above MirrorFill)
//
// THE MASK BUFFER (Surf) is a screen-parallel layer: same layout as WorkingSurface (pitch
// WorkingWidth, screen area at Screen_TopBorder / Screen_LeftBorder), post-zoom. A byte holds the
// palette index this file saw (or wrote) at that pixel, 0 = not water. HeatHaze_BuildMask counts a
// pixel as water only when the world snapshot still holds that SAME index there -- so anything
// drawn over the water later (HP bars, the Alt loot rings, a sprite) drops out on its own, the same
// trick the snapshot/final-frame compare uses for the UI.
//
// WATER COLOUR: water pixels are redrawn darker (WATER_BRIGHTNESS) in the 8-bit frame, so every
// lighting mode and the ripple see it. NOT through LightTable: its rows assume the dungeon palette
// layout of 16-shade ramps (Lights.cpp MakeLightTable steps index+1 inside each block of 16), and
// the town terrain entries 1..127 are not ramps -- entry+N is an unrelated colour, which painted red
// and purple specks along the shore (115 -> 118 is (15,6,14) in the normal town palette, redder in
// others). TerrainDark[] instead maps each entry to the NEAREST terrain entry of the darker colour,
// built from the palette the town actually loaded (FilePalette), so it is right under every one.
//
// REFLECTIONS are drawn into the 8-bit frame itself, only onto mask pixels, AFTER the whole scene
// (a reflection reaches into cells drawn later, whose floor would paint over it otherwise). A hero
// frame is decoded from its CL2 (the format Rle2DrawPal, Engine.cpp ~1718, reads), flipped about its
// lowest opaque row (the feet), darkened through a LightTable row, and thinned out with a 4x4
// ordered dither toward the head -- 8-bit has no alpha, so the dither is the transparency, and the
// ripple then smooths it. Reflected pixels are written into Surf too, so they ripple with the water.
// Heroes only: towners and cows are CEL, not CL2 (DrawDynamicObjectsTown draws them with CEL_Draw);
// adding them means a CEL decoder next to DecodeCl2Frame and a TownWater_NoteTowner.
//
// COUPLINGS the compiler will not catch:
//   - A new screen-parallel world layer must be warped in Draw.cpp Perspective_WarpWorld; Surf is.
//   - The zoom: TownWater_Render runs AFTER DrawIsoLand's 2x stretch, so everything recorded in
//     pre-zoom coordinates (cells, sprites) is multiplied by the zoom factor here. The factor must
//     match the one DrawIsoLand used (Draw.cpp, the call passes !IsZoomDisable).
//   - The floor diamond geometry is DrawTileFill's (TownMap.cpp ~126): 31 rows, offsetY-30..offsetY.
//   - kRiverTiles is Town.TIL data. A different Town.TIL (a town mod) needs this table redone.
//   - thehell3.vcxproj AND thehell3.vcxproj.filters both list this file.
//
// KNOWN-GOOD CHECK: set TH4_TOWNWATER_DEBUG to 1. Every pixel the mask counts as water is painted
// white, and TownWater.log (next to the exe) shows the Town.TIL calibration and, once enough river
// has been on screen, how much of each common entry lies inside river cells. The river must be
// mostly white inside its banks; grass, the bank and rocks must not be. A whole tile never white =
// a river tile missing from kRiverTiles (find its number in the TileEditor). Too little white in
// the listed tiles = add entries from the log (high share, many river pixels) to kWaterEntries.
// ============================================================================================

#define TH4_TOWNWATER_DEBUG 0

// (th4) GRASS (further down): the PUDDLES read the grass art and the cell shares too (NOT ON GRASS)
static int GrassArtIndex( ushort id );
static bool GrassDecodePiece( ushort id, uchar* pix, uchar* opq );	// (th4) the RUNNING RIVER's flow map decodes the water too
static inline const uchar* GrassArtPtr( int index );
static int GrassShareQ8( float fr, float fc );

namespace {

// Town.TIL tiles that contain river water, AS THE TILEEDITOR SHOWS THEM: the tile number
// ("Current") and the number in the top box of "Squares". Every one of these is a shore tile
// (bank + water); kWaterEntries separates the two. Tile 84 is the jetty planks over water;
// 76, 79, 81, 93 and 96 are the bend pieces (96 is the tree standing at a bend). A tile missing here
// shows as a tile-shaped gap in the water effects at that spot.
// Whether the editor counts tiles and squares from 0 or 1 is not assumed: TownWater_OnTownLoaded
// reads Town.TIL and finds the one offset under which all rows match (see Calibrate()).
struct RiverTile { int tile, top; };
const RiverTile kRiverTiles[] = {
	{ 72, 262 }, { 73, 266 }, { 74, 270 }, { 75, 274 }, { 76, 278 }, { 77, 282 }, { 78, 286 },
	{ 79, 290 }, { 80, 294 }, { 81, 298 }, { 82, 302 }, { 83, 306 }, { 84, 310 }, { 89, 330 },
	{ 90, 334 }, { 91, 338 }, { 93, 346 }, { 94, 350 }, { 95, 354 }, { 96, 358 }, { 97, 362 },
};

// Palette entry numbers of the river water (see the header: fixed on purpose). Measured shares
// inside river cells in a normal-town session: 52 = 54%, 99 = 60%, 110 = 62%, 115 = 68%, 119 = 64%.
// Near misses from the same log, if more water is wanted: 59, 62, 75, 81 (45..48%).
const uchar kWaterEntries[] = { 52, 99, 110, 115, 119 };

// ---- tunables --------------------------------------------------------------------------------
// Share log (TH4_TOWNWATER_DEBUG only): collected once this many river-cell pixels have been seen
// over at least this many frames.
const int LEARN_MIN_PIXELS      = 60000;
const int LEARN_MIN_FRAMES      = 30;
// Water look: every water pixel is drawn this much darker (LightTable darkness, 0..15 scale, 0 = off).
const int WATER_BRIGHTNESS      = 70;		// water pixels are redrawn at this % of their colour (100 = off)
// Reflection look.
const int REFLECT_DARK          = 4;		// LightTable darkness, on the 0..15 scale (0 = as bright as the hero)
const int REFLECT_COVER_FEET    = 14;		// dither coverage at the feet, of 16 (16 = solid)
const int REFLECT_COVER_HEAD    = 7;		// dither coverage at the top of the head, of 16

const int MAX_PIECE = 4096;					// FineMap piece numbers stay far below this in town
bool RiverPiece[ MAX_PIECE ];
bool AnyRiverPiece = false;

// (th4) TREE TILES for the glowing leaves of GLOW nights: every Town.TIL tile with leaves -- trees,
// the leaves over house walls (205, 206, 210, 214, 323), the ones near the window of 109, and the
// fallen leaves on the ground (202, 203, 204). 202 / 203 also carry the red light of the church
// windows on the grass (the cross shape), partly in the same palette entries as the leaves -- see
// WINDOW LIGHT below for how those pixels are kept out. Tile numbers as in the TileEditor; their squares
// are read from Town.TIL with the SAME calibration as kRiverTiles (TownWater_OnTownLoaded), so no
// square numbers are needed here. TreePiece marks their pieces.
const int kTreeTiles[] = { 31, 34, 41, 42, 44, 47, 55, 56, 96, 97, 109, 110, 165, 166, 167,
                           202, 203, 204, 205, 206, 210, 214, 323 };
bool TreePiece[ MAX_PIECE ];
// (th4) ROCK SNOW (Ice Age, RockSnow_End below): the Town.TIL tiles of free-standing ROCKS -- the boulder field along the
// town's west edge and the rocks scattered through town (57..63), the big stacked pile by the river (328..332). Found from
// the art (their pixels are mostly kSnowRockEntries with almost no wall / roof entries) and checked on a render of the
// whole town, the candidates tinted (scratchpad rocks\town_tint.py); same numbering and calibration as kTreeTiles. NOT
// here, on purpose: fences, the well, graves, the abyss pit, the cauldron gallows (Mordor: "only about rocks").
const int kRockTiles[] = { 57, 58, 59, 60, 61, 62, 63, 328, 329, 330, 331, 332 };
// FLOOR-ONLY rock tiles: the small two-stone rock (318, eight of them, e.g. by the bridge; 319 is another cut of it) sits
// wholly in the floor layer, and the thin dark post beside it rises into the layer above -- only the floor layer is snowed,
// so the post stays as it is. Missed at first (the search wanted upper-layer rock) and found from Mordor's screenshot of a
// bare rock by the bridge (2026-10-10): a rock missing from both lists simply never gets snow.
const int kFloorRockTiles[] = { 318, 319 };
// ADRIA'S HUT gets the rocks' scattered snow on its ROOF and its LEFT WALL, the one the tree grows by (Mordor, 2026-10-10:
// "add snow to its left side (where tree grows) and roof, the way snow is added on rocks"): its tiles with upper layers,
// plus the floor squares of ADRIA'S SIDE (kHutSideSquares below: the wall's foot is painted into them). Which of their
// pixels: HutSnow_Build (ROCK SNOW, ADRIA'S HUT) -- never the tree in front, never the face with the shelves and door.
const int kHutSnowTiles[] = { 165, 166, 167, 168 };
// THATCH ROOFS get the same scattered snow (Mordor, 2026-10-10: "snow on thatch roofs", all houses of the town): the
// Town.TIL tiles of the seven thatch houses -- Ogden's tavern 101..108 + 111..117, the long house 118..128, Pepin's
// 129..145, the house where Farnham sits and its twin by the church 222..230 + 315, Griswold's 231..248. Found from
// an offline render of the whole town (sight-blocking groups of cells, scratchpad roofs\comps.py). NOT here: 109 / 110
// (the tavern's walls with leaves: tree tiles), Adria's hut (kHutSnowTiles), the church and the catacombs' building
// (slate and stone, not thatch). The cells are found by WHOLE TILES (RoofSnow_BuildAll), never by piece: pieces are
// shared with other tiles (see HOUSE EDGES). Which pixels of them: RoofSnow_Classify (ROCK SNOW, THATCH ROOFS).
const int kRoofTiles[] = { 101, 102, 103, 104, 105, 106, 107, 108, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
                           121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
                           139, 140, 141, 142, 143, 144, 145, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
                           233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 315 };
const int ROOF_TILE_N = (int)( sizeof( kRoofTiles ) / sizeof( kRoofTiles[ 0 ] ) );
int RoofSq[ ROOF_TILE_N ][ 4 ];	// each tile's pieces: up, left, right, down (FineMap values; 0 = tile missing)
// NO-SNOW BOXES: windows under the eaves the rules let through (their frames and sills are thatch-coloured wood, so
// they score as roof): Pepin's left window, the two gable windows of Griswold's house. { anchor tile, x0, y0, x1, y1 }
// in px from the anchor tile's top cell (its DrawTileFill dst, as ADRIA'S SIDE), x1 / y1 exclusive; every whole
// placement of the anchor tile gets them. Measured on the town render; checked to change no pixel outside them.
struct RoofNoSnowBox { int tile, x0, y0, x1, y1; };
const RoofNoSnowBox kRoofNoSnow[] = { { 145, -112, -70, -78, -35 }, { 241, 341, 20, 364, 59 }, { 241, 364, 3, 396, 59 } };
const int ROOF_NOSNOW_N = (int)( sizeof( kRoofNoSnow ) / sizeof( kRoofNoSnow[ 0 ] ) );
int RoofNoSnowSq[ ROOF_NOSNOW_N ][ 4 ];
// THE CATACOMBS BUILDING (the stone mausoleum over the catacombs' stairs; Ravenholm has the same one) gets the rocks'
// snow on its ROOF (Mordor, 2026-10-10: "catacomb entry covered in snow", "in Ravenholm it exists as well, there too"):
// its slate roof is no thatch (kRoofTiles' colour score does not fit it), so its roof is a measured OUTLINE instead,
// like ADRIA'S SIDE. The building = tile 322 (the ANCHOR, never swapped), 321 two rows up (320 while the catacombs are
// closed: TownMap.cpp LoadSPTownMap; the roof is the same in both) and 323 two columns left (the bush in front of its
// right wall). kCataRoof = the roof's outline -- the lit slate slope and the dark slope behind the ridge, down to their
// eaves; not the frieze slab over the door, not the walls -- in px from tile 322's top cell (its DrawTileFill dst: x 0 = left
// corner, y 0 = its bottom row), traced on the town render (scratchpad cata\p3.png). Which pixels: CataSnow_BuildAll.
struct CataPart { int tile, dr, dc; };
const CataPart kCataParts[] = { { 322, 0, 0 }, { 321, -2, 0 }, { 320, -2, 0 }, { 323, 0, -2 } };	// the anchor first
const int CATA_PART_N = (int)( sizeof( kCataParts ) / sizeof( kCataParts[ 0 ] ) );
int CataSq[ CATA_PART_N ][ 4 ];
const double kCataRoof[][ 2 ] = { { -87, -122.6 }, { 18, -175.4 }, { 55, -173 }, { 106, -127.4 }, { 0, -75.4 }, { -42, -128.6 } };
const int CATA_ROOF_N = (int)( sizeof( kCataRoof ) / sizeof( kCataRoof[ 0 ] ) );
// THE ROCKS' COVERAGE (Mordor: "I would prefer the rock coverage for it"): with the rocks' ROCK_THRESHOLD the bright slate
// came out 65 % white (mean brightness 0.85 against the rocks' 0.38, and brighter pixels get more snow); this threshold
// gives it the rocks' share -- 24 % of its eligible pixels against the rocks' 23 % (measured over all of Tristram's rock
// pixels, scratchpad cata\share.py) -- with the rocks' noise and brightness push unchanged, so the lit slope still gets
// more than the shaded one. FINER SPOTS (Mordor: "big spots of snow, can you make more of them but smaller?"): its own
// noise scales, CATA_SCALE1 / 2 (px), about two thirds of the rocks' 3 / 7 -- more, smaller patches at the same share
// (the threshold for 23 % hardly moves with the scale: 0.805 at 3 / 7, 0.797 at 2 / 4.5). 1.5 / 3.5 was tried offline:
// single-pixel speckle, the "pixel noise" the rocks' scales were picked to avoid.
const float CATA_THRESHOLD = 0.80f, CATA_SCALE1 = 2.0f, CATA_SCALE2 = 4.5f;
uchar RockPiece[ MAX_PIECE ];	// 0 not a rock, 1 the whole column, 2 the floor layer only (kFloorRockTiles), 3 Adria's hut (kHutSnowTiles)
bool TreePieceReady = false;	// (th4) set by TownWater_OnTownLoaded; Ravenholm fills it itself if needed (TownWater_CellBegin)
// (th4) HOUSE tiles among the tree tiles: every one of them shows a house wall or roof shaded with
// the same dark entries as the leaves (from the TileEditor screens: 109 wall with window, 110 wall
// with a tree, 165 / 166 / 167 Adria's hut, 205 / 206 / 210 / 214 / 323 walls and roofs with leaves).
// ONLY pixels from these tiles get the two CLUSTER checks of the LEAF NEIGHBOURHOOD RULE below (the
// pure tree tiles only get its first check -- the cluster checks would take thin crowns' glow away).
const int kHutLeafTiles[] = { 109, 110, 165, 166, 167, 205, 206, 210, 214, 323 };
bool HutLeafPiece[ MAX_PIECE ];
// (th4) HALO BOOST: the tree by Wirt (its leaves are on tiles 110 -- a house wall with the tree in
// front -- and 109, the wall with the window), and the bush at the catacombs entrance (323) keep
// fewer leaves after the house checks above, and the thin-leaved trees 44 and 47 have few leaves to
// begin with, so they got much less of the soft halo than a full tree. Their leaves add HALO_BOOST_X
// times the halo; which pixels glow does not change.
const int kHaloBoostTiles[] = { 44, 47, 109, 110, 323 };
bool HaloBoostPiece[ MAX_PIECE ];
const int HALO_BOOST_X = 3;
// (th4) THE WELL (SNOW GROUND): the well by Cain is one Town.TIL tile -- 71, or 342 while the Poisoned Water quest
// runs (TownMap.cpp LoadTownMap, SetTileSquare at 60, 70) -- and its lower wall and base ring are painted INTO the
// floor tiles of its 4 cells, in the same palette entries as the dirt around it, so no colour rule can tell them from
// the ground (the votes snowed its whole lower part white -- the basin's inside too -- and left its top dark).
// Mordor wants it HALF BURIED: WellQuad marks the 4 pieces (1 + which square: 0 up, 1 left, 2 right, 3 down,
// SetTileSquare's order) and SnowCellVotes decides every pixel inside the WELL SHAPE (the wall: x WELL_X0 .. WELL_X1
// above WELL_CY, and the base ring: an ellipse) by hand: the WALL (x WELL_WALL_CX -+ WELL_WALL_RX) stays bare above its
// SNOW LINE -- the wall's footprint ellipse raised by WELL_SNOW_DEPTH, so the snow line curves round the cylinder like
// a real drift -- and the rest (the lower wall, the ring) is snow. TILE coordinates = x, y from the tile's TOP cell's
// DrawTileFill dst (x 0 = its left corner, y 0 = its bottom row). Measured on tile 71's floor art (342 has the same
// wall); the well's front is ~32 px tall (rim y -16 .. ring y +16), so a depth of 16 buries half of it. Same
// calibration as kTreeTiles. A new well art needs the numbers measured again.
const int kWellTiles[] = { 71, 342 };
uchar WellQuad[ MAX_PIECE ];
const float WELL_CX = 35.0f, WELL_CY = 0.0f, WELL_RX = 36.0f, WELL_RY = 16.5f, WELL_X0 = 0.0f, WELL_X1 = 71.0f;
const float WELL_WALL_CX = 32.0f, WELL_WALL_RX = 32.0f, WELL_SNOW_DEPTH = 16.0f;
// (th4) SNOW FLOORS (SNOW GROUND): floor squares of HOUSE cells that are all ground, so they get snow whole -- their
// art is too dull for the house rule's ground evidence (SNOW GROUND RULES, HOUSE CELLS), and loosening that rule
// changed 142 cells, the church's and Adria's among them. { Town.TIL tile (TileEditor number, same calibration as
// kTreeTiles), square 0 up / 1 left / 2 right / 3 down (SetTileSquare's order) }. To add one: find the cell's tile
// and square like THE WELL's (the tile's top cell has even row and col). 239 / 0 = the floor of Griswold's open shed,
// where the stool stands (map cell 64, 62; Mordor: "must be covered with snow").
const int kSnowFloorSquares[][ 2 ] = { { 239, 0 } };
bool SnowAllPiece[ MAX_PIECE ];
// (th4) BARE FLOORS (SNOW GROUND): the opposite -- floor squares that never get snow, whatever the votes. Same format.
// 321 / 3 = the stone porch in front of the catacombs' door (map cell 49, 21; Mordor: "the tile that leads to the
// catacombs should not be covered in snow"); 320 / 3 = the same porch while the catacombs are still closed
// (TownMap.cpp LoadSPTownMap, SetTileSquare 48, 20, 320). Its two neighbours on the porch were tried too: a large
// dark diamond with straight edges.
const int kBareFloorSquares[][ 2 ] = { { 321, 3 }, { 320, 3 } };
bool SnowNonePiece[ MAX_PIECE ];
// (th4) ROCK FLOORS (SNOW GROUND): WALKABLE floor squares whose art is stone -- judged like walk-blocked rock (SnowCellKind
// 3, the rock vote: stone stays bare, ground gets snow; ROCK SNOW then scatters its flecks on the stone), not as open
// ground (all snow). Same format. 329 / 1, 329 / 3 and 330 / 3 = the floor of the CAVES' ENTRANCE in the big rock pile,
// the "leg" of stone under the cave mouth (map cells 17, 70 / 17, 71 / 17, 69; Mordor: "it should follow the common trend
// of the rock here"). Each is used once on the map. NOT done for every walkable stone piece: piece 218 alone (ground with
// pebbles) is in 55 cells whose snow was approved as it is.
const int kRockFloorSquares[][ 2 ] = { { 329, 1 }, { 329, 3 }, { 330, 3 } };
// The same rule for floor pieces that NO Town.TIL tile holds -- TownMap.cpp writes them into FineMap directly, so they
// are listed as FineMap values. 1337 / 1338 = the front of the OPEN CRYPT's slab, where its stairs start: the only two
// walkable cells of it (FillDMap1OpenedCrypt, map cells 36, 24 / 37, 24), so as open ground they were solid white while
// the walk-blocked crypt cells beside them had the fences' patchy look (Mordor: "notice how it's full white"); 1329 = the
// same for the CLOSED crypt, its one walkable cell (FillTownMap2ClosedCrypt, 36, 24). The Lost Treasure quest's Disused
// Reliquary (FillDMapClosed / OpenedDisusedReliquary) is built from the same pieces and gets the same look.
// COUPLING: copied from those four functions -- if their pieces change, change these too.
const int kRockFloorPieces[] = { 1337, 1338, 1329 };
bool SnowRockFloorPiece[ MAX_PIECE ];
// (th4) ADRIA'S SIDE (SNOW GROUND): the strip of ground along the left wall of Adria's hut, where the tree grows, lies
// in the hut's own cells -- tiles 165 and 166 (both kHutLeafTiles) and the top square of 162 -- and their floor art
// holds the BOTTOM OF THE WALL too, in the same dark colours as the grass in the hut's shadow (and the light from the
// cracks), so the house rule left the whole strip bare (Mordor). The wall stands on a straight BASE LINE, rising up-left
// at 1 : 2 from the hut's front corner: SnowCellVotes gives these cells snow below it and keeps them bare above it, left
// of the corner only (right of it, round the door, the votes decide as everywhere). Coordinates from the top cell of
// tile 165 (its DrawTileFill dst: x 0 = left corner, y 0 = bottom row), found on the map at run time (SnowVotesEnsure),
// so only tile numbers are written here. Corner measured on the town render; snowing the cells whole cut the wall short.
// THE EDGE is not the bare line (Mordor: "a strict line ... I liked the snowlines on the fences more, this cow-like
// pattern"): within HUT_EDGE_BAND px of the line the ART decides, like ROCKS IN THE FLOOR -- snow where at least
// HUT_EDGE_VOTE % of the 5 x 5 window's house pixels are not structure (kSnowRockEntries + kSnowWallEntries) -- and the
// line itself wobbles by HUT_EDGE_WOBBLE px of smooth value noise (HutBlotch). A wider band (16) put white blobs on the
// wall and the leaves; pure noise with no art read as a wavy line, not as patches.
const int kHutSideSquares[][ 2 ] = { { 165, -1 }, { 166, -1 }, { 162, 0 } };	// { tile, square; -1 = all four }
const int HUT_SIDE_ANCHOR_TILE = 165;
bool HutSidePiece[ MAX_PIECE ];
int HutAnchorPiece = 0, HutAnchorRow = -1, HutAnchorCol = -1;
const float HUT_CORNER_X = 159.5f, HUT_CORNER_Y = 62.5f;
const float HUT_EDGE_BAND = 10.0f, HUT_EDGE_WOBBLE = 3.0f, HUT_EDGE_CELL = 5.0f;
const int HUT_EDGE_VOTE = 60;
// (th4) HOUSE EDGES (SNOW GROUND): Pepin's house (Mordor: "same thing about Pepin's house") -- its walls' feet ended
// in a strict line where the house rule cut them. On these tiles a house pixel the votes left bare gets the ART vote
// of the 5 x 5 window's open + house pixels (at least HUT_EDGE_VOTE % not structure -> snow) when open ground lies
// within HOUSE_EDGE_BELOW px straight below it (+-2 px): a wall's foot, the patchy fence-like edge. Not for every
// house: tried town-wide, it put snow on the roof of Ogden's inn where it borders the ground behind (the votes do
// not know what the roofs cover). A house that needs it gets its Town.TIL tiles here (same calibration as kTreeTiles).
// The cells are found by WHOLE TILES -- all four squares in place (SnowVotesEnsure, HouseEdgeCell) -- never by piece:
// the pieces are shared with other tiles (Pepin's 538 is also in tiles 195 / 198 at 18..21, 24..27 and 48..49, 62..63),
// and a piece lookup gave another house Pepin's rules and anchored PEPIN'S FRONT at 19, 24 (found by a debug log, 2026-10-09).
// Tiles: Pepin's house 134..142, 145; the gable wall of the house where Farnham sits, 226 / 227 (Mordor: "apply the
// snowy cow pattern to that wall") -- the same two tiles also stand once more, in its twin by the church (26..29, 52..53);
// the church's entry side with the round towers, 187..191, 205, 210 (map 20..31, 24..31; the snow line runs along them).
const int kHouseEdgeTiles[] = { 134, 135, 136, 137, 138, 139, 140, 141, 142, 145, 226, 227, 187, 188, 189, 190, 191, 205, 210 };
const int HOUSE_EDGE_N = (int)( sizeof( kHouseEdgeTiles ) / sizeof( kHouseEdgeTiles[ 0 ] ) );
const int HOUSE_EDGE_BELOW = 10;
int HouseEdgeSq[ HOUSE_EDGE_N ][ 4 ];	// each tile's pieces: up, left, right, down (FineMap values; 0 = tile missing)
bool HouseEdgeCell[ 112 ][ 112 ];
// (th4) PEPIN'S FRONT (SNOW GROUND, on the kHouseEdgeTiles cells): the walk-blocked cells along the front (mark 3)
// held the door's and the wall's bottoms; the rock vote snowed them (wall stone is no rock colour: white blots on the
// door) and a grassy strip in the house cells under the window stayed bare, jutting out like a shelf (Mordor: "the
// snowline at the same level for the whole front"). So on these cells: mark-3 pixels vote like THE EDGE (open, walk-
// blocked and house pixels, at least HUT_EDGE_VOTE % not structure); right of the door the WINDOW WALL'S FOOT is a
// measured line (snow below, bare above, the art votes within PEP_FRONT_BAND, wobbling like ADRIA'S SIDE); the DOOR
// is bare down to its foot (burying its lower half gave striped blue snow on the planks). Coordinates from the top
// cell of tile PEP_ANCHOR_TILE (its DrawTileFill dst), found on the map at run time like HutAnchor.
const int PEP_ANCHOR_TILE = 145;
int PepAnchorRow = -1, PepAnchorCol = -1;
const float PEP_DOOR_X0 = 96.0f, PEP_DOOR_X1 = 150.0f, PEP_DOOR_FOOT = 37.0f;			// the door: x range, its foot
const float PEP_WALL_X1 = 240.0f, PEP_WALL_Y0 = -4.0f, PEP_WALL_SLOPE = -0.5f;			// the window wall's foot from x PEP_DOOR_X1
const float PEP_FRONT_BAND = 4.0f, PEP_FRONT_WOBBLE = 2.0f;
// (th4) SNOW LINES (SNOW GROUND): along the church's entry side with the round towers, and round Ogden's tavern, the
// snow ended in the sawtooth of the cells' diamonds -- the walls there are drawn ON TOP of the floor, so no art vote can
// reach them -- and Mordor asked for "a nice-looking snow line that carefully hugs the church" (then the same for the
// tavern). So these lines are drawn BY HAND: points traced on the town render along the walls' feet (the church: the
// door step, the bases of the two round towers, the wall behind the bush -- there through the tips of the old sawtooth,
// the leaves hide the wall; the tavern: its left wing, the gable, the door, the right wall up to Wirt's tree). Within
// SNOW_LINE_BAND px of a line every recorded floor pixel (open, walk-blocked or house) is snow below it and bare above
// it, with HutBlotch's small wobble. Each line's coordinates are from the top cell of its ANCHOR TILE (its DrawTileFill
// dst), the tile found WHOLE on the map (SnowVotesEnsure). Cells within SNOW_LINE_REACH of an anchor are always voted
// (no SNOW_DEC_ALL shortcut). A new line = a row in kSnowLines; re-trace the points if the art changes.
struct SnowLineDef { int tile; const float ( *pts )[ 2 ]; int n; bool cow; bool edge; };	// cow: THE CHAOS (below); edge: ADRIA'S EDGE (below)
const float kChurchLine[][ 2 ] = { { -203, -12 }, { -170, 5 }, { -160, 10 }, { -103, 12 }, { -63, 17 }, { -27, 17 }, { 3, 18 },
                                   { 40, 6 }, { 96, 1 }, { 140, 0 } };
// THE TAVERN'S LINE was DRAWN BY MORDOR (2026-10-10, "this is my take how the border should go"): red on a render of the
// tavern with the old line over it, converted to these coordinates by fitting the old line in his picture (to about a
// third of a px; scratchpad rocks\red_pts.txt), one point per 4 px. It runs into Ogden's open doorway and, on the left,
// higher up the wall by the barrels. Its bumps are his: no CHAOS on it (cow false) -- the chaos (and its "calm span" at
// the door, tried the same day) moved and bit the edge he drew. To change it, redraw rather than nudge single points.
const float kTavernLine[][ 2 ] = {
	{ -261, -34 }, { -257, -29 }, { -253, -27 }, { -249, -25 }, { -244, -25 }, { -240, -28 }, { -236, -27 }, { -232, -20 }, { -227, -19 }, { -223, -17 },
	{ -219, -15 }, { -215, -18 }, { -210, -22 }, { -206, -29 }, { -202, -29 }, { -198, -27 }, { -193, -26 }, { -189, -25 }, { -185, -22 }, { -181, -19 },
	{ -176, -16 }, { -172, -14 }, { -168, -11 }, { -164, -9 }, { -159, -6 }, { -155, -2 }, { -151, 2 }, { -147, 6 }, { -142, 7 }, { -138, 8 },
	{ -134, 10 }, { -130, 12 }, { -125, 14 }, { -121, 15 }, { -117, 15 }, { -113, 12 }, { -108, 8 }, { -104, 7 }, { -100, 10 }, { -96, 11 },
	{ -91, 11 }, { -87, 10 }, { -83, 6 }, { -79, 11 }, { -74, 12 }, { -70, 17 }, { -66, 19 }, { -62, 22 }, { -57, 22 }, { -53, 23 },
	{ -49, 21 }, { -45, 19 }, { -40, 16 }, { -36, 12 }, { -32, 9 }, { -28, 7 }, { -23, 5 }, { -19, 7 }, { -15, 10 }, { -11, 12 },
	{ -6, 15 }, { -2, 18 }, { 2, 20 }, { 7, 23 }, { 11, 26 }, { 15, 28 }, { 19, 30 }, { 24, 32 }, { 28, 34 }, { 32, 36 },
	{ 36, 35 }, { 41, 35 }, { 45, 35 }, { 49, 32 }, { 53, 31 }, { 58, 30 }, { 62, 25 }, { 66, 22 }, { 70, 16 }, { 75, 14 },
	{ 79, 10 }, { 83, 10 }, { 87, 13 }, { 92, 16 }, { 96, 12 }, { 100, 8 }, { 104, 6 }, { 109, 5 }, { 113, 2 }, { 117, -2 },
	{ 121, -5 }, { 126, -7 }, { 130, -9 }, { 134, -11 }, { 138, -16 }, { 143, -16 }, { 147, -16 }, { 151, -17 }, { 155, -21 }, { 160, -23 },
	{ 164, -24 }, { 168, -25 }, { 172, -27 }, { 177, -29 }, { 181, -30 }, { 185, -33 }, { 189, -36 }, { 194, -38 }, { 198, -42 }, { 201, -43 } };
const SnowLineDef kSnowLines[] = {
	{ 210, kChurchLine, (int)( sizeof( kChurchLine ) / sizeof( kChurchLine[ 0 ] ) ), false, false },	// the church, round-tower entry side
	{ 113, kTavernLine, (int)( sizeof( kTavernLine ) / sizeof( kTavernLine[ 0 ] ) ), false, true },	// Ogden's tavern, front and right side (Mordor's own line, ADRIA'S EDGE)
};
const int SNOW_LINE_N = (int)( sizeof( kSnowLines ) / sizeof( kSnowLines[ 0 ] ) );
int SnowLineSq[ SNOW_LINE_N ][ 4 ];		// each anchor tile's pieces (TownWater_OnTownLoaded)
int SnowLineRow[ SNOW_LINE_N ], SnowLineCol[ SNOW_LINE_N ];	// where each anchor stands on this map, -1 = not found
const float SNOW_LINE_BAND = 24.0f, SNOW_LINE_WOBBLE = 1.5f;
// ADRIA'S EDGE on a snow line (edge = true; the tavern, Mordor 2026-10-10: "look at Adria's hut side where you added the cow
// pattern -- do the same to that snowline on the tavern"): exactly THE EDGE of ADRIA'S SIDE -- the line wobbles by
// HUT_EDGE_WOBBLE px, and within HUT_EDGE_BAND px of it the art votes (edgeVote in SnowCellVotes: ground colours snow, wall
// and rock colours bare), patchy like the fences' feet; further out, up to SNOW_LINE_BAND, the line decides as before.
// THE CHAOS (a line with cow = true -- none now: the tavern's line is Mordor's own drawing, see kTavernLine. He asked for it
// first: "it feels like you carefully used scissors there, give it some
// chaos"): a traced line sits right at the wall's foot, so its wobble fell under the wall and the edge seen was the
// wall's own straight bottom. So the snow starts SNOW_COW_DROP px lower, BITES of up to SNOW_COW_BITE px are taken out
// of it (coarse noise), and BARE SPOTS (fine noise) dot the first SNOW_COW_REACH px, fewer further from the wall:
// uneven dark ground between the wall and the snow, like the fence's feet. Pixels within SNOW_COW_BAND of the line.
const float SNOW_COW_DROP = 10.0f, SNOW_COW_BITE = 12.0f, SNOW_COW_SPOT = 0.35f, SNOW_COW_REACH = 20.0f, SNOW_COW_BAND = 32.0f;
const int SNOW_LINE_REACH = 9;

// (th4) LEAF ENTRIES: the palette entries of the leaves, picked from a calibration log of the tree
// tiles' pixels against all other town tiles (share of the entry's pixels that sit in tree tiles):
// the reddish / purple entries that are mostly tree -- 65 (64%), 117 (48%), 70 (39%), 96 (38%),
// 89 (37%), 88 (29%), 109 (20%), 106 (19%) -- plus 49, the bright leaf highlight (12%: the same
// colour also appears in other tiles, but only pixels inside tree tiles are ever used). Left out on
// purpose: the olive / green entries 87, 94, 56 (grass and moss at the tree feet), the browns
// 45, 33 (wood, windows) and the saturated reds 68, 107 (window glass, the red cross).
const uchar kLeafEntries[] = { 49, 65, 70, 88, 89, 96, 106, 109, 117 };
bool LeafIdx[ 256 ];
// (th4) LEAF NEIGHBOURHOOD RULE. The dark leaf entries (88, 89, 106, 109, 117) are also the shading
// of grass and of house walls and roofs inside the same tiles. A dark leaf pixel counts only with a
// bright leaf entry within LEAF_NEAR_R -- in EVERY tree tile (checked on a one-off frame dump: every
// crown, thin ones included, keeps its leaves; the dots on the grass under the trees drop out).
// The house tiles (kHutLeafTiles) also get the two cluster checks below (Adria's hut, the house
// behind Wirt). Measured on a one-off frame dump at the hut (1x zoom): of the dots on the
// wall above the door only 5 of 212 had a BRIGHT leaf entry (49, 65, 70, 96) within 2 px, against
// 2181 of 2708 in a tree canopy. So, on the finished frame, in a (2 LEAF_NEAR_R + 1)^2 window of
// world pixels:
//   - a dark leaf pixel counts only with a bright leaf entry in the window;
//   - a bright leaf pixel counts only with at least LEAF_BRIGHT_MIN leaf-entry pixels in the window
//     (lone bright specks on the hut's door area).
//   - and every leaf pixel needs at least LEAF_WIDE_BRIGHT_MIN bright leaf pixels in the wider
//     (2 LEAF_WIDE_R + 1)^2 window: the last specks at the hut's door had at most 4 there, real
//     canopy leaves 13 on the median (10th percentile 4).
// On that frame: wall 212 -> 0, door foot 129 -> 0, roof 21 -> 0, canopy 2708 -> 1824.
const uchar kLeafBright[] = { 49, 65, 70, 96 };
bool LeafBrightIdx[ 256 ];
const int LEAF_NEAR_R          = 2;
const int LEAF_BRIGHT_MIN      = 4;
const int LEAF_WIDE_R          = 4;
const int LEAF_WIDE_BRIGHT_MIN = 5;
// (th4) WINDOW LIGHT: the red light of the church windows on the grass is drawn around a core of
// saturated red, entry 68 (64,21,21) -- the only saturated red the calibration log found inside the
// tree tiles; leaves never contain it. So a leaf-entry pixel with an entry-68 pixel within
// WINDOW_LIGHT_R pixels (on the drawn frame, before the zoom) belongs to the window light and does
// not glow. If some cross pixels are too far from any 68 pixel, they simply glow as leaves.
const uchar WINDOW_LIGHT_ENTRY = 68;
const int   WINDOW_LIGHT_R     = 3;

bool WaterIdx[ 256 ];						// kWaterEntries as a lookup
uchar TerrainDark[ 128 ];					// entry 1..127 -> nearest terrain entry at WATER_BRIGHTNESS % (BuildTerrainDark)
bool TerrainDarkReady = false;
PALETTEENTRY TerrainDarkPal[ 128 ];			// (th4) the palette it was built from (EnsureTerrainDark)
bool Learned = false;						// share log written (debug only)
uint InRiverCount[ 256 ], TotalCount[ 256 ];
int LearnFrames = 0, LearnPixels = 0;

struct Cell { int x, y, artL, artR; };		// pre-zoom WorkingSurface coords of a river cell (DrawTileFill's dst); its floor's RIVER ART maps
const int MAX_CELLS = 2048;
Cell Cells[ MAX_CELLS ];
// (th4) RIVER ART (TownWater_Render step 1): per floor TILE ID (as TileMap layer 0 holds it), the kWaterEntries pixels of
// its art -- 32 x 32, row 0 = the BOTTOM row (as PutTile draws), the entry there or 0. Decoded on first use like the GRASS
// art (GrassDecodePiece), kept until the level's CEL or the level changes (TownWater_BeginFrame, TownWater_OnTownLoaded).
std::vector<int> RiverArtIdx;				// tile id -> its map in RiverArt (in 1024-byte steps), -1 = not decoded yet
std::vector<uchar> RiverArt;
// (th4) ICE RIVER: the SAME tiles' WHOLE art (every opaque pixel, 0 = transparent), at the same index -- the mirror needs
// to know a bank pixel as the floor's own too (MSurf 1). RiverArt keeps only the water: TownWater_FeetBare, the flow map
// and step 1's water test count its non-zero pixels as water, so it must stay as it is. Decoded and cleared with it.
std::vector<uchar> RiverArtFull;
const int* RiverArtCel = 0; int RiverArtDun = -1;
int CellCount = 0;

struct HeroNote { bool valid; int x, y, frame, width; char* cl2; };	// pre-zoom, as CL2_Draw got them
HeroNote Heroes[ PlayersMax_4 ];

uchar* Surf = 0; int SurfSize = 0;			// the mask buffer, see the header
std::vector<int> RiverReflPix;				// (th4) RUNNING RIVER: this frame's river reflection pixels, screen-relative post-zoom (y << 16 | x)
// (th4) THE MAP ANCHOR: where the centre of map cell (0,0) is this frame, screen-relative PRE-zoom px -- from the
// first cell TownWater_FloorDrawn sees (its diamond centre (x + 32, y - 15) moved back by (row, col)). The GRASS
// and the RUNNING RIVER measure map positions from it (Grass.hlsl, River.hlsl). Found again every frame.
int MapC00X = 0, MapC00Y = 0; bool MapC00Set = false;
bool MaskValid = false;
bool MirrorValid = false;					// (th4) ICE RIVER: Surf holds this frame's water for dx_utility.cpp IceRiver_BuildMask
struct Box { int x0, y0, x1, y1; };			// screen-relative, x1/y1 exclusive
Box PrevBox = { 0, 0, 0, 0 };
bool PrevWarped = false;					// Perspective_WarpWorld resampled Surf after the last Render

const uchar Bayer4[ 16 ] = { 0, 8, 2, 10, 12, 4, 14, 6, 3, 11, 1, 9, 15, 7, 13, 5 };

#if TH4_TOWNWATER_DEBUG
void Log( const char* fmt, ... )
{
	FILE* f = fopen( "TownWater.log", "a" );
	if( !f ) return;
	va_list ap; va_start( ap, fmt ); vfprintf( f, fmt, ap ); va_end( ap );
	fclose( f );
}
#else
inline void Log( const char*, ... ) {}
#endif

bool InTown() { return Dungeon == DUN_0_TOWN && !Dungeon->isQuest; }
// (th4) ICE TREES: under the Ice Age game changer the trees are SNOW-COVERED (SNOW ON THE TREES, at
// TownWater_CellEnd: snow along the top of every branch and leaf clump, in the 8-bit frame) and their
// leaves lightly frosted, with only a faint glow left (Mordor, 2026-10-09: "not glowing (that much at
// least) ... snow-covered instead") -- in the normal town and in Ravenholm, exactly where it snows
// (Snowfall_Active), by day and by night. The frost uses the same leaf recording and filtering as the
// glow nights (GLOWING LEAVES below); its colours and halo: dx_utility.cpp BioGlow_Present, ICE_* constants. Ravenholm is a quest map
// drawn by Draw.cpp DrawSceneDungeonTop, which calls TownWater_CellBegin / CellEnd there too; it
// uses the town's tiles (Main.cpp loads NLevels\TownData\Town.TIL for DT_0_TOWN), so TreePiece
// applies -- the town itself is built from Levels\TownData\Town.TIL and drawn with the NLevels
// pieces, so the two files agree on these tiles.
// (IceTrees_Active itself is defined right after this namespace: it is global, declared in funcs.h.)
// The river still gets the mask when only the reflection is on, or when it is frozen: the mask is
// where reflections may go. Ripples are a separate question, TownWater_RiverRipples().
// (th4) Never in Classic, whatever the config says (Mordor's call): the Classic river is left exactly
// as drawn -- no mask, no darkening, no reflections (and no ripple, TownWater_RiverRipples).
// (th4) ICE RIVER (dx_utility.cpp, Ice Age town by day): needs the river mask too, whatever the two options say
bool FeatureOn() { return GameMode != GM_CLASSIC && ( WaterShimmer > 0 || WaterReflection || IceRiver_Shows() ); }
// (th4) RAVENHOLM'S RIVER: Ravenholm (DUN_44, a quest map built from the town's tiles -- its river is made of the
// same kRiverTiles, checked against Ravenholm.dun) gets the hero REFLECTIONS (under Ice Age too) and the old
// RIPPLE of the cave water maps (PostFx.hlsl; not under Ice Age, where it is frozen -- TownWater_RiverRipples):
// the village has gone to hell and its water is STAGNANT (Mordor) -- no RUNNING RIVER there, no darkening of the
// water, no glowing river. It is drawn by Draw.cpp DrawSceneDungeonTop, which notes the river cells
// (TownWater_NoteCell), and Draw.cpp DrawPlayer notes the heroes (TownWater_NotePlayer), as TownMap.cpp does for
// the town. Ravenholm is fully lit (FullLighting), so its frame holds the art's own entries and kWaterEntries
// finds the water exactly as in town. dx_utility.cpp PostFx_LevelHasWater / HeatHaze_BuildMask take its mask too.
bool RavenRiver() { return Dungeon == DUN_44_RAVENHOLM && FeatureOn(); }
bool RiverLevel() { return InTown() || Dungeon == DUN_44_RAVENHOLM; }	// (th4) the levels with this file's river mask

bool EnsureSurface()
{
	const int size = ( MaxWorkingHeight + 256 ) * MaxWorkingWidth;	// same size as LightSurface (dx.cpp)
	if( size <= 0 ) return false;
	if( SurfSize != size ){
		free( Surf );
		Surf = (uchar*)calloc( size, 1 );
		SurfSize = Surf ? size : 0;
		PrevBox = { 0, 0, 0, 0 };
	}
	return Surf != 0;
}

#if TH4_TOWNWATER_DEBUG
// Share log: per palette entry, its pixels inside river cells vs on the whole screen, over the
// first frames that show river. This is how kWaterEntries was chosen; it changes nothing at run
// time. Entries with a high share AND many river pixels are water.
void LogShares( const uchar* frame, int riverPixels )
{
	const int W = ScreenWidth, H = ScreenHeight;
	for( int y = 0; y < H; ++y ){
		const uchar* s = frame + y * WorkingWidth;
		for( int x = 0; x < W; ++x ) ++TotalCount[ s[ x ] ];
	}
	LearnPixels += riverPixels;
	if( ++LearnFrames < LEARN_MIN_FRAMES || LearnPixels < LEARN_MIN_PIXELS ) return;
	for( int i = 1; i <= 127; ++i ){
		if( (unsigned long long)InRiverCount[ i ] * 1000 < (unsigned long long)LearnPixels ) continue;
		Log( "  entry %3d river %8u of %8u (%3u%%)%s\n", i, InRiverCount[ i ], TotalCount[ i ],
			(uint)( TotalCount[ i ] ? (unsigned long long)InRiverCount[ i ] * 100 / TotalCount[ i ] : 0 ),
			WaterIdx[ i ] ? "  <- kWaterEntries" : "" );
	}
	Log( "shares after %d frames, %d river pixels\n", LearnFrames, LearnPixels );
	Learned = true;
}
#endif

// Decode one CL2 frame into Pix/Opq (row 0 = BOTTOM row, as CL2 stores it). Returns the row count.
// Format (Rle2DrawPal, Engine.cpp ~1718): signed byte b >= 0 skips b transparent pixels; b < 0,
// n = -b: n <= 65 copies n bytes, n > 65 repeats the next byte n - 65 times. Rows wrap at width.
const int MAX_W = 256, MAX_H = 256;
uchar Pix[ MAX_W * MAX_H ], Opq[ MAX_W * MAX_H ];
int DecodeCl2Frame( char* cl2, int frame, int width )
{
	if( !cl2 || width <= 0 || width > MAX_W ) return 0;
	const uint* header = (const uint*)cl2;
	const uint frameCount = header[ 0 ];
	if( frame < 1 || (uint)frame > frameCount || frameCount > 50 ) return 0;	// same sanity limit as DrawPlayer
	const char* from = cl2 + header[ frame ];
	const int begin = *(const ushort*)from;										// offset of the pixel data (after the 5-word section table)
	const int len = (int)( header[ frame + 1 ] - header[ frame ] ) - begin;
	if( begin <= 0 || len <= 0 ) return 0;
	const signed char* s = (const signed char*)from + begin;
	const signed char* e = s + len;
	memset( Opq, 0, sizeof( Opq ) );
	int x = 0, r = 0;
	while( s < e && r < MAX_H ){
		int b = *s++;
		if( b >= 0 ){
			x += b;
			while( x >= width ){ x -= width; ++r; }
			continue;
		}
		int n = -b;
		const bool fill = n > 65;
		if( fill ){ n -= 65; if( s >= e ) break; }
		const uchar fillV = fill ? (uchar)*s++ : 0;
		for( int k = 0; k < n && r < MAX_H; ++k ){
			if( !fill && s >= e ) break;
			const uchar v = fill ? fillV : (uchar)*s++;
			Pix[ r * width + x ] = v; Opq[ r * width + x ] = 1;
			if( ++x >= width ){ x = 0; ++r; }
		}
	}
	return r + ( x ? 1 : 0 ) > MAX_H ? MAX_H : r + ( x ? 1 : 0 );
}

// TerrainDark: for every town terrain entry, the terrain entry (1..127) closest to its colour at
// WATER_BRIGHTNESS %, weighted like the eye (green most). Built from FilePalette, the town palette
// actually loaded -- normal, Classic, multiplayer and Ice Age each get their own. Only 1..127 are
// candidates: those are the entries the day/night blend treats as terrain (palette.cpp
// DAYNIGHT_TERRAIN_FIRST/LAST), so a darkened pixel keeps darkening with the rest of the ground.
void BuildTerrainDark()
{
	TerrainDark[ 0 ] = 0;
	for( int v = 1; v < 128; ++v ){
		const int r = FilePalette[ v ].peRed * WATER_BRIGHTNESS / 100;
		const int g = FilePalette[ v ].peGreen * WATER_BRIGHTNESS / 100;
		const int b = FilePalette[ v ].peBlue * WATER_BRIGHTNESS / 100;
		int best = v, bestD = 0x7FFFFFFF;
		for( int i = 1; i < 128; ++i ){
			const int dr = FilePalette[ i ].peRed - r, dg = FilePalette[ i ].peGreen - g, db = FilePalette[ i ].peBlue - b;
			const int d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
			if( d < bestD ){ bestD = d; best = i; }
		}
		TerrainDark[ v ] = (uchar)best;
		if( WaterIdx[ v ] ) Log( "darker water: entry %d rgb(%d,%d,%d) -> %d rgb(%d,%d,%d)\n", v,
			FilePalette[ v ].peRed, FilePalette[ v ].peGreen, FilePalette[ v ].peBlue,
			best, FilePalette[ best ].peRed, FilePalette[ best ].peGreen, FilePalette[ best ].peBlue );
	}
	memcpy( TerrainDarkPal, FilePalette, sizeof( TerrainDarkPal ) );
	TerrainDarkReady = true;
}
// (th4) built for the palette loaded NOW: walking from the town into Ravenholm (RAVENHOLM'S RIVER) changes the
// palette without a TownWater_OnTownLoaded, and the town's table would colour Ravenholm's reflections
void EnsureTerrainDark()
{
	if( !TerrainDarkReady || memcmp( TerrainDarkPal, FilePalette, sizeof( TerrainDarkPal ) ) ) BuildTerrainDark();
}

// LightTable row for a darkness on the 0..15 scale (Light4Levels keeps only rows 0..3).
int DarkRow( int of15 )
{
	if( of15 < 0 ) of15 = 0; if( of15 > 15 ) of15 = 15;
	return LightMax > 0 ? ( of15 * LightMax + 7 ) / 15 : 0;
}

// writeBuf: the river's mask (Surf) holds the palette index of each water pixel and must follow the
// reflection (HeatHaze_BuildMask compares it with the world snapshot); the PUDDLE mask (PSurf) holds
// puddle depth codes for Wet.hlsl, which must stay as they are -- so puddles pass false.
void DrawReflection( const HeroNote& h, int z, uchar* frame, uchar* buf, int darkRow, bool writeBuf = true )
{
	const int rows = DecodeCl2Frame( h.cl2, h.frame, h.width );
	if( rows <= 0 ) return;
	const int w = h.width;
	int feet = -1, head = -1;
	for( int r = 0; r < rows; ++r ){
		for( int x = 0; x < w; ++x ) if( Opq[ r * w + x ] ){ if( feet < 0 ) feet = r; head = r; break; }
	}
	if( feet < 0 ) return;
	const int span = head > feet ? head - feet : 1;
	const int W = ScreenWidth, H = ScreenHeight;
	const uchar* dark = &LightTable[ darkRow * 256 ];
	// Sprite row r is drawn on pre-zoom line y - r (CL2_Draw: bottom row at y). Mirroring about
	// the feet row puts it on line (y - feet) + 1 + (r - feet).
	for( int r = feet; r <= head; ++r ){
		const int d = r - feet;
		const int wy = h.y - feet + 1 + d;
		const int sy = ( wy - Screen_TopBorder ) * z;
		if( sy >= H ) break;
		if( sy + z <= 0 ) continue;
		const int cover = REFLECT_COVER_FEET - ( REFLECT_COVER_FEET - REFLECT_COVER_HEAD ) * d / span;
		for( int x = 0; x < w; ++x ){
			if( !Opq[ r * w + x ] ) continue;
			const int wx = h.x + x;
			if( Bayer4[ ( wy & 3 ) * 4 + ( wx & 3 ) ] >= cover ) continue;
			// sprite entries 128..255 are proper 16-shade ramps, so LightTable is right for them; a
			// sprite pixel in the terrain range would get an unrelated colour from it (see WATER
			// COLOUR in the header), so those go through TerrainDark instead
			const uchar pv = Pix[ r * w + x ];
			const uchar col = pv >= 128 ? dark[ pv ] : TerrainDark[ pv ];
			if( !col ) continue;										// 0 is "no water" in Surf
			const int sx = ( wx - Screen_LeftBorder ) * z;
			for( int by = 0; by < z; ++by ){
				const int py = sy + by;
				if( py < 0 || py >= H ) continue;
				for( int bx = 0; bx < z; ++bx ){
					const int px = sx + bx;
					if( px < 0 || px >= W ) continue;
					const int p = py * WorkingWidth + px;
					if( buf[ p ] ){
					frame[ p ] = col;
					if( writeBuf ){ buf[ p ] = col; RiverReflPix.push_back( py << 16 | px ); }	// (th4) RUNNING RIVER: kept out of the slide
				}
				}
			}
		}
	}
}

// ============================================================================================
// (th4) PUDDLES -- WET GROUND (config WetGround 0..10, default 5). How wet the town is comes from
// Weather.cpp Weather_WetQ8: soaked during a STORM night, drying out over the day after it.
// WHERE: puddles lie in fixed "low spots" -- the same cells in every game, the way real puddles
// come back to the same places after every rain. A cell may hold the centre of one (PUDDLE_CHANCE_PCT,
// a hash of row/col) if it and all 8 neighbours are walkable ground that is not river
// (TileBlockWalking, RiverPiece), so a puddle never runs into a house, a fence or the river.
// NOT ON GRASS (Mordor: water soaks into a lawn; puddles stay on the paths and the square): the centre
// cell must be bare -- its grass share (GRASS, GrassCellDens) at most PUDDLE_HOST_MAX_GRASS -- and a
// pixel whose ART is grass (a green or grass-speck entry, or a shadow lying on grass) is never water
// where the grass share there (bilinear across cells, like Grass.hlsl) is above PUDDLE_PIXEL_GRASS /
// PUDDLE_PIXEL_SHADE, and no pixel at all is where it is above PUDDLE_PIXEL_LAWN (the dirt specks of a
// lawn's texture stayed wet and left loose dots of water in the grass): a puddle reaching the edge of a
// path stops where the grass begins, and the green specks inside a path stay water. Measured on the town art: lawn cells hold 40..70% grass entries (the
// texture is mixed), bare path / square cells 0..10%; 55 of the town's cells may host a puddle now, 307
// did before -- fewer puddles on purpose (Mordor: "it's ok if there is less of them"), the chance stays.
// Whether GrassBlades is on or not: the art and the shares are worked out for the puddles too. Its shape
// is a main ellipse with up to two smaller ones beside it (flattened 2:1 like the floor) and a ragged
// rim from world-anchored value noise. Each pixel gets a DEPTH (255 = middle .. 1 = rim). As the town
// dries, only pixels deeper than 255 - wetness stay water (PuddleThr), so puddles shrink to their
// middles and vanish; while it starts to rain they grow from the middle out.
// HOW, per frame (each call site has a one-line pointer back here):
//   TownWater_BeginFrame ...... is it wet now (PuddlesOn, PuddleThr), forget last frame's pixels
//   TownMap.cpp DrawSceneTownTop, right after a cell's FLOOR layer (layer 0): TownWater_FloorDrawn
//     records that cell's puddle pixels and the colour the floor drew there. The per-cell pixel lists
//     are worked out once, the first time a cell is drawn (ComputeCellPuddles), and dropped when the
//     town is loaded again (TownWater_OnTownLoaded).
//   TownWater_Render -> RenderPuddles (after the zoom): a recorded pixel whose colour is still on the
//     finished frame is visible ground (anything drawn later -- a hero, a tree crown, a wall -- changed
//     it). Those pixels are redrawn darker (TerrainDark, the river's water colour), marked in PSurf with
//     a code 128..255 (128 = the rim of the water, 255 = a third of the way in or deeper), and get the
//     heroes' reflections (DrawReflection, writeBuf false, so PSurf keeps its codes).
//   Draw.cpp Perspective_WarpWorld warps PSurf (TownWater_PuddleSurface) with the other world layers.
//   dx_utility.cpp Wet_BuildMask (TownWater_PuddleRows) turns PSurf + the frame into the WET MAP for
//     Wet.hlsl: the whole wet ground darker, puddles mirroring the sky, raindrop rings while it rains.
// Without shaders (drawmode surface) the puddles still show: darker water with reflections.
// KNOWN-GOOD CHECK: a storm night in Tristram -- as the rain starts, dark patches grow on the paths and
// the grass; the hero's reflection shows in them; rings dance in them while it rains. Next morning
// the ground is darker and the puddles mirror the grey sky, then shrink and are gone around
// late afternoon. Never inside a house, on the river, under a hero or NPC, or in Ice Age / Classic.
const int PUDDLE_CHANCE_PCT = 12;	// share of eligible cells holding the centre of a puddle
const int PUDDLE_HOST_MAX_GRASS = 51;	// (th4) NOT ON GRASS: a puddle's centre cell holds at most 20% grass (of 255)
const int PUDDLE_PIXEL_GRASS    = 64;	// ... a grass pixel is dry where the grass share is above 25%
const int PUDDLE_PIXEL_SHADE    = 89;	// ... a shadow pixel (shadows lie on paths too) where it is above 35%
const int PUDDLE_PIXEL_LAWN     = 102;	// ... and every pixel where it is above 40% (lawn cells hold 40..70%)
struct PuddlePx { short dx, dy; uchar depth; };	// a puddle pixel of one cell: offset from the cell's DrawTileFill
												// dst (dx 0..63, dy -30..0) and its depth 1..255
std::vector<PuddlePx> PuddlePool;
int PuddleFirst[ 112 ][ 112 ];		// index into PuddlePool, -1 = not worked out yet
ushort PuddleCnt[ 112 ][ 112 ];
struct PuddleRec { int rel; uchar val, depth; };	// recorded this frame: screen-relative pre-zoom (y << 16 | x), floor colour, depth
PuddleRec* PuddleRecs = 0; size_t PuddleRecN = 0, PuddleRecCap = 0;	// plain growing buffer (see LeafPx)
bool PuddlesOn = false;			// wet this frame (TownWater_BeginFrame)
int PuddleThr = 255;			// a pixel is water only if its depth is above this (255 - wetness)
uchar* PSurf = 0; int PSurfSize = 0;	// PUDDLE MASK: screen-parallel like Surf, 0 = no puddle, 128..255 = puddle (rim .. deep)
Box PPrevBox = { 0, 0, 0, 0 };
bool PPrevWarped = false, PuddleValid = false;

uint PHash( uint x ) { x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16; return x; }
uint CellHash( int r, int c, uint salt ) { return PHash( (uint)r * 73856093U ^ (uint)c * 19349663U ^ salt ); }

// walkable ground that is not river (puddle cells and their neighbours)
bool GroundCell( int r, int c )
{
	if( r < 0 || c < 0 || r >= 112 || c >= 112 ) return false;
	const int piece = FineMap[ r ][ c ];
	return piece > 0 && piece < 2049 && !TileBlockWalking[ piece ] && !RiverPiece[ piece ];
}
bool PuddleHost( int r, int c )
{
	if( CellHash( r, c, 0x51ED27U ) % 100 >= PUDDLE_CHANCE_PCT ) return false;
	if( GrassShareQ8( (float)r, (float)c ) > PUDDLE_HOST_MAX_GRASS ) return false;	// (th4) NOT ON GRASS: the centre cell is bare
	for( int dr = -1; dr <= 1; ++dr ) for( int dc = -1; dc <= 1; ++dc ) if( !GroundCell( r + dr, c + dc ) ) return false;
	return true;
}
struct PEllipse { float cx, cy, rx, ry; };
// a host cell's puddle: up to 3 ellipses in host-local px (origin = its DrawTileFill dst; the floor
// diamond's middle is (32, -15)); returns how many
int HostEllipses( int r, int c, PEllipse* e )
{
	const uint h = CellHash( r, c, 0xA511E9B3U );
	const float rx = 10.0f + (float)( ( h >> 7 ) % 13 );	// 10..22
	e[ 0 ] = { 32.0f + ( (int)( h & 15 ) - 8 ) * 1.2f, -15.0f + (float)( (int)( ( h >> 4 ) & 7 ) - 4 ), rx, rx * 0.5f };
	int n = 1;
	const int sats = (int)( ( h >> 11 ) % 3 );
	for( int k = 0; k < sats; ++k ){
		const uint hk = PHash( h + 0x9E3779B9U * (uint)( k + 1 ) );
		const float ang = ( hk & 1023 ) / 1024.0f * 6.2831853f;
		const float dist = rx * ( 0.55f + ( ( hk >> 10 ) & 255 ) / 255.0f * 0.45f );
		const float r2 = rx * ( 0.35f + ( ( hk >> 18 ) & 255 ) / 255.0f * 0.30f );
		e[ n++ ] = { e[ 0 ].cx + cosf( ang ) * dist, e[ 0 ].cy + sinf( ang ) * dist * 0.5f, r2, r2 * 0.5f };
	}
	return n;
}
// value noise on the ground, 0..1, lattice PUDDLE_NOISE_PX world px (y doubled: flat on the 2:1 floor)
float PNoise( float x, float y )
{
	const float PUDDLE_NOISE_PX = 6.0f;
	const float fx = x / PUDDLE_NOISE_PX, fy = y / PUDDLE_NOISE_PX;
	const int ix = (int)floorf( fx ), iy = (int)floorf( fy );
	const float tx = fx - ix, ty = fy - iy;
	auto v = []( int a, int b ){ return ( PHash( (uint)a * 0x8DA6B343U ^ (uint)b * 0xD8163841U ) & 0xFFFF ) / 65535.0f; };
	const float sx = tx * tx * ( 3.0f - 2.0f * tx ), sy = ty * ty * ( 3.0f - 2.0f * ty );
	const float a = v( ix, iy ) + ( v( ix + 1, iy ) - v( ix, iy ) ) * sx;
	const float b = v( ix, iy + 1 ) + ( v( ix + 1, iy + 1 ) - v( ix, iy + 1 ) ) * sx;
	return a + ( b - a ) * sy;
}
// the puddle pixels of cell (r, c): every floor-diamond pixel (DrawTileFill geometry) that lies in the
// puddle of this cell or of a neighbour. A host's dst is this cell's dst + (32 (dr - dc), 16 (dr + dc)).
void ComputeCellPuddles( int r, int c )
{
	PuddleFirst[ r ][ c ] = (int)PuddlePool.size();
	PuddleCnt[ r ][ c ] = 0;
	if( !GroundCell( r, c ) ) return;
	PEllipse e[ 9 ][ 3 ]; int en[ 9 ], hx[ 9 ], hy[ 9 ], hosts = 0;
	for( int dr = -1; dr <= 1; ++dr ) for( int dc = -1; dc <= 1; ++dc ){
		if( !PuddleHost( r + dr, c + dc ) ) continue;
		en[ hosts ] = HostEllipses( r + dr, c + dc, e[ hosts ] );
		hx[ hosts ] = 32 * ( dr - dc ); hy[ hosts ] = 16 * ( dr + dc );
		++hosts;
	}
	if( !hosts ) return;
	// (th4) NOT ON GRASS: the grass classes of this cell's two floor pieces (both indices first: decoding grows GrassArt)
	const TileColumn& tc = TileMap[ IsometricMapOffset( r, c ) ];
	const int il = GrassArtIndex( tc.Layer[ 0 ].Left ), ir = GrassArtIndex( tc.Layer[ 0 ].Right );
	const uchar* artL = GrassArtPtr( il ); const uchar* artR = GrassArtPtr( ir );
	for( int k = 0; k <= 30; ++k ){
		const int y = -k;
		int x0, wd;
		if( k <= 15 ){ x0 = 30 - 2 * k; wd = 4 * ( k + 1 ); }
		else         { const int j = k - 15; x0 = 2 * j; wd = 64 - 4 * j; }
		for( int x = x0; x < x0 + wd; ++x ){
			float d = 0.0f;
			for( int h = 0; h < hosts; ++h ){
				for( int q = 0; q < en[ h ]; ++q ){
					const PEllipse& el = e[ h ][ q ];
					const float ex = ( x - hx[ h ] - el.cx ) / el.rx, ey = ( y - hy[ h ] - el.cy ) / el.ry;
					d = max( d, 1.0f - ( ex * ex + ey * ey ) );
				}
			}
			if( d <= 0.0f ) continue;
			d += ( PNoise( (float)( 32 * ( r - c ) + x ), 2.0f * (float)( 16 * ( r + c ) + y ) ) - 0.5f ) * 0.30f;	// ragged rim
			if( d <= 0.02f ) continue;
			// (th4) NOT ON GRASS: grass art where the cell shares say "lawn" stays dry. The pixel's map position:
			// the cell's diamond centre (32, -15) is map (r + 0.5, c + 0.5), and the shares sit at those centres.
			{
				const uchar cls = x < 32 ? artL[ -y * 32 + x ] : artR[ -y * 32 + x - 32 ];	// piece row 0 = the bottom row (dy 0)
				const float X = x + 0.5f - 32.0f, Y = y + 0.5f + 15.0f;
				const int share = GrassShareQ8( r + ( X / 32.0f + Y / 16.0f ) * 0.5f, c + ( Y / 16.0f - X / 32.0f ) * 0.5f );
				if( share > ( !cls ? PUDDLE_PIXEL_LAWN : cls == 3 ? PUDDLE_PIXEL_SHADE : PUDDLE_PIXEL_GRASS ) ) continue;
			}
			const int dv = (int)( min( 1.0f, d ) * 255.0f );
			if( PuddleCnt[ r ][ c ] == 0xFFFF ) return;
			PuddlePool.push_back( { (short)x, (short)y, (uchar)max( 1, dv ) } );
			++PuddleCnt[ r ][ c ];
		}
	}
}
bool EnsurePuddleSurface()
{
	const int size = ( MaxWorkingHeight + 256 ) * MaxWorkingWidth;	// same as Surf
	if( size <= 0 ) return false;
	if( PSurfSize != size ){
		free( PSurf );
		PSurf = (uchar*)calloc( size, 1 );
		PSurfSize = PSurf ? size : 0;
		PPrevBox = { 0, 0, 0, 0 };
	}
	return PSurf != 0;
}
} // namespace
bool IceTrees_Active() { return IceAgeTreeSnow && Snowfall_Active(); }
// (th4) ICE TREES (config IceAgeTreeSnow, default on; was IceTreeGlow): IceTrees_Active above, see the note at InTown()

// (th4) RUNNING RIVER -- THE FLOW MAP: the direction the water runs at every map cell, for River.hlsl
// (dx_utility.cpp RIVER FLOW). Mordor: the river flows LEFT (south-west on screen). Per stream (a connected set
// of river cells, RiverPiece, 8-neighbour):
//   DOWNSTREAM = toward its MOUTH, its left end (the cells within RIVER_MOUTH_BAND of its smallest row - col, i.e.
//     its leftmost on screen). Every cell gets its distance ALONG THE CHANNEL to the mouth -- Dijkstra over the
//     stream's cells, each step as long as it is on screen (a map step (dr, dc) is (32 (dr - dc), 16 (dr + dc))
//     px) -- and the water runs downhill in it: where the right-hand river's two arms meet, both run into the
//     joined stream. (One fixed direction for the whole town was the simpler idea; it crossed the banks in bends.)
//   THE DIRECTION = the main axis of the WATER ITSELF -- the kWaterEntries pixels of the floor art (layer 0, decoded
//     like the GRASS art, GrassDecodePiece) in the cell and its 8 neighbours -- signed downstream. The river tiles are
//     coarse and the water meanders inside them: the distance's own gradient was off the water by 15 degrees
//     typically and up to 70 at places (measured offline on the whole town against the water pixels' axis), and the
//     water ran across its banks (Mordor, near the cows). The water's axis: 7 degrees typically, 21 at 9 cells in 10.
//     Where the water shows no clear axis (fewer than RIVER_AXIS_MIN_PX pixels around, or not RIVER_AXIS_ELONG times
//     longer than wide: a pool), the distance's gradient is used instead.
//   Then one smoothing pass over the 3 x 3 cells of the same stream.
//   THE STREAM PHASE, for River.hlsl's ripple: how far downstream a cell lies, in world px -- the distance to the mouth
//     (negated, x 32) as the first guess, then RIVER_PHASE_PASSES over-relaxed passes that pull each cell toward what
//     its stream neighbours and the flow direction say (phi_b + (P_a - P_b) . mean direction), so the phase rises along
//     the WATER's axis, not the cells'. The ripple's wave fronts are lines of equal phase. They used to be worked out in
//     the shader as dot(map position, direction): that moves along the direction only where the direction is the same
//     all around -- in every bend the position term (thousands of px from cell (0,0)) took over, and on the lower river,
//     which runs ACROSS that position vector, the ripples ran backwards on ~40 % of the water (Mordor: "the down river
//     has some wrong flows"; the upper river runs along it, ~12 %). Measured offline: with the stored phase < 1 %.
// Stored as an A8R8G8B8 texel, texel (x = row, y = col): R/G = the screen direction x/y (128 = 0); B/A = the stream
// phase as cos / sin of 2 pi phi / RIVER_WAVE_PERIOD (128 = 0) -- wrapped, so 8 bits are enough, and stored as a vector
// so the bilinear filter blends it without a seam (neighbouring cells differ by < 64 px = a quarter turn). River.hlsl
// reads it with bilinear filtering, so the speed eases off toward the banks (the cells around a river hold 0). Streams under
// RIVER_FLOW_MIN_CELLS cells (two small pieces of river tiles lie elsewhere in the town) get no flow: River.hlsl
// ripples them in place. Built at the FIRST TOWN FRAME after TownWater_OnTownLoaded (RiverFlowDirty, TownWater_BeginFrame):
// it reads the floor art (CurrentLevelCELFile / MIN), loaded for sure by then. Town.TIL / the sector maps changed =
// nothing to redo here: it is all worked out from FineMap and the art. kWaterEntries changed = the axis changes too.
// KNOWN-GOOD CHECK: the town by day, WaterShimmer on: both rivers run toward their lower-left ends, along every bend
// (by the cows and the bridge too, and through the abyss gate where the lower river's arms meet -- JoinRiverWater),
// the water never crosses a bank, the ripples travel downstream with it; a hero's reflection shimmers but does not slide.
namespace {
const int   RIVER_FLOW_MIN_CELLS = 40;
const int   RIVER_MOUTH_BAND     = 2;
const int   RIVER_AXIS_MIN_PX    = 40;
const float RIVER_AXIS_ELONG     = 2.0f;
const int   RIVER_JOIN_MIN_PX    = 16;		// JoinRiverWater: a cell needs this much floor water to join
const int   RIVER_PHASE_PASSES   = 200;		// THE STREAM PHASE: relaxation passes (measured: 200 is settled)
const float RIVER_PHASE_SOR      = 1.9f;	// and their over-relaxation
const float RIVER_WAVE_PERIOD    = 256.0f;	// world px per turn of the stored phase; dx_utility.cpp RIVER_WAVES are whole numbers of waves per turn
uint RiverFlowTex[ 112 * 112 ];
uint RiverFlowGen = 0;
bool RiverFlowValid = false, RiverFlowDirty = false;
}

// (th4) RUNNING RIVER -- WATER THAT JOINS THE RIVER (BuildRiverFlow calls this first). Some cells carry the river's own
// water in their floor art although their tile is not in kRiverTiles: in Tristram the Hellfire pieces of the ABYSS GATE,
// where the two arms of the lower river meet -- sector tiles 316 / 317 and the FineMap overrides of TownMap.cpp
// FillTownMap1ClosedAbyss / FillDMap3OpenedAbyss (the closed and the opened gate carry different pieces). Left out, the
// water there stood still and cut the lower river into three streams. They are found from the ART, so there is no second
// list of those numbers to drift: the floor water (kWaterEntries, MIN entries 14 / 15 as in BuildRiverFlow) of the river
// cells and of every cell within 2 of them is laid out in world pixels and flooded (4-neighbour) from the river cells'
// water; a cell with RIVER_JOIN_MIN_PX or more water, at least half of it reached, joins: its PIECE becomes a RiverPiece
// (so every cell with that piece is river) -- the water mask, the puddles and the flow map all take it from there.
// Measured on the town, both gate states: the gate's pieces are reached 70..100 %, the blue rocks (tiles 57..63, which
// use the same palette entries, a few touching the river) 1..34 % -- nothing in between.
// KNOWN-GOOD CHECK: the log ("River: piece ... joins") lists 15 pieces with the gate closed (1160..1162, 1259..1269, 1283)
// and 13 with it open (1160..1164, 1262, 1264, 1294, 1296..1299, 1313).
static void JoinRiverWater()
{
	if( !CurrentLevelMINFile || !CurrentLevelCELFile ) return;
	static uchar nearRiver[ 112 ][ 112 ];
	memset( nearRiver, 0, sizeof( nearRiver ) );
	for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
		const int p = FineMap[ r ][ c ];
		if( p <= 0 || p >= MAX_PIECE || !RiverPiece[ p ] ) continue;
		for( int r2 = max( r - 2, 0 ); r2 <= min( r + 2, 111 ); ++r2 ) for( int c2 = max( c - 2, 0 ); c2 <= min( c + 2, 111 ); ++c2 ) nearRiver[ r2 ][ c2 ] = 1;
	}
	// a world pixel: x = 32 (row - col) + x in the piece + 32 for the right half + 3552 (0..7167),
	// y = 16 (row + col) - row from the piece's bottom + 31 (0..3583) -- BuildRiverFlow's moments, shifted to start at 0
	const int W = 7168, H = 3584;
	std::vector<uint> water( W * H / 32, 0 ), reached( W * H / 32, 0 );	// one bit per pixel
	auto bit = []( const std::vector<uint>& v, int k ){ return ( v[ k >> 5 ] >> ( k & 31 ) & 1 ) != 0; };
	auto set = []( std::vector<uint>& v, int k ){ v[ k >> 5 ] |= 1u << ( k & 31 ); };
	std::vector<int> stack;
	std::vector<std::pair<int, int>> other;	// ( cell, pixel ) of the water of the cells that are not river, in cell order
	uchar pix[ 1024 ], opq[ 1024 ];
	for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
		const int p = FineMap[ r ][ c ];
		if( !nearRiver[ r ][ c ] || p <= 0 || p >= MAX_PIECE ) continue;
		const bool isRiver = RiverPiece[ p ];
		const ushort* mine = (const ushort*)( CurrentLevelMINFile + 32 * ( p - 1 ) );
		for( int half = 0; half < 2; ++half ){
			const ushort id = mine[ 14 + half ];
			if( !id || !GrassDecodePiece( id, pix, opq ) ) continue;
			for( int b = 0; b < 32; ++b ) for( int x = 0; x < 32; ++x ){
				const int i = b * 32 + x;
				if( !opq[ i ] || !WaterIdx[ pix[ i ] ] ) continue;
				const int k = ( 16 * ( r + c ) - b + 31 ) * W + 32 * ( r - c ) + x + 32 * half + 3552;
				set( water, k );
				if( !isRiver ) other.push_back( { r * 112 + c, k } );
				else if( !bit( reached, k ) ){ set( reached, k ); stack.push_back( k ); }
			}
		}
	}
	while( !stack.empty() ){
		const int k = stack.back(); stack.pop_back();
		const int x = k % W;
		const int nb[ 4 ] = { x > 0 ? k - 1 : -1, x < W - 1 ? k + 1 : -1, k - W, k + W };
		for( int n : nb ){
			if( n < 0 || n >= W * H || !bit( water, n ) || bit( reached, n ) ) continue;
			set( reached, n ); stack.push_back( n );
		}
	}
	for( size_t i = 0; i < other.size(); ){
		const int cell = other[ i ].first;
		int n = 0, got = 0;
		for( ; i < other.size() && other[ i ].first == cell; ++i ){ ++n; if( bit( reached, other[ i ].second ) ) ++got; }
		const int p = FineMap[ cell / 112 ][ cell % 112 ];
		if( n < RIVER_JOIN_MIN_PX || 2 * got < n || RiverPiece[ p ] ) continue;
		RiverPiece[ p ] = true;
		Log( "River: piece %d at %d,%d joins the river (%d of its %d water px reach it)\n", p, cell / 112, cell % 112, got, n );
	}
}

static void BuildRiverFlow()
{
	RiverFlowDirty = false;
	JoinRiverWater();	// (th4) the abyss gate's water first, so the lower river is one stream (WATER THAT JOINS THE RIVER)
	static short comp[ 112 ][ 112 ];
	static float dist[ 112 ][ 112 ], fx[ 112 ][ 112 ], fy[ 112 ][ 112 ], sx[ 112 ][ 112 ], sy[ 112 ][ 112 ], phi[ 112 ][ 112 ];
	static double mom[ 112 ][ 112 ][ 6 ];	// the cell's water pixels: count, sum x, sum y, sum xx, sum yy, sum xy (world px from cell (0,0)'s centre)
	auto river = []( int r, int c ){ if( (uint)r >= 112 || (uint)c >= 112 ) return false; const int p = FineMap[ r ][ c ]; return p > 0 && p < MAX_PIECE && RiverPiece[ p ]; };
	static const int DR[ 8 ] = { -1, -1, -1, 0, 0, 1, 1, 1 }, DC[ 8 ] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	float dirX[ 8 ], dirY[ 8 ], stepLen[ 8 ];	// each map step on screen: its direction, its length in cells (32 px)
	for( int k = 0; k < 8; ++k ){
		const float x = 32.0f * ( DR[ k ] - DC[ k ] ), y = 16.0f * ( DR[ k ] + DC[ k ] ), l = sqrtf( x * x + y * y );
		dirX[ k ] = x / l; dirY[ k ] = y / l; stepLen[ k ] = l / 32.0f;
	}
	memset( comp, 0xFF, sizeof( comp ) ); memset( fx, 0, sizeof( fx ) ); memset( fy, 0, sizeof( fy ) ); memset( mom, 0, sizeof( mom ) );
	// the water pixels of every river cell: its floor pieces are MIN entries 14 / 15 (layer 0 left / right, as
	// TownMap.cpp InitIsometricTileMap reads them); piece-local x 0..31, row b from the bottom, the diamond's centre at (32, -15)
	if( CurrentLevelMINFile && CurrentLevelCELFile ){
		uchar pix[ 1024 ], opq[ 1024 ];
		for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
			if( !river( r, c ) ) continue;
			const ushort* mine = (const ushort*)( CurrentLevelMINFile + 32 * ( FineMap[ r ][ c ] - 1 ) );
			const double cx = 32.0 * ( r - c ), cy = 16.0 * ( r + c );
			double* m = mom[ r ][ c ];
			for( int half = 0; half < 2; ++half ){
				const ushort id = mine[ 14 + half ];
				if( !id || !GrassDecodePiece( id, pix, opq ) ) continue;
				for( int b = 0; b < 32; ++b ) for( int x = 0; x < 32; ++x ){
					const int i = b * 32 + x;
					if( !opq[ i ] || !WaterIdx[ pix[ i ] ] ) continue;
					const double px = cx + x + 32 * half - 32 + 0.5, py = cy - b + 15 + 0.5;
					m[ 0 ] += 1; m[ 1 ] += px; m[ 2 ] += py; m[ 3 ] += px * px; m[ 4 ] += py * py; m[ 5 ] += px * py;
				}
			}
		}
	}
	std::vector<int> cells, queue;
	int nComp = 0;
	for( int r0 = 0; r0 < 112; ++r0 ) for( int c0 = 0; c0 < 112; ++c0 ){
		if( comp[ r0 ][ c0 ] >= 0 || !river( r0, c0 ) ) continue;
		// one stream
		cells.clear(); queue.assign( 1, r0 * 112 + c0 ); comp[ r0 ][ c0 ] = (short)nComp;
		for( size_t q = 0; q < queue.size(); ++q ){
			const int r = queue[ q ] / 112, c = queue[ q ] % 112;
			cells.push_back( queue[ q ] );
			for( int k = 0; k < 8; ++k ){
				const int r2 = r + DR[ k ], c2 = c + DC[ k ];
				if( river( r2, c2 ) && comp[ r2 ][ c2 ] < 0 ){ comp[ r2 ][ c2 ] = (short)nComp; queue.push_back( r2 * 112 + c2 ); }
			}
		}
		if( (int)cells.size() >= RIVER_FLOW_MIN_CELLS ){
			// its mouth (the left end) and the distance to it along the channel, in screen cells
			int minX = INT_MAX;
			for( int v : cells ) minX = min( minX, v / 112 - v % 112 );
			std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> pq;
			for( int v : cells ){
				const bool mouth = v / 112 - v % 112 <= minX + RIVER_MOUTH_BAND;
				dist[ v / 112 ][ v % 112 ] = mouth ? 0.0f : 1e30f;
				if( mouth ) pq.push( { 0.0f, v } );
			}
			while( !pq.empty() ){
				const float d0 = pq.top().first; const int v = pq.top().second; pq.pop();
				const int r = v / 112, c = v % 112;
				if( d0 > dist[ r ][ c ] ) continue;
				for( int k = 0; k < 8; ++k ){
					const int r2 = r + DR[ k ], c2 = c + DC[ k ];
					if( (uint)r2 >= 112 || (uint)c2 >= 112 || comp[ r2 ][ c2 ] != nComp ) continue;
					const float d1 = d0 + stepLen[ k ];
					if( d1 < dist[ r2 ][ c2 ] ){ dist[ r2 ][ c2 ] = d1; pq.push( { d1, r2 * 112 + c2 } ); }
				}
			}
			for( int v : cells ){
				const int r = v / 112, c = v % 112;
				// downhill in the distance: toward the neighbours nearer the mouth (at the mouth: away from the ones behind it)
				float gx = 0, gy = 0;
				double n = 0, smx = 0, smy = 0, sxx = 0, syy = 0, sxy = 0;	// the water of the 3 x 3 cells
				for( int k = 0; k < 8; ++k ){
					const int r2 = r + DR[ k ], c2 = c + DC[ k ];
					if( (uint)r2 >= 112 || (uint)c2 >= 112 || comp[ r2 ][ c2 ] != nComp ) continue;
					const float w = ( dist[ r ][ c ] - dist[ r2 ][ c2 ] ) / stepLen[ k ];
					gx += w * dirX[ k ]; gy += w * dirY[ k ];
					const double* m = mom[ r2 ][ c2 ];
					n += m[ 0 ]; smx += m[ 1 ]; smy += m[ 2 ]; sxx += m[ 3 ]; syy += m[ 4 ]; sxy += m[ 5 ];
				}
				{ const double* m = mom[ r ][ c ]; n += m[ 0 ]; smx += m[ 1 ]; smy += m[ 2 ]; sxx += m[ 3 ]; syy += m[ 4 ]; sxy += m[ 5 ]; }
				float ax = gx, ay = gy;	// no clear water axis: the gradient
				if( n >= RIVER_AXIS_MIN_PX ){
					const double mx = smx / n, my = smy / n;
					const double cxx = sxx / n - mx * mx, cyy = syy / n - my * my, cxy = sxy / n - mx * my;
					const double root = sqrt( 0.25 * ( cxx - cyy ) * ( cxx - cyy ) + cxy * cxy );
					const double l1 = 0.5 * ( cxx + cyy ) + root, l2 = 0.5 * ( cxx + cyy ) - root;
					if( l1 >= RIVER_AXIS_ELONG * ( l2 > 1e-6 ? l2 : 1e-6 ) ){
						const double ang = 0.5 * atan2( 2.0 * cxy, cxx - cyy );
						ax = (float)cos( ang ); ay = (float)sin( ang );
						if( ax * gx + ay * gy < 0 ){ ax = -ax; ay = -ay; }	// downstream
					}
				}
				const float l = sqrtf( ax * ax + ay * ay );
				if( l > 1e-4f ){ fx[ r ][ c ] = ax / l; fy[ r ][ c ] = ay / l; }
			}
		}
		++nComp;
	}
	// one smoothing pass over the 3 x 3 cells of the same stream
	for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
		sx[ r ][ c ] = sy[ r ][ c ] = 0;
		if( fx[ r ][ c ] == 0 && fy[ r ][ c ] == 0 ) continue;
		float ax = 0, ay = 0;
		for( int dr = -1; dr <= 1; ++dr ) for( int dc = -1; dc <= 1; ++dc ){
			const int r2 = r + dr, c2 = c + dc;
			if( (uint)r2 < 112 && (uint)c2 < 112 && comp[ r2 ][ c2 ] == comp[ r ][ c ] ){ ax += fx[ r2 ][ c2 ]; ay += fy[ r2 ][ c2 ]; }
		}
		const float l = sqrtf( ax * ax + ay * ay );
		if( l > 1e-4f ){ sx[ r ][ c ] = ax / l; sy[ r ][ c ] = ay / l; }
	}
	// THE STREAM PHASE (see above): from the distance to the mouth, relaxed toward the flow direction, stream by stream
	auto flowing = [&]( int r, int c ){ return sx[ r ][ c ] != 0 || sy[ r ][ c ] != 0; };
	for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ) phi[ r ][ c ] = flowing( r, c ) ? -32.0f * dist[ r ][ c ] : 0.0f;
	for( int pass = 0; pass < RIVER_PHASE_PASSES; ++pass ) for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
		if( !flowing( r, c ) ) continue;
		float s = 0; int n = 0;
		for( int k = 0; k < 8; ++k ){
			const int r2 = r + DR[ k ], c2 = c + DC[ k ];
			if( (uint)r2 >= 112 || (uint)c2 >= 112 || comp[ r2 ][ c2 ] != comp[ r ][ c ] || !flowing( r2, c2 ) ) continue;
			const float mx = sx[ r ][ c ] + sx[ r2 ][ c2 ], my = sy[ r ][ c ] + sy[ r2 ][ c2 ], l = sqrtf( mx * mx + my * my );
			if( l < 1e-4f ) continue;
			s += phi[ r2 ][ c2 ] - ( 32.0f * ( DR[ k ] - DC[ k ] ) * mx + 16.0f * ( DR[ k ] + DC[ k ] ) * my ) / l;	// + (P_a - P_b) . mean direction
			++n;
		}
		if( n ) phi[ r ][ c ] += RIVER_PHASE_SOR * ( s / n - phi[ r ][ c ] );
	}
	for( int r = 0; r < 112; ++r ) for( int c = 0; c < 112; ++c ){
		const bool on = flowing( r, c );
		const float turn = on ? 6.2831853f * phi[ r ][ c ] / RIVER_WAVE_PERIOD : 0.0f;
		const uint R = (uint)( 128 + (int)lroundf( sx[ r ][ c ] * 127.0f ) ), G = (uint)( 128 + (int)lroundf( sy[ r ][ c ] * 127.0f ) );
		const uint B = on ? (uint)( 128 + (int)lroundf( cosf( turn ) * 127.0f ) ) : 128u, A = on ? (uint)( 128 + (int)lroundf( sinf( turn ) * 127.0f ) ) : 128u;
		RiverFlowTex[ c * 112 + r ] = A << 24 | R << 16 | G << 8 | B;
	}
	RiverFlowValid = true;
	if( !++RiverFlowGen ) RiverFlowGen = 1;	// 0 = "never uploaded" in dx_utility.cpp
}

// (th4) RUNNING RIVER: the flow map (112 x 112 A8R8G8B8 texels, see BuildRiverFlow) and THE MAP ANCHOR (the centre
// of map cell (0,0), screen-relative PRE-zoom px) for dx_utility.cpp River_Present; false = no running river this
// frame (not the town, the river mask is off -- WaterShimmer off, Ice Age, Classic -- or nothing drawn yet).
// wavePeriod = RIVER_WAVE_PERIOD (world px per turn of the stored stream phase), for the ripple's time phases.
bool TownWater_RiverFlow( const uint*& texels, uint& gen, int* c00, float& wavePeriod )
{
	if( !InTown() || !MaskValid || !RiverFlowValid || !MapC00Set ) return false;
	texels = RiverFlowTex; gen = RiverFlowGen;
	c00[ 0 ] = MapC00X; c00[ 1 ] = MapC00Y;
	wavePeriod = RIVER_WAVE_PERIOD;
	return true;
}

// (th4) RUNNING RIVER: this frame's river reflection pixels (screen-relative post-zoom, y << 16 | x) for
// dx_utility.cpp HeatHaze_BuildMask, which gives their mask texels their own code (they ripple but do not slide).
const int* TownWater_ReflectionPixels( int& n )
{
	n = (int)RiverReflPix.size();
	return n ? RiverReflPix.data() : 0;
}

// (th4) LoadTownMap (TownMap.cpp) hands over Town.TIL while it is still loaded.
// Calibrate: the editor's numbers may be 0- or 1-based for tiles and for squares. Try the four
// readings and keep the one under which EVERY kRiverTiles row matches Town.TIL (a row matches when
// the smallest of its four squares equals the editor's top value). FineMap holds square + 1
// (SetTileSquare, TownMap.cpp ~344), and that is what DrawSceneTownTop hands to TownWater_NoteCell.
static void JobLines_Join();	// (th4) THE JOB LINES (below RecordGround)
static void GroundRec_Join();
void TownWater_OnTownLoaded( const TileSquare* tiles, int tileCount )
{
	JobLines_Join();	// (th4) THE JOB LINES: the workers read LeafIdx and WaterIdx, refilled below
	memset( RiverPiece, 0, sizeof( RiverPiece ) );
	memset( TreePiece, 0, sizeof( TreePiece ) );
	memset( RockPiece, 0, sizeof( RockPiece ) );	// (th4) ROCK SNOW
	memset( HutLeafPiece, 0, sizeof( HutLeafPiece ) );
	memset( HaloBoostPiece, 0, sizeof( HaloBoostPiece ) );
	memset( WellQuad, 0, sizeof( WellQuad ) );
	memset( SnowAllPiece, 0, sizeof( SnowAllPiece ) );
	memset( SnowNonePiece, 0, sizeof( SnowNonePiece ) );
	memset( SnowRockFloorPiece, 0, sizeof( SnowRockFloorPiece ) );	// (th4) ROCK FLOORS
	memset( SnowLineSq, 0, sizeof( SnowLineSq ) );
	memset( HutSidePiece, 0, sizeof( HutSidePiece ) ); HutAnchorPiece = 0;
	memset( HouseEdgeSq, 0, sizeof( HouseEdgeSq ) );
	memset( RoofSq, 0, sizeof( RoofSq ) ); memset( RoofNoSnowSq, 0, sizeof( RoofNoSnowSq ) );	// (th4) THATCH ROOFS
	memset( CataSq, 0, sizeof( CataSq ) );	// (th4) THE CATACOMBS BUILDING
	memset( PuddleFirst, 0xFF, sizeof( PuddleFirst ) ); memset( PuddleCnt, 0, sizeof( PuddleCnt ) ); PuddlePool.clear();	// (th4) PUDDLES: worked out again per cell
	AnyRiverPiece = false;
	Learned = false;
	memset( InRiverCount, 0, sizeof( InRiverCount ) ); memset( TotalCount, 0, sizeof( TotalCount ) );
	LearnFrames = LearnPixels = 0;
	memset( WaterIdx, 0, sizeof( WaterIdx ) );
	for( uchar e : kWaterEntries ) WaterIdx[ e ] = true;
	RiverArtIdx.clear(); RiverArt.clear(); RiverArtFull.clear();	// (th4) RIVER ART: decoded again (another town art may sit at the same address)
	memset( LeafIdx, 0, sizeof( LeafIdx ) );
	for( uchar e : kLeafEntries ) LeafIdx[ e ] = true;
	memset( LeafBrightIdx, 0, sizeof( LeafBrightIdx ) );
	for( uchar e : kLeafBright ) LeafBrightIdx[ e ] = true;
	TerrainDarkReady = false;	// FilePalette is the town's by the first TownWater_Render (Main.cpp picks it before CreateLevel)
	if( !tiles || tileCount <= 0 ) return;
	const int rowsN = (int)( sizeof( kRiverTiles ) / sizeof( kRiverTiles[ 0 ] ) );
	auto minSquare = []( const TileSquare& t ){ return min( min( (int)t.up, (int)t.left ), min( (int)t.right, (int)t.down ) ); };
	int bestBase = -1, bestOff = 0, bestHits = -1;
	for( int base = 1; base >= 0; --base ){			// tile number - base = index into Town.TIL
		for( int off = 0; off <= 1; ++off ){		// file square = editor square - off
			int hits = 0;
			for( int i = 0; i < rowsN; ++i ){
				const int idx = kRiverTiles[ i ].tile - base;
				if( idx >= 0 && idx < tileCount && minSquare( tiles[ idx ] ) == kRiverTiles[ i ].top - off ) ++hits;
			}
			if( hits > bestHits ){ bestHits = hits; bestBase = base; bestOff = off; }
		}
	}
	Log( "Town.TIL: %d tiles, calibration tile-%d square-%d matches %d of %d rows\n", tileCount, bestBase, bestOff, bestHits, rowsN );
	for( int i = 0; i < rowsN; ++i ){
		const int idx = kRiverTiles[ i ].tile - bestBase;
		if( idx < 0 || idx >= tileCount || minSquare( tiles[ idx ] ) != kRiverTiles[ i ].top - bestOff ){
			Log( "  tile %d: NO MATCH, skipped\n", kRiverTiles[ i ].tile );
			continue;
		}
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){
			const int piece = sq[ k ] + 1;
			if( piece > 0 && piece < MAX_PIECE ){ RiverPiece[ piece ] = true; AnyRiverPiece = true; }
		}
	}
	// tree tiles: same calibration (bestBase), no square check needed
	for( int tile : kTreeTiles ){
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE ) TreePiece[ piece ] = true; }
	}
	TreePieceReady = true;
	for( int tile : kRockTiles ){	// (th4) ROCK SNOW: same calibration
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE ) RockPiece[ piece ] = 1; }
	}
	for( int tile : kFloorRockTiles ){	// (th4) ROCK SNOW: floor layer only; never turns a whole-column piece into a floor-only one
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE && !RockPiece[ piece ] ) RockPiece[ piece ] = 2; }
	}
	for( int tile : kHutLeafTiles ){
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE ) HutLeafPiece[ piece ] = true; }
	}
	for( int tile : kHaloBoostTiles ){
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE ) HaloBoostPiece[ piece ] = true; }
	}
	for( int tile : kWellTiles ){	// (th4) THE WELL (SNOW GROUND)
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE ) WellQuad[ piece ] = (uchar)( 1 + k ); }
	}
	for( const auto& fs : kSnowFloorSquares ){	// (th4) SNOW FLOORS (SNOW GROUND)
		const int idx = fs[ 0 ] - bestBase;
		if( idx < 0 || idx >= tileCount || fs[ 1 ] < 0 || fs[ 1 ] > 3 ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		const int piece = sq[ fs[ 1 ] ] + 1;
		if( piece > 0 && piece < MAX_PIECE ) SnowAllPiece[ piece ] = true;
	}
	for( int l = 0; l < SNOW_LINE_N; ++l ){	// (th4) SNOW LINES (SNOW GROUND): the anchor tiles' pieces, placed in SnowVotesEnsure
		const int idx = kSnowLines[ l ].tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		SnowLineSq[ l ][ 0 ] = t.up + 1; SnowLineSq[ l ][ 1 ] = t.left + 1; SnowLineSq[ l ][ 2 ] = t.right + 1; SnowLineSq[ l ][ 3 ] = t.down + 1;
	}
	for( const auto& fs : kBareFloorSquares ){	// (th4) BARE FLOORS (SNOW GROUND)
		const int idx = fs[ 0 ] - bestBase;
		if( idx < 0 || idx >= tileCount || fs[ 1 ] < 0 || fs[ 1 ] > 3 ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		const int piece = sq[ fs[ 1 ] ] + 1;
		if( piece > 0 && piece < MAX_PIECE ) SnowNonePiece[ piece ] = true;
	}
	for( const auto& fs : kRockFloorSquares ){	// (th4) ROCK FLOORS (SNOW GROUND)
		const int idx = fs[ 0 ] - bestBase;
		if( idx < 0 || idx >= tileCount || fs[ 1 ] < 0 || fs[ 1 ] > 3 ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		const int piece = sq[ fs[ 1 ] ] + 1;
		if( piece > 0 && piece < MAX_PIECE ) SnowRockFloorPiece[ piece ] = true;
	}
	for( int piece : kRockFloorPieces ) if( piece > 0 && piece < MAX_PIECE ) SnowRockFloorPiece[ piece ] = true;	// (th4) ROCK FLOORS: the crypt's, by piece
	for( int i = 0; i < HOUSE_EDGE_N; ++i ){	// (th4) HOUSE EDGES (SNOW GROUND): the tiles' pieces, placed in SnowVotesEnsure
		const int idx = kHouseEdgeTiles[ i ] - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		HouseEdgeSq[ i ][ 0 ] = t.up + 1; HouseEdgeSq[ i ][ 1 ] = t.left + 1; HouseEdgeSq[ i ][ 2 ] = t.right + 1; HouseEdgeSq[ i ][ 3 ] = t.down + 1;
	}
	for( const auto& hs : kHutSideSquares ){	// (th4) ADRIA'S SIDE (SNOW GROUND)
		const int idx = hs[ 0 ] - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){
			if( hs[ 1 ] >= 0 && hs[ 1 ] != k ) continue;
			const int piece = sq[ k ] + 1;
			if( piece > 0 && piece < MAX_PIECE ) HutSidePiece[ piece ] = true;
		}
		if( hs[ 0 ] == HUT_SIDE_ANCHOR_TILE ) HutAnchorPiece = t.up + 1;
	}
	for( int tile : kHutSnowTiles ){	// (th4) ROCK SNOW on ADRIA'S HUT: same calibration; never over a rock's value
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		const int sq[ 4 ] = { t.up, t.left, t.right, t.down };
		for( int k = 0; k < 4; ++k ){ const int piece = sq[ k ] + 1; if( piece > 0 && piece < MAX_PIECE && !RockPiece[ piece ] ) RockPiece[ piece ] = 3; }
	}
	for( int piece = 1; piece < MAX_PIECE; ++piece ) if( HutSidePiece[ piece ] && !RockPiece[ piece ] ) RockPiece[ piece ] = 3;	// ... and ADRIA'S SIDE's floors
	for( int i = 0; i < ROOF_TILE_N + ROOF_NOSNOW_N; ++i ){	// (th4) THATCH ROOFS: the tiles' pieces, placed in RoofSnow_BuildAll
		const int tile = i < ROOF_TILE_N ? kRoofTiles[ i ] : kRoofNoSnow[ i - ROOF_TILE_N ].tile;
		const int idx = tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		int* q = i < ROOF_TILE_N ? RoofSq[ i ] : RoofNoSnowSq[ i - ROOF_TILE_N ];
		q[ 0 ] = t.up + 1; q[ 1 ] = t.left + 1; q[ 2 ] = t.right + 1; q[ 3 ] = t.down + 1;
	}
	for( int i = 0; i < CATA_PART_N; ++i ){	// (th4) THE CATACOMBS BUILDING: same, placed in CataSnow_BuildAll
		const int idx = kCataParts[ i ].tile - bestBase;
		if( idx < 0 || idx >= tileCount ) continue;
		const TileSquare& t = tiles[ idx ];
		CataSq[ i ][ 0 ] = t.up + 1; CataSq[ i ][ 1 ] = t.left + 1; CataSq[ i ][ 2 ] = t.right + 1; CataSq[ i ][ 3 ] = t.down + 1;
	}
	RiverFlowDirty = true; RiverFlowValid = false;	// (th4) RUNNING RIVER: the flow map is worked out again at the first town frame (BuildRiverFlow)
}

// (th4) GLOWING LEAVES (GLOW nights, Fireflies.cpp GlowNight_Active): while a TREE cell's tile layers
// are drawn (TownMap.cpp DrawSceneTownTop calls TownWater_CellBegin / CellEnd around them), every
// pixel they change to a leaf entry (kLeafEntries) is recorded, pre-zoom. After the frame is
// finished and zoomed (TownWater_Render), each recorded leaf that is still showing -- nothing drawn
// later covered it -- is handed to the BioGlow shader as a LEAF pixel (BioGlow_SetWater codes
// 128..255; water uses 1..127) with its own brightness, and feeds the halo. BioGlow.hlsl colours
// leaves pink-violet in true colour (LEAF_DEEP..LEAF_BRIGHT, LEAF_MIX). Costs one before/after
// compare of each tree cell's column (64 x 288 px), on glow nights only.
namespace {
int LeafX0, LeafY0, LeafX1, LeafY1, LeafFloorY = 0; bool LeafOn = false, LeafHut = false, LeafHalo = false;
const uchar* LeafWas = 0;	// this cell's column BEFORE it was drawn (TownWater_CellBegin), w x rows, in the TREE SCAN pool
// recorded leaves: a plain growing buffer, not vectors (tens of thousands of push_backs a frame showed
// up in the profiler, the Debug build's iterator checks most of all)
struct LeafPx {
	int rel;		// screen-relative pre-zoom (y << 16 | x)
	uchar val;		// its palette entry
	uchar fromHut;	// bit 0: from a house tile (neighbourhood rule applies), bit 1: halo boost
	uchar boost;	// SPARSE CROWN boost of this leaf's cell, x10 (10 = none)
};
LeafPx* Leaves = 0; size_t LeafN = 0, LeafCap = 0;
// (th4) SPARSE CROWNS: trees with few leaves (the thin autumn crowns) got little halo and read as a
// few dim dots next to the dense trees. A cell's leaf count is known when its layers are drawn
// (TownWater_CellEnd); below LEAF_DENSE_REF leaves its leaves get a boost of LEAF_DENSE_REF / count
// (at most LEAF_BOOST_MAX): that much more halo per leaf, and a brighter place on the pink ramp.
// Measured on a one-off frame dump: a dense crown column holds 400..700+ leaf pixels, a thin one 50..150.
// CROWNS ONLY: pixels of the cell's floor layer (its bottom 32 rows -- grass, the shadow at the
// trunk's foot, the ground tiles' fallen leaves) never get the boost; boosted, they turned into
// bright pink dots on the ground under every tree.
const int LEAF_DENSE_REF = 300;
const int LEAF_BOOST_MAX = 3;
// (th4) TREE SWAY (config TreeSway). WHEN the wind blows and how hard: dx_utility.cpp Wind_Strength.
// The MOTION is drawn on the GPU (TreeSway.hlsl, run by dx_utility.cpp TreeSway_Present), smoothly,
// moving ONLY tree pixels: this file tells it which pixels are which.
//   RECORD (TownWater_CellEnd, while TreeSway_Active or SunRays_Active -- dx_utility.cpp SUN RAYS reads
//   layer 1 too, for where the crowns are): for each pure TREE cell (not the house tiles,
//   kHutLeafTiles -- their walls must not bend) every pixel it drew above its floor layer: its
//   colour, the colour UNDER it before the tree was drawn (LeafWas, the "was" snapshot -- what shows
//   once the crown moves off it) and its height; plus an AFTER-TREE snapshot of the box around
//   those pixels, widened by SWAY_MARGIN (not the whole 64 x 288 column: most of it is empty).
//   LAYERS (BuildSwayLayers, TownWater_Render; PRE-ZOOM resolution, one texel per world pixel --
//   a tree pixel is a z x z block after the zoom anyway, and 4x fewer texels at 2x zoom):
//     L1 per pixel = A: tree number + 1 (drawing order; 0 = not a tree pixel), G: the tree pixel's
//        palette index, B: the index under it, R: the bend weight -- growing linearly with the
//        height above the cell's ground point (none up to SWAY_FOOT_H, full from SWAY_FULL_H).
//        (Pixels just outside a crown find its weight in the SHADER, which looks a few pixels
//        upwind; widening the weight here on the CPU cost ~10 ms a frame on a 1056 x 594 screen.)
//     L2 per pixel = K: the first tree (drawing order) after which this pixel never changed again.
//        A tree pixel of tree o may be drawn here only if o >= K: something drawn after tree o (a
//        hero, a house, a later tree) is in front of it. 255 = no tree may land here.
//   The shader then only ever writes tree colours, where draw order allows, and fills the spot a
//   crown moved off with the colour that was under it. Nothing else moves.
// (Two rejected versions, so they are not tried again: a screen-space warp of a widened region round
// each crown moved the ground and the hero with it; moving the tree in the 8-bit frame on the CPU
// can only step whole pixels, and the leaves jittered.)
// PERFORMANCE: everything here runs only while the wind blows (and the leaf part only while leaves
// glow), on small boxes, at pre-zoom resolution. Measured in a Debug build on a 1056 x 594 town
// full of trees: recording + layers + upload ~2.5 ms a frame. Two things that cost far more and must
// not come back: widening the weight on the CPU over the whole screen (~10 ms -- the shader looks
// upwind instead) and per-pixel vector push_backs (slow, worst in Debug -- plain buffers instead).
// KNOWN-GOOD CHECK: TreeSway 10 under Ice Age (always windy): crowns sway smoothly to the right,
// trunk feet stay put, the ground and the hero next to a tree never move, a tree behind a house slides
// under the roof, the white crowns stay white.
int LeafBaseY = 0; bool LeafSway = false;
// (th4) SUN RAYS: trees that never let light through (Mordor's picks, by eye in game: bare trees whose few
// branches made short plank-shaped strips, and a branch hanging in the air). Normal town, map SQUARES --
// the top-left cell (row, col) of a tile's 2 x 2 cells, as TownMap.cpp LoadTownSector lays them out.
// By square, not by tile number: the same tiles (44, 47, 56) stand elsewhere in town with good rays.
// To find a tree's square: the cows stand at TownMap.cpp CowsRows / CowsCols, and a cell (row, col) is at
// screen offset 32 * (row - col), 16 * (row + col) from another (1x) -- match the screenshot to that.
// The tree still sways; only its crown stops being a source (TownWater_CellEnd -> SwayCell.noSun ->
// BuildSunGround marks its pixels SUN_GROUND_NO_SOURCE -> SunRays.hlsl crownAt).
struct NoSunSquare { int row, col; };
const NoSunSquare kNoSunSquares[] = {
	{ 78, 54 },		// tile 47: bare tree by the river, left of the cows
	{ 86, 54 },		// tile 44: bare tree south-east of the cows
	{ 86, 60 },		// tile 56: the branch hanging in the air south-west of the cows
};
bool LeafNoSun = false;							// this cell's crown is in kNoSunSquares
const uchar SUN_GROUND_YES = 255, SUN_GROUND_WATER = 128, SUN_GROUND_NO_SOURCE = 64;	// SUN GROUND layer codes (SunRays.hlsl s3)
struct SwayCell {
	int ex0, ey0, ex1, ey1;		// after-tree snapshot box (its pixels' box + SWAY_MARGIN), WorkingSurface coords (pre-zoom), x1/y1 exclusive
	size_t afterOfs;			// its rows in SwayAfter
	size_t first, count;		// its pixels in SwayPix
	bool noSun;					// SUN RAYS: this crown lets no light through (kNoSunSquares)
};
std::vector<SwayCell> SwayCells;
std::vector<uchar> SwayAfter;		// the cells' after-tree snapshots, one after another
// the tree pixels of this frame: a plain growing buffer, not vectors -- tens of thousands of
// push_backs a frame were a real cost (especially in a Debug build)
struct SwayPx { int rel; uchar val, bg; short hgt; };	// screen-relative pre-zoom (y << 16 | x), its colour, the colour under it, height above the ground point
SwayPx* SwayPix = 0; size_t SwayPixN = 0, SwayPixCap = 0;
const int SWAY_FOOT_H  = 32;		// pre-zoom px above the ground point where the bend starts
const int SWAY_FULL_H  = 200;		// ... and where it is full
const int SWAY_MARGIN  = 4;		// pre-zoom px the snapshot box reaches past the tree's pixels: must cover how far
									// TreeSway.hlsl looks upwind (4 world px; TreeSway 10 shifts about 3.1)
// the layers, pre-zoom resolution, kept correct everywhere (each frame clears what the last one wrote)
uint* SwayL1 = 0; uchar* SwayL2 = 0; int SwayLW = 0, SwayLH = 0;
int SwayBox[4] = { 0, 0, 0, 0 };	// x0, y0, x1, y1 of what this frame wrote (pre-zoom px)
bool SwayLayersValid = false;
// (th4) PERFORMANCE: does this frame need layer 2 (K) at all? Only the swaying trees read it (TreeSway.hlsl);
// the SUN RAYS read layer 1 alone. On a sunny day WITHOUT wind the after-tree snapshots (SwayAfter) and the
// K pass are skipped, and TownWater_SwayLayers hands out no layer 2 -- so TreeSway_Present stays off for that
// frame. Decided once per frame in TownWater_BeginFrame, so the recording and the layers always agree.
bool SwayNeedK = false;
}

// =================================================================================================================
// (th4) THE JOB LINES (Mordor, 2026-10-11: work off the main thread, never a pixel different). Two per-cell recordings
// of the tile loop only READ the frame as it is right after their cell was drawn, and write their own outputs, which
// nothing reads before TownWater_Render:
//   TREE SCAN (TownWater_CellEnd -> TreeScan_Run): the TREE SWAY / GLOWING LEAVES records of each tree cell, from its
//     column before the cell (LeafWas, TownWater_CellBegin) and after it (+ SWAY_MARGIN, for the after-tree snapshot);
//   GROUND RECORDING (TownWater_FloorDrawn -> RecordGround -> GroundRec_Run): the SUN GROUND / GRASS / SNOW GROUND
//     layers of each floor cell, from its 64 x 32 floor box right after the floor was drawn.
// So each cell becomes a JOB: the pixels it reads are copied into THE POOL (fixed blocks, never moved within a frame),
// and the line's ONE worker thread runs its jobs IN POSTED ORDER -- the same code on the same pixels, so the same
// records in the same order as inline (the ground layers' overlapping strips are written in draw order too). The main
// thread draws on meanwhile, and waits for both lines (JobLines_Join) at the start of TownWater_Render and of
// TownWater_BeginFrame (before the outputs are reset), in TownWater_OnTownLoaded, and before anything grows what a
// ground job reads -- the art caches and the votes (SnowArtSlot, GrassArtIndex, SnowCellVotes call GroundRec_Join).
// What a cell decides (its piece, votes, strips, art slots) stays on the main thread: it reads and grows those caches,
// and TownWater_FeetBare reads the votes during the draw. SnowOnTree stays on the main thread too: it paints into the
// frame BEFORE the cells in front are drawn.
// Measured among the Ice Age town's trees (2026-10-11): the scan ~1.3..2 ms and the recording ~1.6 ms a frame left the
// main thread, which now only copies (~0.3 + ~0.7 ms); both workers finish before TownWater_Render needs them.
// JOB_LINES false = everything inline, on the live frame (identical output; also the fallback if a thread cannot start).
// TH4_JOBLINES_VERIFY 1 = on every 8th frame both: inline into SHADOW outputs and the worker into the real ones,
// compared at TownWater_Render; the counts go to TH4_joblines.log next to the exe (JobLines_VerifyLog), and every
// "mismatched" must stay 0. Turn it on after changing what a scan reads: the copy (TownWater_CellEnd, RecordGround) must
// cover all of it, and this proves it does. It proved this version on 2531 frames of the Ice Age town (2026-10-11).
#define TH4_JOBLINES_VERIFY 0
namespace {
const bool JOB_LINES = true;
// THE POOL: fixed blocks, never moved within a frame (a worker reads them while the main thread takes more); emptied
// by JobLines_FrameStart, after both lines are done
const size_t JOB_POOL_BLOCK = (size_t)8 << 20;
std::vector<uchar*> JobPoolBlocks; std::vector<size_t> JobPoolSizes;
size_t JobPoolCur = 0, JobPoolOfs = 0;
#if TH4_JOBLINES_VERIFY
bool VerifyThis = false; unsigned VerifyTick = 0;	// every 8th frame is checked, so the log's fps stay close to the real ones
int TreeJobsNow = 0, GroundJobsNow = 0;			// cells of this frame that were checked
#endif
// ONE LINE: a worker thread that runs Run( job ) for every posted job, in order. Made on first use and never
// destroyed (TreeLine, GroundLine): a worker asleep at exit touches nothing, so there is no destruction order to keep.
template< class Job, void (*Run)( const Job& ) > struct JobLine {
	static const int MAX_JOBS = 4096;		// a frame has ~200 tree and ~1100 floor cells
	Job jobs[ MAX_JOBS ];
	std::atomic<int> posted{ 0 }, done{ 0 };
	std::atomic<bool> sleeping{ false };
	std::mutex mtx; std::condition_variable cv;
	std::thread worker;
	bool on = false, tried = false;		// main thread only
	void Loop()
	{
		for( ;; ){
			const int d = done.load();
			if( d < posted.load() ){
				Run( jobs[ d ] );
				done.store( d + 1 );
				continue;
			}
			// asleep until a job is posted: Post wakes it when it saw `sleeping`; the flag is set before the wait tests
			// its condition, so a job posted in between is never missed
			std::unique_lock<std::mutex> lk( mtx );
			sleeping.store( true );
			cv.wait( lk, [ this ]{ return done.load() < posted.load(); } );
			sleeping.store( false );
		}
	}
	void Join(){ while( done.load() < posted.load() ) std::this_thread::yield(); }	// every posted job done
	void Reset(){ Join(); posted.store( 0 ); done.store( 0 ); }
	void Inline( const Job& j ){ Join(); Run( j ); }	// right here, after every job posted before it (the order holds)
	void Post( const Job& j )
	{
		if( !on ){	// the worker starts on first use; without it, the job runs inline
			if( !tried ){ tried = true; try{ worker = std::thread( [ this ]{ Loop(); } ); on = true; }catch( ... ){} }
			if( !on ){ Inline( j ); return; }
		}
		const int n = posted.load();
		if( n >= MAX_JOBS ){ Inline( j ); return; }	// full (never seen)
		jobs[ n ] = j;
		posted.store( n + 1 );
		if( sleeping.load() ){ std::lock_guard<std::mutex> lk( mtx ); cv.notify_one(); }
	}
};
// TREE SCAN (above)
struct TreeJob {
	int x0, x1, y0, y1, floorY, baseY;			// the column (LeafX0 ..), its floor's first row, its ground point
	bool sway, leafRec, hut, halo, noSun, needK;	// LeafSway, the leaf recording, LeafHut, LeafHalo, LeafNoSun, SwayNeedK
	const uchar* was;							// the column before the cell, w x rows
	const uchar* now; int nx0, ny0, nstride;	// the frame after the cell: pixel ( x, y ) = now[ ( y - ny0 ) * nstride + x - nx0 ]
	int scrL, scrT, scrW, scrH;					// Screen_LeftBorder, Screen_TopBorder, ScreenWidth, ScreenHeight
};
struct TreeOut { SwayPx*& pix; size_t& pixN; size_t& pixCap; LeafPx*& leaves; size_t& leafN; size_t& leafCap;
                 std::vector<SwayCell>& cells; std::vector<uchar>& after; };
#if TH4_JOBLINES_VERIFY
SwayPx* VSwayPix = 0; size_t VSwayPixN = 0, VSwayPixCap = 0;	// the SHADOW outputs of the inline scan
LeafPx* VLeaves = 0; size_t VLeafN = 0, VLeafCap = 0;
std::vector<SwayCell> VSwayCells; std::vector<uchar> VSwayAfter;
#endif
}
#if TH4_JOBLINES_VERIFY
namespace { long long TreeVerifyFrames = 0, TreeVerifyBad = 0, GroundVerifyFrames = 0, GroundVerifyBad = 0; }
#endif
static uchar* JobPool_Alloc( size_t n )
{
	while( true ){
		if( JobPoolCur < JobPoolBlocks.size() && JobPoolOfs + n <= JobPoolSizes[ JobPoolCur ] ){
			uchar* p = JobPoolBlocks[ JobPoolCur ] + JobPoolOfs; JobPoolOfs += n; return p;
		}
		if( JobPoolCur + 1 < JobPoolBlocks.size() && n <= JobPoolSizes[ JobPoolCur + 1 ] ){ ++JobPoolCur; JobPoolOfs = 0; continue; }
		const size_t sz = n > JOB_POOL_BLOCK ? n : JOB_POOL_BLOCK;	// a new block after the last one
		uchar* b = (uchar*)malloc( sz );
		if( !b ) return 0;
		JobPoolBlocks.push_back( b ); JobPoolSizes.push_back( sz );
		JobPoolCur = JobPoolBlocks.size() - 1; JobPoolOfs = 0;
	}
}
// THE SCAN of one tree cell (the old TownWater_CellEnd body, reading through the job): records the cell's tree pixels
// (TREE SWAY) and leaf pixels (GLOWING LEAVES) into o, plus its SwayCell and after-tree snapshot
static void TreeScan_Run( const TreeJob& j, TreeOut& o )
{
	const bool leafRec = j.leafRec;
	if( !j.sway && !leafRec ) return;
	const int w = j.x1 - j.x0, rows = j.y1 - j.y0;
	const size_t colPixels = (size_t)w * rows;		// at most every pixel of the column
	// TREE SWAY buffer
	SwayCell c;
	c.first = o.pixN;
	c.noSun = j.noSun;
	bool sway = j.sway;
	if( sway && o.pixN + colPixels > o.pixCap ){
		const size_t cap = max( o.pixN + colPixels, max( (size_t)65536, o.pixCap * 2 ) );
		SwayPx* p = (SwayPx*)realloc( o.pix, cap * sizeof( SwayPx ) );
		if( p ){ o.pix = p; o.pixCap = cap; } else sway = false;
	}
	SwayPx* const pix = o.pix;
	size_t pixN = o.pixN;
	auto rowAt = [ &j ]( int yy ){ return j.now + (ptrdiff_t)( yy - j.ny0 ) * j.nstride + ( j.x0 - j.nx0 ); };	// the column's row yy
	int px0 = INT_MAX, py0 = INT_MAX, px1 = INT_MIN, py1 = INT_MIN;	// box of this tree's pixels
	// GLOWING LEAVES buffer
	bool leaves = leafRec;
	const size_t firstLeaf = o.leafN;
	if( leaves && o.leafN + colPixels > o.leafCap ){
		const size_t cap = max( o.leafN + colPixels, max( (size_t)65536, o.leafCap * 2 ) );
		LeafPx* p = (LeafPx*)realloc( o.leaves, cap * sizeof( LeafPx ) );
		if( p ){ o.leaves = p; o.leafCap = cap; } else leaves = false;
	}
	LeafPx* const lv = o.leaves;
	size_t leafN = o.leafN;
	// WINDOW LIGHT is only possible within WINDOW_LIGHT_R rows of a row that holds the window-light red
	// at all -- almost no column has one, so the per-pixel search below is skipped almost always
	bool anyWindow = false;
	std::array<bool, 32 * 9 + 1> rowWin{};	// the column is at most 32 * 8 + 1 rows (TownWater_CellBegin)
	bool* const rowWinP = rowWin.data();
	const int rowWinN = (int)rowWin.size();
	if( leaves ){
		for( int yy = j.y0; yy < j.y1 && yy - j.y0 < rowWinN; ++yy ){
			if( memchr( rowAt( yy ), WINDOW_LIGHT_ENTRY, w ) ){ rowWinP[ yy - j.y0 ] = true; anyWindow = true; }
		}
	}
	const bool rowsKnown = rows <= rowWinN;
	const uchar fromHut = ( j.hut ? 1 : 0 ) | ( j.halo ? 2 : 0 );	// bit 0 house checks, bit 1 halo boost
	int crown = 0;	// this cell's leaves outside the floor layer (SPARSE CROWNS)
	for( int yy = j.y0; yy < j.y1; ++yy ){
		const int hgt = j.baseY - yy;
		const bool swayRow = sway && yy < j.floorY && hgt > SWAY_FOOT_H;
		if( !swayRow && !leaves ) continue;
		const uchar* now = rowAt( yy );
		const uchar* was = j.was + (size_t)( yy - j.y0 ) * w;
		bool winNear = false;
		if( leaves ){
			winNear = anyWindow;
			if( anyWindow && rowsKnown ){
				winNear = false;
				for( int r = max( 0, yy - j.y0 - WINDOW_LIGHT_R ); r <= min( rows - 1, yy - j.y0 + WINDOW_LIGHT_R ); ++r ) if( rowWinP[ r ] ){ winNear = true; break; }
			}
		}
		const int relY = ( yy - j.scrT ) << 16;
		int xx = 0;
		while( xx < w ){
			if( xx + 16 <= w ){
				const __m128i a = _mm_loadu_si128( (const __m128i*)( now + xx ) ), b = _mm_loadu_si128( (const __m128i*)( was + xx ) );
				if( _mm_movemask_epi8( _mm_cmpeq_epi8( a, b ) ) == 0xFFFF ){ xx += 16; continue; }	// 16 unchanged pixels
			}
			const int xe = xx + 16 < w ? xx + 16 : w;
			for( ; xx < xe; ++xx ){
				const uchar v = now[ xx ];
				if( v == was[ xx ] ) continue;
				if( swayRow ){	// TREE SWAY (above)
					SwayPx& q = pix[ pixN++ ];
					q.rel = relY | ( j.x0 + xx - j.scrL );
					q.val = v; q.bg = was[ xx ]; q.hgt = (short)hgt;
					if( j.x0 + xx < px0 ) px0 = j.x0 + xx;
					if( j.x0 + xx + 1 > px1 ) px1 = j.x0 + xx + 1;
					if( yy < py0 ) py0 = yy;
					py1 = yy + 1;
				}
				if( leaves && LeafIdx[ v ] ){	// GLOWING LEAVES
					// WINDOW LIGHT: a window-light red nearby means this pixel is light on the grass, not a leaf
					bool windowLight = false;
					if( winNear ){
						for( int dy = -WINDOW_LIGHT_R; dy <= WINDOW_LIGHT_R && !windowLight; ++dy ){
							const int y2 = yy + dy;
							if( y2 < j.y0 || y2 >= j.y1 ) continue;
							const uchar* r2 = rowAt( y2 );
							for( int dx = -WINDOW_LIGHT_R; dx <= WINDOW_LIGHT_R; ++dx ){
								const int x2 = xx + dx;
								if( x2 >= 0 && x2 < w && r2[ x2 ] == WINDOW_LIGHT_ENTRY ){ windowLight = true; break; }
							}
						}
					}
					if( !windowLight ){
						LeafPx& q = lv[ leafN++ ];
						q.rel = relY | ( j.x0 + xx - j.scrL );
						q.val = v;
						q.fromHut = fromHut;
						q.boost = yy >= j.floorY ? 10 : 0;	// floor layer: never boosted; crown: set below
						crown += yy < j.floorY;
					}
				}
			}
		}
	}
	o.pixN = pixN; o.leafN = leafN;
	if( sway ){	// TREE SWAY: the after-tree snapshot of the box around the pixels
		c.count = o.pixN - c.first;
		if( c.count ){
			c.ex0 = max( j.scrL, px0 - SWAY_MARGIN ); c.ex1 = min( j.scrL + j.scrW, px1 + SWAY_MARGIN );
			c.ey0 = max( j.scrT, py0 - SWAY_MARGIN ); c.ey1 = min( j.scrT + j.scrH, py1 + SWAY_MARGIN );
		}
		if( c.count && c.ex1 > c.ex0 && c.ey1 > c.ey0 ){
			c.afterOfs = o.after.size();
			if( j.needK ){	// only layer 2 (K) reads the snapshot (SwayNeedK above)
				const int ew = c.ex1 - c.ex0;
				o.after.resize( c.afterOfs + (size_t)ew * ( c.ey1 - c.ey0 ) );
				uchar* const aft = o.after.data() + c.afterOfs;
				for( int y = c.ey0; y < c.ey1; ++y ) memcpy( aft + (size_t)( y - c.ey0 ) * ew, j.now + (ptrdiff_t)( y - j.ny0 ) * j.nstride + ( c.ex0 - j.nx0 ), ew );
			}
			o.cells.push_back( c );
		}else{
			o.pixN = c.first;
		}
	}
	if( leaves ){	// SPARSE CROWNS: boost for this cell's crown leaves (counted without the floor layer)
		int boost10 = 10;
		if( crown > 0 && crown < LEAF_DENSE_REF ) boost10 = min( LEAF_BOOST_MAX * 10, LEAF_DENSE_REF * 10 / crown );
		for( size_t k = firstLeaf; k < o.leafN; ++k ) if( !lv[ k ].boost ) lv[ k ].boost = (uchar)boost10;
	}
}
static void TreeScan_RunReal( const TreeJob& j )	// into the real outputs
{
	TreeOut out{ SwayPix, SwayPixN, SwayPixCap, Leaves, LeafN, LeafCap, SwayCells, SwayAfter };
	TreeScan_Run( j, out );
}
typedef JobLine< TreeJob, TreeScan_RunReal > TreeLineT;
static TreeLineT& TreeLine(){ static TreeLineT* line = new TreeLineT; return *line; }
#if TH4_JOBLINES_VERIFY
// the worker's records against the inline scan's (one result per checked frame with tree cells)
static void TreeScan_VerifyFrame()
{
	if( !VerifyThis || !TreeJobsNow ) return;
	bool same = SwayPixN == VSwayPixN && LeafN == VLeafN && SwayCells.size() == VSwayCells.size() && SwayAfter == VSwayAfter;
	for( size_t k = 0; same && k < SwayPixN; ++k ){ const SwayPx& a = SwayPix[ k ]; const SwayPx& b = VSwayPix[ k ]; same = a.rel == b.rel && a.val == b.val && a.bg == b.bg && a.hgt == b.hgt; }
	for( size_t k = 0; same && k < LeafN; ++k ){ const LeafPx& a = Leaves[ k ]; const LeafPx& b = VLeaves[ k ]; same = a.rel == b.rel && a.val == b.val && a.fromHut == b.fromHut && a.boost == b.boost; }
	for( size_t k = 0; same && k < SwayCells.size(); ++k ){
		const SwayCell& a = SwayCells[ k ]; const SwayCell& b = VSwayCells[ k ];
		same = a.ex0 == b.ex0 && a.ey0 == b.ey0 && a.ex1 == b.ex1 && a.ey1 == b.ey1 && a.afterOfs == b.afterOfs && a.first == b.first && a.count == b.count && a.noSun == b.noSun;
	}
	++TreeVerifyFrames; if( !same ) ++TreeVerifyBad;
}
#endif
static void RockSnow_Begin( int piece, int x, int y, int row, int col );	// (th4) ROCK SNOW (below, before BuildSnowGround)
static void RockSnow_End( int upMin = 0, int upMax = 256 );	// paints only px with up in [ upMin, upMax ) (TownWater_TileRedrawn)
static void RockSnow_BeginFrame();
static void RockSnow_ToLayer( const uchar* frame, int z );
void TownWater_CellBegin( int piece, int x, int y, int row, int col )
{
	LeafOn = false;
	LeafNoSun = false;
	if( row >= 0 && InTown() ){	// (th4) SUN RAYS: is this cell part of a square in kNoSunSquares?
		for( const NoSunSquare& q : kNoSunSquares ){
			if( row >= q.row && row <= q.row + 1 && col >= q.col && col <= q.col + 1 ){ LeafNoSun = true; break; }
		}
	}
	// (th4) ICE TREES in Ravenholm: a game loaded straight into Ravenholm has not loaded the town yet,
	// so the tree table is filled from the town's Town.TIL here, once (the same call LoadTownMap makes).
	if( !TreePieceReady && Dungeon == DUN_44_RAVENHOLM && ( IceTrees_Active() || TreeSway_Active() || Grass_Active() || RavenRiver() ) ){	// (th4) GRASS, RAVENHOLM'S RIVER: they need RiverPiece / TreePiece
		int bytes = 0;
		auto til = (TileSquare*)LoadFile( "Levels\\TownData\\Town.TIL", &bytes );
		TownWater_OnTownLoaded( til, bytes / (int)sizeof( TileSquare ) );
		FreeMem( til );
	}
	RockSnow_Begin( piece, x, y, row, col );	// (th4) ROCK SNOW: a rock cell's column (no-op for any other cell)
	if( !WorkingSurface || piece <= 0 || piece >= MAX_PIECE || !TreePiece[ piece ] || !( GlowNight_Active() || BloodNight_Active() || IceTrees_Active() || TreeSway_Active() || SunRays_Active() ) ) return;
	LeafX0 = max( (int)Screen_LeftBorder, x ); LeafX1 = min( (int)Screen_LeftBorder + ScreenWidth, x + 64 );
	LeafY0 = max( (int)Screen_TopBorder, y - 32 * 8 ); LeafY1 = min( (int)Screen_TopBorder + ScreenHeight, y + 1 );
	if( LeafX1 <= LeafX0 || LeafY1 <= LeafY0 ) return;
	LeafHut = HutLeafPiece[ piece ];
	LeafHalo = HaloBoostPiece[ piece ];
	LeafFloorY = y - 31;	// first row of the cell's floor layer (the tile's bottom 32 rows end at y)
	LeafBaseY = y;
	LeafSway = ( TreeSway_Active() || SunRays_Active() ) && !HutLeafPiece[ piece ];	// TREE SWAY (and SUN RAYS, same layer): pure tree tiles only
	const int w = LeafX1 - LeafX0;
	uchar* const snap = JobPool_Alloc( (size_t)w * ( LeafY1 - LeafY0 ) );	// THE JOB LINES: straight into the pool (the worker reads it later)
	if( !snap ) return;
	for( int yy = LeafY0; yy < LeafY1; ++yy ) memcpy( snap + (size_t)( yy - LeafY0 ) * w, WorkingSurface + YOffsetHashTable[ yy ] + LeafX0, w );
	LeafWas = snap;
	LeafOn = true;
}
// (th4) SNOW ON THE TREES (ICE TREES above, Ice Age only): when a tree cell's column has been drawn, every pixel the
// tree drew with NO pixel of the same tree directly above it -- the top of each branch and each leaf clump -- turns
// to snow (TreeSnow1); a leaf pixel one row lower gets a greyer snow (TreeSnow2), so leaf clumps hold more of it
// than bare branches. "Drew" = differs from the column's snapshot taken before the cell (LeafWas, TownWater_CellBegin),
// the same test the TREE SWAY / leaf scan below makes; the floor layer's rows are left alone (LeafFloorY). House tiles
// among the tree tiles (HutLeafPiece) get snow on their LEAVES only, never along their roofs and walls.
// It is painted into the 8-bit frame right here, before that scan, so everything downstream takes it as part of the
// tree: TREE SWAY moves the snow with the branches, the leaf frost skips it (a snow pixel is not a leaf entry, so it
// does not glow either), and it darkens at night with the ground. The two entries are the palette's own snow:
// the nearest terrain entries (1..127, the range the day / night blend darkens) to TREE_SNOW_RGB in FilePalette --
// under the Ice Age town palette that is the ground's snow itself (entry 18 = 183,194,206 in iceage\0town_P.pal).
// Checked offline on the whole town (the town render + iceage\0town_P.pal): every crown gets a white top on each
// leaf clump and branch, the trunks stay dark. A column cut by the top of the screen gets no snow on its cut row.
namespace {
const int TREE_SNOW_RGB[ 2 ][ 3 ] = { { 240, 244, 252 }, { 150, 160, 176 } };	// top of a clump / the row under it
uchar TreeSnow1 = 0, TreeSnow2 = 0;
}
static void PickTreeSnow()
{
	uchar* out[ 2 ] = { &TreeSnow1, &TreeSnow2 };
	for( int s = 0; s < 2; ++s ){
		int best = 1, bestD = 0x7FFFFFFF;
		for( int i = 1; i < 128; ++i ){
			const int dr = FilePalette[ i ].peRed - TREE_SNOW_RGB[ s ][ 0 ], dg = FilePalette[ i ].peGreen - TREE_SNOW_RGB[ s ][ 1 ], db = FilePalette[ i ].peBlue - TREE_SNOW_RGB[ s ][ 2 ];
			const int d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
			if( d < bestD ){ bestD = d; best = i; }
		}
		*out[ s ] = (uchar)best;
	}
}
// The test "did the tree draw the pixel above?" is kept as one mask row per row (TreeMask, three rows rotating),
// refreshed after the row's snow is painted, so it reads exactly what the per-pixel lookups into the frame used to
// read, in the same top-to-bottom order -- the same pixels, checked against the old code on 20000 random columns
// (measured 2026-10-09: the lookups cost ~1.6 ms a frame among Ravenholm's trees).
namespace { std::vector<uchar> TreeMask; }
static void SnowOnTree()
{
	if( !TreeSnow1 ) return;
	const int w = LeafX1 - LeafX0, yEnd = min( LeafY1, LeafFloorY );
	if( yEnd <= LeafY0 || w <= 0 ) return;
	const bool cutTop = LeafY0 <= (int)Screen_TopBorder;	// the column was clipped at the screen's top: its first row has no "above"
	TreeMask.assign( (size_t)3 * w, 0 );
	uchar* m[ 3 ] = { &TreeMask[ 0 ], &TreeMask[ w ], &TreeMask[ 2 * w ] };
	const uchar* const snapBase = LeafWas;
	const __m128i one = _mm_set1_epi8( 1 ), zero = _mm_setzero_si128();
	for( int yy = LeafY0; yy < yEnd; ++yy ){
		const int k = yy - LeafY0;
		uchar* m0 = m[ k % 3 ];							// this row: 1 = the tree drew it (differs from the snapshot)
		const uchar* m1 = k >= 1 ? m[ ( k + 2 ) % 3 ] : 0;	// the row above, as painted; rows above the column: nothing
		const uchar* m2 = k >= 2 ? m[ ( k + 1 ) % 3 ] : 0;	// two rows above
		uchar* now = WorkingSurface + YOffsetHashTable[ yy ] + LeafX0;
		const uchar* was = snapBase + (size_t)k * w;
		int any = 0, xx = 0;
		for( ; xx + 16 <= w; xx += 16 ){
			const __m128i d = _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( now + xx ) ), _mm_loadu_si128( (const __m128i*)( was + xx ) ) ), one );
			_mm_storeu_si128( (__m128i*)( m0 + xx ), d );
			any |= _mm_movemask_epi8( _mm_cmpgt_epi8( d, zero ) );
		}
		for( ; xx < w; ++xx ){ m0[ xx ] = now[ xx ] != was[ xx ]; any |= m0[ xx ]; }
		if( !any || ( cutTop && yy == LeafY0 ) ) continue;
		for( xx = 0; xx < w; ++xx ){
			if( !m0[ xx ] ) continue;					// background: the tree did not draw here
			const uchar v = now[ xx ];
			const bool leaf = LeafIdx[ v ] != 0;
			if( LeafHut && !leaf ) continue;			// a house tile: its roof and walls stay as they are
			if( !( m1 && m1[ xx ] ) ) now[ xx ] = TreeSnow1;					// the top of a branch or clump
			else if( leaf && !( m2 && m2[ xx ] ) ) now[ xx ] = TreeSnow2;	// a leaf clump's second row
			else continue;
			m0[ xx ] = now[ xx ] != was[ xx ];			// as the frame now reads (snow the same as the background: not drawn)
		}
	}
}

void TownWater_CellEnd()
{
	RockSnow_End();	// (th4) ROCK SNOW (below): a rock cell is never a tree cell -- but ADRIA'S HUT's 165..167 and the CATACOMBS
					// BUILDING's 323 are both: their snow is painted first and never on a leaf entry, and on house tiles
					// (kHutLeafTiles) SNOW ON THE TREES snows leaf entries only
	if( !LeafOn ) return;
	LeafOn = false;
	if( IceTrees_Active() ) SnowOnTree();	// (th4) SNOW ON THE TREES (above): BEFORE the scan below, so the sway and leaf layers see the snow
	// ONE scan of the column serves both users of it: TREE SWAY (every pixel the tree drew above its
	// floor layer) and the GLOWING LEAVES (every leaf-entry pixel it drew) -- TreeScan_Run, on THE JOB LINES (tree line):
	// here the cell only becomes a job (its "after" pixels copied into the pool) and is posted.
	const bool leafRec = GlowNight_Active() || BloodNight_Active() || IceTrees_Active();	// the leaf buffer is only emptied by GlowLeaves, which runs only then
	if( !LeafSway && !leafRec ) return;
	TreeJob j;
	j.x0 = LeafX0; j.x1 = LeafX1; j.y0 = LeafY0; j.y1 = LeafY1; j.floorY = LeafFloorY; j.baseY = LeafBaseY;
	j.sway = LeafSway; j.leafRec = leafRec; j.hut = LeafHut; j.halo = LeafHalo; j.noSun = LeafNoSun; j.needK = SwayNeedK;
	j.was = LeafWas;
	j.scrL = Screen_LeftBorder; j.scrT = Screen_TopBorder; j.scrW = ScreenWidth; j.scrH = ScreenHeight;
	j.now = WorkingSurface; j.nx0 = 0; j.ny0 = 0; j.nstride = WorkingWidth;	// the live frame (YOffsetHashTable[ y ] == y * WorkingWidth)
#if TH4_JOBLINES_VERIFY
	if( VerifyThis ){ TreeOut v{ VSwayPix, VSwayPixN, VSwayPixCap, VLeaves, VLeafN, VLeafCap, VSwayCells, VSwayAfter }; TreeScan_Run( j, v ); ++TreeJobsNow; }	// the reference
#endif
	if( !JOB_LINES ){ TreeLine().Inline( j ); return; }
	// what the scan reads of the frame: the column, and the after-tree snapshot box -- the tree's pixels (rows above the
	// floor layer, inside the column) widened by SWAY_MARGIN and cut to the screen; its bottom never passes LeafY1
	const int cx0 = max( j.scrL, j.x0 - SWAY_MARGIN ), cx1 = min( j.scrL + j.scrW, j.x1 + SWAY_MARGIN );
	const int cy0 = max( j.scrT, j.y0 - SWAY_MARGIN ), cy1 = j.y1;
	const int cw = cx1 - cx0;
	uchar* const now = JobPool_Alloc( (size_t)cw * ( cy1 - cy0 ) );
	if( !now ){ TreeLine().Inline( j ); return; }
	for( int yy = cy0; yy < cy1; ++yy ) memcpy( now + (size_t)( yy - cy0 ) * cw, WorkingSurface + YOffsetHashTable[ yy ] + cx0, cw );
	j.now = now; j.nx0 = cx0; j.ny0 = cy0; j.nstride = cw;
	TreeLine().Post( j );
}


// after the zoom: hand the leaves that are still visible to the shader
static void GlowLeaves( const uchar* frame, int z )
{
	const int P = WorkingWidth, W = ScreenWidth, H = ScreenHeight;
	// brightness of a leaf entry in the day palette, stretched over the leaf entries' own range
	auto lum = []( int i ){ return ( FilePalette[ i ].peRed * 2 + FilePalette[ i ].peGreen * 4 + FilePalette[ i ].peBlue * 3 ) / 9; };
	int lo = 255, hi = 0;
	for( uchar e : kLeafEntries ){ lo = min( lo, lum( e ) ); hi = max( hi, lum( e ) ); }
	if( hi <= lo ) hi = lo + 1;
	int lumOf[ 256 ];
	for( int i = 0; i < 256; ++i ) lumOf[ i ] = lum( i );
	for( size_t k = 0; k < LeafN; ++k ){
		const LeafPx& q = Leaves[ k ];
		const int rx = ( q.rel & 0xFFFF ) * z, ry = ( q.rel >> 16 ) * z;
		if( rx >= W || ry >= H || frame[ ry * P + rx ] != q.val ) continue;	// covered by something drawn later
		// LEAF NEIGHBOURHOOD RULE, sampled once per world pixel (every z screen px):
		// (A bright leaf of a pure tree tile passes without looking; a dark one stops at the first bright
		// neighbour; only house-tile leaves need the full count.)
		const bool bright = LeafBrightIdx[ q.val ];
		const bool hut = ( q.fromHut & 1 ) != 0;
		if( !bright || hut ){
			int leafN = 0; bool brightNear = false;
			for( int dy = -LEAF_NEAR_R; dy <= LEAF_NEAR_R && ( hut || !brightNear ); ++dy ){
				const int yy = ry + dy * z; if( yy < 0 || yy >= H ) continue;
				for( int dx = -LEAF_NEAR_R; dx <= LEAF_NEAR_R; ++dx ){
					const int xx = rx + dx * z; if( xx < 0 || xx >= W ) continue;
					const uchar e = frame[ yy * P + xx ];
					if( LeafIdx[ e ] ) ++leafN;
					if( LeafBrightIdx[ e ] ){ brightNear = true; if( !hut ) break; }
				}
			}
			// every tree tile: a dark leaf entry counts only next to a bright one (grass shading under trees)
			if( !bright && !brightNear ) continue;
			// house tiles: also the cluster checks (lone bright specks on Adria's hut's door area)
			if( hut ){
				if( bright && leafN < LEAF_BRIGHT_MIN ) continue;
				int brightWide = 0;
				for( int dy = -LEAF_WIDE_R; dy <= LEAF_WIDE_R && brightWide < LEAF_WIDE_BRIGHT_MIN; ++dy ){
					const int yy = ry + dy * z; if( yy < 0 || yy >= H ) continue;
					for( int dx = -LEAF_WIDE_R; dx <= LEAF_WIDE_R; ++dx ){
						const int xx = rx + dx * z; if( xx < 0 || xx >= W ) continue;
						if( LeafBrightIdx[ frame[ yy * P + xx ] ] ) ++brightWide;
					}
				}
				if( brightWide < LEAF_WIDE_BRIGHT_MIN ) continue;
			}
		}
		const int boost10 = q.boost;
		int code = 128 + ( lumOf[ q.val ] - lo ) * 127 / ( hi - lo );
		code = min( 255, code + ( boost10 - 10 ) * 4 );	// sparse crowns: brighter on the pink ramp
		// the z x z block of BioGlow_SetWater( rx + ox, ry + oy, code ), clipped to the screen, then BioGlow_Add -- in one call
		// (dx_utility.cpp BioGlow_AddLeaf does exactly that)
		BioGlow_AddLeaf( rx, ry, z, code, 3 * z * z * boost10 / 10 * ( ( q.fromHut & 2 ) ? HALO_BOOST_X : 1 ) );	// sparse crowns / HALO BOOST: more halo per leaf
	}
	LeafN = 0;
}

// (th4) TREE SWAY: the two layers TreeSway.hlsl needs (TREE SWAY above), at PRE-ZOOM resolution
// (texel = world pixel). The frame it reads is the finished one, after the zoom: pre-zoom pixel
// (x, y) is the z x z block at (x z, y z), all one colour.
static void BuildSwayLayers( const uchar* frame, int z )
{
	const int P = WorkingWidth;
	const int W = ( ScreenWidth + z - 1 ) / z, H = ( ScreenHeight + z - 1 ) / z;	// pre-zoom screen size
	SwayLayersValid = false;
	if( SwayLW != W || SwayLH != H ){
		free( SwayL1 ); free( SwayL2 );
		SwayL1 = (uint*)malloc( (size_t)W * H * 4 ); SwayL2 = (uchar*)malloc( (size_t)W * H );
		if( !SwayL1 || !SwayL2 ){ SwayLW = SwayLH = 0; return; }
		memset( SwayL1, 0, (size_t)W * H * 4 ); memset( SwayL2, 255, (size_t)W * H );
		SwayLW = W; SwayLH = H; SwayBox[0] = SwayBox[1] = SwayBox[2] = SwayBox[3] = 0;
	}
	// forget what the last frame wrote
	for( int y = SwayBox[1]; y < SwayBox[3]; ++y ){
		const int n = SwayBox[2] - SwayBox[0];
		memset( SwayL1 + (size_t)y * W + SwayBox[0], 0, (size_t)n * 4 );
		memset( SwayL2 + (size_t)y * W + SwayBox[0], 255, n );
	}
	// pre-zoom pixel (x, y) of the finished frame = frame[ y z P + x z ]
	const int GW = ScreenWidth, GH = ScreenHeight;
	int bx0 = W, by0 = H, bx1 = 0, by1 = 0;
	const int cells = (int)min( SwayCells.size(), (size_t)254 );	// tree numbers 0..253 (A = number + 1, K 255 = none)
	for( int o = 0; o < cells; ++o ){
		const SwayCell& c = SwayCells[ o ];
		const int ew = c.ex1 - c.ex0;
		const int x0 = c.ex0 - Screen_LeftBorder, y0 = c.ey0 - Screen_TopBorder;	// screen-relative pre-zoom
		const int x1 = min( W, c.ex1 - Screen_LeftBorder ), y1 = min( H, c.ey1 - Screen_TopBorder );
		// K: the pixels of this box that never changed after this tree was drawn (not needed without wind: SwayNeedK)
		if( SwayNeedK ) for( int y = y0; y < y1; ++y ){
			if( y * z >= GH ) break;
			const uchar* aft = SwayAfter.data() + c.afterOfs + (size_t)( y - y0 ) * ew;
			uchar* k2 = SwayL2 + (size_t)y * W;
			const uchar* fr = frame + y * z * P;
			const int xe = x1 * z <= GW ? x1 : ( GW + z - 1 ) / z;
			for( int x = x0; x < xe; ++x ){
				if( k2[ x ] == 255 && fr[ x * z ] == aft[ x - x0 ] ) k2[ x ] = (uchar)o;
			}
		}
		bx0 = min( bx0, x0 ); by0 = min( by0, y0 ); bx1 = max( bx1, x1 ); by1 = max( by1, y1 );
		// the tree's pixels still showing
		for( size_t k = c.first; k < c.first + c.count; ++k ){
			const SwayPx& q = SwayPix[ k ];
			const int px = q.rel & 0xFFFF, py = q.rel >> 16;
			if( px >= W || py >= H || px * z >= GW || py * z >= GH || frame[ py * z * P + px * z ] != q.val ) continue;	// covered by something drawn later
			int wt = ( q.hgt - SWAY_FOOT_H ) * 255 / ( SWAY_FULL_H - SWAY_FOOT_H );
			wt = wt < 1 ? 1 : wt > 255 ? 255 : wt;
			SwayL1[ (size_t)py * W + px ] = (uint)( o + 1 ) << 24 | (uint)wt << 16 | (uint)q.val << 8 | q.bg;
		}
	}
	if( bx1 <= bx0 || by1 <= by0 ){ SwayBox[0] = SwayBox[1] = SwayBox[2] = SwayBox[3] = 0; return; }
	SwayBox[0] = max( 0, bx0 ); SwayBox[1] = max( 0, by0 ); SwayBox[2] = bx1; SwayBox[3] = by1;
	SwayLayersValid = true;
}

// (th4) TREE SWAY: this frame's layers for dx_utility.cpp TreeSway_Present; false = no tree moves.
// w, h = the layers' (pre-zoom) size; box = what this frame wrote (pre-zoom px x0, y0, x1, y1); the
// buffers are right everywhere else too. l2 = 0 when this frame built no layer 2 (SwayNeedK: sun rays only).
bool TownWater_SwayLayers( const uint*& l1, const uchar*& l2, int& w, int& h, int* box )
{
	l1 = SwayL1; l2 = SwayNeedK ? SwayL2 : 0; w = SwayLW; h = SwayLH;
	for( int i = 0; i < 4; ++i ) box[ i ] = SwayBox[ i ];
	return SwayLayersValid;
}

// (th4) SUN GROUND (dx_utility.cpp SUN RAYS, SunRays.hlsl): where sunlight may land -- the GROUND only,
// never a roof, a wall, a fence or a sprite (Mordor: no rays over the rooftops). Each walkable cell's FLOOR
// layer (GroundCell, the puddles' rule, plus the tree cells, whose floor is the grass under the crown, plus
// the RIVER cells -- their grass banks and the water itself: GroundCell leaves the river out for the
// puddles, and the rays then broke off at every river tile's edge, Mordor 2026-10-04) is recorded right after it is drawn (TownWater_FloorDrawn: its 64 x 32 diamond, the value
// of every pixel, pre-zoom); after the frame (BuildSunGround, TownWater_Render) a pixel is ground if it
// still shows the colour its floor gave it -- nothing drawn later covered it. Same idea as the puddles
// (PUDDLES above). The GRASS (below) also records walk-blocked cells there, as mark 3, which this skips: the
// sun rays keep to walkable / tree / river ground as before. Only while SunRays_Active. Layer: pre-zoom size like the sway layers, SUN_GROUND_YES (255)
// = ground, SUN_GROUND_WATER (128) = the river's water (kWaterEntries in a river cell: exactly what the
// WaterShimmer ripple moves -- dx_utility.cpp draws the rays there AFTER the ripple, so they stay still),
// SUN_GROUND_NO_SOURCE (64) = a crown pixel of kNoSunSquares (no light comes through it), 0 = none of these.
namespace {
bool SunGroundOn = false;
uchar* GroundVal = 0; uchar* GroundMark = 0; uchar* SunGround = 0;
int SunGW = 0, SunGH = 0;
bool SunGroundValid = false;
// (th4) SNOW GROUND (Ice Age town; dx_utility.cpp SnowGround_Active / SnowGround_Present + SnowGround.hlsl): the same
// recording (RecordGround) gives the snow its ground -- SnowLay, pre-zoom like SunGround: per pixel the art's palette
// entry of a recorded floor pixel that nothing covered later and that the votes call ground -- walkable and
// tree cells (mark 1), the river's banks (mark 1 in river cells; its water is mark 2), walk-blocked cells (mark 3: rocks,
// posts, fences, the well) and house cells (mark 4: blocking sight, or a house tile among the tree tiles) -- 0 = none
// (the frozen water, walls, roofs, rocks, sprites, items). The rules: SNOW GROUND RULES (below BuildSunGround).
bool SnowGroundOn = false;
uchar* SnowLay = 0;
bool SnowLayValid = false;
// (th4) SNOW GROUND -- THE VOTES (SnowCellVotes, SNOW GROUND RULES): which floor pixels of a map cell are ground,
// worked out once per cell; RecordGround copies them into SnowDec with the cell's pixels
const int SNOW_DEC_NOT_YET = -1, SNOW_DEC_NONE = -2, SNOW_DEC_ALL = -3;
const int SNOW_DEC_SIZE = 1088;		// RecordGround's diamond: 2 x ( 2 + 4 + .. + 32 ) x 2 halves (GRASS_DIAMOND_HALF x 2)
std::vector<int> SnowDecOf;			// [ row * 112 + col ]: SNOW_DEC_*, or the offset of the cell's votes in SnowDecPool
std::vector<uchar> SnowDecPool;		// SNOW_DEC_SIZE bytes per distinct set of votes: RecordGround's diamond row by row, 1 = ground
int SnowRowStart[ 32 ];				// where row k (0..31, from the top) of RecordGround's diamond starts in those bytes
uchar* SnowDec = 0;					// per pre-zoom pixel like GroundMark: the votes of the cell that recorded it (0 / 1)
uchar* SnowArtLay = 0;				// per pre-zoom pixel: the ART's palette entry of the recorded floor (THE ART, RecordGround)
}

// ============================================================================================
// (th4) GRASS (config GrassBlades 0..10, default 10): blades of grass growing out of the grass of the normal
// town and of Ravenholm, swaying when the wind blows. Never in Classic, never under the Ice Age (dx_utility.cpp
// Grass_Active, the one place that decides). The blades are DRAWN by Grass.hlsl (dx_utility.cpp GRASS has the
// look and the wind); THIS part says where a blade may stand and where one may be seen. It shares the SUN
// GROUND recording above (RecordGround: the floor diamonds of walkable / tree / river cells, right after each
// cell's floor is drawn), which runs whenever the sun rays OR the grass want it.
//   WHICH PIXELS ARE GRASS: palette entry NUMBERS of the town art (kGrassCore / kGrassMixed / kGrassShade),
//     read from the ART of each floor piece, NOT from the frame (when a floor is recorded the two agree, in the town and
//     in Ravenholm: Ravenholm goes through Draw.cpp DrawSceneDungeonTop but is FULLY LIT -- thehell2.cpp
//     LoadQuestDungeonMainMapTown sets FullLighting, so DarkLevel is 0 on every cell; it is not light-mapped). Each floor
//     piece (TileMap layer 0, left and right) is decoded once from the level CEL the way Draw.cpp PutTile
//     draws it (GrassDecodePiece -- keep the two alike) into a 32 x 32 map of grass classes, cached per tile
//     id (GrassArtIndex) until the level's CEL or the level changes. The PALETTE never matters: under
//     Towx / Towz / L0_N / L0_DARK05 (palette.cpp) the same entries are grass, only their colour differs --
//     and a blade takes its colour from the screen pixel it grows from (Grass.hlsl), so grass the palette
//     paints the colour of hay grows hay-coloured blades.
//   HOW MUCH GRASS A CELL HAS (GrassCellDens, 112 x 112 map cells, 0..255 = the share of the cell's floor
//     diamond that is grass): Grass.hlsl reads it with bilinear filtering across cells, so the grass thins out
//     smoothly toward the paths and stray green specks on a path or a stone floor grow nothing. Worked out
//     again whenever FineMap changes (GrassMapSig, a hash of it, checked once per frame).
//     GRASS IN SHADOW (Mordor: "shadow doesn't stop grass from growing"): the green pixels count in full; the
//     baked-shadow pixels (kGrassShade) count by how much of the LIT ground around is green -- the cell itself
//     and the WALKABLE cells within GRASS_LAWN_REACH, green / ( green + bare ). The shadow entries are not only
//     on lawns (measured: 48..71 % of their pixels lie in lawns, 2..14 % on paths), so a shadow across a path
//     must not turn into lawn -- and a strip of shadow along a house is judged by the open ground beside it, not
//     by the floor under the house or the foot of its wall (both bare). Measured on the whole town: the house
//     strips and the shaded lawns get blades, the paths, the square and the dirt under the porches none.
//   WHERE A BLADE MAY BE SEEN: the recorded floor pixels that nothing covered later (the SUN GROUND test:
//     the finished frame still shows the colour the floor gave them). A blade is drawn only over visible
//     ground -- never over a sprite, a wall, a fence, an item, the river's water or the UI -- so a hero
//     standing in the grass is always in front of it (simple, and never wrong by much: blades are short).
//     WALK-BLOCKED cells are recorded for the grass too (RecordGround, mark 3: not for the sun rays, not for the
//     puddles): the strip of lawn along a house's wall, the foot of fences and walls, the grass between the rocks
//     are part of the house's / fence's / rocks' cells, and were left bare with a hard edge (Mordor). Whatever
//     their wall, roof or rock covers fails the test above, as for every other cell.
//   THE LAYER (BuildGrassLayer, from TownWater_Render; PRE-ZOOM like the sway layers, one byte per world px):
//     0 no blade may show here, 1 visible ground, 2 a green grass entry (a blade may stand here), 3 a grey /
//     olive / brown speck of the grass texture (fewer, shorter blades), 4 grass in a baked shadow (needs a
//     fuller cell). Puddle pixels (PUDDLES above) grow none: a blade there took the colour of the mirrored sky.
//     Plus the BLOCK map (GRASS_BLOCK px squares, 255 = a blade may reach in here), so Grass.hlsl skips the
//     rest of the screen with one look-up, and the box of the grass, so its quad covers only that.
//   THE ANCHOR (THE MAP ANCHOR, MapC00X / Y, shared with the RUNNING RIVER): where the centre of map cell (0,0) is
//     in the layer. Grass.hlsl measures everything from it: the cell densities, and the per-blade hash -- so every
//     blade belongs to its spot of ground and stays the same when the screen scrolls.
// The entries were measured offline on the whole town (TH4data's NLevels\TownData\Town.CEL laid out by
// Sector1s..4s.DUN, the way TownMap.cpp LoadTownMap does): for every entry, the share of its floor pixels
// lying in a green area (at least half of a 9 x 9 window "green" = hue 70..140, saturation >= 0.35 in
// Levels\TownData\town.pal). A NEW TOWN ART (a Town.CEL with other entries) needs these lists redone.
// KNOWN-GOOD CHECK: a windy day in Tristram (TreeSway is on, the trees move): the lawns are full of short
// blades leaning and waving with the gusts, the paths stay bare, a hero walking through the grass is never
// covered by it, the river and the puddles grow none; a calm day: the same blades, standing still. Under the
// Ice Age and in Classic: none. Ravenholm: the same, always windy.
namespace {
const uchar kGrassCore[]  = { 54, 69, 71, 74, 83, 85, 90, 93, 102, 104, 105 };	// the greens (66..86% in grass)
const uchar kGrassMixed[] = { 56, 66, 67, 82, 88, 95, 100, 103 };				// grey / olive / brown specks of the grass texture (60..76%)
const uchar kGrassShade[] = { 109, 111, 112, 113, 114, 116, 122 };			// the baked shadows of trees, houses and the signpost lying ON the grass
uchar GrassClassOf[ 256 ];				// palette entry -> 0 not grass, 1 core, 2 mixed, 3 shade (layer codes 2..4 = class + 1)
bool GrassOn = false;					// Grass_Active this frame (TownWater_BeginFrame)
std::vector<int> GrassArtIdx;			// tile id (as TileMap holds it) -> its class map in GrassArt, -1 = not decoded yet
std::vector<uchar> GrassArt;			// class maps, 1024 bytes each, row 0 = the BOTTOM row (as PutTile draws)
struct GrassArtCount { ushort core[ 2 ], shade[ 2 ], bare[ 2 ]; };	// per class map: kGrassCore / kGrassShade / bare (no grass class,
std::vector<GrassArtCount> GrassArtCnt;	// not water) pixels in the LEFT [0] / RIGHT [1] half of a floor diamond -- GrassBuildDens
const int GRASS_LAWN_REACH = 2;			// GRASS IN SHADOW: the lit ground of the walkable cells this far around (5 x 5) judges a shadow
const int GRASS_LAWN_MIN_PX = 128;		// ... when it has at least this many lit pixels (else the shadow counts as no grass)
const int* GrassArtCel = 0; int GrassArtDun = -1;	// what the cache was built for
uchar GrassCellDens[ 112 ][ 112 ];		// [row][col], 0..255
uint GrassMapSig = 0, GrassDensGen = 0;	// GrassDensGen grows each time GrassCellDens changes (dx_utility.cpp uploads it then)
bool GrassDensValid = false;
uchar* GrassCls = 0;					// the class of each recorded ground pixel (pre-zoom, SunGW x SunGH, like GroundVal)
uchar* GrassLay = 0;					// THE LAYER (above), same size
std::vector<uchar> GrassBlkRaw, GrassBlk;	// roots per block, and the reach of them (the BLOCK map)
int GrassBW = 0, GrassBH = 0;
int GrassBox[ 4 ] = { 0, 0, 0, 0 };		// x0, y0, x1, y1 of the blocks holding grass, pre-zoom px
bool GrassLayValid = false;
const int GRASS_BLOCK = 16;				// block size, pre-zoom px: must exceed the tallest blade + 2 (Grass.hlsl looks at most that far down)
const int GRASS_DIAMOND_HALF = 544;		// pixels of one half of a floor diamond as RecordGround lays it out (2 x (2 + 4 + ... + 32))
}

// (th4) GRASS: one floor piece of the level CEL decoded the way Draw.cpp PutTile draws it (keep the two alike),
// into 32 x 32 entries and an opaque flag, row 0 = the BOTTOM row. The tile id is resolved as DrawDunCelTrans
// does for an unlit draw (DarkLevel 0): bit 0x8000 = a pre-darkened copy (Halls.cpp InitDarkenDungeonTiles),
// mapped back to its CEL frame through DungeonTilesOffset. Bits 12..14 = PutTile's type. False = no such frame.
static bool GrassDecodePiece( ushort id, uchar* pix, uchar* opq )
{
	memset( opq, 0, 1024 );
	if( !CurrentLevelCELFile ) return false;
	if( id & 0x8000 ) id = (ushort)( DungeonTilesOffset[ 16 * ( id & 0xFFF ) ] + ( id & 0xF000 ) );
	const int frame = id & 0x0FFF, type = ( id >> 12 ) & 7;
	const int frames = CurrentLevelCELFile[ 0 ] & 0xFFFF;
	if( frame < 1 || frame > frames ) return false;
	const uchar* cel = (const uchar*)CurrentLevelCELFile;
	const uchar* s = cel + CurrentLevelCELFile[ frame ];
	const uchar* e = cel + CurrentLevelCELFile[ frame + 1 ];
	auto put = [&]( int y, int x, int n, const uchar* src ){	// one run; a damaged frame stops at its own end
		if( src + n > e ) n = (int)( e - src );
		for( int k = 0; k < n; ++k ){ pix[ y * 32 + x + k ] = src[ k ]; opq[ y * 32 + x + k ] = 1; }
	};
	if( type == 0 ){											// opaque 32 x 32 square
		for( int y = 0; y < 32; ++y, s += 32 ) put( y, 0, 32, s );
	}else if( type == 1 ){										// transparent: runs, >= 0 = that many pixels, < 0 = skip
		for( int y = 0; y < 32 && s < e; ++y ){
			for( int x = 0; x < 32 && s < e; ){
				const int w = (signed char)*s++;
				if( w >= 0 ){ const int n = min( w, 32 - x ); put( y, x, n, s ); s += w; x += w; }
				else x -= w;
			}
		}
	}else if( type == 2 || type == 3 ){							// left / right triangle
		int shift = type == 2 ? 2 : 0;
		for( int h = 32, skip = 30, y = 0; skip != 32; --h, shift ^= 2, ++y ){
			put( y, type == 2 ? skip : 0, 32 - skip, s + shift );
			s += 32 - ( skip - shift );
			if( h > 17 ) skip -= 2; else skip += 2;
		}
	}else{														// 4 left / 5..7 right trapezoid
		int shift = type == 4 ? 2 : 0, h = 32, skip = 30, y = 0;
		for( ; h > 16; --h, shift ^= 2, ++y, skip -= 2 ){
			put( y, type == 4 ? skip : 0, 32 - skip, s + shift );
			s += 32 - ( skip - shift );
		}
		for( ; h > 0; --h, ++y, s += 32 ) put( y, 0, 32, s );
	}
	return true;
}

// (th4) GRASS: the class map of a floor piece (decoded on first use), as an index into GrassArt. Take
// pointers (GrassArtPtr) only after every GrassArtIndex call of the moment: decoding grows GrassArt.
static int GrassArtIndex( ushort id )
{
	int& slot = GrassArtIdx[ id ];
	if( slot >= 0 ) return slot;
	GroundRec_Join();	// (th4) THE JOB LINES: GrassArt grows below -- no ground job may still be reading it
	uchar pix[ 1024 ], opq[ 1024 ];
	const size_t at = GrassArt.size();
	GrassArt.resize( at + 1024 );
	uchar* cls = &GrassArt[ at ];
	const bool ok = id && GrassDecodePiece( id, pix, opq );
	for( int i = 0; i < 1024; ++i ) cls[ i ] = ( ok && opq[ i ] ) ? GrassClassOf[ pix[ i ] ] : 0;
	// green / shadow / bare pixels inside the floor diamond's left half (x 32 - half .. 31) and right half (x 0 .. half - 1)
	static bool water[ 256 ], waterSet = false;
	if( !waterSet ){ for( uchar e : kWaterEntries ) water[ e ] = true; waterSet = true; }
	GrassArtCount n = {};
	for( int b = 0; b < 32; ++b ){
		const int half = b <= 15 ? 2 * ( b + 1 ) : 2 * ( 32 - b );	// RecordGround's rows, counted from the bottom
		for( int x = 0; x < 32; ++x ){
			const int i = b * 32 + x;
			if( !ok || !opq[ i ] ) continue;
			ushort* k = cls[ i ] == 1 ? n.core : cls[ i ] == 3 ? n.shade : ( cls[ i ] == 0 && !water[ pix[ i ] ] ) ? n.bare : 0;
			if( !k ) continue;	// a speck of the grass texture (kGrassMixed) or water: neither green nor bare
			if( x >= 32 - half ) ++k[ 0 ];
			if( x < half ) ++k[ 1 ];
		}
	}
	GrassArtCnt.push_back( n );
	slot = (int)( at / 1024 );
	return slot;
}
static inline const uchar* GrassArtPtr( int index ) { return &GrassArt[ (size_t)index * 1024 ]; }

// (th4) GRASS: GrassCellDens for the whole map, from the class maps of every cell's two floor pieces -- the green
// pixels, plus the shadow pixels weighted by the lawn around them (GRASS IN SHADOW, above)
static void GrassBuildDens()
{
	static ushort core[ 112 ][ 112 ], shade[ 112 ][ 112 ], bare[ 112 ][ 112 ];
	static bool walk[ 112 ][ 112 ];
	for( int r = 0; r < 112; ++r ){
		for( int c = 0; c < 112; ++c ){
			const int piece = FineMap[ r ][ c ];
			walk[ r ][ c ] = piece > 0 && piece < 2049 && !TileBlockWalking[ piece ];
			if( !piece ){ core[ r ][ c ] = shade[ r ][ c ] = bare[ r ][ c ] = 0; continue; }
			const TileColumn& t = TileMap[ IsometricMapOffset( r, c ) ];
			const int il = GrassArtIndex( t.Layer[ 0 ].Left ), ir = GrassArtIndex( t.Layer[ 0 ].Right );
			const GrassArtCount& L = GrassArtCnt[ il ]; const GrassArtCount& R = GrassArtCnt[ ir ];
			core[ r ][ c ] = L.core[ 0 ] + R.core[ 1 ]; shade[ r ][ c ] = L.shade[ 0 ] + R.shade[ 1 ]; bare[ r ][ c ] = L.bare[ 0 ] + R.bare[ 1 ];
		}
	}
	for( int r = 0; r < 112; ++r ){
		for( int c = 0; c < 112; ++c ){
			float lawn = 0.0f;	// how green the lit ground around is: the cell itself and the walkable cells within reach
			if( shade[ r ][ c ] ){
				int g = 0, b = 0;
				for( int r2 = max( r - GRASS_LAWN_REACH, 0 ); r2 <= min( r + GRASS_LAWN_REACH, 111 ); ++r2 ){
					for( int c2 = max( c - GRASS_LAWN_REACH, 0 ); c2 <= min( c + GRASS_LAWN_REACH, 111 ); ++c2 ){
						if( ( r2 != r || c2 != c ) && !walk[ r2 ][ c2 ] ) continue;
						g += core[ r2 ][ c2 ]; b += bare[ r2 ][ c2 ];
					}
				}
				if( g + b >= GRASS_LAWN_MIN_PX ) lawn = (float)g / ( g + b );
			}
			GrassCellDens[ r ][ c ] = (uchar)min( 255, (int)( ( core[ r ][ c ] + shade[ r ][ c ] * lawn ) * 255.0f / ( 2 * GRASS_DIAMOND_HALF ) ) );
		}
	}
}

// (th4) GRASS: the grass share at map point (fr, fc) in CELL units (cell (r, c)'s centre is (r, c)), 0..255,
// bilinear between the cells like Grass.hlsl's s3 -- for the PUDDLES (NOT ON GRASS)
static int GrassShareQ8( float fr, float fc )
{
	if( !GrassDensValid ) return 0;
	const int r0 = (int)floorf( fr ), c0 = (int)floorf( fc );
	const float tr = fr - r0, tc = fc - c0;
	auto d = []( int r, int c ){ r = r < 0 ? 0 : r > 111 ? 111 : r; c = c < 0 ? 0 : c > 111 ? 111 : c; return (float)GrassCellDens[ r ][ c ]; };
	const float a = d( r0, c0 ) + ( d( r0 + 1, c0 ) - d( r0, c0 ) ) * tr;
	const float b = d( r0, c0 + 1 ) + ( d( r0 + 1, c0 + 1 ) - d( r0, c0 + 1 ) ) * tr;
	return (int)( a + ( b - a ) * tc + 0.5f );
}

// (th4) GRASS: TownWater_BeginFrame, while Grass_Active OR the town is wet (PUDDLES, NOT ON GRASS) -- the class
// table, the art cache, the cell densities. New densities drop the worked-out puddles: they were cut by the old ones.
static void GrassArtEnsure()
{
	static bool classes = false;
	if( !classes ){
		memset( GrassClassOf, 0, sizeof( GrassClassOf ) );
		for( uchar e : kGrassCore )  GrassClassOf[ e ] = 1;
		for( uchar e : kGrassMixed ) GrassClassOf[ e ] = 2;
		for( uchar e : kGrassShade ) GrassClassOf[ e ] = 3;
		classes = true;
	}
	if( GrassArtCel != CurrentLevelCELFile || GrassArtDun != (int)(DUNGEON)Dungeon || GrassArtIdx.empty() ){
		GrassArtIdx.assign( 65536, -1 );
		GrassArt.clear(); GrassArtCnt.clear();
		GrassArtCel = CurrentLevelCELFile; GrassArtDun = (int)(DUNGEON)Dungeon;
		GrassDensValid = false;
	}
	uint sig = 2166136261u;	// FNV-1a over FineMap: the map changes on level load and on quest tile swaps
	for( int i = 0; i < 112 * 112; ++i ) sig = ( sig ^ (uint)FineMap[ 0 ][ i ] ) * 16777619u;
	if( !GrassDensValid || sig != GrassMapSig ){
		GrassBuildDens();
		GrassMapSig = sig; GrassDensValid = true;
		if( !++GrassDensGen ) GrassDensGen = 1;	// 0 = "never uploaded" in dx_utility.cpp
		memset( PuddleFirst, 0xFF, sizeof( PuddleFirst ) ); memset( PuddleCnt, 0, sizeof( PuddleCnt ) ); PuddlePool.clear();
	}
}

static int SnowCellVotes( int r, int c );	// (th4) SNOW GROUND -- THE VOTES, below BuildSunGround
static int SnowCellKind( int r, int c );	// (th4) SNOW GROUND, same place
static int SnowArtSlot( ushort id );	// (th4) SNOW GROUND -- THE ART, same place
static inline const uchar* SnowArtPtr( int slot );
static bool SnowStrip( int kind, int r, int c, int side );	// (th4) SNOW GROUND -- THE STRIPS, same place

// (th4) THE JOB LINES (above TreeScan_Run) -- GROUND RECORDING: RecordGround decides per cell on the main thread (its
// piece, votes, strips, art slots) and copies the cell's 64 x 32 floor box from the frame into the pool; GroundRec_Run,
// on the ground line, writes the layers from that copy row by row -- the loop that was RecordGround's own.
namespace {
struct GroundJob {
	int x, y;									// the cell's floor dst (TownWater_FloorDrawn)
	int L, T, gw, gh;							// Screen_LeftBorder, Screen_TopBorder, SunGW, SunGH
	bool snowOn, grassOn, river, snowHouse, grassOnly, snowRockFloor, stripL, stripR;
	int snowVotes;								// SNOW_DEC_NONE / SNOW_DEC_ALL, or the cell's votes in SnowDecPool
	int artSlot[ 2 ], grassIdx[ 2 ];			// SnowArtSlot / GrassArtIndex of the floor's left and right piece
	const uchar* box;							// the frame's rows y - 31 .. y at x .. x + 63, 64 bytes a row; 0 = the live frame
};
struct GroundOut { uchar* val; uchar* mark; uchar* art; uchar* dec; uchar* cls; };	// GroundVal, GroundMark, SnowArtLay, SnowDec, GrassCls
#if TH4_JOBLINES_VERIFY
uchar* VGround[ 5 ] = {}; int VGroundW = 0, VGroundH = 0;	// the SHADOW layers, GroundOut's order
bool VGroundReady = false;									// copied from the real ones at this frame's start
#endif
}
// one cell's rows into the layers o
static void GroundRec_Run( const GroundJob& j, const GroundOut& o )
{
	const uchar* const votePool = j.snowVotes >= 0 ? &SnowDecPool[ (size_t)j.snowVotes ] : 0;	// this cell's votes, taken once (not per row)
	const uchar* snowArt[ 2 ] = { 0, 0 };
	if( j.snowOn ){ snowArt[ 0 ] = SnowArtPtr( j.artSlot[ 0 ] ); snowArt[ 1 ] = SnowArtPtr( j.artSlot[ 1 ] ); }
	const uchar* artL = 0; const uchar* artR = 0;
	if( j.grassOn ){ artL = GrassArtPtr( j.grassIdx[ 0 ] ); artR = GrassArtPtr( j.grassIdx[ 1 ] ); }
	const int x = j.x, y = j.y, L = j.L, T = j.T, GW = j.gw, GH = j.gh;
	for( int k = 0; k < 32; ++k ){						// the floor diamond, top row first: 4 .. 64 .. 4 px wide
		const int yy = y - 31 + k, py = yy - T;
		if( py < 0 || py >= GH ) continue;
		const int half = k < 16 ? 2 * ( k + 1 ) : 2 * ( 32 - k );
		int xa = x + 32 - half, xb = x + 32 + half;		// screen x run [xa, xb), clipped to the layer
		const int x0 = xa;								// (th4) SNOW GROUND: where the row starts in the cell's votes
		if( k < 16 ){ if( !j.stripL ) xa += 2; if( !j.stripR ) xb -= 2; }	// (th4) SNOW GROUND -- THE STRIPS (always kept without the snow)
		if( xa < L ) xa = L;
		if( xb > L + GW ) xb = L + GW;
		if( xa >= xb ) continue;
		const uchar* src = j.box ? j.box + k * 64 - x : WorkingSurface + YOffsetHashTable[ yy ];	// frame pixel xx of this row = src[ xx ]
		const int base = py * GW - L;				// layer index of screen x xx = base + xx
		memcpy( o.val + ( base + xa ), src + xa, xb - xa );
		if( j.snowOn ){									// THE ART (RecordGround): where the piece has no pixel (a strip), the frame's
			const int b = 31 - k;
			for( int side = 0; side < 2; ++side ){			// the run in each piece: one copy where the piece's row is solid there
				const int pa = max( xa, x + 32 * side ), pb = min( xb, x + 32 * side + 32 );
				if( pa >= pb ) continue;
				const uchar* a = snowArt[ side ] + b * 32 + ( pa - x - 32 * side );
				const uchar* op = a + 1024;
				if( !memchr( op, 0, pb - pa ) ) memcpy( o.art + base + pa, a, pb - pa );
				else for( int i = 0; i < pb - pa; ++i ) o.art[ base + pa + i ] = op[ i ] ? a[ i ] : src[ pa + i ];
			}
			for( int xx = xa; xx < min( xb, x ); ++xx ) o.art[ base + xx ] = src[ xx ];		// outside both pieces
			for( int xx = max( xa, x + 64 ); xx < xb; ++xx ) o.art[ base + xx ] = src[ xx ];
		}
		if( j.river ){
			const uchar* w = j.snowOn ? o.art + base : src;	// THE ART (the same entries as the frame's: Ravenholm is fully lit too)
			for( int xx = xa; xx < xb; ++xx ) o.mark[ base + xx ] = WaterIdx[ w[ xx ] ] ? 2 : 1;	// 2 = the river's water (the ripple's own test)
		}else{
			memset( o.mark + ( base + xa ), j.snowHouse ? 4 : ( j.grassOnly || j.snowRockFloor ) ? 3 : 1, xb - xa );	// 3 = ground for the grass only (BuildSunGround skips it); 4 = a house cell, for the snow only
		}
		if( j.snowOn ){									// (th4) SNOW GROUND: the votes of these pixels
			uchar* dd = o.dec + ( base + xa );
			if( j.snowVotes >= 0 ) memcpy( dd, votePool + SnowRowStart[ k ] + ( xa - x0 ), xb - xa );
			else memset( dd, j.snowVotes == SNOW_DEC_ALL ? 1 : 0, xb - xa );
		}
		if( j.grassOn ){									// piece row b (0 = bottom); left piece x .. x + 31, right piece x + 32 .. x + 63
			const int b = 31 - k;
			const int xm = min( xb, x + 32 );
			if( xm > xa ) memcpy( o.cls + ( base + xa ), artL + b * 32 + ( xa - x ), xm - xa );
			const int xr = max( xa, x + 32 );
			if( xb > xr ) memcpy( o.cls + ( base + xr ), artR + b * 32 + ( xr - x - 32 ), xb - xr );
		}
	}
}
static void GroundRec_RunReal( const GroundJob& j )	// into the real layers
{
	GroundRec_Run( j, GroundOut{ GroundVal, GroundMark, SnowArtLay, SnowDec, GrassCls } );
}
typedef JobLine< GroundJob, GroundRec_RunReal > GroundLineT;
static GroundLineT& GroundLine(){ static GroundLineT* line = new GroundLineT; return *line; }
static void RecordGround( int row, int col, int x, int y )
{
	const int piece = FineMap[ row ][ col ];
	const bool tree = piece > 0 && piece < MAX_PIECE && TreePiece[ piece ];
	const bool river = piece > 0 && piece < MAX_PIECE && RiverPiece[ piece ];	// bank grass AND the water (Mordor)
	// (th4) GRASS: a walk-blocked cell (a house's, a fence's, the rocks') -- for the grass only, mark 3 (WHERE A BLADE MAY BE SEEN)
	const bool grassOnly = piece > 0 && !GroundCell( row, col ) && !tree && !river;
	// (th4) SNOW GROUND: the same cells are recorded for the snow too -- the floor round rocks, posts, fences, the well
	// (Mordor: "floor around rocks or trees too needs to be buried") -- and so are the HOUSE CELLS, as mark 4: the
	// walk-blocked cells that also block sight (houses, the church, the catacombs, the cross graves) and the house
	// tiles among the tree tiles (HutLeafPiece: Adria's hut, the church's walls with leaves, the catacombs' bush).
	// Their floor tiles carry painted roof and wall art -- the town was rendered whole and cut into tiles, so a cell's
	// floor shows whatever stands in front of it -- so the votes judge every pixel of theirs by its art (SNOW GROUND
	// RULES, HOUSE CELLS). The grass and the snow never run together (Grass_Active is off under the Ice Age), so the
	// grass keeps every walk-blocked cell as mark 3 and the house tiles as tree cells (mark 1), as before.
	const bool snowHouse = SnowGroundOn && piece > 0 && piece < MAX_PIECE
	                    && ( HutLeafPiece[ piece ] || ( grassOnly && piece < 2049 && TileBlockSight[ piece ] ) );
	const bool snowRockFloor = SnowGroundOn && piece > 0 && piece < MAX_PIECE && !grassOnly && !tree && !river && SnowRockFloorPiece[ piece ];	// (th4) ROCK FLOORS: mark 3, as SnowCellKind says
	if( grassOnly ? !( GrassOn || ( SnowGroundOn && piece < 2049 ) ) : ( !GroundCell( row, col ) && !tree && !river ) ) return;
	// (th4) SNOW GROUND: the cell's votes, worked out on its first draw (SnowCellKind there must agree with the marks below)
	int snowVotes = SNOW_DEC_NONE;
	bool stripL = true, stripR = true;	// (th4) SNOW GROUND -- THE STRIPS: the two 2 px edges of the top half, per side
	if( SnowGroundOn ){
		int& votes = SnowDecOf[ (size_t)row * 112 + col ];
		if( votes == SNOW_DEC_NOT_YET ) votes = SnowCellVotes( row, col );
		snowVotes = votes;
		const int kind = SnowCellKind( row, col );
		stripL = SnowStrip( kind, row, col, 0 ); stripR = SnowStrip( kind, row, col, 1 );
	}
	GroundJob j;	// (th4) THE JOB LINES (GROUND RECORDING, above): the rest of the cell's recording, as a job
	j.x = x; j.y = y; j.L = Screen_LeftBorder; j.T = Screen_TopBorder; j.gw = SunGW; j.gh = SunGH;
	j.snowOn = SnowGroundOn; j.grassOn = GrassOn; j.river = river; j.snowHouse = snowHouse; j.grassOnly = grassOnly; j.snowRockFloor = snowRockFloor;
	j.stripL = stripL; j.stripR = stripR; j.snowVotes = snowVotes;
	j.artSlot[ 0 ] = j.artSlot[ 1 ] = j.grassIdx[ 0 ] = j.grassIdx[ 1 ] = 0;
	// (th4) SNOW GROUND -- THE ART: the floor's own palette entries (SnowArtSlot), for the snow's light (SnowGround.hlsl:
	// finished luminance / the art's) and the water test (kWaterEntries). They equal the frame's, in the town and in
	// Ravenholm (FULLY LIT: thehell2.cpp LoadQuestDungeonMainMapTown sets FullLighting, DarkLevel 0 on every cell).
	if( SnowGroundOn ){
		const TileColumn& t = TileMap[ IsometricMapOffset( row, col ) ];
		j.artSlot[ 0 ] = SnowArtSlot( t.Layer[ 0 ].Left ); j.artSlot[ 1 ] = SnowArtSlot( t.Layer[ 0 ].Right );
	}
	// (th4) GRASS (above): the grass classes of this cell's two floor pieces
	if( GrassOn ){
		const TileColumn& t = TileMap[ IsometricMapOffset( row, col ) ];
		j.grassIdx[ 0 ] = GrassArtIndex( t.Layer[ 0 ].Left ); j.grassIdx[ 1 ] = GrassArtIndex( t.Layer[ 0 ].Right );
	}
	j.box = 0;	// the live frame
#if TH4_JOBLINES_VERIFY
	if( VerifyThis && VGroundReady ){ GroundRec_Run( j, GroundOut{ VGround[ 0 ], VGround[ 1 ], VGround[ 2 ], VGround[ 3 ], VGround[ 4 ] } ); ++GroundJobsNow; }	// the reference
#endif
	if( !JOB_LINES ){ GroundLine().Inline( j ); return; }
	// what GroundRec_Run reads of the frame: rows y - 31 .. y inside the layer, x .. x + 63 cut to it (its runs never leave that)
	const int cxa = max( j.L, x ), cxb = min( j.L + SunGW, x + 64 );
	if( cxa >= cxb ) return;	// no row reaches the layer: GroundRec_Run would write nothing
	uchar* const box = JobPool_Alloc( 64 * 32 );
	if( !box ){ GroundLine().Inline( j ); return; }
	for( int k = 0; k < 32; ++k ){
		const int yy = y - 31 + k, py = yy - j.T;
		if( py < 0 || py >= SunGH ) continue;	// GroundRec_Run skips the row too
		memcpy( box + k * 64 + ( cxa - x ), WorkingSurface + YOffsetHashTable[ yy ] + cxa, cxb - cxa );
	}
	j.box = box;
	GroundLine().Post( j );
}
static void GroundRec_Join(){ GroundLine().Join(); }
static void JobLines_Join(){ TreeLine().Join(); GroundLine().Join(); }
// (th4) THE JOB LINES: a new frame (TownWater_BeginFrame) -- both lines done, their jobs and the pool free again
static void JobLines_FrameStart()
{
	TreeLine().Reset(); GroundLine().Reset();
	JobPoolCur = 0; JobPoolOfs = 0;
#if TH4_JOBLINES_VERIFY
	VSwayPixN = 0; VLeafN = 0; VSwayCells.clear(); VSwayAfter.clear();
	TreeJobsNow = GroundJobsNow = 0;
	VerifyThis = ( ++VerifyTick & 7 ) == 0;
	VGroundReady = false;
#endif
}
#if TH4_JOBLINES_VERIFY
// the shadow layers start as the real ones (TownWater_BeginFrame, once the layers are ready for the frame)
static void GroundVerify_Start()
{
	if( !VerifyThis || !SunGW || !SunGH || !( SunGroundOn || GrassOn || SnowGroundOn ) ) return;
	uchar* const real[ 5 ] = { GroundVal, GroundMark, SnowArtLay, SnowDec, GrassCls };
	const size_t n = (size_t)SunGW * SunGH;
	if( VGroundW != SunGW || VGroundH != SunGH ){
		VGroundW = VGroundH = 0;
		for( uchar*& v : VGround ){ free( v ); v = (uchar*)malloc( n ); if( !v ) return; }
		VGroundW = SunGW; VGroundH = SunGH;
	}
	for( int i = 0; i < 5; ++i ) if( !real[ i ] ) return;
	for( int i = 0; i < 5; ++i ) memcpy( VGround[ i ], real[ i ], n );
	VGroundReady = true;
}
// the worker's layers against the inline recording's (one result per checked frame with floor cells)
static void GroundVerify_Frame()
{
	if( !VGroundReady || !GroundJobsNow ) return;
	const uchar* const real[ 5 ] = { GroundVal, GroundMark, SnowArtLay, SnowDec, GrassCls };
	const size_t n = (size_t)SunGW * SunGH;
	bool same = true;
	for( int i = 0; i < 5 && same; ++i ) same = !memcmp( VGround[ i ], real[ i ], n );
	++GroundVerifyFrames; if( !same ) ++GroundVerifyBad;
}
// the counts, every 256 checked frames and at once on a mismatch, as one line of TH4_joblines.log
static void JobLines_VerifyLog()
{
	static long long lastFrames = 0, lastBad = 0;
	const long long frames = TreeVerifyFrames + GroundVerifyFrames, bad = TreeVerifyBad + GroundVerifyBad;
	if( frames - lastFrames < 256 && bad == lastBad ) return;
	lastFrames = frames; lastBad = bad;
	if( FILE* f = fopen( "TH4_joblines.log", "a" ) ){
		fprintf( f, "tree scan: %lld frames compared, %lld mismatched; ground recording: %lld frames compared, %lld mismatched\n",
			TreeVerifyFrames, TreeVerifyBad, GroundVerifyFrames, GroundVerifyBad );
		fclose( f );
	}
}
#endif
// after the zoom: pre-zoom pixel (x, y) of the finished frame is frame[ y z P + x z ]
static void BuildSunGround( const uchar* frame, int z )
{
	SunGroundValid = false;
	if( !SunGroundOn || !SunGround ) return;
	const int P = WorkingWidth;
	for( int y = 0; y < SunGH; ++y ){
		uchar* g = SunGround + (size_t)y * SunGW;
		const uchar* val = GroundVal + (size_t)y * SunGW;
		const uchar* mark = GroundMark + (size_t)y * SunGW;
		if( y * z >= ScreenHeight ){ memset( g, 0, SunGW ); continue; }
		const uchar* fr = frame + y * z * P;
		for( int x = 0; x < SunGW; ++x ) g[ x ] = ( ( mark[ x ] == 1 || mark[ x ] == 2 ) && x * z < ScreenWidth && fr[ x * z ] == val[ x ] ) ? ( mark[ x ] == 2 ? SUN_GROUND_WATER : SUN_GROUND_YES ) : 0;	// not 3: the grass's walk-blocked cells (GRASS)
	}
	// crowns that let no light through (kNoSunSquares): their pixels still showing get SUN_GROUND_NO_SOURCE
	// (a crown pixel is never ground, so the two codes never meet)
	for( const SwayCell& c : SwayCells ){
		if( !c.noSun ) continue;
		for( size_t k = c.first; k < c.first + c.count; ++k ){
			const SwayPx& q = SwayPix[ k ];
			const int px = q.rel & 0xFFFF, py = q.rel >> 16;
			if( px >= SunGW || py >= SunGH || px * z >= ScreenWidth || py * z >= ScreenHeight || frame[ py * z * P + px * z ] != q.val ) continue;
			SunGround[ (size_t)py * SunGW + px ] = SUN_GROUND_NO_SOURCE;
		}
	}
	SunGroundValid = true;
}
// (th4) SUN GROUND: this frame's ground layer for dx_utility.cpp SunRays_Present; false = none.
bool TownWater_SunGround( const uchar*& g, int& w, int& h )
{
	g = SunGround; w = SunGW; h = SunGH;
	return SunGroundValid;
}
// (th4) SNOW GROUND RULES (SNOW GROUND above): which recorded floor pixels get snow -- SnowLay = the art's entry there, 0 = none.
// Like the GRASS it goes by the ART's palette entries, three lists measured offline on the whole town (TH4data's
// NLevels\TownData\Town.CEL laid out by Sector1s..4s.DUN, NLevels\TownData\Town.SOL): for every entry, its share among
// the pixels the cells' upper layers draw (walls, roofs, rock tops, fences) against its share among the floor of open
// ground (walkable cells whose 8 neighbours are walkable too), each share taken of its own set's total -- the "score",
// 0 = only ever ground, 1 = only ever structure. A NEW TOWN ART (another Town.CEL) needs all three lists measured
// again, like kGrassCore.
// THE VOTES (SnowCellVotes), per floor pixel over the 5 x 5 window around it, with the marks RecordGround gives:
//   OPEN GROUND (mark 1) is snow, except THE RIVER: more than SNOW_WATER_MAX % of the window is frozen water (mark 2)
//     -- a speck of the water's texture that is not a water entry, not the bank (they were light blue dots all over the
//     frozen river). Tried and rejected: judging open pixels beside a walk-blocked cell by the rock vote -- it drew thin
//     dark lines along those cells' edges.
//   WATER SPECKS ON THE BANK -- NO BLACK DOTS (Mordor, 2026-10-10: "leave a trace so YOU DON'T DO THAT AGAIN"): the
//     reverse case. A river tile's bank art uses the water's colours too (the bend tree's baked shadow, tile 96, is full
//     of them), and every such pixel was marked water (RecordGround: WaterIdx ? 2 : 1) -- never snowed, and not water
//     either (the river's own mask drops lone grains), so it stayed as bare dark art: single near-black dots in the snow
//     of the bank's shadows, by day and by night. Now a water pixel with less than SNOW_SPECK_WATER % water in its 5 x 5
//     window is BANK (mark 1) and takes the bank's rules -- snow, or bare inside THE RIVER'S RIM like the pixels around
//     it. Measured on the whole town: 2.5 % of the water-coloured pixels, exactly the dot pattern; the real water's edge
//     keeps 40 %+ (a few edge pixels flip, inside the rim, where they stay bare art as before). A debug view proved the
//     dots were holes in this layer (TH4_SNOWDOTS_DEBUG, removed); the snow's light was not the cause.
//   ROCKS IN THE FLOOR (mark 3, walk-blocked cells): the lower half of a rock (a post, the well) is painted INTO the
//     floor tile, so snowing every visible floor pixel buried the rocks' bottoms and left their tops sticking out
//     (Mordor: "look at this circus"): a pixel is ground where at least SNOW_ROCK_VOTE % of the window's walk-blocked
//     pixels are NOT kSnowRockEntries (the entries lying on walk-blocked cells' floors for 60..99 % of their pixels;
//     ground entries 3..12 %, the baked shadows 20..50 %). A per-pixel test alone left white specks in mixed floors;
//     the vote cleans them. Mordor called the fence "perfect" and the abyss pit "frozen solid, don't change it", and
//     when a later round changed this rule he had it RESTORED: walk-blocked cells keep exactly this vote, THE STRIPS
//     (SnowStrip) and no SHOWS test, which reproduces that look to the pixel (checked offline on the whole town).
//     Tried and rejected: counting the open pixels in this vote too (it reshaped the fence's snow).
//   HOUSE CELLS (mark 4, RecordGround): walls, roofs, porches and, at the edges, ground. Colour alone cannot tell a
//     wall in shade from ground in shade (the church's dark brick uses the lawns' shadow entries, kGrassShade), so a
//     house pixel needs POSITIVE evidence of ground: in the window's house pixels at least SNOW_HOUSE_GROUND %
//     kSnowGroundEntries (lit greens and browns, score <= 0.15) and at most SNOW_HOUSE_WALL % structure
//     (kSnowRockEntries + kSnowWallEntries, score >= 0.75; the shadow entries are neither). Tried and rejected: the
//     rock-style vote with shadows counted as ground (whole diamonds of the church's dark wall turned light blue);
//     counting the darkest shadow entries as wall (snow patches on the tower's lit stone); snow creeping up from the
//     ground below ("drifts": flat-coloured blocks and stripes under Adria's hut on its stilts). What stays bare: dark
//     stone with no ground colour in it -- the catacombs' entrance, the smithy's floor slab.
// The votes depend only on the map and its art, never on the frame, so they are worked out ONCE per map cell, on its
// first draw (like the PUDDLES), over the cell and its 8 neighbours; identical sets of votes are stored once (the town
// has 2460 mixed cells, 505 distinct). Per frame, 250 000 .. 570 000 pixels of a 1080p frame would each have to look
// at 25 others (counted offline; the time was not measured). A new FineMap (quest tile swaps: the crypt, the abyss) or
// new level art starts them over (SnowVotesEnsure).
// PER FRAME (BuildSnowGround, SSE2; z = 2, the old Alt+Z zoom, takes the scalar loops): a pixel gets snow when its
// cell voted it ground (SnowDec) and it SHOWS: the finished frame still has the recorded colour AND at least
// SNOW_SPECK_MIN of its 8 neighbours do too -- a wall that happens to repeat the colour of the floor behind it shows
// single pixels, which were white dots all over the church tower and the walls. Walk-blocked pixels skip that test
// (ROCKS IN THE FLOOR: their look is the restored one). Each SSE2 loop has a scalar tail that
// is its reference.
namespace {
const uchar kSnowRockEntries[] = { 0, 13, 16, 19, 20, 21, 22, 24, 27, 28, 31, 33, 35, 36, 37, 38, 41, 43, 44, 46, 50, 51, 53,
                                   59, 64, 72, 75, 79, 80, 84, 91, 99, 101, 107, 110, 118, 119, 121, 124, 126, 127 };
const uchar kSnowWallEntries[] = { 1, 3, 6, 7, 11, 14, 18, 23, 26, 32, 40, 45, 52, 62, 63, 78, 81, 86, 92, 94, 96, 98, 106,
                                   108, 120, 123, 125 };	// the other structure entries (score >= 0.75): walls, roofs, thatch
const uchar kSnowGroundEntries[] = { 8, 12, 15, 17, 49, 54, 56, 57, 66, 67, 69, 71, 83, 85, 90, 93, 102, 104, 105 };	// score <= 0.15
const int SNOW_ROCK_R = 2;				// every vote looks at a 5 x 5 window
const int SNOW_ROCK_VOTE = 60;			// ROCKS IN THE FLOOR: at least 60 % not rock
const int SNOW_HOUSE_GROUND = 35;		// HOUSE CELLS: at least 35 % ground entries ...
const int SNOW_HOUSE_WALL = 40;			// ... and at most 40 % structure entries
const int SNOW_WATER_MAX = 50;			// THE RIVER: more than 50 % water around = a speck in the ice
const int SNOW_SPECK_WATER = 35;		// WATER SPECKS ON THE BANK: less than 35 % water around = a speck on the bank
const int SNOW_SPECK_MIN = 3;			// SHOWS: at least 3 of the 8 neighbours show their floor too
const int SNOW_LONE_MIN = 2;			// NO LONE SNOW: at least 2 of the 8 neighbours have snow too
// THE RIVER'S RIM (SnowCellVotes): open ground (mark 1) with at least SNOW_RIM_WATER frozen-water pixels (mark 2) in the
// (2 SNOW_RIM_X + 1) x (2 SNOW_RIM_Y + 1) box around it gets no snow, so the light frosty bank art right at the water shows
// between the water and the snow, as the river looked before the snow (v0.766; Mordor). A count, not "any water": single
// water-coloured specks on the banks punched dark ovals. In the per-cell votes, not per frame: it depends on the map
// only, and per frame it cost 6..7 ms of CPU at 1056 x 594 (measured 2026-10-09).
const int SNOW_RIM_X = 7, SNOW_RIM_Y = 3, SNOW_RIM_WATER = 12;
const int SNOW_CRUMB_MIN = 8;			// NO SNOW CRUMBS ON HOUSES: at least 8 snow pixels in the 5 x 5 window
enum { SE_ROCK = 1, SE_WALL = 2, SE_GROUND = 4 };
uchar SnowEntry[ 256 ];					// SE_* bits of every palette entry, from the three lists
std::unordered_map<unsigned long long, int> SnowDecShared;	// a hash of a set of votes -> its offset in SnowDecPool
std::vector<int> SnowArtIdx;			// floor tile id (as TileMap holds it) -> its slot in SnowArt, -1 = not decoded yet
std::vector<uchar> SnowArt;				// 2048 bytes per slot: the 32 x 32 entries (row 0 = the BOTTOM row), then opaque flags
const int* SnowArtCel = 0; int SnowArtDun = -1;	// what the art and the votes were worked out for
uint SnowMapSig = 0;
std::vector<uchar> SnowVis;				// per pre-zoom pixel: 1 = recorded floor that the finished frame still shows
std::vector<uchar> SnowRow;				// one row of column sums with 1 px of margin each side (scratch)
std::vector<uchar> SnowRow5;			// the same for 5 rows, 2 px of margin each side (step 4)
}

// (th4) SNOW GROUND: row k (0..31, from the top) of the DRAWN floor diamond covers x 32 - half .. 32 + half - 1;
// RecordGround's layout (SnowWideHalf) is 2 px wider per side in the top half (k < 16): THE STRIPS
static inline int SnowDiamondHalf( int k ) { return k < 16 ? 2 * k : 2 * ( 32 - k ); }
static inline int SnowWideHalf( int k ) { return k < 16 ? 2 * ( k + 1 ) : 2 * ( 32 - k ); }

// (th4) SNOW GROUND: how RecordGround marks map cell (r, c)'s floor for the snow -- 0 not recorded, 1 open ground
// (walkable, tree), 2 a river cell (per pixel: 2 water, 1 bank), 3 walk-blocked, 4 a house cell. Keep the two alike.
static int SnowCellKind( int r, int c )
{
	if( (uint)r >= 112 || (uint)c >= 112 ) return 0;
	const int piece = FineMap[ r ][ c ];
	if( piece <= 0 || piece >= MAX_PIECE ) return 0;
	const bool tree = TreePiece[ piece ], river = RiverPiece[ piece ];
	const bool grassOnly = !GroundCell( r, c ) && !tree && !river;
	if( river ) return 2;
	if( HutLeafPiece[ piece ] || ( grassOnly && piece < 2049 && TileBlockSight[ piece ] ) ) return 4;
	if( grassOnly ) return piece < 2049 ? 3 : 0;
	if( SnowRockFloorPiece[ piece ] ) return 3;	// (th4) ROCK FLOORS: walkable, but judged as rock (RecordGround marks it 3 too)
	return 1;
}

// (th4) SNOW GROUND -- THE STRIPS: RecordGround's diamond is 2 px wider per side in its top half than the floor the
// cell DREW; each strip lies on the lower edge of the cell behind on that side (side 0 = left, cell (r - 1, c); side 1
// = right, cell (r, c - 1)). Under the snow a strip is recorded only by a walk-blocked cell, or by a cell beside a
// walk-blocked cell behind: that is exactly what the fence, the rocks and the abyss pit had when Mordor called them
// right, so they keep their look to the pixel. Elsewhere the strips held the lower edge of a house's wall or roof and
// made thin white lines (the line across the church tower, Ogden's roof). A house cell never records them.
static bool SnowStrip( int kind, int r, int c, int side )
{
	if( kind == 3 ) return true;
	if( kind == 4 || !kind ) return false;
	return SnowCellKind( side ? r : r - 1, side ? c - 1 : c ) == 3;
}

// (th4) SNOW GROUND: a floor tile's art, decoded once (GrassDecodePiece) into its slot in SnowArt. Take pointers
// (SnowArtPtr) only after every SnowArtSlot call of the moment: decoding grows SnowArt.
static int SnowArtSlot( ushort id )
{
	int& slot = SnowArtIdx[ id ];
	if( slot >= 0 ) return slot;
	GroundRec_Join();	// (th4) THE JOB LINES: SnowArt grows below -- no ground job may still be reading it
	slot = (int)( SnowArt.size() / 2048 );
	SnowArt.resize( SnowArt.size() + 2048 );	// zeroed: no frame = nothing opaque
	uchar* p = &SnowArt[ (size_t)slot * 2048 ];
	if( id ) GrassDecodePiece( id, p, p + 1024 );
	return slot;
}
static inline const uchar* SnowArtPtr( int slot ) { return &SnowArt[ (size_t)slot * 2048 ]; }

// (th4) SNOW GROUND -- ADRIA'S SIDE: smooth value noise, -1 .. 1, two octaves, cells HUT_EDGE_CELL px wide and half as
// tall (flat on the 2 : 1 ground); a pure function of the position, so the edge never changes
static float HutHash( int ix, int iy )
{
	uint x = (uint)ix * 374761393u + (uint)iy * 668265263u;
	x = ( x ^ ( x >> 13 ) ) * 1274126177u;
	return ( ( x ^ ( x >> 16 ) ) & 0xFFFF ) / 65535.0f;
}
static float HutNoise( float x, float y )
{
	const int ix = (int)floorf( x ), iy = (int)floorf( y );
	const float fx = x - ix, fy = y - iy, sx = fx * fx * ( 3 - 2 * fx ), sy = fy * fy * ( 3 - 2 * fy );
	const float a = HutHash( ix, iy ) + ( HutHash( ix + 1, iy ) - HutHash( ix, iy ) ) * sx;
	const float b = HutHash( ix, iy + 1 ) + ( HutHash( ix + 1, iy + 1 ) - HutHash( ix, iy + 1 ) ) * sx;
	return a + ( b - a ) * sy;
}
static float HutBlotchC( float X, float Y, float c )	// the same with cells c px wide
{
	const float n = 0.65f * HutNoise( X / c, Y / c * 2 ) + 0.35f * HutNoise( X / ( c * 0.5f ) + 17.3f, Y / ( c * 0.5f ) * 2 + 5.1f );
	return n * 2 - 1;
}
static float HutBlotch( float X, float Y )
{
	const float c = HUT_EDGE_CELL;
	const float n = 0.65f * HutNoise( X / c, Y / c * 2 ) + 0.35f * HutNoise( X / ( c * 0.5f ) + 17.3f, Y / ( c * 0.5f ) * 2 + 5.1f );
	return n * 2 - 1;
}

// (th4) SNOW GROUND -- THE VOTES of map cell (r, c) (above): SNOW_DEC_ALL / SNOW_DEC_NONE, or the offset of its
// SNOW_DEC_SIZE votes in SnowDecPool (RecordGround's layout, row by row: SnowRowStart)
static int SnowCellVotes( int r, int c )
{
	int kinds[ 9 ];							// the cell and its 8 neighbours, [ ( dr + 1 ) * 3 + dc + 1 ]
	bool plain = SnowCellKind( r, c ) == 1;	// open ground with no river cell around: no vote can say no
	for( int n = 0; n < 9; ++n ){
		kinds[ n ] = SnowCellKind( r + n / 3 - 1, c + n % 3 - 1 );
		if( kinds[ n ] == 2 ) plain = false;
	}
	if( !kinds[ 4 ] || SnowNonePiece[ FineMap[ r ][ c ] ] ) return SNOW_DEC_NONE;	// BARE FLOORS
	bool lineNear[ SNOW_LINE_N ], anyLine = false;	// SNOW LINES: cells near an anchor are always voted
	for( int l = 0; l < SNOW_LINE_N; ++l ){
		lineNear[ l ] = SnowLineRow[ l ] >= 0 && abs( r - SnowLineRow[ l ] ) <= SNOW_LINE_REACH && abs( c - SnowLineCol[ l ] ) <= SNOW_LINE_REACH;
		anyLine |= lineNear[ l ];
	}
	if( ( plain && !anyLine ) || SnowAllPiece[ FineMap[ r ][ c ] ] ) return SNOW_DEC_ALL;	// SNOW FLOORS: all ground, whatever the votes
	// THE NEIGHBOURHOOD: the marks and entries RecordGround leaves around this cell's diamond, in a box of the diamond
	// + R px, worked out the way the game makes them: the floors (layer 0) of the 5 x 5 cells around are painted in
	// DRAW ORDER (DrawSceneTownTop: by row + col, then row) -- a floor piece is often a whole 32 x 32 square, not a
	// diamond, so it paints over its neighbours' edges -- and right after each of the 3 x 3 cells is painted, the
	// pixels it records (its drawn diamond + THE STRIPS it keeps) take its mark and the colour painted there by then.
	// Cell (r + dr, c + dc)'s floor pieces lie at x 32 (dr - dc) .. + 63, y 16 (dr + dc) - 31 .. + 0 from this cell's
	// DrawTileFill dst; box (bx, by) = that offset + ( R, 31 + R ). The upper layers never reach a floor recorded later.
	// The box's margins: MX / MY px left-right / above (the votes' R, THE RIVER'S RIM's wider window), below HOUSE EDGES
	const int R = SNOW_ROCK_R, MX = max( SNOW_ROCK_R, SNOW_RIM_X ), MY = max( SNOW_ROCK_R, SNOW_RIM_Y );
	const int BW = 64 + 2 * MX, BH = 32 + MY + max( SNOW_ROCK_R, max( SNOW_RIM_Y, HOUSE_EDGE_BELOW ) );
	uchar bm[ BW * BH ] = {}, bv[ BW * BH ] = {}, paint[ BW * BH ] = {};
	int order[ 25 ], slot[ 25 ][ 2 ];		// [ ( dr + 2 ) * 5 + dc + 2 ]
	for( int n = 0; n < 25; ++n ){			// every decode first: it grows SnowArt
		order[ n ] = n; slot[ n ][ 0 ] = slot[ n ][ 1 ] = -1;
		const int rr = r + n / 5 - 2, cc = c + n % 5 - 2;
		if( (uint)rr >= 112 || (uint)cc >= 112 || FineMap[ rr ][ cc ] <= 0 ) continue;
		const TileColumn& t = TileMap[ IsometricMapOffset( rr, cc ) ];
		slot[ n ][ 0 ] = SnowArtSlot( t.Layer[ 0 ].Left ); slot[ n ][ 1 ] = SnowArtSlot( t.Layer[ 0 ].Right );
	}
	auto drawKey = []( int n ){ const int dr = n / 5 - 2, dc = n % 5 - 2; return ( dr + dc ) * 16 + dr; };
	std::sort( order, order + 25, [&]( int a, int b ){ return drawKey( a ) < drawKey( b ); } );
	for( int q = 0; q < 25; ++q ){
		const int n = order[ q ];
		if( slot[ n ][ 0 ] < 0 ) continue;
		const int dr = n / 5 - 2, dc = n % 5 - 2, ox = 32 * ( dr - dc ) + MX, oy = 16 * ( dr + dc ) + MY;
		for( int side = 0; side < 2; ++side ){	// paint the floor's two pieces where they are opaque
			const uchar* art = SnowArtPtr( slot[ n ][ side ] );
			for( int k = 0; k < 32; ++k ){
				const int by = oy + k;
				if( by < 0 || by >= BH ) continue;
				const uchar* pix = art + ( 31 - k ) * 32; const uchar* opq = pix + 1024;
				for( int xr = max( 0, -ox - 32 * side ); xr < min( 32, BW - ox - 32 * side ); ++xr ){
					if( opq[ xr ] ) paint[ by * BW + ox + 32 * side + xr ] = pix[ xr ];
				}
			}
		}
		if( dr < -1 || dr > 1 || dc < -1 || dc > 1 ) continue;
		const int kind = kinds[ ( dr + 1 ) * 3 + dc + 1 ];
		if( !kind ) continue;
		const bool strip[ 2 ] = { SnowStrip( kind, r + dr, c + dc, 0 ), SnowStrip( kind, r + dr, c + dc, 1 ) };
		for( int k = 0; k < 32; ++k ){			// what it records: RecordGround's rows, its strips cut as there
			const int by = oy + k;
			if( by < 0 || by >= BH ) continue;
			int xa = 32 - SnowWideHalf( k ), xb = 32 + SnowWideHalf( k );
			if( k < 16 ){ if( !strip[ 0 ] ) xa += 2; if( !strip[ 1 ] ) xb -= 2; }
			for( int xr = max( xa, -ox ); xr < min( xb, BW - ox ); ++xr ){
				const int i = by * BW + ox + xr;
				bv[ i ] = paint[ i ];
				bm[ i ] = (uchar)( kind == 2 ? ( WaterIdx[ paint[ i ] ] ? 2 : 1 ) : kind );
			}
		}
	}
	// the votes of this cell's pixels, RecordGround's layout row by row (SnowRowStart); pixels this cell does not
	// record (a strip it leaves out) get a vote too, which RecordGround never copies
	uchar votes[ SNOW_DEC_SIZE ];
	int o = 0, ground = 0;
	const int wellQuad = WellQuad[ FineMap[ r ][ c ] ];	// THE WELL: this cell's offset in the well's tile
	const int wdr = wellQuad == 2 || wellQuad == 4, wdc = wellQuad == 3 || wellQuad == 4;
	const bool hutSide = HutSidePiece[ FineMap[ r ][ c ] ] && HutAnchorRow >= 0;	// ADRIA'S SIDE
	const bool houseEdge = HouseEdgeCell[ r ][ c ];				// HOUSE EDGES
	const int pdr = r - PepAnchorRow, pdc = c - PepAnchorCol;
	const bool pepFront = houseEdge && PepAnchorRow >= 0 && abs( pdr ) <= 6 && abs( pdc ) <= 8;	// PEPIN'S FRONT: his cells only
	auto edgeVote = [&]( int bx, int by ){						// THE EDGE's art vote over open, walk-blocked and house pixels
		int nn = 0, g = 0;
		for( int yy = by - R; yy <= by + R; ++yy ) for( int xx = bx - R; xx <= bx + R; ++xx ){
			const uchar mm = bm[ yy * BW + xx ];
			if( mm != 1 && mm != 3 && mm != 4 ) continue;
			++nn; g += !( SnowEntry[ bv[ yy * BW + xx ] ] & ( SE_ROCK | SE_WALL ) );
		}
		return nn && g * 100 >= HUT_EDGE_VOTE * nn;
	};
	const int hdr = r - HutAnchorRow, hdc = c - HutAnchorCol;
	for( int k = 0; k < 32; ++k ){
		const int half = SnowWideHalf( k ), by = MY + k;
		for( int bx = MX + 32 - half; bx < MX + 32 + half; ++bx ){
			uchar m = bm[ by * BW + bx ];
			if( m == 2 ){								// WATER SPECKS ON THE BANK (NO BLACK DOTS, above): a speck is bank
				int n = 0, a = 0;
				for( int yy = by - R; yy <= by + R; ++yy ) for( int xx = bx - R; xx <= bx + R; ++xx ){
					const uchar mm = bm[ yy * BW + xx ];
					if( !mm ) continue;
					++n; a += mm == 2;
				}
				if( a * 100 < SNOW_SPECK_WATER * n ) m = 1;
			}
			bool yes = false;
			int well = -1;								// THE WELL SHAPE, in tile coordinates: 0 bare, 1 snow, -1 not in it
			if( wellQuad ){
				const float X = (float)( bx - MX + 32 * ( wdr - wdc ) ) + 0.5f, Y = (float)( k - 31 + 16 * ( wdr + wdc ) ) + 0.5f;
				const float ex = ( X - WELL_CX ) / WELL_RX, ey = ( Y - WELL_CY ) / WELL_RY;
				if( ( Y < WELL_CY && X >= WELL_X0 && X < WELL_X1 ) || ex * ex + ey * ey <= 1.0f ){
					const float wx = ( X - WELL_WALL_CX ) / WELL_WALL_RX, wallRy = WELL_RY * WELL_WALL_RX / WELL_RX;
					well = ( fabsf( wx ) < 1.0f && Y < WELL_CY - WELL_SNOW_DEPTH + wallRy * sqrtf( 1.0f - wx * wx ) ) ? 0 : 1;
				}
			}
			if( hutSide ){								// ADRIA'S SIDE: below the wall's base line, left of the corner; THE EDGE
				const float X = (float)( bx - MX + 32 * ( hdr - hdc ) ) + 0.5f, Y = (float)( k - 31 + 16 * ( hdr + hdc ) ) + 0.5f;
				if( X <= HUT_CORNER_X ){
					const float d = Y - ( HUT_CORNER_Y + ( X - HUT_CORNER_X ) * 0.5f ) + HUT_EDGE_WOBBLE * HutBlotch( X, Y );
					if( d > HUT_EDGE_BAND ) well = 1;
					else if( d < -HUT_EDGE_BAND ) well = 0;
					else{
						int n = 0, g = 0;
						for( int yy = by - R; yy <= by + R; ++yy ) for( int xx = bx - R; xx <= bx + R; ++xx ){
							if( bm[ yy * BW + xx ] != 4 ) continue;
							++n; g += !( SnowEntry[ bv[ yy * BW + xx ] ] & ( SE_ROCK | SE_WALL ) );
						}
						well = n && g * 100 >= HUT_EDGE_VOTE * n;
					}
				}
			}
			if( pepFront && ( m == 3 || m == 4 ) ){		// PEPIN'S FRONT
				const float X = (float)( bx - MX + 32 * ( pdr - pdc ) ) + 0.5f, Y = (float)( k - 31 + 16 * ( pdr + pdc ) ) + 0.5f;
				if( X >= PEP_DOOR_X0 && X < PEP_DOOR_X1 && Y < PEP_DOOR_FOOT ) well = 0;
				else if( X >= PEP_DOOR_X1 && X <= PEP_WALL_X1 ){
					const float d = Y - ( PEP_WALL_Y0 + ( X - PEP_DOOR_X1 ) * PEP_WALL_SLOPE ) + PEP_FRONT_WOBBLE * HutBlotch( X, Y );
					well = d > PEP_FRONT_BAND ? 1 : d < -PEP_FRONT_BAND ? 0 : edgeVote( bx, by );
				}else if( m == 3 ) well = edgeVote( bx, by );
			}
			for( int l = 0; l < SNOW_LINE_N && anyLine && ( m == 1 || m == 3 || m == 4 ); ++l ){	// SNOW LINES
				if( !lineNear[ l ] ) continue;
				const int ldr = r - SnowLineRow[ l ], ldc = c - SnowLineCol[ l ];
				const float X = (float)( bx - MX + 32 * ( ldr - ldc ) ), Y = (float)( k - 31 + 16 * ( ldr + ldc ) );
				const float ( *pts )[ 2 ] = kSnowLines[ l ].pts; const int nPts = kSnowLines[ l ].n;
				if( X < pts[ 0 ][ 0 ] || X > pts[ nPts - 1 ][ 0 ] ) continue;
				float L = pts[ 0 ][ 1 ];
				for( int q = 0; q + 1 < nPts; ++q ){
					if( X >= pts[ q ][ 0 ] && X <= pts[ q + 1 ][ 0 ] ){ L = pts[ q ][ 1 ] + ( pts[ q + 1 ][ 1 ] - pts[ q ][ 1 ] ) * ( X - pts[ q ][ 0 ] ) / ( pts[ q + 1 ][ 0 ] - pts[ q ][ 0 ] ); break; }
				}
				if( kSnowLines[ l ].cow ){						// THE CHAOS
					if( fabsf( Y - L ) > SNOW_COW_BAND ) continue;
					float d = Y - ( L + SNOW_COW_DROP ) + SNOW_LINE_WOBBLE * HutBlotch( X, Y );
					d -= SNOW_COW_BITE * max( 0.0f, HutBlotchC( X + 37, Y + 11, 10.0f ) );
					bool snow = d > 0;
					if( snow && d < SNOW_COW_REACH && HutBlotchC( X + 211, Y + 73, 4.0f ) > SNOW_COW_SPOT + ( 1 - SNOW_COW_SPOT ) * d / SNOW_COW_REACH ) snow = false;
					well = snow ? 1 : 0;
					continue;
				}
				if( kSnowLines[ l ].edge ){						// ADRIA'S EDGE (above)
					const float d = Y - L + HUT_EDGE_WOBBLE * HutBlotch( X, Y );
					if( fabsf( d ) <= HUT_EDGE_BAND ) well = edgeVote( bx, by ) ? 1 : 0;
					else if( fabsf( d ) <= SNOW_LINE_BAND ) well = d > 0 ? 1 : 0;
					continue;
				}
				const float d = Y - L + SNOW_LINE_WOBBLE * HutBlotch( X, Y );
				if( fabsf( d ) <= SNOW_LINE_BAND ) well = d > 0 ? 1 : 0;
			}
			if( well >= 0 ) yes = well == 1;
			else if( m == 1 || m == 3 || m == 4 ){
				int n = 0, a = 0, b = 0;
				for( int yy = by - R; yy <= by + R; ++yy ){
					for( int xx = bx - R; xx <= bx + R; ++xx ){
						const uchar mm = bm[ yy * BW + xx ], e = SnowEntry[ bv[ yy * BW + xx ] ];
						if( m == 1 ){				// n recorded, a water
							if( !mm ) continue;
							++n; a += mm == 2;
						}else if( mm == m ){		// the cell kind's own pixels: a not rock (3) / ground (4), b structure (4)
							++n;
							if( m == 3 ) a += !( e & SE_ROCK );
							else{ a += ( e & SE_GROUND ) != 0; b += ( e & ( SE_ROCK | SE_WALL ) ) != 0; }
						}
					}
				}
				if( m == 1 ) yes = a * 100 <= SNOW_WATER_MAX * n;									// THE RIVER
				else if( m == 3 ) yes = n && a * 100 >= SNOW_ROCK_VOTE * n;							// ROCKS IN THE FLOOR
				else yes = n && a * 100 >= SNOW_HOUSE_GROUND * n && b * 100 <= SNOW_HOUSE_WALL * n;	// HOUSE CELLS
				if( m == 4 && !yes && houseEdge ){				// HOUSE EDGES: a wall's foot over open ground
					bool below = false;
					for( int d = 1; d <= HOUSE_EDGE_BELOW && !below; ++d ){
						const int yy = by + d;
						if( yy >= BH ) break;
						for( int dx = -2; dx <= 2; dx += 2 ) if( bx + dx >= 0 && bx + dx < BW && bm[ yy * BW + bx + dx ] == 1 ) below = true;
					}
					if( below ){
						int nn = 0, g = 0;
						for( int yy = by - R; yy <= by + R; ++yy ) for( int xx = bx - R; xx <= bx + R; ++xx ){
							const uchar mm = bm[ yy * BW + xx ];
							if( mm != 1 && mm != 4 ) continue;
							++nn; g += !( SnowEntry[ bv[ yy * BW + xx ] ] & ( SE_ROCK | SE_WALL ) );
						}
						yes = nn && g * 100 >= HUT_EDGE_VOTE * nn;
					}
				}
			}
			if( yes && m == 1 ){							// THE RIVER'S RIM (SNOW_RIM_*): open ground beside a stretch of water
				int w = 0;
				for( int yy = by - SNOW_RIM_Y; yy <= by + SNOW_RIM_Y; ++yy ) for( int xx = bx - SNOW_RIM_X; xx <= bx + SNOW_RIM_X; ++xx ) w += bm[ yy * BW + xx ] == 2;
				if( w >= SNOW_RIM_WATER ) yes = false;
			}
			votes[ o++ ] = yes;
			ground += yes;
		}
	}
	if( ground == o ) return SNOW_DEC_ALL;
	if( !ground ) return SNOW_DEC_NONE;
	unsigned long long h = 1469598103934665603ull;	// FNV-1a: identical sets of votes are stored once
	for( int i = 0; i < SNOW_DEC_SIZE; ++i ) h = ( h ^ votes[ i ] ) * 1099511628211ull;
	auto it = SnowDecShared.find( h );
	if( it != SnowDecShared.end() && !memcmp( &SnowDecPool[ it->second ], votes, SNOW_DEC_SIZE ) ) return it->second;
	GroundRec_Join();	// (th4) THE JOB LINES: SnowDecPool grows below -- no ground job may still be reading it
	const int at = (int)SnowDecPool.size();
	SnowDecPool.insert( SnowDecPool.end(), votes, votes + SNOW_DEC_SIZE );
	if( it == SnowDecShared.end() ) SnowDecShared.emplace( h, at );
	return at;
}

// (th4) SNOW GROUND: TownWater_BeginFrame while the snow is on -- the entry classes, the art cache, and the votes
// started over whenever the map or the level art changed
static void SnowVotesEnsure()
{
	// RAVENHOLM: a game loaded straight into Ravenholm has not loaded the town yet -- the votes need TreePiece /
	// RiverPiece / HutLeafPiece (SnowCellKind), so the tables are filled from Town.TIL first, as TownWater_CellBegin does
	if( !TreePieceReady && Dungeon == DUN_44_RAVENHOLM ){
		int bytes = 0;
		auto til = (TileSquare*)LoadFile( "Levels\\TownData\\Town.TIL", &bytes );
		TownWater_OnTownLoaded( til, bytes / (int)sizeof( TileSquare ) );
		FreeMem( til );
	}
	static bool tables = false;
	if( !tables ){
		memset( SnowEntry, 0, sizeof( SnowEntry ) );
		for( uchar e : kSnowRockEntries ) SnowEntry[ e ] |= SE_ROCK;
		for( uchar e : kSnowWallEntries ) SnowEntry[ e ] |= SE_WALL;
		for( uchar e : kSnowGroundEntries ) SnowEntry[ e ] |= SE_GROUND;
		for( int k = 0, at = 0; k < 32; ++k ){ SnowRowStart[ k ] = at; at += 2 * SnowWideHalf( k ); }	// SNOW_DEC_SIZE in all
		tables = true;
	}
	if( SnowArtCel != CurrentLevelCELFile || SnowArtDun != (int)(DUNGEON)Dungeon || SnowArtIdx.empty() ){
		SnowArtIdx.assign( 65536, -1 ); SnowArt.clear();
		SnowArtCel = CurrentLevelCELFile; SnowArtDun = (int)(DUNGEON)Dungeon;
		SnowDecOf.clear();	// the votes go with the art
	}
	uint sig = 2166136261u;	// FNV-1a over FineMap, like GrassArtEnsure: it changes on level load and on quest tile swaps
	for( int i = 0; i < 112 * 112; ++i ) sig = ( sig ^ (uint)FineMap[ 0 ][ i ] ) * 16777619u;
	if( SnowDecOf.empty() || sig != SnowMapSig ){
		SnowDecOf.assign( 112 * 112, SNOW_DEC_NOT_YET );
		SnowDecPool.clear(); SnowDecShared.clear();
		SnowMapSig = sig;
		HutAnchorRow = HutAnchorCol = -1;	// ADRIA'S SIDE: where tile 165's top cell is on this map
		for( int r = 0; r < 112 && HutAnchorRow < 0 && HutAnchorPiece > 0; ++r ){
			for( int c = 0; c < 112; ++c ) if( FineMap[ r ][ c ] == HutAnchorPiece ){ HutAnchorRow = r; HutAnchorCol = c; break; }
		}
		PepAnchorRow = PepAnchorCol = -1;	// HOUSE EDGES: where its tiles stand, whole (all four squares); PEPIN'S FRONT: tile 145
		memset( HouseEdgeCell, 0, sizeof( HouseEdgeCell ) );
		for( int i = 0; i < HOUSE_EDGE_N; ++i ){
			const int* q = HouseEdgeSq[ i ];
			if( q[ 0 ] <= 0 ) continue;
			for( int r = 0; r < 111; ++r ) for( int c = 0; c < 111; ++c ){
				if( FineMap[ r ][ c ] != q[ 0 ] || FineMap[ r + 1 ][ c ] != q[ 1 ] || FineMap[ r ][ c + 1 ] != q[ 2 ] || FineMap[ r + 1 ][ c + 1 ] != q[ 3 ] ) continue;
				HouseEdgeCell[ r ][ c ] = HouseEdgeCell[ r + 1 ][ c ] = HouseEdgeCell[ r ][ c + 1 ] = HouseEdgeCell[ r + 1 ][ c + 1 ] = true;
				if( kHouseEdgeTiles[ i ] == PEP_ANCHOR_TILE && PepAnchorRow < 0 ){ PepAnchorRow = r; PepAnchorCol = c; }
			}
		}
		for( int l = 0; l < SNOW_LINE_N; ++l ){	// SNOW LINES: where each anchor tile stands, whole
			SnowLineRow[ l ] = SnowLineCol[ l ] = -1;
			const int* q = SnowLineSq[ l ];
			for( int r = 0; r < 111 && q[ 0 ] > 0 && SnowLineRow[ l ] < 0; ++r ) for( int c = 0; c < 111; ++c ){
				if( FineMap[ r ][ c ] != q[ 0 ] || FineMap[ r + 1 ][ c ] != q[ 1 ] || FineMap[ r ][ c + 1 ] != q[ 2 ] || FineMap[ r + 1 ][ c + 1 ] != q[ 3 ] ) continue;
				SnowLineRow[ l ] = r; SnowLineCol[ l ] = c; break;
			}
		}
	}
}

// =================================================================================================================
// (th4) ROCK SNOW -- Ice Age: snow SCATTERED all over the rocks (Mordor, 2026-10-10: "snow should be sort of scattered all
// over these rocks"; approved in game the same day). Snow caps on the stones' tops were tried first and rejected in game;
// do not bring them back unasked. TRISTRAM AND RAVENHOLM (Mordor, 2026-10-10: "snowbound rocks there, and thatch roofs").
// WHICH ROCKS: kRockTiles / kFloorRockTiles -> RockPiece (above); also ADRIA'S HUT's roof and left wall (RockPiece 3, below). WHEN: RockSnowOn = the SNOW GROUND is on in the normal town
// or Ravenholm (its pass draws the snow). RAVENHOLM is drawn by Draw.cpp DrawSceneDungeonTop, but it is FULLY LIT
// (thehell2.cpp LoadQuestDungeonMainMapTown sets FullLighting, so its LightMap is all 0 and DarkLevel never darkens a
// tile): its frame holds the art's own entries, as Tristram's does, and the snow is painted the same way. Its two hooks:
// DrawSceneDungeonTop's TownWater_CellBegin passes the map cell, and DrawMapCell (a cell's column repainted over a moving
// hero or monster, like the town's DrawMostTopTile) calls TownWater_TileRedrawn for the layers it repainted only.
// WHICH PIXELS: every STONE pixel of a rock cell's art -- not black (entry 0: outlines, kept so each stone keeps its shape)
// and, within the floor layer, a rock entry (the floor of a rock tile is mostly ground) -- gets snow where a noise pattern
// says so: two octaves of value noise (ROCK_SCALE1 / 2 px: flecks and clumps) in WORLD pixels, so the pattern runs on
// across cell borders and never moves or flickers, plus a push from the art's brightness (lit faces, which face up and
// toward the light, get much more; the darkest faces and undersides little): snow where
// noise + ROCK_BRIGHT_K * ( brightness - 0.5 ) > ROCK_THRESHOLD. Roughly a third of a stone. Numbers picked on an offline
// render of the boulder field (scratchpad rocks\scatter_proto.py, S1 3, S2 7, BK 0.45, TH 0.6; finer flecks looked like
// pixel noise).
// HOW: that decision depends only on the art and on the world position, so it is worked out ONCE per map cell, from the
// cell's art (GrassDecodePiece, every layer -- the floor only for kFloorRockTiles), on its first draw: THE CACHE,
// RockCacheOf / RockCachePool, started over when the map or the level art changes (SnowMapSig, the CEL). Per frame
// TownWater_CellEnd only paints the cell's cached pixels into the 8-bit frame (RockSnowE, the palette's own snow, as SNOW
// ON THE TREES picks it) and records them (RockSnowPx); RockSnow_ToLayer (end of BuildSnowGround) hands every one the
// finished frame still shows to the snow pass, so it is the ground snow's own white and light (day / night); anything
// drawn later over a rock (a hero, a rock in front) hides it. The first version did the noise per pixel per frame against
// a snapshot of the column -- the same picture, but the boulder field cost a noticeable part of the frame (Mordor).
namespace {
const float ROCK_SCALE1 = 3.0f, ROCK_SCALE2 = 7.0f, ROCK_BRIGHT_K = 0.45f, ROCK_THRESHOLD = 0.6f;
const float ROCK_LUM0 = 0.05f, ROCK_LUM1 = 0.22f;	// art luminance from the darkest to a fully lit face (the rocks: ~0.04 .. 0.29)
bool RockSnowOn = false, RockOn = false;
int RockX = 0, RockY = 0, RockRow = 0, RockCol = 0;	// the cell being drawn: its column's dst (left x, bottom row y), map cell
struct RockSnowPix { int rel; uchar val; };	// rel = the SNOW GROUND layer index (pre-zoom, borders off)
// a plain growing buffer (as LeafPx): RockSnow_End records every painted pixel, tens of thousands a frame among the
// houses and rocks, and a std::vector push_back per pixel was most of RockSnow_End's time (measured: 2.3 ms)
RockSnowPix* RockSnowPx = 0; size_t RockSnowN = 0, RockSnowCap = 0;
uchar RockSnowE = 0;
bool RockEntryIdx[ 256 ];
bool RockEntryReady = false;
float RockBright[ 256 ];			// brightness 0..1 of each entry (FilePalette)
// THE CACHE: per map cell (row * 112 + col) -1 = not worked out yet, else the index in RockCachePool of [ count, offsets ];
// an offset = up * 64 + dx from the column's dst: the pixel at ( x + dx, y - up ), up = 32 * layer + the piece's row
std::vector<int> RockCacheOf;
std::vector<ushort> RockCachePool;
uint RockCacheSig = 0;
const void* RockCacheCel = 0;
uint RockHash( int a, int b, int c )
{
	uint h = (uint)a * 73856093u ^ (uint)b * 19349663u ^ (uint)c * 83492791u;
	h = ( h ^ ( h >> 13 ) ) * 0x5bd1e995u;
	return h ^ ( h >> 15 );
}
float RockNoise( int x, int y, float scale, int seed )	// smooth value noise 0..1
{
	const float fx = x / scale, fy = y / scale;
	const int ix = (int)floorf( fx ), iy = (int)floorf( fy );
	float tx = fx - ix, ty = fy - iy;
	tx = tx * tx * ( 3 - 2 * tx ); ty = ty * ty * ( 3 - 2 * ty );
	auto h = [&]( int a, int b ){ return ( RockHash( a, b, seed ) & 1023 ) / 1023.0f; };
	return ( h( ix, iy ) * ( 1 - tx ) + h( ix + 1, iy ) * tx ) * ( 1 - ty ) + ( h( ix, iy + 1 ) * ( 1 - tx ) + h( ix + 1, iy + 1 ) * tx ) * ty;
}
// ADRIA'S HUT (RockPiece 3, kHutSnowTiles): the same noise and cache, but its columns also hold the TREE in front (165..167
// are tree tiles) and the face with the shelves and the door, so a pixel is snowed only if (HutSnow_Build):
//  - its ART entry is a hut colour: kHutRoofEntries (the planks) or kHutWallEntries (the left wall's near-black wood),
//    measured offline against the tree's pixels outside the hut (the tree is browns: 97, 78, 70, 65, 47, 111, 89, ...).
//    No leaf entry (kLeafEntries) is in either list -- 106 shades the roof too and was left out for that -- so the leaves
//    never change and SNOW ON THE TREES / the leaf frost see them exactly as before;
//  - at least HUT_SNOW_NEIGH % of its 5 x 5 window in the column's art is hut colour (the few roof-like greys in the
//    crown sit among browns: white dots in the tree without this);
//  - it is ROOF or LEFT WALL by position, in ADRIA'S SIDE's coordinates (tile 165's top cell, HutAnchorRow / Col; cells
//    beyond HUT_SNOW_REACH get none): left of the front corner (HUT_CORNER_X) above the wall's foot (the base line), right
//    of it only above the eave over the shelves (HUT_EAVE_*: y -37 at the corner, rising 0.3 px per px, measured on the
//    art). Floor-layer pixels (ADRIA'S SIDE's squares only) stay HUT_FOOT_MARGIN px clear of the foot line: the snow
//    ground votes there (THE EDGE, +-HUT_EDGE_BAND and the wobble), and a fleck beside a voted pixel would fail its SHOWS test;
//  - the noise says so: the roof like a rock (lit planks more), the wall -- dark, a shaded face, which the rocks' rule
//    left almost bare -- at a fixed HUT_WALL_THRESHOLD with no brightness push. The light shining through the wall's
//    cracks is no hut colour: snowed by the rocks' rule it drew white vertical streaks.
// Checked on an offline render of the hut under iceage\0town_P.pal (scratchpad hut\proto4.py): patches on the roof, also
// under the crown, the branches crossing them dark; small flecks on the left wall; the tree and the shelf face unchanged.
const uchar kHutRoofEntries[] = { 59, 62, 81, 82, 86, 91, 100, 101, 110, 112 };
const uchar kHutWallEntries[] = { 121, 122, 124, 126 };
const float HUT_EAVE_Y = -37.0f, HUT_EAVE_SLOPE = -0.3f, HUT_FOOT_MARGIN = 16.0f, HUT_WALL_THRESHOLD = 0.7f;
const int HUT_SNOW_NEIGH = 60, HUT_SNOW_REACH = 8;
uchar HutEntry[ 256 ];				// 1 roof, 2 wall (kHutRoofEntries / kHutWallEntries)
// THATCH ROOFS (kRoofTiles above; RoofSnow_BuildAll / RoofSnow_Classify below): a house's roof is told from its walls,
// windows and doors on the picture the game shows -- a COMPOSITE of the house's area, every map cell's column painted
// from the art in draw order (DrawSceneTownTop: by row + col, then row) -- because a house is cut into many cells and
// the roof lies in their floor tiles and upper layers alike. Worked out once per town map (with THE CACHE), the snowed
// pixels of each cell stored in RockCachePool like a rock's: per frame nothing but the painting.
// THE SCORE: kRoofScore[ art entry ] = 100 x log( share among roof pixels / share among wall pixels ), measured on boxes
// of thatch and of walls drawn on the town render (roofs: Pepin's shaded and lit faces, the tavern's two faces, Griswold's,
// the gable house's; walls: Pepin's, the tavern's two, Farnham's house's), +0.002 on each share; the lit window and fire
// colours (bright reds, oranges, yellows) are set to -300 (window light, never roof). A pixel's score = the mean over its
// window of the composite (clipped to the house's box). A pixel gets snow when, in this order:
//  - it belongs to a cell of this house (whole roof tiles), in an upper layer or in the floor of a sight-blocking cell;
//    not black (entry 0);
//  - SMALL: its 7 x 7 score (ROOF_R1) > ROOF_SMALL_Q, and BIG: its 17 x 17 score (ROOF_R2) > ROOF_BIG_Q (window frames
//    and sills score as roof at 7 x 7);
//  - NOT BETWEEN WALLS: a WALL pixel (7 x 7 score < ROOF_WALL_Q) within ROOF_BETWEEN px left AND right of it, or on 3 of
//    the 4 sides (the gable windows and the dormers' fronts);
//  - NOT ABOVE A DOOR: going straight down, open ground (the floor of a walkable cell) comes within ROOF_DOOR px before
//    a run of ROOF_RUN wall pixels (doors, open porches, the eaves' ends over the ground);
//  - outside kRoofNoSnow;
//  - the rocks' noise rule (as rocks: noise + brightness push > ROCK_THRESHOLD).
// Tried and rejected on the render: a colour list like the hut's (the town's thatch shares its colours with the walls'
// lit stones and the ground; half the roof stayed bare); per-cell columns instead of the composite (a column cannot see
// the wall of the cell beside it: whole cells of wall were snowed, and cells whose floor tile is all roof got bare
// stripes); "no wall above" scans (vertical bare stripes under every dormer and gable).
// Checked offline (scratchpad roofs\final_proto.py, the reference this code follows rule by rule): all seven roofs get
// the rocks' look, walls, doors and windows stay as they were, and no snowed pixel lies within 3 px of open ground (the
// door rule keeps a wall between the roof snow and the SNOW GROUND, whose edges are therefore untouched).
const short kRoofScore[ 256 ] = {
	-225, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300,    1, -300, -300,	// 0..15
	-300, -300,    0, -300,   15, -300,    0,    0, -300,    0,   70,    0, -300, -300,  122,    5,	// 16..31
	   0, -300, -300,    0, -300,    0, -300,    0,  168, -300,   -4,    4, -300, -300,    0,  229,	// 32..47
	 251, -300,    0,   -4,   17, -300,   83,  -10,    7,  135, -300,   26,  265,  206,   33,   87,	// 48..63
	-300,    0,   77,  158, -300,    2,   90,   27, -300,  -21,   -3,   12,  165,  296,  174, -300,	// 64..79
	 194,   18,  -11,  200, -300,  162,   37,   31,    4,   14,    2,   54,  113,    5,   66,   43,	// 80..95
	 -20,  235,  186,   12,  -26,   31,  106,  165,  -13,  -91,   -5,  -41,  150,  -47,  -44,  167,	// 96..111
	-109,  -64,   99, -100, -123,  -65,  -54,  -47,  116, -207, -153,  107, -112,   -8, -183,  -76,	// 112..127
	   0,    0,    0,    0,    0,    0,    0,    0,    0, -300, -300, -300, -300, -300, -300,    0,	// 128..143
	   0, -300, -300, -300, -300, -300, -300,    0,    0, -300, -300, -300, -300, -300, -300,    0,	// 144..159
	   0,    0,    0,    0,    0,    0, -300, -300, -300, -300, -300, -300,    0,    0,    0,    0,	// 160..175
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// 176..191
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, -300,    0,    0,    0,    0,	// 192..207
	   0,    0, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300,    0,    0,    0,	// 208..223
	   0,    0, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300, -300,    0,    0,    0,	// 224..239
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0	// 240..255
};
const int ROOF_SMALL_Q = 20, ROOF_BIG_Q = 20, ROOF_WALL_Q = -30;	// mean score (x 100, as kRoofScore)
const int ROOF_R1 = 3, ROOF_R2 = 8, ROOF_BETWEEN = 20, ROOF_DOOR = 100, ROOF_RUN = 6;
enum { RC_HOUSE = 1, RC_SIGHT = 2, RC_WALK = 4 };	// RoofSnow_Classify's cell bits
uchar RoofCellOf[ 112 * 112 ];		// 1 = a roof cell: its snow comes from RoofSnow_BuildAll (RockSnow_Begin / _Build)
}

// THATCH ROOFS (above): one house's snow, worked out on its COMPOSITE -- bw x bh px whose top left is world px ( X0, Y0 )
// (world px of map cell ( r, c )'s column: x 32 ( r - c ) .. + 63, its floor's bottom row at y 16 ( r + c )): comp = the
// art entry shown, own = 1 + the map cell ( r * 112 + c ) that drew it (0 = none), lay = its layer (0 = floor);
// cellBits = RC_* of every map cell; noSnow = the placed kRoofNoSnow boxes in world px, 4 ints each (x0, y0, x1, y1).
// Out: every snowed pixel as { map cell, up * 64 + dx from the cell's column dst } (THE CACHE's offsets).
// Uses only its arguments, kRoofScore, RockNoise and RockBright -- keep it so (it is checked against the offline
// reference with the same inputs).
static void RoofSnow_Classify( int bw, int bh, int X0, int Y0, const uchar* comp, const ushort* own, const uchar* lay,
                               const uchar* cellBits, const int* noSnow, int noSnowN, std::vector<std::pair<int, ushort>>& out )
{
	const int sw = bw + 1;
	std::vector<int> sat( (size_t)sw * ( bh + 1 ), 0 );		// summed scores
	for( int y = 0; y < bh; ++y ){
		int rs = 0;
		for( int x = 0; x < bw; ++x ){ rs += kRoofScore[ comp[ y * bw + x ] ]; sat[ ( y + 1 ) * sw + x + 1 ] = sat[ y * sw + x + 1 ] + rs; }
	}
	auto win = [&]( int x, int y, int R, int q ){				// mean score of the window clipped to the box > q (x 100)?
		const int x0 = max( 0, x - R ), x1 = min( bw, x + R + 1 ), y0 = max( 0, y - R ), y1 = min( bh, y + R + 1 );
		const int s = sat[ y1 * sw + x1 ] - sat[ y0 * sw + x1 ] - sat[ y1 * sw + x0 ] + sat[ y0 * sw + x0 ];
		return s - q * ( x1 - x0 ) * ( y1 - y0 );				// > 0: above q, < 0: below
	};
	// per pixel, as O(1) look-ups (the plain scans, up to 180 steps per pixel, cost ~80 ms for the town's roofs):
	// rowW / colW = wall counts so far along the row / the column (NOT BETWEEN WALLS);
	// THE DOOR SCAN, "going down from y: ground within ROOF_DOOR px before ROOF_RUN walls in a row" (a ground pixel
	// counts as ground even where it also scores as wall), is: the nearest ground below, g (nextG), comes before the
	// end of the first full wall run that starts below y -- that run starts at nextRun (where the walls straight down,
	// wallDown, are at least ROOF_RUN) and ends ROOF_RUN - 1 px lower. Same pixels as the scan (checked offline).
	const int n = bw * bh;
	std::vector<uchar> wall( n ), ground( n );
	for( int y = 0; y < bh; ++y ) for( int x = 0; x < bw; ++x ){
		const int o = y * bw + x;
		wall[ o ] = win( x, y, ROOF_R1, ROOF_WALL_Q ) < 0;
		ground[ o ] = own[ o ] && !lay[ o ] && ( cellBits[ own[ o ] - 1 ] & RC_WALK );
	}
	const ushort NONE = 0xFFFF;									// a box is far below 65535 px either way
	std::vector<ushort> rowW( (size_t)( bw + 1 ) * bh ), colW( (size_t)bw * ( bh + 1 ) ), nextG( n ), nextRun( n );
	std::vector<uchar> wallDown( n );							// walls straight down, from here; stops counting at 255
	for( int y = 0; y < bh; ++y ) for( int x = 0; x < bw; ++x ){	// row by row (column by column it ran ~5x slower)
		rowW[ y * ( bw + 1 ) + x + 1 ] = (ushort)( rowW[ y * ( bw + 1 ) + x ] + wall[ y * bw + x ] );
		colW[ ( y + 1 ) * bw + x ] = (ushort)( colW[ y * bw + x ] + wall[ y * bw + x ] );
	}
	std::vector<ushort> gBelow( bw, NONE ), runBelow( bw, NONE );	// per column, the nearest strictly below the row; none below the box
	for( int y = bh - 1; y >= 0; --y ) for( int x = 0; x < bw; ++x ){
		const int o = y * bw + x;
		nextG[ o ] = gBelow[ x ]; nextRun[ o ] = runBelow[ x ];
		wallDown[ o ] = wall[ o ] ? (uchar)min( 255, 1 + ( y + 1 < bh ? wallDown[ o + bw ] : 0 ) ) : 0;
		if( ground[ o ] ) gBelow[ x ] = (ushort)y;
		if( wallDown[ o ] >= ROOF_RUN ) runBelow[ x ] = (ushort)y;
	}
	auto rowWalls = [&]( int y, int x0, int x1 ){ return rowW[ y * ( bw + 1 ) + x1 ] - rowW[ y * ( bw + 1 ) + x0 ]; };	// [ x0, x1 )
	auto colWalls = [&]( int x, int y0, int y1 ){ return colW[ y1 * bw + x ] - colW[ y0 * bw + x ]; };
	for( int y = 0; y < bh; ++y ) for( int x = 0; x < bw; ++x ){
		const int o = y * bw + x, k = own[ o ];
		if( !k || !( cellBits[ k - 1 ] & RC_HOUSE ) ) continue;
		if( !lay[ o ] && !( cellBits[ k - 1 ] & RC_SIGHT ) ) continue;		// the floor: only a sight-blocking cell's
		const uchar v = comp[ o ];
		if( !v ) continue;
		if( win( x, y, ROOF_R1, ROOF_SMALL_Q ) <= 0 || win( x, y, ROOF_R2, ROOF_BIG_Q ) <= 0 ) continue;	// SMALL, BIG
		const bool l = rowWalls( y, max( 0, x - ROOF_BETWEEN ), x ) > 0, r = rowWalls( y, x + 1, min( bw, x + ROOF_BETWEEN + 1 ) ) > 0;	// NOT BETWEEN WALLS
		const bool u = colWalls( x, max( 0, y - ROOF_BETWEEN ), y ) > 0, d = colWalls( x, y + 1, min( bh, y + ROOF_BETWEEN + 1 ) ) > 0;
		if( ( l && r ) || l + r + u + d >= 3 ) continue;
		const int g = nextG[ o ], run = nextRun[ o ];			// NOT ABOVE A DOOR
		if( g != NONE && g - y <= ROOF_DOOR && ( run == NONE || g <= run + ROOF_RUN - 1 ) ) continue;
		const int wx = X0 + x, wy = Y0 + y;						// WORLD px, as the rocks
		bool boxed = false;
		for( int i = 0; i < noSnowN && !boxed; ++i ){ const int* b = noSnow + 4 * i; boxed = wx >= b[ 0 ] && wx < b[ 2 ] && wy >= b[ 1 ] && wy < b[ 3 ]; }
		if( boxed ) continue;
		const float nz = 0.55f * RockNoise( wx, wy, ROCK_SCALE1, 1 ) + 0.45f * RockNoise( wx, wy, ROCK_SCALE2, 2 );
		if( nz + ROCK_BRIGHT_K * ( RockBright[ v ] - 0.5f ) <= ROCK_THRESHOLD ) continue;
		const int rr = ( k - 1 ) / 112, cc = ( k - 1 ) % 112;
		out.push_back( { k - 1, (ushort)( ( 16 * ( rr + cc ) - wy ) * 64 + ( wx - 32 * ( rr - cc ) ) ) } );
	}
}

// THATCH ROOFS (above): every roof cell's snow into THE CACHE, once per map (RockSnow_BeginFrame, with the cache reset).
// The houses are the 8-connected groups of roof cells; each is classified on the composite of its area plus margins wide
// enough for the windows the tests read (ROOF_BETWEEN + ROOF_R1 around, ROOF_DOOR + ROOF_R1 below). A roof cell whose
// piece is a rock's (RockPiece: piece 26, a plain piece in 253 cells, is also in tile 123) keeps the rock rule it had.
static void RoofSnow_BuildAll()
{
	memset( RoofCellOf, 0, sizeof( RoofCellOf ) );
	static int house[ 112 * 112 ];
	for( int i = 0; i < 112 * 112; ++i ) house[ i ] = -1;
	bool any = false;
	for( int i = 0; i < ROOF_TILE_N; ++i ){					// whole placements of the roof tiles
		const int* q = RoofSq[ i ];
		if( q[ 0 ] <= 0 ) continue;
		for( int r = 0; r < 111; ++r ) for( int c = 0; c < 111; ++c ){
			if( FineMap[ r ][ c ] != q[ 0 ] || FineMap[ r + 1 ][ c ] != q[ 1 ] || FineMap[ r ][ c + 1 ] != q[ 2 ] || FineMap[ r + 1 ][ c + 1 ] != q[ 3 ] ) continue;
			house[ r * 112 + c ] = house[ ( r + 1 ) * 112 + c ] = house[ r * 112 + c + 1 ] = house[ ( r + 1 ) * 112 + c + 1 ] = -2;
			any = true;
		}
	}
	if( !any ) return;
	std::vector<int> noSnow;									// the no-snow boxes, placed
	for( int i = 0; i < ROOF_NOSNOW_N; ++i ){
		const int* q = RoofNoSnowSq[ i ];
		if( q[ 0 ] <= 0 ) continue;
		for( int r = 0; r < 111; ++r ) for( int c = 0; c < 111; ++c ){
			if( FineMap[ r ][ c ] != q[ 0 ] || FineMap[ r + 1 ][ c ] != q[ 1 ] || FineMap[ r ][ c + 1 ] != q[ 2 ] || FineMap[ r + 1 ][ c + 1 ] != q[ 3 ] ) continue;
			const RoofNoSnowBox& b = kRoofNoSnow[ i ];
			const int ax = 32 * ( r - c ), ay = 16 * ( r + c );
			noSnow.insert( noSnow.end(), { ax + b.x0, ay + b.y0, ax + b.x1, ay + b.y1 } );
		}
	}
	int houses = 0;											// the 8-connected groups
	std::vector<int> stack;
	for( int s = 0; s < 112 * 112; ++s ){
		if( house[ s ] != -2 ) continue;
		house[ s ] = houses; stack.assign( 1, s );
		while( !stack.empty() ){
			const int a = stack.back(); stack.pop_back();
			for( int dr = -1; dr <= 1; ++dr ) for( int dc = -1; dc <= 1; ++dc ){
				const int r = a / 112 + dr, c = a % 112 + dc;
				if( (uint)r < 112 && (uint)c < 112 && house[ r * 112 + c ] == -2 ){ house[ r * 112 + c ] = houses; stack.push_back( r * 112 + c ); }
			}
		}
		++houses;
	}
	std::vector<int> artIdx( 65536, -1 );						// the pieces' art, decoded once for all houses
	std::vector<uchar> art;									// 2048 bytes per piece: entries (row 0 = bottom), opaque flags
	auto pieceArt = [&]( ushort id ) -> const uchar* {
		int& at = artIdx[ id ];
		if( at < 0 ){
			at = (int)( art.size() / 2048 ); art.resize( art.size() + 2048 );
			if( !GrassDecodePiece( id, &art[ (size_t)at * 2048 ], &art[ (size_t)at * 2048 + 1024 ] ) ) memset( &art[ (size_t)at * 2048 + 1024 ], 0, 1024 );
		}
		return &art[ (size_t)at * 2048 ];
	};
	std::vector<std::vector<ushort>> px( 112 * 112 );
	std::vector<uchar> bits( 112 * 112 );
	std::vector<std::pair<int, ushort>> out;
	const int MX = ROOF_BETWEEN + ROOF_R1 + 1, MBOT = ROOF_DOOR + ROOF_R1 + 1;
	for( int h = 0; h < houses; ++h ){
		int X0 = INT_MAX, X1 = INT_MIN, Y0 = INT_MAX, Y1 = INT_MIN;
		for( int s = 0; s < 112 * 112; ++s ){
			const int r = s / 112, c = s % 112, piece = FineMap[ r ][ c ];
			bits[ s ] = (uchar)( ( house[ s ] == h ? RC_HOUSE : 0 ) | ( piece > 0 && piece < 2049 && TileBlockSight[ piece ] ? RC_SIGHT : 0 )
			                     | ( piece > 0 && piece < 2049 && !TileBlockWalking[ piece ] ? RC_WALK : 0 ) );
			if( house[ s ] != h ) continue;
			X0 = min( X0, 32 * ( r - c ) ); X1 = max( X1, 32 * ( r - c ) + 64 );
			Y0 = min( Y0, 16 * ( r + c ) - 255 ); Y1 = max( Y1, 16 * ( r + c ) + 1 );
		}
		X0 -= MX; X1 += MX; Y0 -= MX; Y1 += MBOT;
		const int bw = X1 - X0, bh = Y1 - Y0;
		std::vector<uchar> comp( (size_t)bw * bh, 0 ), lay( (size_t)bw * bh, 0 );
		std::vector<ushort> own( (size_t)bw * bh, 0 );
		for( int sum = 0; sum <= 222; ++sum ) for( int r = max( 0, sum - 111 ); r <= min( 111, sum ); ++r ){	// DRAW ORDER
			const int c = sum - r;
			if( FineMap[ r ][ c ] <= 0 ) continue;
			const int cx = 32 * ( r - c ) - X0, cy = 16 * ( r + c ) - Y0;	// the column's dst in the box
			if( cx + 64 <= 0 || cx >= bw || cy < 0 || cy - 255 >= bh ) continue;
			const TileColumn& t = TileMap[ IsometricMapOffset( r, c ) ];
			for( int li = 0; li < 8; ++li ) for( int half = 0; half < 2; ++half ){
				const ushort id = half ? t.Layer[ li ].Right : t.Layer[ li ].Left;
				if( !id ) continue;
				const uchar* a = pieceArt( id );
				for( int py = 0; py < 32; ++py ){
					const int y = cy - 32 * li - py;
					if( y < 0 || y >= bh ) continue;
					for( int pxx = 0; pxx < 32; ++pxx ){
						const int x = cx + 32 * half + pxx;
						if( x < 0 || x >= bw || !a[ 1024 + py * 32 + pxx ] ) continue;
						const int o = y * bw + x;
						comp[ o ] = a[ py * 32 + pxx ]; own[ o ] = (ushort)( r * 112 + c + 1 ); lay[ o ] = (uchar)li;
					}
				}
			}
		}
		out.clear();
		RoofSnow_Classify( bw, bh, X0, Y0, comp.data(), own.data(), lay.data(), bits.data(), noSnow.data(), (int)noSnow.size() / 4, out );
		for( const auto& p : out ) px[ p.first ].push_back( p.second );
	}
	for( int s = 0; s < 112 * 112; ++s ){						// into THE CACHE
		if( house[ s ] < 0 ) continue;
		const int piece = FineMap[ s / 112 ][ s % 112 ];
		if( piece > 0 && piece < MAX_PIECE && RockPiece[ piece ] ) continue;	// a rock's piece: the rock rule, as before
		RoofCellOf[ s ] = 1;
		RockCacheOf[ s ] = (int)RockCachePool.size();
		RockCachePool.push_back( (ushort)px[ s ].size() );
		RockCachePool.insert( RockCachePool.end(), px[ s ].begin(), px[ s ].end() );
	}
}

// THE CATACOMBS BUILDING (kCataParts above): its roof's snow into THE CACHE, once per map, right after the thatch roofs
// (it uses their RoofCellOf: RockSnow_Begin / _Build treat its cells the same way). Per cell of a whole placement of
// the building, every pixel of the column (all 8 layers) inside kCataRoof, not black, not a leaf entry (the bush in
// front, tile 323), gets the rocks' noise rule at CATA_THRESHOLD and CATA_SCALE1 / 2 (THE ROCKS' COVERAGE, FINER SPOTS) --
// small patches on the lit slate slope, flecks on the dark slope behind the ridge (checked offline on the town render and
// on Ravenholm's, open and closed: scratchpad cata\cata_proto2.py th=0.8 s1=2 s2=4.5).
// A cell whose piece is a rock's keeps the rock rule (as the thatch roofs; none of the building's pieces is, today).
static bool CataPlaced( int i, int r, int c )
{
	const int* q = CataSq[ i ];
	return q[ 0 ] > 0 && r >= 0 && c >= 0 && r < 111 && c < 111 && FineMap[ r ][ c ] == q[ 0 ] && FineMap[ r + 1 ][ c ] == q[ 1 ]
	    && FineMap[ r ][ c + 1 ] == q[ 2 ] && FineMap[ r + 1 ][ c + 1 ] == q[ 3 ];
}
static void CataSnow_BuildAll()
{
	uchar pix[ 1024 ], opq[ 1024 ];
	for( int ar = 0; ar < 111; ++ar ) for( int ac = 0; ac < 111; ++ac ){
		if( !CataPlaced( 0, ar, ac ) ) continue;						// the anchor, tile 322
		for( int p = 0; p < CATA_PART_N; ++p ){
			const int pr = ar + kCataParts[ p ].dr, pc = ac + kCataParts[ p ].dc;
			if( !CataPlaced( p, pr, pc ) ) continue;
			for( int s = 0; s < 4; ++s ){
				const int r = pr + ( s & 1 ), c = pc + ( s >> 1 ), piece = FineMap[ r ][ c ];
				if( piece > 0 && piece < MAX_PIECE && RockPiece[ piece ] ) continue;	// a rock's piece: the rock rule, as before
				const TileColumn& t = TileMap[ IsometricMapOffset( r, c ) ];
				const int at = (int)RockCachePool.size();
				RockCachePool.push_back( 0 );
				int n = 0;
				for( int li = 0; li < 8; ++li ) for( int half = 0; half < 2; ++half ){
					const ushort id = half ? t.Layer[ li ].Right : t.Layer[ li ].Left;
					if( !id || !GrassDecodePiece( id, pix, opq ) ) continue;
					for( int py = 0; py < 32; ++py ) for( int px = 0; px < 32; ++px ){	// py 0 = the piece's BOTTOM row
						const int k = py * 32 + px;
						const uchar v = pix[ k ];
						if( !opq[ k ] || !v || LeafIdx[ v ] ) continue;
						const int dx = 32 * half + px, up = 32 * li + py;
						const int wx = 32 * ( r - c ) + dx, wy = 16 * ( r + c ) - up;	// WORLD px, as the rocks
						const double X = wx - 32 * ( ar - ac ) + 0.5, Y = wy - 16 * ( ar + ac ) + 0.5;
						bool in = false;										// even-odd test against kCataRoof
						for( int e = 0, f = CATA_ROOF_N - 1; e < CATA_ROOF_N; f = e++ ){
							const double x1 = kCataRoof[ f ][ 0 ], y1 = kCataRoof[ f ][ 1 ], x2 = kCataRoof[ e ][ 0 ], y2 = kCataRoof[ e ][ 1 ];
							if( ( y1 > Y ) != ( y2 > Y ) && X < ( x2 - x1 ) * ( Y - y1 ) / ( y2 - y1 ) + x1 ) in = !in;
						}
						if( !in ) continue;
						const float nz = 0.55f * RockNoise( wx, wy, CATA_SCALE1, 1 ) + 0.45f * RockNoise( wx, wy, CATA_SCALE2, 2 );	// FINER SPOTS (above)
						if( nz + ROCK_BRIGHT_K * ( RockBright[ v ] - 0.5f ) <= CATA_THRESHOLD ) continue;	// THE ROCKS' COVERAGE (above)
						RockCachePool.push_back( (ushort)( up * 64 + dx ) );
						++n;
					}
				}
				RockCachePool[ at ] = (ushort)n;
				RoofCellOf[ r * 112 + c ] = 1;
				RockCacheOf[ r * 112 + c ] = at;
			}
		}
	}
}

static void RockSnow_BeginFrame()
{
	RockOn = false;
	RockSnowN = 0;
	RockSnowOn = SnowGroundOn && ( InTown() || Dungeon == DUN_44_RAVENHOLM ) && SunGW > 0 && SunGH > 0;
	if( !RockSnowOn ) return;
	if( !RockEntryReady ){
		for( uchar e : kSnowRockEntries ) RockEntryIdx[ e ] = true;
		for( uchar e : kHutRoofEntries ) HutEntry[ e ] = 1;	// ADRIA'S HUT (above)
		for( uchar e : kHutWallEntries ) HutEntry[ e ] = 2;
		RockEntryReady = true;
	}
	int best = 1, bestD = 0x7FFFFFFF;	// the palette's snow: as SNOW ON THE TREES picks it (TREE_SNOW_RGB[ 0 ], entries 1..127)
	for( int i = 0; i < 256; ++i ){
		const int r = FilePalette[ i ].peRed, g = FilePalette[ i ].peGreen, b = FilePalette[ i ].peBlue;
		const float lum = ( 0.299f * r + 0.587f * g + 0.114f * b ) / 255.0f;
		RockBright[ i ] = min( 1.0f, max( 0.0f, ( lum - ROCK_LUM0 ) / ( ROCK_LUM1 - ROCK_LUM0 ) ) );
		if( i < 1 || i > 127 ) continue;
		const int dr = r - 240, dg = g - 244, db = b - 252, d = 2 * dr * dr + 4 * dg * dg + 3 * db * db;
		if( d < bestD ){ bestD = d; best = i; }
	}
	RockSnowE = (uchar)best;
	if( RockCacheOf.empty() || RockCacheSig != SnowMapSig || RockCacheCel != (const void*)CurrentLevelCELFile ){	// THE CACHE: another map / art
		RockCacheOf.assign( 112 * 112, -1 ); RockCachePool.clear();
		RockCacheSig = SnowMapSig; RockCacheCel = (const void*)CurrentLevelCELFile;
		RoofSnow_BuildAll();	// THATCH ROOFS: after RockBright, which their noise rule reads
		CataSnow_BuildAll();	// THE CATACOMBS BUILDING: after them (RoofSnow_BuildAll clears RoofCellOf)
	}
}

// ADRIA'S HUT (above): the snowed pixels of hut cell ( row, col )'s column, appended to RockCachePool, counted in n
static void HutSnow_Build( int row, int col, int& n )
{
	const int dr = row - HutAnchorRow, dc = col - HutAnchorCol;
	if( HutAnchorRow < 0 || abs( dr ) > HUT_SNOW_REACH || abs( dc ) > HUT_SNOW_REACH ) return;
	const int piece = FineMap[ row ][ col ];
	const TileColumn& t = TileMap[ IsometricMapOffset( row, col ) ];
	static uchar art[ 256 * 64 ], opq[ 256 * 64 ];	// the whole column: index up * 64 + dx (up 0 = the floor's bottom row)
	memset( opq, 0, sizeof( opq ) );
	uchar pix[ 1024 ], op[ 1024 ];
	for( int i = 0; i < 8; ++i ) for( int half = 0; half < 2; ++half ){
		const ushort id = half ? t.Layer[ i ].Right : t.Layer[ i ].Left;
		if( !id || !GrassDecodePiece( id, pix, op ) ) continue;
		for( int py = 0; py < 32; ++py ) for( int px = 0; px < 32; ++px ){
			const int k = py * 32 + px, a = ( 32 * i + py ) * 64 + 32 * half + px;
			if( op[ k ] ){ art[ a ] = pix[ k ]; opq[ a ] = 1; }
		}
	}
	for( int up = 0; up < 256; ++up ) for( int dx = 0; dx < 64; ++dx ){
		const int a = up * 64 + dx;
		if( !opq[ a ] ) continue;
		const uchar v = art[ a ], cls = HutEntry[ v ];
		if( !cls || ( up < 32 && !HutSidePiece[ piece ] ) ) continue;		// a hut colour; the floor only on ADRIA'S SIDE
		const float X = (float)( 32 * ( dr - dc ) + dx ), Y = (float)( 16 * ( dr + dc ) - up );
		if( X > HUT_CORNER_X ){
			if( Y >= HUT_EAVE_Y + ( X - HUT_CORNER_X ) * HUT_EAVE_SLOPE ) continue;	// the shelf face
		}else if( Y >= HUT_CORNER_Y + ( X - HUT_CORNER_X ) * 0.5f - ( up < 32 ? HUT_FOOT_MARGIN : 0.0f ) ) continue;	// the foot
		int nn = 0, h = 0;
		for( int uu = max( 0, up - 2 ); uu <= min( 255, up + 2 ); ++uu ) for( int xx = max( 0, dx - 2 ); xx <= min( 63, dx + 2 ); ++xx ){
			const int b = uu * 64 + xx;
			if( opq[ b ] ){ ++nn; h += HutEntry[ art[ b ] ] != 0; }
		}
		if( h * 100 < HUT_SNOW_NEIGH * nn ) continue;
		const int wx = 32 * ( row - col ) + dx, wy = 16 * ( row + col ) - up;	// WORLD px, as the rocks
		const float nz = 0.55f * RockNoise( wx, wy, ROCK_SCALE1, 1 ) + 0.45f * RockNoise( wx, wy, ROCK_SCALE2, 2 );
		if( cls == 1 ? nz + ROCK_BRIGHT_K * ( RockBright[ v ] - 0.5f ) <= ROCK_THRESHOLD : nz <= HUT_WALL_THRESHOLD ) continue;
		RockCachePool.push_back( (ushort)( up * 64 + dx ) );
		++n;
	}
}

// THE CACHE (above): the snowed pixels of map cell ( row, col )'s column, worked out from its art
static int RockSnow_Build( int row, int col )
{
	const int piece = FineMap[ row ][ col ];
	const TileColumn& t = TileMap[ IsometricMapOffset( row, col ) ];
	const int layers = RockPiece[ piece ] == 1 ? 8 : 1;	// kFloorRockTiles: the floor only
	const int at = (int)RockCachePool.size();
	RockCachePool.push_back( 0 );
	uchar pix[ 1024 ], opq[ 1024 ];
	int n = 0;
	if( RoofCellOf[ row * 112 + col ] ) return at;	// THATCH ROOFS: RoofSnow_BuildAll filled it; never the rock rule (an empty entry)
	if( RockPiece[ piece ] == 3 ){ HutSnow_Build( row, col, n ); RockCachePool[ at ] = (ushort)n; return at; }	// ADRIA'S HUT
	for( int i = 0; i < layers; ++i ) for( int half = 0; half < 2; ++half ){
		const ushort id = half ? t.Layer[ i ].Right : t.Layer[ i ].Left;
		if( !id || !GrassDecodePiece( id, pix, opq ) ) continue;
		for( int py = 0; py < 32; ++py ) for( int px = 0; px < 32; ++px ){	// py 0 = the piece's BOTTOM row
			const int k = py * 32 + px;
			const uchar v = pix[ k ];
			if( !opq[ k ] || v == 0 || ( i == 0 && !RockEntryIdx[ v ] ) ) continue;	// not stone
			const int dx = 32 * half + px, up = 32 * i + py;
			const int wx = 32 * ( row - col ) + dx, wy = 16 * ( row + col ) - up;	// WORLD px: fixed to the map
			const float nz = 0.55f * RockNoise( wx, wy, ROCK_SCALE1, 1 ) + 0.45f * RockNoise( wx, wy, ROCK_SCALE2, 2 );
			if( nz + ROCK_BRIGHT_K * ( RockBright[ v ] - 0.5f ) <= ROCK_THRESHOLD ) continue;
			RockCachePool.push_back( (ushort)( up * 64 + dx ) );
			++n;
		}
	}
	RockCachePool[ at ] = (ushort)n;
	return at;
}

static void RockSnow_Begin( int piece, int x, int y, int row, int col )
{
	RockOn = false;
	if( !RockSnowOn || !WorkingSurface || row < 0 || (uint)row >= 112 || (uint)col >= 112 || piece <= 0 || piece >= MAX_PIECE ) return;
	if( !RockPiece[ piece ] && !RoofCellOf[ row * 112 + col ] ) return;	// a rock, the hut, or a THATCH ROOF cell
	RockX = x; RockY = y; RockRow = row; RockCol = col;
	RockOn = true;
}

static void RockSnow_End( int upMin, int upMax )
{
	if( !RockOn ) return;
	RockOn = false;
	int& at = RockCacheOf[ RockRow * 112 + RockCol ];
	if( at < 0 ) at = RockSnow_Build( RockRow, RockCol );
	const ushort* o = &RockCachePool[ at ];
	const int n = o[ 0 ], L = Screen_LeftBorder, T = Screen_TopBorder;
	if( !n ) return;
	if( RockSnowN + n > RockSnowCap ){	// room for every pixel of this cell (RockSnowPx above)
		const size_t cap = max( RockSnowN + n, max( (size_t)65536, RockSnowCap * 2 ) );
		RockSnowPix* p = (RockSnowPix*)realloc( RockSnowPx, cap * sizeof( RockSnowPix ) );
		if( !p ) return;
		RockSnowPx = p; RockSnowCap = cap;
	}
	RockSnowPix* out = RockSnowPx + RockSnowN;
	const uchar e = RockSnowE;
	uchar* const ws = WorkingSurface;
	const int W = SunGW;
	// the whole column (x RockX .. + 63, y RockY - 255 .. RockY) inside the drawn part and every layer wanted: the same
	// pixels and records as the tested loop below, without its tests
	if( upMin <= 0 && upMax >= 256 && RockX - L >= 0 && RockX + 63 - L < SunGW && RockY - 255 - T >= 0 && RockY - T < SunGH ){
		for( int i = 1; i <= n; ++i ){
			const int x = RockX + ( o[ i ] & 63 ), y = RockY - ( o[ i ] >> 6 );
			ws[ YOffsetHashTable[ y ] + x ] = e;
			out->rel = ( y - T ) * W + ( x - L ); out->val = e; ++out;
		}
	}else{
		for( int i = 1; i <= n; ++i ){
			const int up = o[ i ] >> 6;
			if( up < upMin || up >= upMax ) continue;
			const int x = RockX + ( o[ i ] & 63 ), y = RockY - up;
			const int lx = x - L, ly = y - T;
			if( lx < 0 || ly < 0 || lx >= SunGW || ly >= SunGH ) continue;	// off the drawn part of the screen
			ws[ YOffsetHashTable[ y ] + x ] = e;
			out->rel = ly * W + lx; out->val = e; ++out;
		}
	}
	RockSnowN = (size_t)( out - RockSnowPx );
}

// (th4) ROCK SNOW (above): TownMap.cpp DrawMostTopTile draws a cell's column a SECOND time, over a hero standing "under"
// it (Player.cpp UnderArcType: the cell up-left of him has upper layers or blocks walking -- a rock does), without the
// TownWater_CellBegin / CellEnd pair, so the rock came out bare while he stood there ("when I move, some rocks lose their
// snow", Mordor). x, y = the column's dst as TownWater_CellBegin takes it. The cached pixels are painted again (and
// recorded again -- a duplicate in RockSnowPx is harmless). upMin / upMax: only the pixels of the layers the redraw
// repainted (up = 32 x layer + row in it), or snow lands on the sprite in front -- RAVENHOLM's Draw.cpp DrawMapCell
// repaints no floor of an open cell and not the top layer; the town's DrawMostTopTile repaints them all (the defaults).
void TownWater_TileRedrawn( int row, int col, int x, int y, int upMin, int upMax )
{
	if( (uint)row >= 112 || (uint)col >= 112 ) return;
	RockSnow_Begin( FineMap[ row ][ col ], x, y, row, col );
	RockSnow_End( upMin, upMax );
}

// (th4) ROCK SNOW (above): the recorded snow pixels the finished frame still shows join the snow layer. Same visibility
// test and coordinates as BuildSnowGround's step 1 (pre-zoom layer px; the frame is the zoomed one).
static void RockSnow_ToLayer( const uchar* frame, int z )
{
	if( !RockSnowN || !SnowLay ) return;
	const int W = SunGW, P = WorkingWidth;
	const RockSnowPix* const end = RockSnowPx + RockSnowN;
	for( const RockSnowPix* p = RockSnowPx; p < end; ++p ){
		const RockSnowPix& c = *p;
		const int y = c.rel / W, x = c.rel - y * W;
		if( y * z >= ScreenHeight || x * z >= ScreenWidth ) continue;
		if( frame[ (size_t)y * z * P + x * z ] == c.val ) SnowLay[ c.rel ] = c.val;
	}
}

static void BuildSnowGround( const uchar* frame, int z )
{
	SnowLayValid = false;
	if( !SnowGroundOn || !SnowLay || !SnowDec || !SunGW || z < 1 ) return;
	const int W = SunGW, H = SunGH, P = WorkingWidth;
	SnowVis.assign( (size_t)W * H, 0 );
	const __m128i zero = _mm_setzero_si128(), one = _mm_set1_epi8( 1 ), three = _mm_set1_epi8( 3 ), speck = _mm_set1_epi8( SNOW_SPECK_MIN );
	// 1. SnowVis: recorded floor that the finished frame still shows
	for( int y = 0; y < H && y * z < ScreenHeight; ++y ){
		uchar* vis = &SnowVis[ (size_t)y * W ];
		const uchar* val = GroundVal + (size_t)y * W;
		const uchar* mark = GroundMark + (size_t)y * W;
		const uchar* fr = frame + (size_t)y * z * P;
		int x = 0;
		if( z == 1 ){
			for( ; x + 16 <= W; x += 16 ){
				const __m128i same = _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( fr + x ) ), _mm_loadu_si128( (const __m128i*)( val + x ) ) );
				const __m128i none = _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( mark + x ) ), zero );
				_mm_storeu_si128( (__m128i*)( vis + x ), _mm_and_si128( _mm_andnot_si128( none, same ), one ) );
			}
		}
		for( ; x < W; ++x ) vis[ x ] = mark[ x ] && x * z < ScreenWidth && fr[ x * z ] == val[ x ];
	}
	// 2. per row: SHOWS (the 3 x 3 sum of SnowVis, from column sums) and the cell's votes (SnowDec, RecordGround)
	SnowRow.assign( (size_t)W + 2, 0 );
	uchar* cs = &SnowRow[ 1 ];		// cs[ x ] = SnowVis of rows y - 1 .. y + 1 at column x; cs[ -1 ] = cs[ W ] = 0
	for( int y = 0; y < H; ++y ){
		uchar* g = SnowLay + (size_t)y * W;
		if( y * z >= ScreenHeight ){ memset( g, 0, W ); continue; }
		const uchar* vis = &SnowVis[ (size_t)y * W ];
		const uchar* up = y > 0 ? vis - W : 0;
		const uchar* dn = y + 1 < H ? vis + W : 0;
		const uchar* art = SnowArtLay + (size_t)y * W;	// THE ART (RecordGround): what goes to SnowGround.hlsl
		const uchar* dec = SnowDec + (size_t)y * W;	// written wherever the mark is, so only read where SnowVis is 1
		const uchar* mark = GroundMark + (size_t)y * W;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ){
			__m128i s = _mm_loadu_si128( (const __m128i*)( vis + x ) );
			if( up ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( up + x ) ) );
			if( dn ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( dn + x ) ) );
			_mm_storeu_si128( (__m128i*)( cs + x ), s );
		}
		for( ; x < W; ++x ) cs[ x ] = vis[ x ] + ( up ? up[ x ] : 0 ) + ( dn ? dn[ x ] : 0 );
		for( x = 0; x + 16 <= W; x += 16 ){
			const __m128i sum = _mm_add_epi8( _mm_add_epi8( _mm_loadu_si128( (const __m128i*)( cs + x - 1 ) ), _mm_loadu_si128( (const __m128i*)( cs + x ) ) ),
			                                  _mm_loadu_si128( (const __m128i*)( cs + x + 1 ) ) );	// 0 .. 9, the pixel itself included
			const __m128i voted = _mm_and_si128( _mm_loadu_si128( (const __m128i*)( vis + x ) ), _mm_loadu_si128( (const __m128i*)( dec + x ) ) );
			const __m128i shows = _mm_or_si128( _mm_cmpgt_epi8( sum, speck ), _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( mark + x ) ), three ) );	// not walk-blocked
			const __m128i ok = _mm_and_si128( _mm_cmpeq_epi8( voted, one ), shows );
			_mm_storeu_si128( (__m128i*)( g + x ), _mm_and_si128( ok, _mm_loadu_si128( (const __m128i*)( art + x ) ) ) );
		}
		for( ; x < W; ++x ) g[ x ] = ( ( vis[ x ] & dec[ x ] ) == 1 && ( cs[ x - 1 ] + cs[ x ] + cs[ x + 1 ] > SNOW_SPECK_MIN || mark[ x ] == 3 ) ) ? art[ x ] : 0;
	}
	// 3. NO LONE SNOW: a snow pixel with fewer than SNOW_LONE_MIN snow pixels among its 8 neighbours is dropped -- votes
	// that pass for single pixels of wall or roof art made bright white dots on the houses (Ogden's walls, roof edges;
	// Mordor's screenshot, 2026-10-09). The fence and the rocks keep their look (checked offline): their snow comes in
	// patches. SnowVis (done with) holds 1 where SnowLay has snow; the same column sums as step 2.
	for( int y = 0; y < H; ++y ){
		uchar* nz = &SnowVis[ (size_t)y * W ];
		const uchar* g = SnowLay + (size_t)y * W;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ) _mm_storeu_si128( (__m128i*)( nz + x ), _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( g + x ) ), zero ), one ) );
		for( ; x < W; ++x ) nz[ x ] = g[ x ] != 0;
	}
	const __m128i lone = _mm_set1_epi8( SNOW_LONE_MIN );	// the sum includes the pixel itself: keep when sum > SNOW_LONE_MIN
	for( int y = 0; y < H; ++y ){
		uchar* g = SnowLay + (size_t)y * W;
		const uchar* nz = &SnowVis[ (size_t)y * W ];
		const uchar* up = y > 0 ? nz - W : 0;
		const uchar* dn = y + 1 < H ? nz + W : 0;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ){
			__m128i s = _mm_loadu_si128( (const __m128i*)( nz + x ) );
			if( up ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( up + x ) ) );
			if( dn ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( dn + x ) ) );
			_mm_storeu_si128( (__m128i*)( cs + x ), s );
		}
		for( ; x < W; ++x ) cs[ x ] = nz[ x ] + ( up ? up[ x ] : 0 ) + ( dn ? dn[ x ] : 0 );
		for( x = 0; x + 16 <= W; x += 16 ){
			const __m128i sum = _mm_add_epi8( _mm_add_epi8( _mm_loadu_si128( (const __m128i*)( cs + x - 1 ) ), _mm_loadu_si128( (const __m128i*)( cs + x ) ) ),
			                                  _mm_loadu_si128( (const __m128i*)( cs + x + 1 ) ) );
			_mm_storeu_si128( (__m128i*)( g + x ), _mm_and_si128( _mm_loadu_si128( (const __m128i*)( g + x ) ), _mm_cmpgt_epi8( sum, lone ) ) );
		}
		for( ; x < W; ++x ) if( cs[ x - 1 ] + cs[ x ] + cs[ x + 1 ] <= SNOW_LONE_MIN ) g[ x ] = 0;
	}
	// 4. NO SNOW CRUMBS ON HOUSES: on house and walk-blocked pixels (marks 3 / 4) a snow pixel needs SNOW_CRUMB_MIN snow
	// pixels in its 5 x 5 window (itself included) -- clusters of 2..10 pixels of wall art survived step 3 as bright dots
	// on Ogden's walls (Mordor, 2026-10-09). Open ground keeps its small patches: snow seen between a bush's leaves is
	// right. The fence and the rocks lost 43 / 6 of ~40000 snow pixels in the offline check -- no visible change.
	for( int y = 0; y < H; ++y ){
		uchar* nz = &SnowVis[ (size_t)y * W ];
		const uchar* g = SnowLay + (size_t)y * W;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ) _mm_storeu_si128( (__m128i*)( nz + x ), _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( g + x ) ), zero ), one ) );
		for( ; x < W; ++x ) nz[ x ] = g[ x ] != 0;
	}
	SnowRow5.assign( (size_t)W + 4, 0 );
	uchar* c5 = &SnowRow5[ 2 ];		// c5[ x ] = snow in rows y - 2 .. y + 2 at column x; two 0 columns each side
	const __m128i crumb = _mm_set1_epi8( SNOW_CRUMB_MIN );
	const uchar* const visBase = SnowVis.data();
	for( int y = 0; y < H; ++y ){
		uchar* g = SnowLay + (size_t)y * W;
		const uchar* mark = GroundMark + (size_t)y * W;
		// the rows y - 2 .. y + 2 that exist, taken once per row (a checked vector access per 16 px and row was most of
		// this function's time); the same sums in the same order as before
		const uchar* rows5[ 5 ]; int nr = 0;
		for( int dy = -2; dy <= 2; ++dy ) if( y + dy >= 0 && y + dy < H ) rows5[ nr++ ] = visBase + (size_t)( y + dy ) * W;
		int x = 0;
		for( ; x + 16 <= W; x += 16 ){
			__m128i s = zero;
			for( int r = 0; r < nr; ++r ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( rows5[ r ] + x ) ) );
			_mm_storeu_si128( (__m128i*)( c5 + x ), s );
		}
		for( ; x < W; ++x ){
			int s = 0;
			for( int r = 0; r < nr; ++r ) s += rows5[ r ][ x ];
			c5[ x ] = (uchar)s;
		}
		for( x = 0; x + 16 <= W; x += 16 ){
			__m128i s = _mm_loadu_si128( (const __m128i*)( c5 + x - 2 ) );
			for( int dx = -1; dx <= 2; ++dx ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( c5 + x + dx ) ) );	// 0 .. 25
			const __m128i mk = _mm_loadu_si128( (const __m128i*)( mark + x ) );
			const __m128i house = _mm_or_si128( _mm_cmpeq_epi8( mk, three ), _mm_cmpeq_epi8( mk, _mm_set1_epi8( 4 ) ) );
			const __m128i drop = _mm_and_si128( house, _mm_cmplt_epi8( s, crumb ) );
			_mm_storeu_si128( (__m128i*)( g + x ), _mm_andnot_si128( drop, _mm_loadu_si128( (const __m128i*)( g + x ) ) ) );
		}
		for( ; x < W; ++x ){
			if( mark[ x ] != 3 && mark[ x ] != 4 ) continue;
			if( c5[ x - 2 ] + c5[ x - 1 ] + c5[ x ] + c5[ x + 1 ] + c5[ x + 2 ] < SNOW_CRUMB_MIN ) g[ x ] = 0;
		}
	}
	RockSnow_ToLayer( frame, z );	// (th4) ROCK SNOW (above): after the filters -- the rock snow is not ground and never voted
	SnowLayValid = true;
}
bool TownWater_SnowGround( const uchar*& g, int& w, int& h )
{
	g = SnowLay; w = SunGW; h = SunGH;
	return SnowLayValid;
}

// (th4) GRASS (above), after the zoom, BEFORE RenderPuddles darkens the puddles: THE LAYER, the BLOCK map and
// the box. Pre-zoom pixel (x, y) of the finished frame is frame[ y z P + x z ]. Runs over the whole screen
// every frame the grass is on, so it goes 16 pixels at a time (SSE2, z = 1 and 2: the only zooms there are);
// the scalar loop is the reference: lay = ( ( mark == 1 || mark == 3 ) && frame == val ) ? 1 + class : 0
// (mark 2 = the river's water grows nothing; 3 = a walk-blocked cell, recorded for the grass only).
static void BuildGrassLayer( const uchar* frame, int z )
{
	GrassLayValid = false;
	if( !GrassOn || !GrassLay || !GrassCls || !MapC00Set || z < 1 ) return;
	const int W = SunGW, H = SunGH, P = WorkingWidth;
	const int BW = ( W + GRASS_BLOCK - 1 ) / GRASS_BLOCK, BH = ( H + GRASS_BLOCK - 1 ) / GRASS_BLOCK;
	GrassBlkRaw.assign( (size_t)BW * BH, 0 );
	GrassBlk.assign( (size_t)BW * BH, 0 );
	GrassBW = BW; GrassBH = BH;
	const __m128i one = _mm_set1_epi8( 1 ), three = _mm_set1_epi8( 3 ), lowByte = _mm_set1_epi16( 0x00FF );
	for( int y = 0; y < H; ++y ){
		uchar* o = GrassLay + (size_t)y * W;
		if( y * z >= ScreenHeight ){ memset( o, 0, W ); continue; }
		const uchar* fr = frame + (size_t)y * z * P;
		const uchar* val = GroundVal + (size_t)y * W;
		const uchar* mark = GroundMark + (size_t)y * W;
		const uchar* cls = GrassCls + (size_t)y * W;
		uchar* braw = &GrassBlkRaw[ (size_t)( y / GRASS_BLOCK ) * BW ];
		int x = 0;
		if( z <= 2 ){
			for( ; x + 16 <= W; x += 16 ){	// (z = 2 reads one pixel past the screen at most: the frame row runs on into its border)
				__m128i fv;
				if( z == 1 ) fv = _mm_loadu_si128( (const __m128i*)( fr + x ) );
				else fv = _mm_packus_epi16( _mm_and_si128( _mm_loadu_si128( (const __m128i*)( fr + 2 * x ) ), lowByte ),
				                            _mm_and_si128( _mm_loadu_si128( (const __m128i*)( fr + 2 * x + 16 ) ), lowByte ) );	// every 2nd pixel
				const __m128i mk = _mm_loadu_si128( (const __m128i*)( mark + x ) );
				const __m128i same = _mm_and_si128( _mm_cmpeq_epi8( fv, _mm_loadu_si128( (const __m128i*)( val + x ) ) ),
				                                    _mm_or_si128( _mm_cmpeq_epi8( mk, one ), _mm_cmpeq_epi8( mk, three ) ) );
				const __m128i out = _mm_and_si128( same, _mm_add_epi8( _mm_loadu_si128( (const __m128i*)( cls + x ) ), one ) );
				_mm_storeu_si128( (__m128i*)( o + x ), out );
				if( _mm_movemask_epi8( _mm_cmpgt_epi8( out, one ) ) ) braw[ x / GRASS_BLOCK ] = 1;	// a root (code 2..4) in this block
			}
		}
		for( ; x < W; ++x ){
			const bool seen = ( mark[ x ] == 1 || mark[ x ] == 3 ) && x * z < ScreenWidth && fr[ x * z ] == val[ x ];
			o[ x ] = seen ? (uchar)( 1 + cls[ x ] ) : 0;
			if( o[ x ] >= 2 ) braw[ x / GRASS_BLOCK ] = 1;
		}
	}
	// no blade out of a puddle (PUDDLES above: their pixels are recorded per floor, still undarkened here)
	if( PuddlesOn ){
		for( size_t k = 0; k < PuddleRecN; ++k ){
			const int px = PuddleRecs[ k ].rel & 0xFFFF, py = PuddleRecs[ k ].rel >> 16;
			if( px < W && py < H && GrassLay[ (size_t)py * W + px ] >= 2 ) GrassLay[ (size_t)py * W + px ] = 1;
		}
	}
	// the reach: a blade stands at most GRASS_BLOCK - 2 px tall and bends less than a block sideways, so the
	// pixels it can reach lie in its own block, the block above, and their neighbours left and right
	int bx0 = BW, by0 = BH, bx1 = 0, by1 = 0;
	for( int by = 0; by < BH; ++by ){
		for( int bx = 0; bx < BW; ++bx ){
			bool any = false;
			for( int dy = 0; dy <= 1 && !any; ++dy ){
				const int yy = by + dy; if( yy >= BH ) break;
				for( int dx = -1; dx <= 1; ++dx ){
					const int xx = bx + dx;
					if( xx >= 0 && xx < BW && GrassBlkRaw[ (size_t)yy * BW + xx ] ){ any = true; break; }
				}
			}
			if( !any ) continue;
			GrassBlk[ (size_t)by * BW + bx ] = 255;
			bx0 = min( bx0, bx ); by0 = min( by0, by ); bx1 = max( bx1, bx + 1 ); by1 = max( by1, by + 1 );
		}
	}
	if( bx1 <= bx0 ) return;	// no grass on screen
	GrassBox[ 0 ] = bx0 * GRASS_BLOCK; GrassBox[ 1 ] = by0 * GRASS_BLOCK;
	GrassBox[ 2 ] = min( W, bx1 * GRASS_BLOCK ); GrassBox[ 3 ] = min( H, by1 * GRASS_BLOCK );
	GrassLayValid = true;
}

// (th4) GRASS: this frame's layers for dx_utility.cpp Grass_Present; false = no grass on screen. lay = THE LAYER
// (w x h, pre-zoom), idx = the palette index the floor gave each pixel (same size; right wherever lay >= 1, i.e.
// on all visible ground: Grass.hlsl colours a blade from it when the UI covers its root), blk = the BLOCK map
// (bw x bh, GRASS_BLOCK px each), box = the grass's box (pre-zoom px x0, y0, x1, y1), c00 = the centre of map
// cell (0,0) in layer px, block = GRASS_BLOCK.
bool TownWater_GrassLayer( const uchar*& lay, const uchar*& idx, int& w, int& h, const uchar*& blk, int& bw, int& bh, int* box, int* c00, int& block )
{
	if( !GrassLayValid ) return false;
	lay = GrassLay; idx = GroundVal; w = SunGW; h = SunGH;
	blk = GrassBlk.data(); bw = GrassBW; bh = GrassBH;
	for( int i = 0; i < 4; ++i ) box[ i ] = GrassBox[ i ];
	c00[ 0 ] = MapC00X; c00[ 1 ] = MapC00Y;
	block = GRASS_BLOCK;
	return true;
}

// (th4) GRASS: the grass share of every map cell, 112 x 112, [row][col], 0..255; gen changes whenever it does.
const uchar* TownWater_GrassCells( uint& gen )
{
	gen = GrassDensGen;
	return GrassDensValid ? &GrassCellDens[ 0 ][ 0 ] : 0;
}

// (th4) Draw.cpp DrawIsoLand, before the cell loop.

void TownWater_BeginFrame()
{
	JobLines_FrameStart();	// (th4) THE JOB LINES: last frame's jobs finished before their outputs are reset below
	if( IceTrees_Active() ) PickTreeSnow();	// (th4) SNOW ON THE TREES: the two snow entries of this level's palette
	LeafN = 0;	// (th4) glowing leaves are recorded again during the loop
	SwayCells.clear(); SwayAfter.clear(); SwayPixN = 0;	// (th4) TREE SWAY, recorded again too
	SwayNeedK = TreeSway_Active();	// (th4) layer 2 only while the trees sway (SwayNeedK)
	CellCount = 0;
	if( RiverArtCel != CurrentLevelCELFile || RiverArtDun != (int)(DUNGEON)Dungeon ){	// (th4) RIVER ART: another level's art
		RiverArtIdx.clear(); RiverArt.clear(); RiverArtFull.clear();
		RiverArtCel = CurrentLevelCELFile; RiverArtDun = (int)(DUNGEON)Dungeon;
	}
	for( int i = 0; i < PlayersMax_4; ++i ) Heroes[ i ].valid = false;
	// (th4) PUDDLES (above): how wet it is this frame; the pixels are recorded again during the loop
	PuddleRecN = 0;
	const int wet = ( WetGround > 0 && InTown() ) ? Weather_WetQ8() : 0;
	PuddlesOn = wet > 0;
	PuddleThr = 255 - min( 255, wet * 255 / 256 );
	// (th4) SUN GROUND (above): recorded again during the loop, while the sun rays or the GRASS are on
	SunGroundOn = InTown() && SunRays_Active();
	GrassOn = Grass_Active();
	SnowGroundOn = ( InTown() || Dungeon == DUN_44_RAVENHOLM ) && SnowGround_Active();	// (th4) SNOW GROUND (above); Ravenholm too
	if( SunGroundOn || GrassOn || SnowGroundOn ){
		const int z = IsZoomDisable ? 1 : 2;
		const int W = ( ScreenWidth + z - 1 ) / z, H = ( ScreenHeight + z - 1 ) / z;
		if( W != SunGW || H != SunGH ){
			free( GroundVal ); free( GroundMark ); free( SunGround ); free( GrassCls ); free( GrassLay ); free( SnowLay ); free( SnowDec ); free( SnowArtLay );
			GroundVal = (uchar*)malloc( (size_t)W * H ); GroundMark = (uchar*)malloc( (size_t)W * H ); SunGround = (uchar*)malloc( (size_t)W * H );
			GrassCls = (uchar*)malloc( (size_t)W * H ); GrassLay = (uchar*)malloc( (size_t)W * H ); SnowLay = (uchar*)malloc( (size_t)W * H );
			SnowDec = (uchar*)malloc( (size_t)W * H ); SnowArtLay = (uchar*)malloc( (size_t)W * H );
			if( !GroundVal || !GroundMark || !SunGround || !GrassCls || !GrassLay || !SnowLay || !SnowDec || !SnowArtLay ){ SunGW = SunGH = 0; SunGroundOn = GrassOn = SnowGroundOn = false; }
			else{ SunGW = W; SunGH = H; }
		}
		if( SunGroundOn || GrassOn || SnowGroundOn ) memset( GroundMark, 0, (size_t)SunGW * SunGH );
	}
#if TH4_JOBLINES_VERIFY
	GroundVerify_Start();	// (th4) THE JOB LINES: the shadow layers start as the real ones
#endif
	if( SnowGroundOn ) SnowVotesEnsure();	// (th4) SNOW GROUND -- THE VOTES: before the first RecordGround of the frame
	if( RiverFlowDirty && InTown() ) BuildRiverFlow();	// (th4) RUNNING RIVER: the flow map, once per town load
	if( GrassOn || PuddlesOn ) GrassArtEnsure();	// (th4) GRASS art cache + cell densities; the PUDDLES use them too (NOT ON GRASS)
	MapC00Set = false;	// (th4) THE MAP ANCHOR: found again by this frame's first TownWater_FloorDrawn
	RockSnow_BeginFrame();	// (th4) ROCK SNOW: after SnowGroundOn and the layer size (SunGW / SunGH) are settled
}

// (th4) PUDDLES (above): TownMap.cpp DrawSceneTownTop, right after the FLOOR layer (layer 0) of cell
// (row, col) was drawn at x, y (its DrawTileFill dst, pre-zoom). Records the cell's puddle pixels that
// are on screen and wet enough, with the colour the floor gave them -- and, for the sun rays, the whole
// floor diamond (SUN GROUND, RecordGround).
void TownWater_FloorDrawn( int row, int col, int x, int y )
{
	if( (uint)row >= 112 || (uint)col >= 112 || !WorkingSurface ) return;
	if( !MapC00Set ){	// (th4) THE MAP ANCHOR (top of the file)
		MapC00X = x + 32 - 32 * ( row - col ) - Screen_LeftBorder;
		MapC00Y = y - 15 - 16 * ( row + col ) - Screen_TopBorder;
		MapC00Set = true;
	}
	if( SunGroundOn || GrassOn || SnowGroundOn ) RecordGround( row, col, x, y );	// (th4) SUN GROUND, GRASS and SNOW GROUND (above)
	if( !PuddlesOn ) return;
	if( PuddleFirst[ row ][ col ] < 0 ) ComputeCellPuddles( row, col );
	const int n = PuddleCnt[ row ][ col ];
	if( !n ) return;
	if( PuddleRecN + n > PuddleRecCap ){
		const size_t cap = max( PuddleRecN + n, max( (size_t)16384, PuddleRecCap * 2 ) );
		PuddleRec* p = (PuddleRec*)realloc( PuddleRecs, cap * sizeof( PuddleRec ) );
		if( !p ) return;
		PuddleRecs = p; PuddleRecCap = cap;
	}
	const PuddlePx* q = &PuddlePool[ PuddleFirst[ row ][ col ] ];
	const int L = Screen_LeftBorder, T = Screen_TopBorder;
	for( int i = 0; i < n; ++i ){
		if( q[ i ].depth <= PuddleThr ) continue;
		const int px = x + q[ i ].dx, py = y + q[ i ].dy;
		if( px < L || px >= L + ScreenWidth || py < T || py >= T + ScreenHeight ) continue;
		PuddleRec& r = PuddleRecs[ PuddleRecN++ ];
		r.rel = ( ( py - T ) << 16 ) | ( px - L );
		r.val = WorkingSurface[ YOffsetHashTable[ py ] + px ];
		r.depth = q[ i ].depth;
	}
}

// (th4) RIVER ART (above): the water map of a floor tile, as an index into RiverArt (decoded on first use).
static int RiverArtIndex( ushort id )
{
	if( RiverArtIdx.empty() ) RiverArtIdx.assign( 65536, -1 );
	int& slot = RiverArtIdx[ id ];
	if( slot >= 0 ) return slot;
	const size_t at = RiverArt.size();
	RiverArt.resize( at + 1024, 0 );
	RiverArtFull.resize( at + 1024, 0 );	// (th4) ICE RIVER: the whole art, same index (above)
	uchar pix[ 1024 ], opq[ 1024 ];
	if( id && GrassDecodePiece( id, pix, opq ) )
		for( int i = 0; i < 1024; ++i ) if( opq[ i ] ){
			RiverArtFull[ at + i ] = pix[ i ];
			if( WaterIdx[ pix[ i ] ] ) RiverArt[ at + i ] = pix[ i ];
		}
	slot = (int)( at / 1024 );
	return slot;
}

// (th4) Snowfall.cpp SNOW DEPTH: does a character drawn at map cell (row, col), its feet (dx, dy) px from that cell's
// floor centre (the hero's xOfs / yOfs; 0, 0 for towners), stand on the FROZEN RIVER? Then its legs show (ice, not
// deep snow). Judged like the SNOW GROUND judges a river cell's floor (SNOW GROUND RULES, THE RIVER): the cell under
// the feet must be a river cell, and more than half of the floor art in a FEET_ICE_W x FEET_ICE_H window round the
// feet must be water (kWaterEntries, the RIVER ART maps). A whole river cell is not enough -- it is bank AND water,
// and on its snowy bank the legs showed (Mordor's screenshot by the river). Pixels of the window outside the cell's
// 64 x 32 floor box do not count. The floor's centre is (32, -15) from its DrawTileFill dst (the PUDDLES' geometry).
// (th4) ... and, under the SNOW GROUND, does the character stand where the ground got NO snow? Then its legs show too:
// burying them pastes back the bare ground behind them, a dark see-through hole in the snow (Wirt under his tree by
// Ogden's tavern: the roots stay bare). Judged by the cell's votes (SnowCellVotes, worked out when the cell's floor was
// recorded, before its characters are drawn) in the same window: less than half snow = bare.
bool TownWater_FeetBare( int row, int col, int dx, int dy )
{
	const int FEET_ICE_W = 8, FEET_ICE_H = 4;	// half sizes of the window
	// the cell the feet are over: one map step is (32, 16) px along row, (-32, 16) along col
	const int dr = (int)floorf( ( dx / 32.0f + dy / 16.0f ) * 0.5f + 0.5f ), dc = (int)floorf( ( dy / 16.0f - dx / 32.0f ) * 0.5f + 0.5f );
	row += dr; col += dc;
	dx -= 32 * ( dr - dc ); dy -= 16 * ( dr + dc );
	if( (uint)row >= 112 || (uint)col >= 112 ) return false;
	const int piece = FineMap[ row ][ col ];
	if( piece <= 0 || piece >= MAX_PIECE ) return false;
	if( !RiverPiece[ piece ] ){								// no river: the SNOW GROUND's votes (above)
		if( !SnowGroundOn || SnowDecOf.empty() ) return false;
		const int votes = SnowDecOf[ (size_t)row * 112 + col ];
		if( votes == SNOW_DEC_NONE ) return true;
		if( votes < 0 ) return false;						// all snow, or not worked out yet
		int n = 0, snow = 0;
		for( int y = -15 + dy - FEET_ICE_H; y <= -15 + dy + FEET_ICE_H; ++y ){
			if( y < -31 || y > 0 ) continue;
			const int k = y + 31, half = SnowWideHalf( k );
			for( int x = 32 + dx - FEET_ICE_W; x <= 32 + dx + FEET_ICE_W; ++x ){
				if( x < 32 - half || x >= 32 + half ) continue;
				++n; snow += SnowDecPool[ (size_t)votes + SnowRowStart[ k ] + x - ( 32 - half ) ];
			}
		}
		return n && snow * 2 < n;
	}
	const TileColumn& t = TileMap[ IsometricMapOffset( row, col ) ];
	const int il = RiverArtIndex( t.Layer[ 0 ].Left ), ir = RiverArtIndex( t.Layer[ 0 ].Right );
	const uchar* artL = &RiverArt[ (size_t)il * 1024 ]; const uchar* artR = &RiverArt[ (size_t)ir * 1024 ];
	int n = 0, water = 0;
	for( int y = -15 + dy - FEET_ICE_H; y <= -15 + dy + FEET_ICE_H; ++y ){
		if( y < -31 || y > 0 ) continue;
		for( int x = 32 + dx - FEET_ICE_W; x <= 32 + dx + FEET_ICE_W; ++x ){
			if( x < 0 || x > 63 ) continue;
			++n;
			water += ( x < 32 ? artL[ -y * 32 + x ] : artR[ -y * 32 + x - 32 ] ) != 0;	// piece row 0 = the bottom row (y 0)
		}
	}
	return n && water * 2 > n;
}

// (th4) TownMap.cpp DrawSceneTownTop (and Draw.cpp DrawSceneDungeonTop for Ravenholm), once per drawn cell:
// piece = FineMap[row][col], x/y = the cell's DrawTileFill dst (pre-zoom WorkingSurface coordinates),
// floorL / floorR = the tile ids its floor is drawn with (TileMap layer 0) -- for the RIVER ART test.
void TownWater_NoteCell( int piece, int x, int y, ushort floorL, ushort floorR )
{
	if( piece <= 0 || piece >= MAX_PIECE || !RiverPiece[ piece ] || CellCount >= MAX_CELLS ) return;
	const int artL = RiverArtIndex( floorL ), artR = RiverArtIndex( floorR );
	Cells[ CellCount++ ] = { x, y, artL, artR };
}

// (th4) TownMap.cpp DrawDynamicObjectsTown, after each hero CL2_Draw: the same x, y, sprite,
// frame and width. A hero drawn twice in one frame (DrawMostTopTile redraws) just overwrites.
void TownWater_NotePlayer( int playerIndex, int x, int y, char* cl2, int frame, int width )
{
	if( (uint)playerIndex >= PlayersMax_4 ) return;
	Heroes[ playerIndex ] = { true, x, y, frame, width, cl2 };
}

// (th4) Draw.cpp, right after DrawIsoLand and before Perspective_WarpWorld (see the header).
// (th4) GLOWING RIVER -- on bioluminescent nights (Fireflies.cpp GlowNight_Active) the river is
// coloured in TRUE COLOUR by the BioGlow shader (dx_utility.cpp BIOGLOW, BioGlow.hlsl), not with
// palette entries (the town palette has only a handful of blues). This step only tells the shader
// where the water is and how bright each water pixel is: BioGlow_SetWater( x, y, 1..255 ) from the
// pixel's brightness in the day palette (FilePalette, so the night blend does not flatten the
// texture), stretched over the brightness range of the water entries. It also feeds the soft halo
// (BioGlow_Add). Every water pixel keeps its own brightness -- that is the river's texture. Two
// small corrections in a GLOW_WIN window (world px each side, around each river pixel):
//   - a NON-water pixel with at least GLOW_FILL_PCT water around it is a mud / stone grain inside
//     the stream (a palette entry not in kWaterEntries, which would stay dark and float on the
//     glow): it is filled in with the average brightness of the water in that window;
//   - a water pixel with less than GLOW_DROP_PCT water around it is a thin line of a water entry
//     along the bridge or a bank (it lit up as a bright speck): it is left out.
// Real water next to a grain has far more water around it than GLOW_DROP_PCT, so the grains no
// longer eat into the stream. Sprite and void pixels (>= 128 or 0: a hero or cow in the water)
// are never filled.
namespace {
PALETTEENTRY GlowPal[ 256 ];
bool GlowReady = false;
short GlowV[ 256 ];				// palette entry -> water brightness 0..254 (-1 = sprite / void entry)
// (th4) Plain growing buffers, not std::vector: GlowRiver writes one entry per glowing pixel, ~23,000 a frame
int* GlowPix = 0; size_t GlowPixCap = 0;		// frame offsets marked this frame (GlowRiver_DropCovered)
uchar* GlowVal = 0; size_t GlowValCap = 0;		// the frame index seen there
size_t GlowPixN = 0;
int* GlowRowLo = 0; size_t GlowRowLoCap = 0;	// per screen row: first / last river pixel (INT_MAX / -1 = none)
int* GlowRowHi = 0; size_t GlowRowHiCap = 0;
// THE WATER COUNTS (GlowRiver, below): per grid row its blocks' columns (CandA..B) and the columns whose row counts are
// needed (NeedA..B); the row counts (water cells among the 2 GLOW_WIN + 1 grid cells around, Gw x Gh); one grid row's
// cells (water 1 / 0) with GLOW_WIN zero cells padded on each side; one block row's window counts
int* GlowCandA = 0; size_t GlowCandACap = 0;
int* GlowCandB = 0; size_t GlowCandBCap = 0;
int* GlowNeedA = 0; size_t GlowNeedACap = 0;
int* GlowNeedB = 0; size_t GlowNeedBCap = 0;
uchar* GlowRowCnt = 0; size_t GlowRowCntCap = 0;
uchar* GlowCells = 0; size_t GlowCellsCap = 0;
uchar* GlowWinCnt = 0; size_t GlowWinCntCap = 0;
const int GLOW_WIN      = 2;	// window half-size, 1x world px (sampled every zoom px)
const int GLOW_FILL_PCT = 60;	// a non-water pixel with this much water around it is a grain in the stream
const int GLOW_DROP_PCT = 30;	// a water pixel with less water than this around it is a thin line, not river
int GlowLum( int i ){ return ( FilePalette[ i ].peRed * 2 + FilePalette[ i ].peGreen * 4 + FilePalette[ i ].peBlue * 3 ) / 9; }
template<class T> bool GlowGrow( T*& p, size_t& cap, size_t need )	// keeps the contents
{
	if( need <= cap ) return true;
	size_t c = cap ? cap : 4096;
	while( c < need ) c *= 2;
	T* q = (T*)realloc( p, c * sizeof( T ) );
	if( !q ) return false;
	p = q; cap = c;
	return true;
}
// first / last non-zero byte of b[ x0 .. x1 ), -1 = none: 16 at a time (SSE2), the same answer as the byte loop
int GlowFirstNonZero( const uchar* b, int x0, int x1 )
{
	const __m128i zero = _mm_setzero_si128();
	int x = x0;
	for( ; x + 16 <= x1; x += 16 ){
		const int m = _mm_movemask_epi8( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( b + x ) ), zero ) ) ^ 0xFFFF;
		if( m ){ unsigned long i; _BitScanForward( &i, (unsigned long)m ); return x + (int)i; }
	}
	for( ; x < x1; ++x ) if( b[ x ] ) return x;
	return -1;
}
int GlowLastNonZero( const uchar* b, int x0, int x1 )
{
	const __m128i zero = _mm_setzero_si128();
	int x = x1;
	for( ; x - 16 >= x0; x -= 16 ){
		const int m = _mm_movemask_epi8( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( b + x - 16 ) ), zero ) ) ^ 0xFFFF;
		if( m ){ unsigned long i; _BitScanReverse( &i, (unsigned long)m ); return x - 16 + (int)i; }
	}
	for( --x; x >= x0; --x ) if( b[ x ] ) return x;
	return -1;
}
}

// Cost: only the river's own pixels are visited -- per screen row, the span from its first to its
// last water pixel, grown by the window -- and the decision is made once per zoomed block (all
// z x z pixels of a block are the same world pixel), with the window sampled every z pixels. The
// brightness of each palette entry is a table (GlowV), rebuilt when the palette changes.
// (th4) THE WATER COUNTS (Mordor, 2026-10-12: a glow night cost ~4.8 ms a frame here, now ~0.9; measured): every block
// and every window sample sits on the GRID of zoomed blocks -- (i z, j z), as a block row starts at a multiple of z and
// the window steps by z -- so the window of block (i, j) is the 5 x 5 grid cells around it, cut to the screen. The
// water in it is counted in two steps, 16 cells at a time (SSE2): each grid row's 5-cell counts once (GlowRowCnt, only
// the columns some block of the 5 rows around needs: GlowNeedA..B), then each block row adds the counts of its 5 rows.
// Only blocks with water around them go on (most of a row's span between two arms of the river has none); a grain
// (not water) that may be filled sums the brightness of its window's water the old way. A cell is water when
// buf != 0 and the frame's entry is 1..127: that is exactly GlowV >= 0 (GlowV is -1 for 0 and 128..255 only).
// The 25 reads per block, each with its own isWater test, were the cost. The counts, areas and so every decision,
// write and halo are the same as the per-block loop's: checked against it offline on 20000 random rivers (z 1..3, odd
// screen sizes, the screen's edges, blood moon or not): the water map, the glow map and GlowPix identical.
static void GlowRiver( uchar* frame, uchar* buf, const Box& box, int z )
{
	const bool bloodRiver = BloodNight_Active();	// once per frame (it hashes the night roll)
	// world anchor (screen position of tile (0,0), as dx_utility.cpp PostFxPresent c9): keeps the blood
	// moon's dithered river halo fixed to the map
	const int arow0 = -PlayerRowPos * z, acol0 = -PlayerColPos * z;
	const int ax0 = 32 * ( arow0 - acol0 ) + ( 200 * z * ( Scroll.xOfs + PlayerShiftY ) / 100 >> 1 ) + Xofs - Screen_LeftBorder;
	const int ay0 = 16 * ( arow0 + acol0 ) + ( 200 * z * ( Scroll.yOfs + PlayerShiftX ) / 100 >> 1 ) + Yofs - Screen_TopBorder;
	GlowPixN = 0;
	if( !GlowReady || memcmp( GlowPal, FilePalette, sizeof( GlowPal ) ) ){
		int lo = 255, hi = 0;
		for( uchar e : kWaterEntries ){ const int l = GlowLum( TerrainDark[ e ] ); lo = min( lo, l ); hi = max( hi, l ); }
		if( hi <= lo ) hi = lo + 1;
		for( int i = 0; i < 256; ++i ){
			if( i == 0 || i >= 128 ){ GlowV[ i ] = -1; continue; }
			const int v = ( GlowLum( i ) - lo ) * 254 / ( hi - lo );
			GlowV[ i ] = (short)( v < 0 ? 0 : ( v > 254 ? 254 : v ) );
		}
		memcpy( GlowPal, FilePalette, sizeof( GlowPal ) );
		GlowReady = true;
	}
	const int P = WorkingWidth, W = ScreenWidth, H = ScreenHeight;
	if( W <= 0 || H <= 0 || z < 1 ) return;
	const int Gw = ( W + z - 1 ) / z, Gh = ( H + z - 1 ) / z;	// the grid: cell (i, j) = pixel (i z, j z)
	// the buffers (they only grow)
	if( !GlowGrow( GlowRowLo, GlowRowLoCap, (size_t)H ) || !GlowGrow( GlowRowHi, GlowRowHiCap, (size_t)H )
	 || !GlowGrow( GlowCandA, GlowCandACap, (size_t)Gh ) || !GlowGrow( GlowCandB, GlowCandBCap, (size_t)Gh )
	 || !GlowGrow( GlowNeedA, GlowNeedACap, (size_t)Gh ) || !GlowGrow( GlowNeedB, GlowNeedBCap, (size_t)Gh )
	 || !GlowGrow( GlowRowCnt, GlowRowCntCap, (size_t)Gw * Gh )
	 || !GlowGrow( GlowCells, GlowCellsCap, (size_t)Gw + 2 * GLOW_WIN ) || !GlowGrow( GlowWinCnt, GlowWinCntCap, (size_t)Gw ) ) return;
	const __m128i zero = _mm_setzero_si128(), one = _mm_set1_epi8( 1 );
	// river span per row
	for( int y = 0; y < H; ++y ){ GlowRowLo[ y ] = INT_MAX; GlowRowHi[ y ] = -1; }
	for( int y = box.y0; y < box.y1; ++y ){
		const uchar* b = buf + y * P;
		const int f = GlowFirstNonZero( b, box.x0, box.x1 );
		if( f < 0 ) continue;
		GlowRowLo[ y ] = f;
		GlowRowHi[ y ] = GlowLastNonZero( b, box.x0, box.x1 );
	}
	const int r = GLOW_WIN * z;
	// 1) each block row's blocks (GlowCandA..B, grid columns), and which columns of the rows around it need counts
	for( int j = 0; j < Gh; ++j ){ GlowCandA[ j ] = 0; GlowCandB[ j ] = -1; GlowNeedA[ j ] = INT_MAX; GlowNeedB[ j ] = -1; }
	const int j0 = box.y0 / z;	// the first block row: by = box.y0 - box.y0 % z
	for( int j = j0; j * z < box.y1; ++j ){
		const int by = j * z;
		// span of this block row: union of the spans of the rows the window reaches
		int lo = INT_MAX, hi = -1;
		for( int yy = max( 0, by - r ); yy <= min( H - 1, by + r ); ++yy ){ lo = min( lo, GlowRowLo[ yy ] ); hi = max( hi, GlowRowHi[ yy ] ); }
		if( hi < 0 ) continue;
		lo = max( 0, lo - r ); hi = min( W - 1, hi + r );
		const int ia = lo / z, ib = hi / z;	// bx = lo - lo % z .. hi, step z
		if( ib < ia ) continue;
		GlowCandA[ j ] = ia; GlowCandB[ j ] = ib;
		for( int jj = max( 0, j - GLOW_WIN ); jj <= min( Gh - 1, j + GLOW_WIN ); ++jj ){
			if( ia < GlowNeedA[ jj ] ) GlowNeedA[ jj ] = ia;
			if( ib > GlowNeedB[ jj ] ) GlowNeedB[ jj ] = ib;
		}
	}
	// 2) the row counts: grid row jj's cells NeedA - GLOW_WIN .. NeedB + GLOW_WIN (water 1 / 0; off the screen 0, as the
	// window never counts those), then each needed column's 5-cell count
	for( int jj = 0; jj < Gh; ++jj ){
		const int na = GlowNeedA[ jj ], nb = GlowNeedB[ jj ];
		if( nb < na ) continue;
		const int c0 = na - GLOW_WIN, c1 = nb + GLOW_WIN;	// GlowCells[ k ] = cell c0 + k
		const int a0 = max( 0, c0 ), a1 = min( Gw - 1, c1 );
		uchar* const cell = GlowCells;						// cell[ i - c0 ], i = c0 .. c1
		for( int i = c0; i < a0; ++i ) cell[ i - c0 ] = 0;
		for( int i = a1 + 1; i <= c1; ++i ) cell[ i - c0 ] = 0;
		const uchar* f = frame + jj * z * P;
		const uchar* b = buf + jj * z * P;
		int i = a0;
		if( z == 1 ) for( ; i + 16 <= a1 + 1; i += 16 ){	// water = buf != 0 and the frame's entry 1..127 (a positive signed byte)
			const __m128i bv = _mm_loadu_si128( (const __m128i*)( b + i ) ), fv = _mm_loadu_si128( (const __m128i*)( f + i ) );
			_mm_storeu_si128( (__m128i*)( cell + i - c0 ), _mm_and_si128( _mm_andnot_si128( _mm_cmpeq_epi8( bv, zero ), _mm_cmpgt_epi8( fv, zero ) ), one ) );
		}
		for( ; i <= a1; ++i ){ const int px = i * z; cell[ i - c0 ] = b[ px ] && GlowV[ f[ px ] ] >= 0; }
		uchar* const rc = GlowRowCnt + (size_t)jj * Gw;
		i = na;
		for( ; i + 16 <= nb + 1; i += 16 ){
			__m128i s = _mm_loadu_si128( (const __m128i*)( cell + i - GLOW_WIN - c0 ) );
			for( int k = -GLOW_WIN + 1; k <= GLOW_WIN; ++k ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( cell + i + k - c0 ) ) );
			_mm_storeu_si128( (__m128i*)( rc + i ), s );
		}
		for( ; i <= nb; ++i ){ int s = 0; for( int k = -GLOW_WIN; k <= GLOW_WIN; ++k ) s += cell[ i + k - c0 ]; rc[ i ] = (uchar)s; }
	}
	// 3) the blocks
	for( int j = j0; j * z < box.y1; ++j ){
		const int ia = GlowCandA[ j ], ib = GlowCandB[ j ];
		if( ib < ia ) continue;
		const int by = j * z;
		const int ja = max( 0, j - GLOW_WIN ), jb = min( Gh - 1, j + GLOW_WIN );
		const int rows = jb - ja + 1;
		const uchar* const fr = frame + by * P;
		const uchar* const br = buf + by * P;
		// the window counts of this block row's blocks: the row counts of its rows added up
		uchar* const wc = GlowWinCnt;
		int i = ia;
		for( ; i + 16 <= ib + 1; i += 16 ){
			__m128i s = _mm_loadu_si128( (const __m128i*)( GlowRowCnt + (size_t)ja * Gw + i ) );
			for( int jj = ja + 1; jj <= jb; ++jj ) s = _mm_add_epi8( s, _mm_loadu_si128( (const __m128i*)( GlowRowCnt + (size_t)jj * Gw + i ) ) );
			_mm_storeu_si128( (__m128i*)( wc + i ), s );
		}
		for( ; i <= ib; ++i ){ int s = 0; for( int jj = ja; jj <= jb; ++jj ) s += GlowRowCnt[ (size_t)jj * Gw + i ]; wc[ i ] = (uchar)s; }
		for( int i0 = ia; i0 <= ib; i0 += 16 ){
			// the blocks of these 16 with water around them (no water: the old loop's "if( !cnt ) continue"), left to right
			int bits;
			if( i0 + 16 <= ib + 1 ) bits = _mm_movemask_epi8( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( wc + i0 ) ), zero ) ) ^ 0xFFFF;
			else{ bits = 0; for( int k = 0; i0 + k <= ib; ++k ) if( wc[ i0 + k ] ) bits |= 1 << k; }
			while( bits ){
				unsigned long kk; _BitScanForward( &kk, (unsigned long)bits ); bits &= bits - 1;
				const int ci = i0 + (int)kk;
				const int bx = ci * z;
				const int fv = GlowV[ fr[ bx ] ];
				if( fv < 0 ) continue;	// void / sprite: never glowing water
				const int cnt = wc[ ci ];
				const int area = rows * ( min( Gw - 1, ci + GLOW_WIN ) - max( 0, ci - GLOW_WIN ) + 1 );	// the window's cells on the screen
				int v;
				if( br[ bx ] ){	// water (fv >= 0): its own brightness (the texture), unless it is a thin line
					if( cnt * 100 < area * GLOW_DROP_PCT ) continue;
					v = fv;
				}else{				// not water: a grain inside the stream gets the water around it
					if( cnt * 100 < area * GLOW_FILL_PCT ) continue;
					int sum = 0;	// the brightness of the window's water: the old per-sample loop, for these few
					for( int jj = ja; jj <= jb; ++jj ){
						const uchar* const fq = frame + jj * z * P;
						const uchar* const bq = buf + jj * z * P;
						for( int ii = max( 0, ci - GLOW_WIN ); ii <= min( Gw - 1, ci + GLOW_WIN ); ++ii ){
							const int q = ii * z;
							if( bq[ q ] && GlowV[ fq[ q ] ] >= 0 ) sum += GlowV[ fq[ q ] ];
						}
					}
					v = sum / cnt;
				}
				// the block's pixels for GlowRiver_DropCovered: x, y clipped to the screen as the writes below
				const int zw = min( z, W - bx ), zh = min( z, H - by );
				const size_t need = GlowPixN + (size_t)zw * zh;
				if( GlowGrow( GlowPix, GlowPixCap, need ) && GlowGrow( GlowVal, GlowValCap, need ) ){
					for( int oy = 0; oy < zh; ++oy )
						for( int ox = 0; ox < zw; ++ox ){
							const int q = ( by + oy ) * P + bx + ox;
							GlowPix[ GlowPixN ] = q; GlowVal[ GlowPixN ] = frame[ q ]; ++GlowPixN;
						}
				}
				// halo along the river. On blood moon nights at BLOOD_RIVER_HALO_PCT of its glow-night
				// strength. The fraction is dithered per block (a block adds its share as a 0/1 chance, the
				// hash keeps it fixed on the map), so small percentages are really that small -- an integer
				// floor of 1 per block would keep a quarter of the halo even at 5%. On blood moon nights
				// the halo also never lights the water itself (BioGlow.hlsl c1.w), only the banks around it.
				const int BLOOD_RIVER_HALO_PCT = 60;
				int amount = ( 1 + v / 90 ) * z * z;
				if( bloodRiver ){
					const int scaled100 = amount * BLOOD_RIVER_HALO_PCT;	// in 1/100 units
					uint h = (uint)( ( bx - ax0 ) / z ) * 73856093U ^ (uint)( ( by - ay0 ) / z ) * 19349663U;
					h ^= h >> 13; h *= 0x5bd1e995U; h ^= h >> 15;
					amount = scaled100 / 100 + ( (int)( h % 100 ) < scaled100 % 100 ? 1 : 0 );
				}
				// the z x z block of BioGlow_SetWater( bx + ox, by + oy, 1 + v / 2 ) (water codes 1..127, leaves use
				// 128..255, GlowLeaves), clipped to the screen, then BioGlow_Add( bx, by, amount ) -- dx_utility.cpp
				// BioGlow_AddLeaf does exactly that in one call; an amount of 0 (a blood moon block whose dither says
				// no) changes no map value
				BioGlow_AddLeaf( bx, by, z, 1 + v / 2, amount );
			}
		}
	}
}

static void GlowRiver_DropCovered( const uchar* frame )
{
	const int P = WorkingWidth;
	const int* const pix = GlowPix; const uchar* const val = GlowVal;
	for( size_t k = 0; k < GlowPixN; ++k ){
		const int p = pix[ k ];
		if( frame[ p ] != val[ k ] ) BioGlow_SetWater( p % P, p / P, 0 );
	}
}

// (th4) PUDDLES (above), after the zoom: the recorded pixels that are still visible become water.
// Does not depend on the river options (only the reflections follow WaterReflection, the one switch
// for hero reflections). Call order: TownWater_Render.
static void RenderPuddles( int z )
{
	PuddleValid = false;
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	// clear what the last frame left (only its box, unless Perspective_WarpWorld spread it)
	if( PSurf && PPrevBox.x1 > PPrevBox.x0 ){
		uchar* b0 = PSurf + P * Screen_TopBorder + Screen_LeftBorder;
		const Box c = PPrevWarped ? Box{ 0, 0, W, H } : PPrevBox;
		for( int y = c.y0; y < c.y1; ++y ) memset( b0 + y * P + c.x0, 0, c.x1 - c.x0 );
	}
	PPrevBox = { 0, 0, 0, 0 };
	PPrevWarped = false;
	if( !PuddlesOn || !PuddleRecN || !SoftSrcSurfacePtr || z < 1 ) return;
	if( !EnsurePuddleSurface() ) return;
	EnsureTerrainDark();
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* buf = PSurf + P * Screen_TopBorder + Screen_LeftBorder;
	const int span = max( 1, 255 - PuddleThr );
	Box box = { W, H, 0, 0 };
	for( size_t k = 0; k < PuddleRecN; ++k ){
		const PuddleRec& q = PuddleRecs[ k ];
		const int rx = ( q.rel & 0xFFFF ) * z, ry = ( q.rel >> 16 ) * z;
		if( rx >= W || ry >= H || frame[ ry * P + rx ] != q.val ) continue;	// covered by something drawn later
		// rim weight: 0 at the water's edge, full a third of the way in (Wet.hlsl softens the rim with it)
		const int code = 128 + min( 127, ( q.depth - PuddleThr ) * 127 * 3 / span );
		for( int oy = 0; oy < z && ry + oy < H; ++oy ){
			uchar* f = frame + ( ry + oy ) * P + rx;
			uchar* b = buf + ( ry + oy ) * P + rx;
			for( int ox = 0; ox < z && rx + ox < W; ++ox ){
				const uchar v = f[ ox ];
				if( v >= 1 && v < 128 ) f[ ox ] = TerrainDark[ v ];	// the river's water colour (WATER COLOUR in the header)
				b[ ox ] = (uchar)code;
			}
		}
		if( rx < box.x0 ) box.x0 = rx; if( rx + z > box.x1 ) box.x1 = min( W, rx + z );
		if( ry < box.y0 ) box.y0 = ry; if( ry + z > box.y1 ) box.y1 = min( H, ry + z );
	}
	if( box.x1 <= box.x0 ) return;
	PPrevBox = box;
	if( WaterReflection ){	// same darkness as the river's reflections (step 4 of TownWater_Render)
		int dark = REFLECT_DARK;
		if( !( PixelLight && g_A8L8Lighting ) ) dark += DayNight_TownSpriteDark();
		const int darkRow = DarkRow( dark );
		for( int i = 0; i < PlayersMax_4; ++i ) if( Heroes[ i ].valid ) DrawReflection( Heroes[ i ], z, frame, buf, darkRow, false );
	}
	PuddleValid = true;
}

// (th4) PUDDLES: Draw.cpp Perspective_WarpWorld -- the whole puddle mask (pitch WorkingWidth), or 0.
uchar* TownWater_PuddleSurface()
{
	if( !PuddleValid ) return 0;
	PPrevWarped = true;	// the warp spreads it beyond PPrevBox; the next RenderPuddles clears all of it
	return PSurf;
}

// (th4) PUDDLES: dx_utility.cpp Wet_BuildMask -- the screen origin of the puddle mask (pitch
// WorkingWidth: 0 = no puddle, 128..255 = puddle, rim .. deep), or 0 when no puddle shows this frame.
const uchar* TownWater_PuddleRows()
{
	if( !PuddleValid || !InTown() || !PSurf ) return 0;
	return PSurf + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
}

// (th4) ICE RIVER -- the river mask made SOLID for the Ice Age mirror (dx_utility.cpp ICE RIVER, IceRiver.hlsl). The mask
// above was made for a ripple, which hides its gaps: the river art is speckled with grains of non-water entries, and its
// darker water is a CHECKERBOARD of a water entry and another one, which the 3-of-4 speckle rule wipes out whole -- under
// a bright mirrored sky both showed as dark grains and square dark bites (offline emulation, 2026-10-10). So, only while
// the mirror shows (IceRiver_Shows: Ice Age town by day -- at night everything here is as before):
//   1. step 1 also records, for every pixel of a river diamond, whether it shows the cell's OWN FLOOR ART (MSurf 1) or
//      something drawn over the river later (MSurf 2: a tree in front of it, a rock, a trunk, a sprite) -- water or not;
//   2. step 2's speckle rule (3 of the 4 direct neighbours) also keeps a pixel with 3 of its 4 DIAGONAL neighbours
//      (MirrorSpeckleVerdicts): a checkerboard has all 4 diagonal ones; a lone grain on the bank, a 2 x 2 grain (2 + 1)
//      and a thin trace along the bank still go (3 of 8 kept those 2 x 2 grains: sky specks on the bank, offline);
//   3. a CLOSING of the water and of what covers the river (MirrorFill: dilate, then erode, by 2 world px, a 5 x 5
//      square) fills the grains and the checkerboards -- only on floor-art pixels within 2 world px of real water --
//      into Surf with their frame index, so the snapshot test, the hero reflections and the mirror all treat them as
//      water. Without the "near real water" test the gaps inside a bush on the bank (covered pixels all round) were
//      filled: sky specks on the bank. A closing never reaches outside the water's box (PrevBox).
// MSurf also tells dx_utility.cpp IceRiver_BuildMask what COVERS the river (2): the branches of a tree standing in front
// of it are terrain colours, and taken for the bank they got the shore's dark line and shallow colour all round them.
// Measured offline on the town: the stream solid, the shoreline as the art draws it; the closing adds a few pixels
// in the bank's tightest bends.
// MSurf has Surf's layout (pitch WorkingWidth, screen origin at Screen_TopBorder / Screen_LeftBorder), so Draw.cpp
// Perspective_WarpWorld warps it with Surf (TownWater_MirrorSurface).
static uchar* MSurf = 0; static int MSurfSize = 0;	// 0 = not a river diamond, 1 = its own floor art, 2 = covered
static Box MPrevBox = { 0, 0, 0, 0 };		// where MSurf was written last (cleared before the next mirror frame)
static bool MPrevWarped = false;			// ... unless Perspective_WarpWorld spread it over the screen
static bool EnsureMirrorSurface()
{
	const int size = ( MaxWorkingHeight + 256 ) * MaxWorkingWidth;	// same as Surf
	if( size <= 0 ) return false;
	if( MSurfSize != size ){
		free( MSurf );
		MSurf = (uchar*)calloc( size, 1 );
		MSurfSize = MSurf ? size : 0;
		MPrevBox = { 0, 0, 0, 0 };
	}
	return MSurf != 0;
}

// 3 of the 4 direct OR 3 of the 4 diagonal neighbours (one zoomed pixel away), the verdicts first (km, as step 2)
static void MirrorSpeckleVerdicts( const uchar* buf, const Box& box, int z, uchar* killMap )
{
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth, bw = box.x1 - box.x0;
	auto w = [&]( int x, int y ){ return x >= 0 && x < W && y >= 0 && y < H && buf[ y * P + x ]; };
	for( int y = box.y0; y < box.y1; ++y ){
		const uchar* row = buf + y * P;
		uchar* km = killMap + ( y - box.y0 ) * bw;
		for( int x = box.x0; x < box.x1; ++x ){
			if( !row[ x ] ){ km[ x - box.x0 ] = 0; continue; }
			const int n4 = w( x - z, y ) + w( x + z, y ) + w( x, y - z ) + w( x, y + z );
			const int nd = w( x - z, y - z ) + w( x + z, y - z ) + w( x - z, y + z ) + w( x + z, y + z );
			km[ x - box.x0 ] = n4 < 3 && nd < 3;
		}
	}
}

// the CLOSING (see above), 16 bytes at a time: masks are bytes in rows padded on both sides, so every load of
// x - R .. x + R stays inside its row. Outside the screen counts as water for the erosion (water running off the
// screen's edge keeps its edge); outside the work area but on screen, as land (no water reaches there).
static void MirrorFill( const uchar* frame, uchar* buf, const uchar* mbuf, const Box& box, int z )
{
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth, R = 2 * z;
	const int gx0 = max( 0, box.x0 - R ), gx1 = min( W, box.x1 + R ), gy0 = max( 0, box.y0 - R ), gy1 = min( H, box.y1 + R );
	const int gw = gx1 - gx0, gh = gy1 - gy0;
	if( gw <= 0 || gh <= 0 ) return;
	const int PAD = 32, S = gw + 2 * PAD, ROWS = gh + 2 * R;	// PAD >= R + 15: a 16-wide load at the row's end stays in
	static std::vector<uchar> a, d;
	a.assign( (size_t)S * ROWS, 0 ); d.assign( (size_t)S * ROWS, 0 );
	auto at = [&]( std::vector<uchar>& v, int y ){ return &v[ (size_t)( y + R ) * S + PAD ]; };	// row y of the work area, column 0
	// two masks in one byte, so one dilation does both: bit 1 = water, or the river covered by something (a tree in
	// front of it) -- the CLOSING; bit 0 = real water only -- dilated, the "near real water" test
	for( int y = 0; y < gh; ++y ){
		const uchar* b = buf + ( gy0 + y ) * P + gx0;
		const uchar* m = mbuf + ( gy0 + y ) * P + gx0;
		uchar* r = at( a, y );
		for( int x = 0; x < gw; ++x ) r[ x ] = b[ x ] ? 3 : ( m[ x ] == 2 ? 2 : 0 );
	}
	const int n16 = ( gw + 15 ) / 16;	// whole 16-byte steps; the last may run into the right pad
	auto hpass = [&]( std::vector<uchar>& src, std::vector<uchar>& dst, bool dil ){
		for( int y = 0; y < gh; ++y ){
			const uchar* s = at( src, y ); uchar* o = at( dst, y );
			for( int i = 0; i < n16; ++i ){
				const int x = i * 16;
				__m128i v = _mm_loadu_si128( (const __m128i*)( s + x - R ) );
				for( int k = -R + 1; k <= R; ++k ){
					const __m128i u = _mm_loadu_si128( (const __m128i*)( s + x + k ) );
					v = dil ? _mm_or_si128( v, u ) : _mm_and_si128( v, u );
				}
				_mm_storeu_si128( (__m128i*)( o + x ), v );
			}
		}
	};
	auto vpass = [&]( std::vector<uchar>& src, std::vector<uchar>& dst, bool dil ){
		for( int y = 0; y < gh; ++y ){
			uchar* o = at( dst, y );
			for( int i = 0; i < n16; ++i ){
				const int x = i * 16;
				__m128i v = _mm_loadu_si128( (const __m128i*)( at( src, y - R ) + x ) );
				for( int k = -R + 1; k <= R; ++k ){
					const __m128i u = _mm_loadu_si128( (const __m128i*)( at( src, y + k ) + x ) );
					v = dil ? _mm_or_si128( v, u ) : _mm_and_si128( v, u );
				}
				_mm_storeu_si128( (__m128i*)( o + x ), v );
			}
		}
	};
	// the pads before an erosion: 0xFF where the work area ends at the screen's edge, else 0
	auto pads = [&]( std::vector<uchar>& v ){
		const uchar l = gx0 == 0 ? 0xFF : 0, r = gx1 == W ? 0xFF : 0, t = gy0 == 0 ? 0xFF : 0, bt = gy1 == H ? 0xFF : 0;
		for( int y = -R; y < gh + R; ++y ){
			uchar* row = &v[ (size_t)( y + R ) * S ];
			if( y < 0 || y >= gh ){ memset( row, y < 0 ? t : bt, S ); continue; }
			memset( row, l, PAD ); memset( row + PAD + gw, r, S - PAD - gw );
		}
	};
	static std::vector<uchar> nearW;
	hpass( a, d, true );	// dilate (both bits)
	vpass( d, a, true );
	nearW = a;				// bit 0 now = within R of real water
	pads( a );
	hpass( a, d, false );	// erode (bit 1 is the closing; bit 0 is not used after this)
	pads( d );
	vpass( d, a, false );
	for( int y = box.y0; y < box.y1; ++y ){
		const uchar* c = at( a, y - gy0 ) - gx0;
		const uchar* nw = at( nearW, y - gy0 ) - gx0;
		const uchar* m = mbuf + y * P;
		const uchar* f = frame + y * P;
		uchar* b = buf + y * P;
		for( int x = box.x0; x < box.x1; ++x ) if( ( c[ x ] & 2 ) && ( nw[ x ] & 1 ) && !b[ x ] && m[ x ] == 1 && f[ x ] ) b[ x ] = f[ x ];
	}
}

void TownWater_Render( int z )
{
	MaskValid = false;
	MirrorValid = false;
	RiverReflPix.clear();	// (th4) RUNNING RIVER: the reflections are drawn again below
	TreeSway_Tick();	// (th4) the wind's clock, once per drawn frame (dx_utility.cpp)
	JobLines_Join();	// (th4) THE JOB LINES: this frame's tree and floor cells done -- read below
#if TH4_JOBLINES_VERIFY
	TreeScan_VerifyFrame(); GroundVerify_Frame(); JobLines_VerifyLog();
#endif
	// (th4) TREE SWAY layers (above) for TreeSway.hlsl; the 8-bit frame is not changed
	if( !SwayCells.empty() && SoftSrcSurfacePtr && z >= 1 ) BuildSwayLayers( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder, z );
	else SwayLayersValid = false;
	// (th4) SUN GROUND (above): which pixels the sun rays may light
	if( SoftSrcSurfacePtr && z >= 1 ) BuildSunGround( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder, z );
	else SunGroundValid = false;
	// (th4) SNOW GROUND (above): which pixels are open ground, and their art
	if( SoftSrcSurfacePtr && z >= 1 ) BuildSnowGround( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder, z );
	else SnowLayValid = false;
	// (th4) GRASS (above): where blades stand and show -- before RenderPuddles changes the puddle pixels
	if( SoftSrcSurfacePtr && z >= 1 ) BuildGrassLayer( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder, z );
	else GrassLayValid = false;
	// (th4) glowing leaves (GLOWING LEAVES above): independent of the water options below
	if( ( InTown() || IceTrees_Active() ) && SoftSrcSurfacePtr && z >= 1 && LeafN
	 && ( GlowNight_Active() || BloodNight_Active() || IceTrees_Active() ) )	// blood moon: gold-red leaves; Ice Age (town, Ravenholm): white
		GlowLeaves( SoftSrcSurfacePtr + WorkingWidth * Screen_TopBorder + Screen_LeftBorder, z );
	// (th4) PUDDLES (above): after the tree layers and leaves have read the finished frame (darkened
	// puddle pixels under a crown would otherwise count as "drawn after the tree" in BuildSwayLayers),
	// before the river steps, and whatever the river options are
	RenderPuddles( z );
	const bool raven = RavenRiver();	// (th4) RAVENHOLM'S RIVER (above): reflections only
	const bool mirror = !raven && IceRiver_Shows();	// (th4) ICE RIVER (above MirrorFill): the mask made solid
	if( !( InTown() || raven ) || !AnyRiverPiece || !FeatureOn() || !SoftSrcSurfacePtr || z < 1 ) return;
	if( !EnsureSurface() ) return;
	EnsureTerrainDark();
	const int W = ScreenWidth, H = ScreenHeight, P = WorkingWidth;
	uchar* frame = SoftSrcSurfacePtr + P * Screen_TopBorder + Screen_LeftBorder;
	uchar* buf = Surf + P * Screen_TopBorder + Screen_LeftBorder;
	// clear what the last frame left: only its box was written, unless Perspective_WarpWorld then
	// resampled the buffer, which spreads it over the whole screen
	if( PrevBox.x1 > PrevBox.x0 ){
		const Box c = PrevWarped ? Box{ 0, 0, W, H } : PrevBox;
		for( int y = c.y0; y < c.y1; ++y ) memset( buf + y * P + c.x0, 0, c.x1 - c.x0 );
	}
	PrevBox = { 0, 0, 0, 0 };
	PrevWarped = false;
	if( !CellCount ) return;

	// 1) river diamonds -> water pixels (DrawTileFill geometry, scaled by the zoom)
	// (th4) A pixel is water only where the frame shows a kWaterEntries entry AND it is the entry the cell's own
	// floor art has there (RIVER ART). This runs on the FINISHED frame, so the diamond also holds what the cells
	// in front drew over it later: the blue rocks (Town.TIL 57..63) beside the river use the same entries, and
	// their tops, the trunk of the bend tree (tile 96) and a sprite's water-coloured pixels counted as water --
	// they flowed with the river, were darkened and mirrored heroes (Mordor: "parts of rocks inherit the flowing
	// river"). Measured offline on the whole town: ~1230 such pixels, all gone with this test; the water itself
	// is unchanged (-20 px at the edges of those rocks, the 3-of-4 speckle rule below).
	const int zsh = z == 1 ? 0 : z == 2 ? 1 : -1;	// screen x -> pre-zoom x: a shift for the zooms there are
	// (th4) Each diamond row covers ONE contiguous screen run: pre-zoom pixels x0..x0+wd-1 become
	// screen columns (x0 - Screen_LeftBorder) * z .. (x0 + wd - Screen_LeftBorder) * z - 1, on the z
	// screen rows sy0..sy0+z-1. So the run is clipped to the screen once per row instead of every
	// pixel being bounds-checked, and the water box grows once per run (its first and last water
	// pixel) instead of per pixel. Same pixels, same writes, same box as the per-pixel loop it
	// replaced (2026-10-02; checked on random cells / zoom 1..3 / off-screen edges).
	Box box = { W, H, 0, 0 };
	int riverPixels = 0;
	uchar* mbuf = 0;	// (th4) ICE RIVER: MSurf (above MirrorFill), cleared where the last mirror frame wrote it
	if( mirror && EnsureMirrorSurface() ){
		mbuf = MSurf + P * Screen_TopBorder + Screen_LeftBorder;
		const Box c = MPrevWarped ? Box{ 0, 0, W, H } : MPrevBox;
		for( int y = c.y0; y < c.y1; ++y ) if( c.x1 > c.x0 ) memset( mbuf + y * P + c.x0, 0, c.x1 - c.x0 );
		MPrevBox = { W, H, 0, 0 };
		MPrevWarped = false;
	}
	for( int c = 0; c < CellCount; ++c ){
		const int ox = Cells[ c ].x, oy = Cells[ c ].y;
		const uchar* artL = &RiverArt[ (size_t)Cells[ c ].artL * 1024 ];	// RIVER ART, row 0 = the bottom row = k 0
		const uchar* artR = &RiverArt[ (size_t)Cells[ c ].artR * 1024 ];
		const uchar* fullL = &RiverArtFull[ (size_t)Cells[ c ].artL * 1024 ];	// (th4) ICE RIVER: the whole art (MSurf)
		const uchar* fullR = &RiverArtFull[ (size_t)Cells[ c ].artR * 1024 ];
		for( int k = 0; k <= 30; ++k ){
			const int y = oy - k;
			int x0, wd;
			if( k <= 15 ){ x0 = ox + 30 - 2 * k; wd = 4 * ( k + 1 ); }
			else         { const int j = k - 15; x0 = ox + 2 * j; wd = 64 - 4 * j; }
			const int sy0 = ( y - Screen_TopBorder ) * z;
			if( sy0 >= H || sy0 + z <= 0 ) continue;
			int sxa = ( x0 - Screen_LeftBorder ) * z, sxb = ( x0 + wd - Screen_LeftBorder ) * z;
			if( sxa < 0 ) sxa = 0;
			if( sxb > W ) sxb = W;
			if( sxa >= sxb ) continue;
			const int sya = sy0 < 0 ? 0 : sy0, syb = sy0 + z > H ? H : sy0 + z;
			for( int sy = sya; sy < syb; ++sy ){
				const uchar* f = frame + sy * P;
				uchar* b = buf + sy * P;
				int first = -1, last = -1;
				riverPixels += sxb - sxa;
				uchar* ma = 0;	// (th4) ICE RIVER: this row of MSurf
				if( mbuf ){
					ma = mbuf + sy * P;
					if( sxa < MPrevBox.x0 ) MPrevBox.x0 = sxa; if( sxb > MPrevBox.x1 ) MPrevBox.x1 = sxb;
					if( sy < MPrevBox.y0 ) MPrevBox.y0 = sy; if( sy >= MPrevBox.y1 ) MPrevBox.y1 = sy + 1;
				}
				for( int sx = sxa; sx < sxb; ++sx ){
					const uchar v = f[ sx ];
#if TH4_TOWNWATER_DEBUG
					if( !Learned ) ++InRiverCount[ v ];
#endif
					if( !WaterIdx[ v ] && !ma ) continue;
					const int dx = Screen_LeftBorder + ( zsh >= 0 ? sx >> zsh : sx / z ) - ox;	// 0..63 across the diamond
					if( ma ) ma[ sx ] = ( dx < 32 ? fullL[ k * 32 + dx ] : fullR[ k * 32 + dx - 32 ] ) == v ? 1 : 2;	// (th4) ICE RIVER: own floor art?
					if( !WaterIdx[ v ] ) continue;
					if( ( dx < 32 ? artL[ k * 32 + dx ] : artR[ k * 32 + dx - 32 ] ) != v ) continue;	// not the floor's own water
					b[ sx ] = v;
					if( first < 0 ) first = sx;
					last = sx;
				}
				if( first >= 0 ){
					if( first < box.x0 ) box.x0 = first; if( last >= box.x1 ) box.x1 = last + 1;
					if( sy < box.y0 ) box.y0 = sy; if( sy >= box.y1 ) box.y1 = sy + 1;
				}
			}
		}
	}
#if TH4_TOWNWATER_DEBUG
	if( !Learned && riverPixels ) LogShares( frame, riverPixels );
#endif
	if( box.x1 <= box.x0 ) return;
	PrevBox = box;

	// 2) speckles: a water pixel needs 3 of its 4 neighbours (one zoomed pixel away) to be water.
	// Drops lone grains of a water entry in the grass; costs the shoreline one pixel.
	// (th4) All verdicts are taken first (killMap, one byte per box pixel) and applied after, as the
	// old kill list did, so a removed pixel never changes a neighbour's count. Columns at least z
	// from both screen edges go 16 at a time with SSE2; the edge columns keep the scalar test.
	const int bw = box.x1 - box.x0, bh = box.y1 - box.y0;
	static std::vector<uchar> killMap;
	if( (int)killMap.size() < bw * bh ) killMap.resize( bw * bh );
	const int xa = box.x0 > z ? box.x0 : z;					// SSE2 range: x - z >= 0 ...
	const int xb = box.x1 < W - z ? box.x1 : W - z;			// ... and x + z < W
	const __m128i zero = _mm_setzero_si128(), one = _mm_set1_epi8( 1 ), three = _mm_set1_epi8( 3 );
	if( mbuf ) MirrorSpeckleVerdicts( buf, box, z, killMap.data() );	// (th4) ICE RIVER: 3 of 8 (above MirrorFill)
	else for( int y = box.y0; y < box.y1; ++y ){
		const uchar* row = buf + y * P;
		const uchar* up = y - z >= 0 ? row - z * P : 0;
		const uchar* dn = y + z < H  ? row + z * P : 0;
		uchar* km = &killMap[ ( y - box.y0 ) * bw ];			// km[x - box.x0] for x in box.x0..box.x1-1
		auto scalar = [&]( int x ){
			if( !row[ x ] ){ km[ x - box.x0 ] = 0; return; }
			const int n = ( x - z >= 0 && row[ x - z ] ) + ( x + z < W && row[ x + z ] )
			            + ( up && up[ x ] ) + ( dn && dn[ x ] );
			km[ x - box.x0 ] = n < 3;
		};
		int x = box.x0;
		for( ; x < box.x1 && x < xa; ++x ) scalar( x );
		for( ; x + 16 <= xb; x += 16 ){
			const __m128i c = _mm_loadu_si128( (const __m128i*)( row + x ) );
			__m128i n = _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( row + x - z ) ), zero ), one );
			n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( row + x + z ) ), zero ), one ) );
			if( up ) n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( up + x ) ), zero ), one ) );
			if( dn ) n = _mm_add_epi8( n, _mm_andnot_si128( _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( dn + x ) ), zero ), one ) );
			const __m128i kill = _mm_andnot_si128( _mm_cmpeq_epi8( c, zero ), _mm_cmplt_epi8( n, three ) );
			_mm_storeu_si128( (__m128i*)( km + x - box.x0 ), _mm_and_si128( kill, one ) );
		}
		for( ; x < box.x1; ++x ) scalar( x );
	}
	for( int y = box.y0; y < box.y1; ++y ){
		uchar* row = buf + y * P + box.x0;
		const uchar* km = &killMap[ ( y - box.y0 ) * bw ];
		int x = 0;
		for( ; x + 16 <= bw; x += 16 ){
			const __m128i keep = _mm_cmpeq_epi8( _mm_loadu_si128( (const __m128i*)( km + x ) ), zero );
			_mm_storeu_si128( (__m128i*)( row + x ), _mm_and_si128( _mm_loadu_si128( (const __m128i*)( row + x ) ), keep ) );
		}
		for( ; x < bw; ++x ) if( km[ x ] ) row[ x ] = 0;
	}

	// 3) darker water: every water pixel goes through TerrainDark (see WATER COLOUR in the header).
	// Surf takes the NEW index, so HeatHaze_BuildMask still finds it (it compares Surf with the world
	// snapshot). Water pixels are always 1..127 (kWaterEntries), and TerrainDark never returns 0,
	// the "no water" value. Runs before the reflection, so the reflection keeps its own darkness.
	// (th4) ICE RIVER: when only the mirror turned this file on (both water options off), the water keeps its undarkened
	// old look -- that is what shows at night, when the mirror is gone
	if( WATER_BRIGHTNESS < 100 && !raven && ( WaterShimmer > 0 || WaterReflection ) ){	// (th4) not Ravenholm's water (RAVENHOLM'S RIVER)
		// (th4) The box of a diagonal river is mostly NOT water, so SSE2 tests 16 pixels at a time
		// (buf non-zero and frame < 128) and skips blocks with none; the pixels that pass get the same
		// TerrainDark lookup one by one. A pixel's test never depends on its neighbours, so updating
		// them inside the block changes nothing.
		for( int y = box.y0; y < box.y1; ++y ){
			uchar* f = frame + y * P;
			uchar* b = buf + y * P;
			int x = box.x0;
			for( ; x + 16 <= box.x1; x += 16 ){
				const __m128i bv = _mm_loadu_si128( (const __m128i*)( b + x ) );
				const __m128i fv = _mm_loadu_si128( (const __m128i*)( f + x ) );
				const __m128i hit = _mm_andnot_si128( _mm_or_si128( _mm_cmpeq_epi8( bv, zero ), _mm_cmplt_epi8( fv, zero ) ), _mm_set1_epi8( (char)0xFF ) );
				int bits = _mm_movemask_epi8( hit );
				while( bits ){
					unsigned long i; _BitScanForward( &i, (unsigned long)bits ); bits &= bits - 1;
					const uchar v = TerrainDark[ f[ x + i ] ];
					f[ x + i ] = v; b[ x + i ] = v;
				}
			}
			for( ; x < box.x1; ++x ){
				if( !b[ x ] || f[ x ] >= 128 ) continue;
				const uchar v = TerrainDark[ f[ x ] ];
				f[ x ] = v; b[ x ] = v;
			}
		}
	}
	if( mbuf ) MirrorFill( frame, buf, mbuf, box, z );	// (th4) ICE RIVER: the grains and checkerboards filled (above), before the reflections

	// 3b) glowing river on bioluminescent nights (GLOWING RIVER above)
	const bool glowNight = GlowNight_Active() || BloodNight_Active();	// blood moon: the river turns crimson the same way
	if( glowNight ) GlowRiver( frame, buf, box, z );

#if TH4_TOWNWATER_DEBUG
	for( int y = box.y0; y < box.y1; ++y ) for( int x = box.x0; x < box.x1; ++x ){
		const int p = y * P + x;
		if( buf[ p ] ){ frame[ p ] = 255; buf[ p ] = 255; }
	}
#endif

	// 4) reflections
	if( WaterReflection ){
		int dark = REFLECT_DARK;
		// Classic (palette) night darkens the terrain entries but not the sprite entries the
		// reflection uses, so add the sprite night shade here. Under Enriched Light the night is
		// in LightSurface and the reflection already sits on the water's (night) light value.
		if( !( PixelLight && g_A8L8Lighting ) ) dark += DayNight_TownSpriteDark();
		const int darkRow = DarkRow( dark );
		for( int i = 0; i < PlayersMax_4; ++i ) if( Heroes[ i ].valid ) DrawReflection( Heroes[ i ], z, frame, buf, darkRow );
	}
	if( glowNight ) GlowRiver_DropCovered( frame );	// (th4) keep the reflections out of the glowing water
	MaskValid = TownWater_RiverRipples();	// frozen (Ice Age): no ripple mask, the ice stays still (Ravenholm too: RAVENHOLM'S RIVER)
	MirrorValid = mbuf != 0;	// (th4) ICE RIVER: the mirror's map is built from Surf + MSurf (TownWater_MirrorRows)
}

// (th4) Does the town river ripple? Off when WaterShimmer is off, and under the Ice Age game changer:
// the river is frozen there, so it keeps its darker colour and the reflections (an ice sheet mirrors
// too) but does not move. Read by dx_utility.cpp PostFx_LevelHasWater (no mask is built at all) and
// by TownWater_Render (MaskValid). Same Ice Age test the rest of the engine uses for "frozen"
// (dx_utility.cpp HeatHaze_LevelHasLava); town is never a quest level, so no isQuest term.
// Never in Classic (FeatureOn): no ripple pass there at all.
bool TownWater_RiverRipples()
{
	return WaterShimmer > 0 && GameMode != GM_CLASSIC && !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) );
}

// (th4) Draw.cpp Perspective_WarpWorld: the whole buffer (pitch WorkingWidth), or 0.
uchar* TownWater_Surface()
{
	if( !MaskValid && !MirrorValid ) return 0;	// (th4) the ICE RIVER reads it against the warped snapshot too
	PrevWarped = true;	// the warp spreads the mask beyond PrevBox; the next Render clears all of it
	return Surf;
}

// (th4) dx_utility.cpp HeatHaze_BuildMask: the screen origin of the mask (pitch WorkingWidth),
// or 0 when this frame has no town water. A pixel is water when its byte is non-zero AND equals
// the world snapshot's index there (see the header).
const uchar* TownWater_MaskRows()
{
	if( !MaskValid || !RiverLevel() || !Surf ) return 0;	// (th4) Ravenholm too (RAVENHOLM'S RIVER)
	return Surf + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
}

// (th4) ICE RIVER: dx_utility.cpp IceRiver_BuildMask -- the same mask (palette index of each water pixel, 0 = none) while
// the Ice Age mirror is on, which never has the ripple mask (MaskValid), and MSurf in cls (ICE RIVER above MirrorFill:
// 2 = the river covered by something drawn later). Both: pitch WorkingWidth, screen origin. 0 = none this frame.
const uchar* TownWater_MirrorRows( const uchar*& cls )
{
	cls = 0;
	if( !MirrorValid || !InTown() || !Surf || !MSurf ) return 0;
	cls = MSurf + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
	return Surf + WorkingWidth * Screen_TopBorder + Screen_LeftBorder;
}

// (th4) ICE RIVER: Draw.cpp Perspective_WarpWorld -- MSurf (the whole buffer, pitch WorkingWidth) while in use, or 0
uchar* TownWater_MirrorSurface()
{
	if( !MirrorValid ) return 0;
	MPrevWarped = true;	// the warp spreads it beyond MPrevBox; the next mirror frame clears all of it
	return MSurf;
}
