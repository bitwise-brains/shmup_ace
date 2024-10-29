#include "game.h"

#define ENEMY_TYPES 10
#define ENEMY_BIG_TYPE 9
#define ENEMY_MAX 8
#define ENEMY_WAVE_MAX 4

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