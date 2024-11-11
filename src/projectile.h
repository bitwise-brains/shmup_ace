#include "game.h"

#define PLAYER_PROJECTILE_MAX 5
#define PLAYER_PROJECTILE_TYPES 4
#define PLAYER_SECONDARY_PROJECTILE_TYPES 0

#define PLAYER_SPRITE_CHANNEL_A 4
#define PLAYER_SPRITE_CHANNEL_B 5

#define ENEMY_PROJECTILE_MAX 6
#define ENEMY_PROJECTILE_TYPES 4
#define ENEMY_SIMPLEPROJECTILE_SPEED 3
#define ENEMY_SIMPLEPROJECTILE_HEIGHT 6

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

// Enemy projectile types
tEnemyProjectileType g_tEnemyProjectileTypes[ENEMY_PROJECTILE_TYPES] = {{ 10,  6, 0, 3},
                                                                        {  5,  6, 2, 3},
                                                                        {  8,  8, 4, 2},
                                                                        { 14, 14, 7, 2}};

// Player projectile types
tPlayerProjectileType g_tPlayerProjectileTypes[PLAYER_PROJECTILE_TYPES] = {{ 30, 12, 0, 0, -6, 4, 0,  8,  7,  TRUE, FALSE, FALSE, 0 },
                                                                           { 25, 10, 0, 0, -6, 4, 0,  8, 10,  TRUE, FALSE, FALSE, 0 },
                                                                           { 20,  8, 0, 0, -7, 3, 0, 10, 12, FALSE, FALSE, FALSE, 0 },
                                                                           { 15,  6, 0, 0, -8, 2, 0, 12, 16,  TRUE, FALSE, FALSE, 0 }};