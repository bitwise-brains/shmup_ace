#include "game.h"

#define PLAYER_PROJECTILE_MAX 5
#define PLAYER_PROJECTILE_TYPES 4
#define PLAYER_SECONDARY_PROJECTILE_TYPES 0

#define PLAYER_SPRITE_CHANNEL_A 4
#define PLAYER_SPRITE_CHANNEL_B 5

#define ENEMY_PROJECTILE_MAX 8
#define ENEMY_PROJECTILE_TYPES 4
#define ENEMY_PROJECTILE_SPEED 3
#define ENEMY_PROJECTILE_WIDTH 10
#define ENEMY_PROJECTILE_HEIGHT 6

#define ENEMY_BOB_CHANNELS 2
#define ENEMY_SPRITE_CHANNELS 4

typedef struct tComplexChannelBounds {
    fix16_t fMin;
    fix16_t fMax;
} tComplexChannelBounds;

typedef struct tSimpleChannelBounds {
    UWORD uwMin;
    UWORD uwMax;
} tSimpleChannelBounds;

typedef struct _tPlayerProjectile {
    tCopBlock *pCopBlock;
    UWORD uwX, uwY;
    UBYTE ubAlive;
    UBYTE ubType;
} tPlayerProjectile;

typedef struct _tPlayerSecondaryProjectile {
    tCopBlock *pCopBlock;
    UWORD uwX;
    UBYTE ubType;
} tPlayerSecondaryProjectile;

typedef struct _tPlayerProjectileType {
    UBYTE ubDamage;
    UBYTE ubDelay;
    BYTE bDeltaX, bDeltaX2, bDeltaY;
    UBYTE ubXOffset, ubXOffset2;
    UBYTE ubWidth, ubHeight;
    UBYTE ubDieOnCollision;
    UBYTE ubWideSprite, ubSpreadShot;
    UBYTE ubSecondarySpriteIndex;
} tPlayerProjectileType;

typedef struct _tEnemyProjectileType {
    UBYTE ubWidth, ubHeight;
    UBYTE ubOffset;
    BYTE bSpeed;
} tEnemyProjectileType;

typedef struct _tComplexEnemyProjectile {
    tCopBlock *pCopBlock;
    fix16_t fX, fY;
    fix16_t fDeltaX, fDeltaY;
    UBYTE ubAlive;
    UBYTE ubChannel;
    UBYTE ubType;    
} tComplexEnemyProjectile;

typedef struct _tSimpleEnemyProjectile {
    tCopBlock *pCopBlock;
    UWORD uwX, uwY;
    BYTE bDeltaX, bDeltaY;
    UBYTE ubAlive;
    UBYTE ubChannel;
    UBYTE ubType;
} tSimpleEnemyProjectile;