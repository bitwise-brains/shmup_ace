#include <ace/managers/bob.h>
#include <ace/types.h>
#include <ace/managers/copper.h>
#include <fixmath/fixmath.h>

#define MAX_PLAYER_PROJECTILES 20;
#define LIGHT_PROJECTILE_OFFSET 0;
#define MED_PROJECTILE_OFFSET 640;

const ULONG ulProjectileAtlasOffsets[] = {0, 640, 1280, 1920};

typedef enum tProjectileTypes {
    BASIC,
    LIGHT, 
    MEDIUM, 
    HEAVY
} tProjectileTypes;

typedef struct _tProjectile {
    tCopBlock *pProjectileBlock;
    WORD wX;
    WORD wY;
    UBYTE ubTimeToLive;
} tProjectile;