#include "stdafx.h"
#include <unordered_map>

// ============================================================================================
// (new by th4) COLD BREATH -- puffs of vapour in the cold (config ColdBreath 0..10, default 5),
// and the green TOXIC FUMES of the fungi.
// ============================================================================================
//
// WHERE (ColdPlace, the one place that decides): under the Ice Age game changer every place that is
// not a quest map (the town and every generated level) plus Ravenholm; and in every game, Ice Age or
// not, the Glacial Caverns and the Frozen Loch. (Mordor's list. The frost on the screen edges,
// dx_utility.cpp Frost_Wanted, covers the same places except Ravenholm.)
// TOXIC FUMES: the fungi (rows marked MOUTH_TOXIC in SpriteMouths[]: Fungal Root, Myconid) exhale
// GREEN puffs EVERYWHERE, cold or not, in every game (Mordor's call: they breathe out spores, not
// condensation). ColdBreath 0 turns them off too. Breath_Active = a cold place, or fumes on screen.
// WHO: heroes, townsfolk and cows, and LIVING monsters -- not undead (BaseMonster MonsterClass
// MON_0_UNDEAD), not stone-cursed (A_15_STONE), not dying, and not the Breathless() ones: golems, the
// magma demon and gargoyle families (rock and stone), the Thorned Hulk and the Flame Fiend.
// HOW:
//   1. NOTES: every sprite drawn this frame is noted (Breath_NotePlayer / NoteMonster / NoteTowner) with
//      the x, y it was drawn at -- pre-zoom WorkingSurface coordinates, the bottom-left corner that
//      CL2_Draw / CEL_Draw get. Call sites: TownMap.cpp DrawDynamicObjectsTown (heroes, towners and cows
//      in town), Draw.cpp DrawPlayer / DrawMonster (dungeons, Ravenholm). Breath_BeginFrame (Draw.cpp
//      DrawIsoLand start) forgets the last frame's notes.
//   2. THE MOUTH, in sprite coordinates (Head::mx = column, Head::mr = rows above the bottom line):
//      - Monsters: SpriteMouths[], one row per monster SPRITE (BaseMonster::AnimationFileName). The
//        facing turns the mouth around the body's axis: column axis + r x DirX, row h - v x DirY / 0.5 --
//        so a side view puts it r px in front of the axis at row h, facing the viewer v px lower, facing
//        away v px higher (behind the head: the puff appears from behind it). Every row was MEASURED on
//        the stand animation from the sprite files and checked by eye in all 8 directions (2026-10-03).
//        A row is used only while the body is there: a frame with no body pixel within BODY_BOX px of
//        the mouth gets no breath (the breath waits for the next frame that has one) -- a serpent
//        slithering low under its standing head, a teleport's last sparkles, the mages' 1-pixel dummy
//        walk frames, a lunge or a recoil.
//        Why a table and not a rule: rules read from the frame (below) put the mouth on wing tips
//        (Slayer, Moon Clan, Reapers, Succubi), on staff orbs (Dark Mage, Magistrate), on spider legs,
//        raised claws and blades, horns and tails; about a quarter of the sprites were wrong whatever
//        the rule. A NEW MONSTER SPRITE belongs in SpriteMouths[]; until it has a row it gets the
//        automatic rule below.
//      - Heroes: fixed -- the sprite's middle column, PLAYER_HEAD_H above the feet (Draw.cpp's overhead
//        HP bar uses the same height); a scan would find the tip of an upright staff or a raised weapon.
//      - Towners, cows, and monster sprites with no SpriteMouths[] row (the Armageddon Walker, whose
//        sprite floats and jumps sideways between facings): read from the frame (WalkCl2 / WalkCel) --
//        its highest opaque row (the TOP) and the middle of the opaque pixels just below it. For a
//        monster with a BaseMonster SpriteHeight the top is taken no higher than SpriteHeight +
//        HEAD_CAP_PAD (a weapon swung above the head is skipped).
//          MOUTH_UPRIGHT       a little below the top, pushed toward the way the creature faces;
//          MOUTH_FRONT         cows (TI_9_COWS): the opaque point of the upper 65% of the body that lies
//                              furthest the way the creature faces (score = facing x * x - facing y * row)
//                              -- a cow holds its head lower than its back;
//          MOUTH_FRONT_IF_WIDE other MON_2_BEAST monsters: the same, only when the body is wider than tall
//                              (FRONT_WIDE_PCT) and seen from a side.
//      The facing: Player::dir; for a monster the facing of the sprite actually drawn (found by its
//      pointer in MonsterSprite::animation[].oriented[] -- Monster::Orientation can disagree while it
//      stands, see Breath_NoteMonster); a cow's direction = which of its
//      animationFrameArray[] it is showing. Results are cached per (sprite, frame): one sprite pointer is
//      one direction of one creature type, so the direction and the row are the same every time.
//      Emptied on a level change.
//   3. BREATHS: each creature breathes out every BREATH_MIN .. BREATH_MIN + BREATH_VAR seconds, its own
//      period and phase (a hash of its id), and not in the frame it is first seen. A breath SPAWNS a puff
//      at the mouth; the puff is fixed to the WORLD from then on (pre-zoom px from tile 0,0, the
//      Atmos_Anchor of dx_utility.cpp), drifts the way its creature faced and SINKS a little (PUFF_SINK:
//      breath leaves the mouth forward and slightly down -- a rising puff read as steam off the
//      forehead; Mordor's call), leans with the town's wind (dx_utility.cpp Wind_Strength), grows and
//      fades over PUFF_LIFE seconds. Walking away leaves it behind, as real breath does.
//   4. DRAWING: dx_utility.cpp Breath_Present calls Breath_Update once per presented frame with the
//      ATMOSPHERE clock (stops on pause), gets the live puffs (screen position, size, opacity; a toxic
//      puff has a NEGATIVE size) and Breath.hlsl draws them as soft see-through vapour over WORLD pixels
//      only, lit by the scene behind them (a puff in a dark corner stays dim), toxic ones green.
//      No shaders (drawmode surface): no breath.
// Nothing here is saved or synced. Breath.cpp is listed in thehell3.vcxproj AND thehell3.vcxproj.filters.
// KNOWN-GOOD CHECK: Ice Age hero in Tristram: every few seconds a small pale puff leaves the face and
// drifts the way the hero looks; Griswold puffs, and each cow puffs from its muzzle whichever way it
// faces; walking leaves the puffs behind; pause freezes them. Ice Age catacombs: goatmen and rats puff,
// skeletons and zombies never. Glacial Caverns without Ice Age: puffs. A normal dungeon without Ice Age,
// or Ravenholm without it: none -- except green puffs from Fungal Roots and Myconids (Mossy Grotto).
// A Slayer puffs from his face, not from the tips of his wings, whichever way he turns.
// ============================================================================================

