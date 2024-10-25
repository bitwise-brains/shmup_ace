#include "game.h"

#define EXPLOSIONS_MAX 3
#define EXPLOSION_ANIM_FRAMES 16
#define BIGEXPLOSION_ANIM_FRAMES 24

typedef struct _tExplosion {
    tBob sBob;
    UBYTE ubFrameIndex;
    UBYTE ubAlive;
} tExplosion;