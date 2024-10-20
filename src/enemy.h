#include "game.h"

#define ENEMY_MAX 8
#define ENEMY_MOVE_SPEED 2
#define ENEMY_FIRE_DELAY 50
#define ENEMY_WIDTH 16
#define ENEMY_HEIGHT 16
#define ENEMY_CENTER_OFFSET_X 8
#define ENEMY_CENTER_OFFSET_Y 8
#define ENEMY_GUN_OFFSET_X 3
#define ENEMY_GUN_OFFSET_Y 15
#define ENEMY_PATH_LENGTH 385
#define ENEMY_SCORE_VALUE 1337

typedef struct _tEnemy {
    UBYTE ubEnemyType;
    tUwCoordYX tPosition;
    tBob sBob;
    BYTE bHealth;
    UBYTE ubCanShoot;    
    UBYTE ubCooldownTimer;
    UBYTE ubDropsPowerup;
    UBYTE ubPowerupType;
    UBYTE ubPathType;
    UWORD uwPathIdx;
    UWORD uwScoreValue;
} tEnemy;

typedef struct _tEnemyType {
    BYTE bHealth;
    UBYTE ubProjectileType;
    UWORD uwScoreValue;
} tEnemyType;

typedef struct _tEnemyPath {
    UWORD uwPath[0];
    UWORD uwPathLength;
} tEnemyPath;

typedef struct _tEnemyWave {
    tEnemy tEnemies[0];
    UBYTE ubNumberOfEnemies;
    UWORD uwSpawnYPosition;
} tEnemyWave;