namespace {

const float PUFF_LIFE  = 2.0f;	// seconds from the breath until the puff is gone
const float PUFF_SINK  = 4.0f;	// px a puff drops below its path over its life (BREATHS in the header)
const float BREATH_MIN = 2.8f;	// a creature breathes out every BREATH_MIN .. BREATH_MIN + BREATH_VAR s
const float BREATH_VAR = 1.4f;
const int MAX_NOTES = 512;
const int MAX_PUFFS = 64;
const int PLAYER_HEAD_H = 78;	// px from a hero's feet to the top of the head (Draw.cpp OverheadBar_PlayerHeight)
const int HEAD_CAP_PAD  = 8;	// a monster's top is taken at most this far above its SpriteHeight
const int FRONT_WIDE_PCT  = 115;	// MOUTH_FRONT_IF_WIDE: the body's width must be over this % of its height
const int BODY_BOX = 5;		// a SpriteMouths[] mouth needs a body pixel at most this far from it (px)

// how the mouth of a sprite with no SpriteMouths[] row is found (THE MOUTH in the header)
enum MouthRule { MOUTH_UPRIGHT, MOUTH_FRONT_IF_WIDE, MOUTH_FRONT };
const int FRONT_BAND_LO = 35;	// MOUTH_FRONT / _IF_WIDE search the body from this % of its height (from its lowest row) up

// (th4) THE MOUTH TABLE -- THE MOUTH in the header. One row per monster sprite: BaseMonster::AnimationFileName
// spelled as in BaseMonster.cpp (compared without case, so "Fat%c" and "fat%c" are one sprite); then
//   h    = the mouth's row above the frame's bottom line, seen from a side (W / E);
//   r    = how far in front of the body's axis it is, seen from a side;
//   v    = how much lower it is seen facing the viewer (and higher facing away) -- measured, NOT r / 2:
//          most heads sit upright, so a snout far forward from the side (r 31) is only 3 px lower from the
//          front;
//   axis = the column of the body's axis (frame width / 2 for most; a few hold a weapon or lean).
// Measured on frame 1 of the stand animation (the n file) from the extracted TH4data.mor / DIABDAT.MPQ:
// the face in the W, S, E and N views, then checked by eye in all 8 directions and on walk frames.
// Mark the MOUTH, not the middle of the face: on a snouted head it is at the bottom of the snout's tip,
// below and in front of the eye -- for the goatmen "down the slope" of the skull. The five goat rows
// were first put at eye height and the puffs came off the forehead (Mordor's correction, measured again
// pixel by pixel); other snouted rows may still sit a little high.
// Monster names after each row are the living ones on that sprite (undead users never breathe).
// No row on purpose: the Armageddon Walker (Monsters\snakb\abi%c.CL2 -- the body floats about 60 px up and
// sits in the left half of the frame facing 0..4, the right half facing 5..7, so no axis fits; it keeps the
// automatic rule), and the Breathless() sprites.
enum { MOUTH_TOXIC = 1 };	// green fumes, exhaled everywhere (TOXIC FUMES in the header)
struct SpriteMouth { const char* sprite; float h, r, v, axis; int flags; };
const SpriteMouth SpriteMouths[] = {
	{ "Monsters\\Bat\\Bat%c.CL2",                          67,     8,     1,    48 },	// Familiar, Quasit, Dark Seraph, Fiend ...
	{ "Monsters\\FalSpear\\Phall%c.CL2",                   49,     7,  3.5f,    64 },	// Green One, Fallen One, Imp Rogue, Carver ...
	{ "Monsters\\FalSword\\Fall%c.CL2",                    50,     7,  1.5f,    66 },	// Imp, Dark One, Fallen One, Yellow One ...
	{ "Monsters\\Rhino\\Rhino%c.CL2",                   79.5f, 18.8f,  3.5f, 79.2f },	// Horned Beast, Hell Spawn, Horned Death, Black Horn ...
	{ "Monsters\\Scav\\Scav%c.CL2",                        29,    13,     2,    64 },	// Scavenger, Ravager, Shadow Beast, Bone Gasher ...
	{ "Monsters\\Mega\\Mega%c.CL2",                        74,    16,     8,    80 },	// Vortex Rogue, Balor, Slayer, Balrog ...
	{ "Monsters\\TSneak\\TSneak%c.CL2",                 60.5f,    13,     1,    64 },	// Blood Warper, Warping Demon, Felltwin, Hybrid ...
	{ "Monsters\\Sneak\\Sneak%c.CL2",                   51.6f,     8,     2,    64 },	// Hidden, Shadow, Unseen, Invisible One ...
	{ "Monsters\\GoatLord\\GoatL%c.CL2",                   75,    11,  2.5f, 75.5f },	// Moon Clan, Hell Clan, Goatlord Boss (18-19), Satyr Lord
	{ "Monsters\\GoatMace\\Goat%c.CL2",                    65,    12,     1,    64 },	// Flesh Clan, Fire Clan, Darkness Clan, Stone Clan ...
	{ "Monsters\\GoatBow\\GoatB%c.CL2",                    66,    11,     1,    64 },	// Night Clan, Blood Clan, Doom Clan, Venom Clan ...
	{ "Monsters\\bSpidr\\bSpidr%c.CL2",                    71,    20,     5,    73 },	// Pit Spider
	{ "Monsters\\Acid\\Acid%c.CL2",                     38.5f, 22.5f,    10, 64.5f },	// Acid Hound, Bile Spitter, Hell Hound, Spitting Terror ...
	{ "Monsters\\Hellbat2\\bhelbt%c.CL2",                  70,     8,     4,    48 },	// Acid Bat, Fire Bat
	{ "Monsters\\FatC\\FatC%c.CL2",                        69,     8,     1,    64 },	// The Butcher, Grim Cleaver, Uber Butcher
	{ "Monsters\\Fat\\Fat%c.CL2",                          69,     8,     1,    64 },	// Bile Demon, Grim Demon, Pit Demon, Flayed One ...
	{ "Monsters\\Eye2\\Eye2%c.CL2",                        76,    12,     6,    70 },	// Beholder
	{ "Monsters\\Nkr\\Nkr%c.CL2",                         111,    10,     1,   113 },	// Abyss Guardian
	{ "Monsters\\Hellbug\\Hellbg%c.CL2",                   96,     5,  2.5f,    99 },	// Pit Fiend, Grimspike
	{ "Monsters\\Snake\\Snake%c.CL2",                      83,    31,     3,    79 },	// Gore Viper, Azure Drake, Cave Snake, Gold Serpent ...
	{ "Monsters\\Thin\\Thin%c.CL2",                        79,    17,     5,    80 },	// Thunder Lord, Death Claw, Storm Demon, Eviscerator ...
	{ "Monsters\\BigFall\\Fallg%c.CL2",                    61,    22,    11,    65 },	// Devil Kin, Imp Brute, Greater Imp, Turbak the Deuce ...
	{ "Monsters\\Clasp\\Clasp%c.CL2",                      32,    16,     2,    88 },	// Lash Worm
	{ "Monsters\\Reaper\\Reap%c.CL2",                      77,    10,     4,    91 },	// Dark Reaper, Blood Slayer, Black Reaper, Death Reaper ...
	{ "Monsters\\Fork\\Fork%c.CL2",                     53.5f,    27,    10,    93 },	// Dark Charger, Wailing Beast, Mud Runner, Pinky ...
	{ "Monsters\\Succ\\Scbs%c.CL2",                        61,     6,     1,    67 },	// Dark Mistress, Succubus, Blood Witch, Dark Witch ...
	{ "Monsters\\Order\\order%c.cl2",                      65,     7,     1,    64 },	// Corrupt Order, Zealot
	{ "Monsters\\DarkMage\\Dmage%c.CL2",                   71,     6,     1, 64.5f },	// Dark Mage, Zhar basemnstr, Advocate
	{ "Monsters\\Mage\\Mage%c.cl2",                        74,     5,     2,    65 },	// Magistrate, Cabalist, Adept, Counselor ...
	{ "Monsters\\Diablo\\Diablo%c.CL2",                   109,     9,     1,    80 },	// Greater Demon, Uber Diablo
	{ "Monsters\\Byclps\\Byclps%c.CL2",                    69,     6,     2,    90 },	// Grunt Beast, Biclops
	{ "Monsters\\Scorp\\Scorp%c.CL2",                      25,    17,  8.5f,    32 },	// Stinger, Doom Tail
	{ "Monsters\\Rat\\Rat%c.CL2",                       57.5f,    19,     3,    51 },	// Plague Eater, Rat Man, Rat Horde
	{ "Monsters\\AntWorm\\Worm%c.CL2",                     62, 36.5f,     4, 95.5f },	// Wyrm, Torchant
	{ "Monsters\\renwar01\\rw%c.CL2",                      65,  4.5f,     2,    64 },	// Corrupt Paladin, Mercenary
	{ "Monsters\\wscorp\\WScorp%c.CL2",                    25,    21,    10,    43 },	// Stinger Lord
	{ "Monsters\\hellbat\\helbat%c.CL2",                   70,     8,     4,    48 },	// Hell Bat
	{ "Monsters\\Warped1\\w1%c.CL2",                       50,     7,  1.5f,    66 },	// Warped One
	{ "Monsters\\Eye\\Eye%c.CL2",                          76,    12,     6,    78 },	// Cacodemon
	{ "Monsters\\satyr\\satyr%c.CL2",                      75,    11,  2.5f, 75.5f },	// Satyr Lord
	{ "Monsters\\Baboon\\Baboon_%c.CL2",                   81,    25,     7,   128 },	// Night Demon
	{ "Monsters\\siegebreaker\\siegebreaker_%c.CL2",      156,    44,    11,   128 },	// Siege Beast, Siegebreaker Beast
	{ "Monsters\\Goatmother\\1\\Onoskelis%c.CL2",          98,    17,     2,    79 },	// Abyssal Succubus
	{ "Monsters\\Minotaur\\minotaur%c.CL2",                96,    16,     6,   112 },	// Blood Lord
	{ "Monsters\\Minotaur\\bloodlord%c.CL2",              108,    20,     8,   112 },	// Steel Lord
	{ "Monsters\\Hell_witch\\HellWitch%c.CL2",             94,  9.5f,     1,    82 },	// Stygian Fury
	{ "Monsters\\Regurgitator\\Regurgitator_%c.CL2",       69,    77,    29,   110 },	// Corpse Eater
	{ "Monsters\\tainted\\tainted_%c.CL2",                 91,    34,     3,   112 },	// Tainted, Afflicted
	{ "Monsters\\council\\councilmember_%c.CL2",          102,    18,     1,   112 },	// Demon Priest
	{ "Monsters\\Izual\\Izual_%c.CL2",                  139.5f,    13,     6,   144 },	// Izual
	{ "Monsters\\blunderbore\\blunderbore_%c.CL2",        190,     4,     1,   175 },	// Blunderer
	{ "Monsters\\salamander\\salamander_%c.CL2",          122,    23,     5,   144 },	// Salamander
	{ "Monsters\\scarab\\scarabdemon_%c.CL2",             118,    17,     8,   128 },	// Scarab, Bone Scarab
	{ "Monsters\\wendigo\\wendigo_%c.CL2",                115, 13.5f,     0, 110.5f },	// Wendigo
	{ "Monsters\\Smith\\_%c.CL2",                         124,    10,     4,   112 },	// Hephasto
	{ "Monsters\\exploder\\_%c.CL2",                      104,    19,     8,    97 },	// Exploder
	{ "Monsters\\Goat_Long_Mace\\GoatmanLongMace_%c.CL2",     97,     9,     3, 96.5f },	// Twilight Clan
	{ "Monsters\\wasserman\\sobsna_%c.CL2",             120.7f,    99, 49.5f,   191 },	// Water Wyrm, Red Wyrm
	{ "Monsters\\spawn\\Spawn%c.CL2",                      24, 10.5f,     3,    83 },	// Biteling
	{ "Monsters\\croc\\200w\\_%c.CL2",                  112.8f,    30,    15,   100 },	// Swamp Beast
	{ "Monsters\\croc\\200w\\brs\\_%c.CL2",             112.8f,    30,    15,   100 },	// Swamp Beast
	{ "Monsters\\bog_crawler\\voreworm_%c.CL2",            87,    20,    12,    74 },	// Bog Crawler, Mud Crawler
	{ "Monsters\\spider_queen\\SpQ_%c.CL2",                92,     9,     2,    86 },	// Spider Queen
	{ "Monsters\\glabrezu\\1\\_%c.CL2",                   102,     7,     1,    87 },	// Flesh Devourer
	{ "Monsters\\chicken\\gronk_%c.CL2",                   90,    28,    13, 86.5f },	// Behemoth
	{ "Monsters\\spider\\bigg\\GiantSpider_%c.CL2",        42, 10.5f,     7, 62.5f },	// Sword Spider
	{ "Monsters\\spider\\small\\SmallSpider_%c.CL2",       31,     7,     4,    31 },	// Spiderling
	{ "Monsters\\boevoy_baran_boris\\trelon_%c.CL2",       84,    37, 18.5f,    93 },	// Leaper
	{ "Monsters\\floater\\GrFeyr_%c.CL2",                  74,  7.5f,     5, 65.5f },	// Floater
	{ "Monsters\\floater\\X2\\GrFeyr_%c.CL2",              74,     6,     6,    66 },	// Floater
	{ "Monsters\\Falbow\\%c.CL2",                          65,    12,     2,    49 },	// Fallen One, Green One
	{ "Monsters\\fungal_root\\shrieker_%c.CL2",            53,    15,     4,    43, MOUTH_TOXIC },	// Fungal Root
	{ "Monsters\\myconid\\myconid_%c.CL2",                 66,     7,     1,    47, MOUTH_TOXIC },	// Myconid
	{ "Monsters\\bugbear\\bgbr_cptn_%c.CL2",            138.5f,     8,     3,   102 },	// Bugbear
	{ "Monsters\\pit_fiend\\pit_fiend_%c.CL2",            120,    42,    16,   107 },	// Pit Fiend
	{ "Monsters\\belhi\\1\\belhifet_%c.CL2",              102,    12,     0,   100 },	// Belhifet Uno
	{ "Monsters\\belhi\\2\\belhifet_%c.CL2",              102,    12,     0,   100 },	// Belhifet Dos
	{ "Monsters\\ICE_TROLL\\ice_troll_%c.CL2",             79, 23.5f,    10,    84 },	// Ice Troll
	{ "Monsters\\orog\\orog_chieftan_%c.CL2",             133,    12,     6,   115 },	// Borosteron
	{ "Monsters\\tentaclist\\X_%c.CL2",                   126,    12,     6,   118 },	// Tentaclist Boss
	{ "Monsters\\tentaclist\\85_death_mauler_%c.CL2",     110,    10,     5, 100.5f },	// Tentaclist, Abyssal Tentaclist
	{ "Monsters\\Black\\Black%c.CL2",                      73,    10,     1,    78 },	// Black Knight, Doom Guard, Steel Lord, Blood Knight
	{ "Monsters\\Diablo\\cls\\Diablo%c.CL2",              108,    12,     1,    79 },	// The Dark Lord
};

bool Cold = false;				// this frame: a cold place (Breath_BeginFrame)
bool Fumes = false;				// this frame: toxic notes last frame, or toxic puffs alive (Breath_BeginFrame)
int LastLevel = -1;				// the Dungeon of the last frame; a change empties the caches

struct Head { short top, bot, x, mx, mr; };	// rows above the sprite's bottom line of the highest / lowest
											// opaque pixel, the head's middle column; the MOUTH (column, row).
											// top < 0: no mouth in this frame
std::unordered_map<unsigned long long, Head> HeadCache;

struct Note { int id, x, y; Head h; int dir; bool toxic; };
Note Notes[ MAX_NOTES ]; int NoteN = 0;

struct Puff { float wx, wy, dx, dy, t0; bool toxic; };	// world position of the mouth at the breath (pre-zoom px from tile 0,0), facing, birth time
Puff Puffs[ MAX_PUFFS ]; int PuffN = 0;
std::unordered_map<int, int> LastBreath;	// creature id -> number of its last breath

uint BHash( uint x ) { x ^= x >> 16; x *= 0x7feb352dU; x ^= x >> 15; x *= 0x846ca68bU; x ^= x >> 16; return x; }

// facing -> screen direction of the breath (pre-zoom px), in DIRECTION order (enums.h): down, down-left,
// left, up-left, up, up-right, right, down-right; y is halved like the 2:1 floor
const float DirX[ 8 ] = { 0.0f, -0.7f, -1.0f, -0.7f, 0.0f, 0.7f, 1.0f, 0.7f };
const float DirY[ 8 ] = { 0.5f, 0.35f, 0.0f, -0.35f, -0.5f, -0.35f, 0.0f, 0.35f };

// the opaque span of every row of the frame being scanned (RowStamp == ScanNo: the row has one)
const int SCAN_MAX_ROWS = 512;
short RowMin[ SCAN_MAX_ROWS ], RowMax[ SCAN_MAX_ROWS ];
uint RowStamp[ SCAN_MAX_ROWS ], ScanNo = 0;

void MarkRun( int r, int x0, int x1, int& top, int& bot )
{
	if( RowStamp[ r ] != ScanNo ){ RowStamp[ r ] = ScanNo; RowMin[ r ] = (short)x0; RowMax[ r ] = (short)x1; }
	else{ if( x0 < RowMin[ r ] ) RowMin[ r ] = (short)x0; if( x1 > RowMax[ r ] ) RowMax[ r ] = (short)x1; }
	if( bot < 0 ) bot = r;
	top = r;	// rows only ever grow while scanning (bottom row first)
}

// UPRIGHT mouth: a little below the top of the head, toward the facing
void UprightMouth( Head& h, int dir )
{
	const int height = max( 1, h.top - h.bot );
	const float down = min( 14.0f, max( 3.0f, height * 0.12f ) );
	const float fwd = 2.0f + height * 0.05f;
	h.mx = (short)( h.x + DirX[ dir ] * fwd );
	h.mr = (short)( h.top - down - DirY[ dir ] * fwd );
}

// the automatic rule (THE MOUTH in the header): the head from the rows just scanned (RowMin / RowMax /
// RowStamp), then the mouth. cap > 0: rows above it are not the head -- the top is the highest opaque row
// at or below it.
bool FinishHead( int top, int bot, int cap, int dir, MouthRule rule, Head& h )
{
	if( top < 0 ) return false;
	if( cap > 0 && top > cap ){
		int r = cap < SCAN_MAX_ROWS ? cap : SCAN_MAX_ROWS - 1;
		while( r >= 0 && RowStamp[ r ] != ScanNo ) --r;
		if( r < 0 ) return false;
		top = r;
	}
	const int HEAD_ROWS = 6;
	int sum = 0, n = 0;
	for( int r = top; r > top - HEAD_ROWS && r >= 0; --r ){
		if( RowStamp[ r ] != ScanNo ) continue;
		sum += RowMin[ r ] + RowMax[ r ]; n += 2;
	}
	if( !n ) return false;
	h.top = (short)top; h.bot = (short)bot; h.x = (short)( sum / n );
	const float fx = DirX[ dir & 7 ], fy = DirY[ dir & 7 ];
	const int height = max( 1, top - bot );
	bool front = rule == MOUTH_FRONT;
	if( rule == MOUTH_FRONT_IF_WIDE && fx != 0.0f ){
		int x0 = INT_MAX, x1 = INT_MIN;
		for( int r = bot; r <= top; ++r ) if( RowStamp[ r ] == ScanNo ){ x0 = min( x0, (int)RowMin[ r ] ); x1 = max( x1, (int)RowMax[ r ] ); }
		front = ( x1 - x0 + 1 ) * 100 > height * FRONT_WIDE_PCT;
	}
	if( front ){	// FRONT RULE: the point of the band furthest the way the creature faces
		const int lo = bot + height * FRONT_BAND_LO / 100, hi = top;
		float best = -1e9f; int bx = 0, br = -1;
		for( int r = hi; r >= lo && r >= 0; --r ){	// from the top down: a higher row wins a near-tie
			if( RowStamp[ r ] != ScanNo ) continue;
			const int x = fx > 0.0f ? RowMax[ r ] : fx < 0.0f ? RowMin[ r ] : ( RowMin[ r ] + RowMax[ r ] ) / 2;
			const float score = fx * x - fy * r;	// rows count up, facing y counts down the screen
			if( score > best + 1.0f ){ best = score; bx = x; br = r; }
		}
		if( br >= 0 ){
			h.mx = (short)( bx + ( fx > 0.0f ? -2 : fx < 0.0f ? 2 : 0 ) );	// just inside the muzzle
			h.mr = (short)( br - 1 );
			return true;
		}
	}
	UprightMouth( h, dir & 7 );
	return true;
}

// CL2 (heroes, monsters): the format TownWater.cpp DecodeCl2Frame and Engine.cpp CL2_Draw read -- the
// frame starts with its section table, whose first word is where the pixels start; bottom row first;
// a byte b >= 0 skips b transparent pixels, b < 0 is a run of -b: up to 65 literal pixels, above that
// -b - 65 copies of one pixel. Runs wrap at the width. The Walk* functions only read the stream and
// call run( row, first column, last column ) for every opaque run. Colour 0 is the creature's SHADOW,
// baked into every monster sprite (black, on the ground): WalkCl2 leaves it out -- it is not the body.
template<class Run> bool WalkCl2( const char* cl2, int frame, int width, Run run )
{
	if( !cl2 || width <= 0 || width > 1024 ) return false;
	const uint* header = (const uint*)cl2;
	const uint frameCount = header[ 0 ];
	if( frame < 1 || (uint)frame > frameCount || frameCount > 128 ) return false;
	const char* from = cl2 + header[ frame ];
	const int begin = *(const ushort*)from;
	const int len = (int)( header[ frame + 1 ] - header[ frame ] ) - begin;
	if( begin <= 0 || len <= 0 ) return false;
	const signed char* s = (const signed char*)from + begin;
	const signed char* e = s + len;
	int x = 0, r = 0;
	while( s < e && r < SCAN_MAX_ROWS ){
		const int b = *s++;
		if( b >= 0 ){
			x += b;
			while( x >= width ){ x -= width; ++r; }
			continue;
		}
		int n = -b;
		const signed char* lit = 0;	// n literal pixels, or (lit == 0) n copies of fill
		signed char fill = 0;
		if( n > 65 ){ n -= 65; if( s >= e ) break; fill = *s++; }
		else{ if( n > e - s ) n = (int)( e - s ); lit = s; s += n; }
		int start = -1;	// first column of the body run being collected
		for( int i = 0; i < n && r < SCAN_MAX_ROWS; ++i ){
			const bool body = ( lit ? lit[ i ] : fill ) != 0;	// colour 0 = the shadow
			if( body && start < 0 ) start = x;
			if( !body && start >= 0 ){ run( r, start, x - 1 ); start = -1; }
			if( ++x >= width ){
				if( start >= 0 ){ run( r, start, width - 1 ); start = -1; }
				x = 0; ++r;
			}
		}
		if( start >= 0 && r < SCAN_MAX_ROWS ) run( r, start, x - 1 );
	}
	return true;
}

// CEL (towners, cows): what CEL_DrawTo / RleDraw (engine.h) read -- the frame starts with its section
// table, whose first word (10 or 12) is where the pixels start; bottom row first; a byte b > 0 is b
// literal pixels, b <= 0 skips -b; rows end at the width.
template<class Run> bool WalkCel( const char* cel, int frame, int width, Run run )
{
	if( !cel || width <= 0 || width > 1024 ) return false;
	const uint* header = (const uint*)cel;
	const uint frameCount = header[ 0 ];
	if( frame < 1 || (uint)frame > frameCount || frameCount > 512 ) return false;
	const char* from = cel + header[ frame ];
	const int begin = *(const ushort*)from;
	const int len = (int)( header[ frame + 1 ] - header[ frame ] ) - begin;
	if( ( begin != 10 && begin != 12 ) || len <= 0 ) return false;
	const signed char* s = (const signed char*)from + begin;
	const signed char* e = s + len;
	int x = 0, r = 0;
	while( s < e && r < SCAN_MAX_ROWS ){
		const int b = *s++;
		if( b > 0 ){
			const int n = min( b, (int)( e - s ) );
			if( x < width ) run( r, x, min( x + n, width ) - 1 );
			s += n; x += n;
		}else x -= b;
		while( x >= width ){ x -= width; ++r; }
	}
	return true;
}

// The mouth of one frame of one sprite (THE MOUTH in the header): from its SpriteMouths[] row sm, if the
// body is there in this frame; else (sm == 0) by the automatic rule -- cap: see FinishHead (0 = none),
// rule: MouthRule. dir is the facing. One sprite pointer is always one direction of one creature type, so
// the cache key needs none of them.
bool HeadOf( const char* spr, int frame, int width, bool cel, const SpriteMouth* sm, int cap, int dir, MouthRule rule, Head& h )
{
	if( !spr ) return false;
	const unsigned long long key = (unsigned long long)(uintptr_t)spr ^ ( (unsigned long long)(uint)frame << 40 ) ^ ( cel ? 1ULL << 63 : 0 );
	auto it = HeadCache.find( key );
	if( it != HeadCache.end() ){ h = it->second; return h.top >= 0; }
	Head nh = { -1, -1, 0, 0, 0 };
	if( sm ){
		const int mx = (int)floorf( sm->axis + sm->r * DirX[ dir & 7 ] + 0.5f );
		const int mr = (int)floorf( sm->h - sm->v * 2.0f * DirY[ dir & 7 ] + 0.5f );
		bool body = false;
		auto atMouth = [&]( int r, int x0, int x1 ){
			if( r >= mr - BODY_BOX && r <= mr + BODY_BOX && x1 >= mx - BODY_BOX && x0 <= mx + BODY_BOX ) body = true;
		};
		if( WalkCl2( spr, frame, width, atMouth ) && body ){ nh.top = (short)mr; nh.bot = 0; nh.x = (short)mx; nh.mx = (short)mx; nh.mr = (short)mr; }
	}else{
		++ScanNo;
		int top = -1, bot = -1;
		auto run = [&]( int r, int x0, int x1 ){ MarkRun( r, x0, x1, top, bot ); };
		const bool read = cel ? WalkCel( spr, frame, width, run ) : WalkCl2( spr, frame, width, run );
		if( !read || !FinishHead( top, bot, cap, dir, rule, nh ) ) nh.top = -1;
	}
	if( HeadCache.size() > 16384 ) HeadCache.clear();
	HeadCache[ key ] = nh;
	h = nh;
	return nh.top >= 0;
}

void AddNote( int id, int x, int y, const Head& h, int dir, bool toxic )
{
	if( NoteN >= MAX_NOTES ) return;
	for( int i = 0; i < NoteN; ++i ) if( Notes[ i ].id == id ){ Notes[ i ] = { id, x, y, h, dir & 7, toxic }; return; }	// drawn twice this frame
	Notes[ NoteN++ ] = { id, x, y, h, dir & 7, toxic };
}

// no breath: golems (constructs: the summoned Golem by its AI, the golem monsters by type), the magma
// demon family -- every monster drawn with Monsters\Magma\Magma%c.CL2, creatures of rock and lava -- the
// gargoyle family -- every monster drawn with Monsters\Gargoyle\Gargo%c.CL2 or Gargu%c.CL2, made of
// stone -- the Thorned Hulk (thorns and wood) and the Flame Fiend (living fire). Mordor's calls. A new
// monster on any of these sprites belongs on this list.
bool Breathless( const Monster& m )
{
	if( m.ai == AI_21_Golem || !m.SpritePtr ) return true;
	switch( m.SpritePtr->baseMonsterIndex ){
	case BM_109_GOLEM: case BM_111_OBSIDIAN_GOLEM: case BM_113_ASH_GOLEM: case BM_125_GOLD_GOLEM:
	case BM_130_BLACK_GOLEM: case BM_218_ICE_GOLEM: case BM_288_EARTH_GOLEM:
	case BM_60_HELL_STONE: case BM_61_BLOOD_STONE: case BM_62_MAULER: case BM_63_LAVA_LORD: case BM_180_CRYPT_STONE:
	case BM_C_365_MAGMA_DEMON: case BM_C_366_BLOOD_STONE: case BM_C_367_HELL_STONE: case BM_C_368_LAVA_LORD:
	case BM_69_WINGED_DEMON: case BM_81_FLYING_DEMON: case BM_82_GARGOYLE: case BM_84_DEATH_WING: case BM_190_WINGED_DEMON:
	case BM_C_349_WINGED_DEMON: case BM_C_350_GARGOYLE: case BM_C_351_BLOOD_CLAW: case BM_C_352_DEATH_WING:
	case BM_271_THORNED_HULK: case BM_273_FLAME_FIEND:
		return true;
	default:
		return false;
	}
}

// the SpriteMouths[] row of a monster's sprite, 0 = none (the automatic rule); looked up once per
// BaseMonster row
const SpriteMouth* MouthOf( const Monster& m )
{
	if( !m.BasePtr || !m.BasePtr->AnimationFileName ) return 0;
	static std::unordered_map<const BaseMonster*, const SpriteMouth*> found;
	auto it = found.find( m.BasePtr );
	if( it != found.end() ) return it->second;
	const SpriteMouth* row = 0;
	for( const SpriteMouth& s : SpriteMouths ) if( !_stricmp( s.sprite, m.BasePtr->AnimationFileName ) ){ row = &s; break; }
	found[ m.BasePtr ] = row;
	return row;
}

// the places that are cold enough (WHERE in the header)
bool ColdPlace()
{
	if( is( (DUNGEON)Dungeon, DUN_38_GLACIAL_CAVERNS, DUN_39_FROZEN_LOCH ) ) return true;	// frozen maps, any game
	if( !( Players[ CurrentPlayerIndex ].gameChanger & BIT( GC_15_ICE_AGE ) ) ) return false;
	return !Dungeon->isQuest || Dungeon == DUN_44_RAVENHOLM;	// Ice Age: every non-quest place, and Ravenholm
}

} // namespace

