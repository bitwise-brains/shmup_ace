#include "game.h"

#define PLAYER_PROJECTILE_MAX 5
#define PLAYER_PROJECTILE_SPEED 6
#define PLAYER_PROJECTILE_WIDTH 16
#define PLAYER_PROJECTILE_HEIGHT 5
#define PLAYER_PROJECTILE_OFFSET_X 7
#define PLAYER_PROJECTILE_OFFSET_Y 0

#define ENEMY_PROJECTILE_MAX 8
#define ENEMY_PROJECTILE_SPEED 3
#define ENEMY_PROJECTILE_WIDTH 6
#define ENEMY_PROJECTILE_HEIGHT 7
#define ENEMY_PROJECTILE_OFFSET_X 3
#define ENEMY_PROJECTILE_OFFSET_Y 3

#define ENEMY_BOB_CHANNELS 2
#define ENEMY_SPRITE_CHANNELS 4

typedef struct tChannelBounds {
    fix16_t fMin;
    fix16_t fMax;
} tChannelBounds;

typedef struct _tPlayerProjectile {
    tCopBlock *pProjectileBlock;
    fix16_t fX;
    fix16_t fY;
    fix16_t fDeltaX;
    fix16_t fDeltaY;
    UBYTE ubDamage;
    UBYTE ubAlive;
    UBYTE ubChannel;
} tPlayerProjectile;

typedef struct _tEnemyProjectile {
    tCopBlock *pProjectileBlock;
    fix16_t fX;
    fix16_t fY;
    fix16_t fDeltaX;
    fix16_t fDeltaY;
    UBYTE ubAlive;
    UBYTE ubChannel;
} tEnemyProjectile;