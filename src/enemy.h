#include "game.h"

#define ENEMY_MAX 6
#define ENEMY_MOVE_SPEED 2
#define ENEMY_FIRE_DELAY 50
#define ENEMY_WIDTH 16
#define ENEMY_HEIGHT 16
#define ENEMY_CENTER_OFFSET_X 8
#define ENEMY_CENTER_OFFSET_Y 8
#define ENEMY_GUN_OFFSET_X 8
#define ENEMY_GUN_OFFSET_Y 16
#define ENEMY_PATH_LENGTH 512
#define ENEMY_SCORE_VALUE 1337

typedef struct _tEnemy {
    tBob sBob;
    tWCoordYX tPosition;
    BYTE bHealth;
    UBYTE ubSpeed;
    UBYTE ubCooldownTimer;
    UBYTE ubDropsPowerup;
    UWORD uwPositionOffsetIdx;
    UWORD uwScoreValue;    
} tEnemy;