// (th4) A cold place, or toxic fumes on screen (WHERE / TOXIC FUMES in the header). Also asked by
// dx_utility.cpp Atmos_NeedMask (the puffs need the world/UI mask).
bool Breath_Active()
{
	if( ColdBreath <= 0 || !gbRunGame ) return false;
	return ColdPlace() || Fumes;
}

// (th4) Draw.cpp DrawIsoLand start.
void Breath_BeginFrame()
{
	bool fumes = false;	// last frame's toxic notes and the live toxic puffs keep the pass going away from the cold
	for( int i = 0; i < NoteN; ++i ) if( Notes[ i ].toxic ) fumes = true;
	for( int i = 0; i < PuffN; ++i ) if( Puffs[ i ].toxic ) fumes = true;
	NoteN = 0;
	const bool on = ColdBreath > 0 && gbRunGame;
	Cold = on && ColdPlace();
	Fumes = on && fumes;
	const int level = (int)(DUNGEON)Dungeon;
	if( level != LastLevel || ( !Cold && !Fumes ) ){
		if( level != LastLevel ) HeadCache.clear();	// sprites are reloaded on a new level
		LastLevel = level;
		LastBreath.clear();
		PuffN = 0;
	}
}

// (th4) TownMap.cpp DrawDynamicObjectsTown / Draw.cpp DrawPlayer, after the hero was drawn.
void Breath_NotePlayer( int playerIndex, int x, int y, const char* cl2, int frame, int width )
{
	if( !Cold || (uint)playerIndex >= PlayersMax_4 ) return;
	const Player& p = Players[ playerIndex ];
	if( p.CurAction == PCA_8_DEATH || !cl2 || width <= 0 ) return;
	Head h = { (short)PLAYER_HEAD_H, 0, (short)( width / 2 ), 0, 0 };	// fixed, see THE MOUTH in the header
	UprightMouth( h, p.dir & 7 );
	(void)frame;
	AddNote( 0x10000 + playerIndex, x, y, h, p.dir, false );
}

