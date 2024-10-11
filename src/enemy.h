#include "game.h"

#define ENEMY_MAX 2
#define ENEMY_MOVE_SPEED 2
#define ENEMY_FIRE_DELAY 50
#define ENEMY_WIDTH 32
#define ENEMY_HEIGHT 28
#define ENEMY_CENTER_OFFSET_X 16
#define ENEMY_CENTER_OFFSET_Y 14
#define ENEMY_GUN_OFFSET_X 16
#define ENEMY_GUN_OFFSET_Y 24
#define ENEMY_PATH_LENGTH 512
#define ENEMY_SCORE_VALUE 1337

typedef struct _tEnemy {
    tBob sBob;
    tWCoordYX tPosition;
    BYTE bHealth;
    UBYTE ubSpeed;
    UBYTE ubCooldownTimer;
    UWORD uwPositionOffsetIdx;
    UWORD uwScoreValue;    
} tEnemy;