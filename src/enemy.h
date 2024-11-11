#include "game.h"

#define ENEMY_TYPES 13
#define ENEMY_BIG_TYPE 12
#define ENEMY_MAX 8
#define ENEMY_WAVE_MAX 4
#define ENEMY_INVINCIBLE_MARGIN 16

typedef struct _tEnemy {
    tUwCoordYX tPosition;
    tBob sBob;
    BYTE bHealth;
    UBYTE ubOnScreen;
    UBYTE ubInvincible;
    UBYTE ubCanShoot;    
    UBYTE ubCooldownTimer;
    UBYTE ubBitmapOffset;
    UBYTE ubEnemyType;
    UBYTE ubPowerupType;
    UBYTE ubWaveIdx;
    UBYTE ubMoveSpeed; // MUST BE MULTIPLE OF 2
    UBYTE ubPathLoops;
    UWORD uwPathArrayOffset;
    UWORD uwPathIdx;
    UWORD uwPathLength;
    UWORD uwPathYOffset;
    UWORD uwScoreValue;
} tEnemy;

typedef struct _tEnemyType {
    BYTE bHealth;
    UBYTE ubWidth, ubHeight;
    UBYTE ubGunOffsetX, ubGunOffsetY;
    UBYTE ubBitmapOffset;
    UBYTE ubMoveSpeed; // MUST BE MULTIPLE OF 2 
    UBYTE ubCanShoot;
    UBYTE ubCooldownTime;
    UBYTE ubProjectileType;
    UBYTE ubPowerupType;
    UWORD uwScoreValue;
} tEnemyType;

typedef struct _tEnemyWave {
    UBYTE ubEnemyType;
    UWORD uwSpawnYPosition;
    BYTE bSpawnOffset;
    UBYTE ubPathType;
    UBYTE ubPathLoops;
} tEnemyWave;

// Enemy types.
tEnemyType g_tEnemyTypes[ENEMY_TYPES] = {{ 10, 16, 16,  3, 15, 0, 4, FALSE,   0, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 1, 4,  TRUE,  60, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 1, 4,  TRUE,  50, 1, 0, 1337},
                                         { 20, 16, 16,  3, 15, 2, 4,  TRUE,  40, 1, 3, 1987},

                                         { 10, 16, 16,  3, 15, 3, 4, FALSE,   0, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 4, 4,  TRUE,  60, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 4, 4,  TRUE,  50, 1, 0, 1337},
                                         { 30, 16, 16,  3, 15, 5, 4,  TRUE,  40, 1, 1, 1987},

                                         { 10, 16, 16,  3, 15, 6, 4, FALSE,   0, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 7, 4,  TRUE,  60, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 7, 4,  TRUE,  50, 1, 0, 1337},
                                         { 30, 16, 16,  3, 15, 8, 4,  TRUE,  40, 1, 2, 1987},

                                         { 40, 32, 32, 16, 16, 0, 2,  TRUE,  50, 1, 0, 2600}};