// (th4) Draw.cpp DrawMonster, after the monster was drawn.
void Breath_NoteMonster( int monsterIndex, int x, int y, const char* cl2, int frame, int width )
{
	if( ColdBreath <= 0 || !gbRunGame || (uint)monsterIndex >= Monsters_Max_Count ) return;
	const Monster& m = Monsters[ monsterIndex ];
	const SpriteMouth* sm = MouthOf( m );
	const bool toxic = sm && ( sm->flags & MOUTH_TOXIC );
	if( !Cold && !toxic ) return;	// breath in the cold, toxic fumes everywhere
	if( m.CurAction == A_6_DEATH || m.CurAction == A_15_STONE || Breathless( m ) ) return;
	if( m.BasePtr && m.BasePtr->MonsterClass == MON_0_UNDEAD ) return;	// the dead do not breathe
	const int cap = m.BasePtr && m.BasePtr->SpriteHeight > 0 ? m.BasePtr->SpriteHeight + HEAD_CAP_PAD : 0;	// automatic rule only
	const MouthRule rule = m.BasePtr && m.BasePtr->MonsterClass == MON_2_BEAST ? MOUTH_FRONT_IF_WIDE : MOUTH_UPRIGHT;
	// the facing of the sprite the game DREW, found by its pointer -- NOT Monster::Orientation: a standing
	// monster can be drawn turned another way than Orientation says (an acid hound in action 13 drawn
	// facing east while Orientation said north-west), and the mouth of the wrong facing was then cached
	// for that frame for good and puffed from the hound's back
	int dir = m.Orientation & 7;
	if( m.SpritePtr ) for( int a = 0; a < 6; ++a ) for( int d = 0; d < 8; ++d )
		if( m.SpritePtr->animation[ a ].oriented[ d ].data == cl2 ){ dir = d; a = 6; break; }
	Head h;
	if( HeadOf( cl2, frame, width, false, sm, cap, dir, rule, h ) ) AddNote( 0x30000 + monsterIndex, x, y, h, dir, toxic );
}

