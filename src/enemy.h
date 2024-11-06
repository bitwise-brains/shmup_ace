#include "game.h"

#define ENEMY_TYPES 10
#define ENEMY_BIG_TYPE 9
#define ENEMY_MAX 8
#define ENEMY_WAVE_MAX 4
#define ENEMY_INVINCIBLE_MARGIN 8

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
    UBYTE ubEnemies[ENEMY_WAVE_MAX];
    UBYTE ubNumberOfEnemies;
    UBYTE ubSpacing;
    UWORD uwSpawnYPosition;
    UWORD uwPathYOffset;
    UBYTE ubPathType;
    UBYTE ubPathLoops;
} tEnemyWave;

// Enemy types.
tEnemyType g_tEnemyTypes[ENEMY_TYPES] = {{ 10, 16, 16,  3, 15, 0, 4, FALSE,   0, 0, 0, 1337},
                                         { 10, 16, 16,  3, 15, 0, 4,  TRUE,  50, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 1, 4,  TRUE,  60, 1, 3, 1987},
                                         { 10, 16, 16,  3, 15, 2, 4, FALSE,   0, 0, 0, 1337},
                                         { 10, 16, 16,  3, 15, 2, 4,  TRUE,  50, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 3, 4,  TRUE,  60, 1, 1, 1987},
                                         { 10, 16, 16,  3, 15, 4, 4, FALSE,   0, 0, 0, 1337},
                                         { 10, 16, 16,  3, 15, 4, 4,  TRUE,  50, 0, 0, 1337},
                                         { 20, 16, 16,  3, 15, 5, 4,  TRUE,  60, 1, 2, 1987},
                                         { 40, 32, 32, 16, 16, 0, 4,  TRUE,  20, 1, 0, 2600}};

// Enemy waves.
UBYTE g_ubWavesInLevel[NUMBER_OF_LEVELS] = {10};
tEnemyWave g_tEnemyWaves[] = {{ {  0, 255, 255, 255}, 1, 0, 3824, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3808, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3792, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3776, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3760, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3744, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3728, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3712, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3696, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3680, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3664, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3648, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3632, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3616, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3600, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3584, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3568, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3552, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3536, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3520, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3504, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3488, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3472, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3456, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3440, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3424, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3408, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3392, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3376, 48, 0, FALSE },
                              { {  0, 255, 255, 255}, 1, 0, 3360, 48, 0, FALSE }};