// GrassCompose.hlsl -- (new by th4) THE MODE 2 of the grass blades (config GrassResolution 1 / 2): the grass layer, drawn
// at the game's resolution or half of it, laid over the window frame. A thin wrapper: the body is Grass.hlsl (its header,
// c5.w THE MODE, says why); it exists because VS rejects two <FxCompile> items on one file and the ps_3_0 slot limit keeps
// it out of the heavy blade pass. EDIT Grass.hlsl, not this.
#define GRASS_COMPOSE
#include "Grass.hlsl"