// (th4) TownMap.cpp DrawDynamicObjectsTown, after a towner or cow was drawn (CEL_Draw's x, y).
void Breath_NoteTowner( int townerIndex, int x, int y )
{
	if( !Cold || (uint)townerIndex >= 16 ) return;
	const Towner& t = Towners[ townerIndex ];
	int dir = DIR_0_D;	// townsfolk face the viewer; a cow shows one of its eight direction animations
	for( int k = 0; k < 8; ++k ) if( t.animationFrameArray[ k ] && t.animationFrameArray[ k ] == t.stayAnim ){ dir = k; break; }
	const MouthRule rule = t.TownerIdentificator == TI_9_COWS ? MOUTH_FRONT : MOUTH_UPRIGHT;	// THE MOUTH in the header
	Head h;
	const bool ok = HeadOf( t.stayAnim, t.CurFrame, t.animWidth, true, 0, 0, dir, rule, h );
	if( ok ) AddNote( 0x20000 + townerIndex, x, y, h, dir, false );
}

// (th4) dx_utility.cpp Breath_Present, once per presented frame: spawn the breaths that came round,
// drop the puffs that are gone, and write the live ones to puffs[] as 4 floats each -- centre x, y in
// screen game px (after the zoom and the Perspective warp), radius in game px (NEGATIVE for a toxic
// puff, Breath.hlsl makes it green), opacity 0..1. now = the ATMOSPHERE clock in s (stops on pause);
// anchorX/Y = screen game px of tile (0,0) (Atmos_Anchor); wind 0..1 (Wind_Strength). Returns how many
// were written.
int Breath_Update( float now, int zoom, float anchorX, float anchorY, float wind, float* puffs, int maxPuffs )
{
	if( !Cold && !Fumes && !NoteN ){ PuffN = 0; return 0; }	// away from the cold only toxic notes are taken
	const int z = zoom < 1 ? 1 : zoom;
	// 1) breaths
	for( int i = 0; i < NoteN; ++i ){
		const Note& n = Notes[ i ];
		const uint h = BHash( (uint)n.id * 2654435761u );
		const float per = BREATH_MIN + BREATH_VAR * ( ( h & 1023 ) / 1023.0f );
		const float off = per * ( ( ( h >> 10 ) & 1023 ) / 1023.0f );
		const int cyc = (int)floorf( ( now + off ) / per );
		auto it = LastBreath.find( n.id );
		if( it == LastBreath.end() ){ LastBreath[ n.id ] = cyc; continue; }	// first seen: wait for its next breath
		if( it->second == cyc ) continue;
		it->second = cyc;
		if( PuffN >= MAX_PUFFS ) continue;
		// the mouth, pre-zoom WorkingSurface px (row r of a sprite is drawn on line y - r)
		const float mx = (float)( n.x + n.h.mx );
		const float my = (float)( n.y - n.h.mr );
		Puff& p = Puffs[ PuffN++ ];
		p.wx = ( ( mx - Screen_LeftBorder ) * z - anchorX ) / z;
		p.wy = ( ( my - Screen_TopBorder ) * z - anchorY ) / z;
		p.dx = DirX[ n.dir ]; p.dy = DirY[ n.dir ];
		p.t0 = now;
		p.toxic = n.toxic;
	}
	// 2) age (a clock that jumped back -- the ATMOSPHERE clock wraps -- also ends a puff)
	int k = 0;
	for( int i = 0; i < PuffN; ++i ){
		const float a = ( now - Puffs[ i ].t0 ) / PUFF_LIFE;
		if( a >= 0.0f && a < 1.0f ) Puffs[ k++ ] = Puffs[ i ];
	}
	PuffN = k;
	// 3) the live puffs, on screen
	int n = 0;
	for( int i = 0; i < PuffN && n < maxPuffs; ++i ){
		const Puff& p = Puffs[ i ];
		const float a = ( now - p.t0 ) / PUFF_LIFE;
		const float e = 1.0f - ( 1.0f - a ) * ( 1.0f - a );	// ease out: fast at the mouth, slow at the end
		const float wx = p.wx + p.dx * ( 2.0f + 9.0f * e ) + wind * 12.0f * a;
		const float wy = p.wy + p.dy * ( 2.0f + 9.0f * e ) + PUFF_SINK * e;	// forward, and a little down
		int sx = (int)( anchorX + wx * z ), sy = (int)( anchorY + wy * z );
		Perspective_WorldToScreen( sx, sy );
		if( sx < -48 || sy < -48 || sx > ScreenWidth + 48 || sy > ScreenHeight + 48 ) continue;
		const float radius = ( 3.0f + 7.5f * e ) * z;	// game px; at zoom 1 a full puff is about a quarter of a hero's height
		puffs[ n * 4 + 0 ] = (float)sx;
		puffs[ n * 4 + 1 ] = (float)sy;
		puffs[ n * 4 + 2 ] = p.toxic ? -radius : radius;	// the sign tells Breath.hlsl to tint it green
		puffs[ n * 4 + 3 ] = ( a < 0.12f ? a / 0.12f : 1.0f ) * powf( 1.0f - a, 1.5f );
		++n;
	}
	return n;
}
