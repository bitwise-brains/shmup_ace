#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define HUD_VIEWPORT_WIDTH 240
#define HUD_VIEWPORT_HEIGHT 12
#define HUD_TEXT_WIDTH 32
#define HUD_SCORE_WIDTH 64
#define HUD_TEXT_HEIGHT 8
#define HUD_TEXT_COLOR 27
#define TILE_VIEWPORT_WIDTH 160
#define TILE_VIEWPORT_HEIGHT 244
#define TILE_VIEWPORT_YMIN 4
#define TILE_VIEWPORT_XMIN 16
#define TILE_VIEWPORT_XMAX 176
#define MAP_WIDTH_IN_TILES 12
#define MAP_HEIGHT_IN_TILES 256
#define BOSS_HEIGHT_IN_TILES 66
#define BOSS_TILES_COUNT 792
//#define BOSS_START_YPOS 1024
#define BOSS_START_YPOS 48
#define MAP_TILES_COUNT 3072
#define CAMERA_START_YPOS 3852
#define CAMERA_Y_MIN 32
#define CAMERA_MOVE_RATE 2
#define CAMERA_MOVE_AMOUNT -1
#define BOUNDS_MIN 8
#define BOUNDS_MAX 65527
#define RNG_IDX_MAX 133

#define CHANNEL_FOR_PLAYER 1
#define CHANNEL_FOR_EXPLOSION 2

#define POWERUP_EXTRALIFE 1
#define POWERUP_SPECIAL 2
#define POWERUP_WEAPON 3
#define POWERUP_FALL_SPEED 1
#define POWERUP_WIDTH 16
#define POWERUP_HEIGHT 16
#define POWERUP_SCORE 5000
#define POWERUP_BOUNCE_TIME 6

#define TEXT_GAMEOVER_WIDTH 64
#define TEXT_GAMEOVER_HEIGHT 55
#define TEXT_GAMEOVER_OFFX 64
#define TEXT_GAMEOVER_OFFY 96

#define TEXT_READY_WIDTH 96
#define TEXT_READY_HEIGHT 32
#define TEXT_READY_OFFX 48
#define TEXT_READY_OFFY 106

#define TEXT_GO_WIDTH 64
#define TEXT_GO_HEIGHT 37
#define TEXT_GO_OFFX 64
#define TEXT_GO_OFFY 104

#define TEXT_STAGECOMPLETE_WIDTH 64
#define TEXT_STAGECOMPLETE_HEIGHT 72
#define TEXT_STAGECOMPLETE_OFFX 64
#define TEXT_STAGECOMPLETE_OFFY 96

#define FLASH_TIME 4
#define FLASH_TIME_SHORT 2

#define BONUS_KILLS_MULTIPLIER 100
#define BONUS_SPECIAL_MULTIPLIER 10000
#define BONUS_DEATHS_MULTIPLIER 15000

// Debug
#define DEBUG_COMMAND_DELAY 20
//#define COLLISIONS_DISABLED

static void initGame();
static void initViews();
static void initAudio();
static void initHud();
static void initBobs();
static void initSprites();

static void processStageStart();
static void processCamera();
static void processHud();
static void processInput();
static void processBobs();
static void processTimers();
static void processPowerups();
static void processWaves();
static void processPlayer();
static void processEnemies();
static void processPlayerProjectiles();
static void processSimpleEnemyProjectiles();
static void processComplexEnemyProjectiles();

static void processPlayerShoot();
static void processPlayerSpecial();
static void processPlayerDie();

static void resetEverything();
//static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType, UBYTE ubSecondary);
static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType);
static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType);
static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2);
static void destroyEnemy(UBYTE ubEnemyIdx);
static void destroyBossTurret(UBYTE ubTurret);
static void createExplosionAtPosition(tUwCoordYX tPosition);
static void createPowerupAtPosition(tUwCoordYX tPosition, UBYTE ubPowerupType);

UWORD uwTilesInStage[] = {261, 350, 352, 247};
BYTE g_bCyclicRng[] = {-27, 46, 6, -21, 11, 42, 2, 59, -19, 49, 15, -17, 38, 4, 24, -21, 57, 40, -23, 6, 43, -12, 57, -10, 35, 57, 21, 3, 51, -10, 7, -18, 21, 59, 52, 23, -5, -27, -2, 57, -20, -1, 56, 13, -25, 0, -11, 46, 12, -26, 9, 53, 9, -30, 11, 25, 2, 30, 60, 38, -16, 46, 5, -15, 7, -10, 44, 25, 61, 20, -4, 46, 25, 1, 40, -18, 15, 32, 10, -19, 61, 29, -9, 26, -6, 20, -30, -1, 62, -25, 24, 51, 10, 48, -19, 54, 9, 33, 1, 40, -23, 8, -20, 58, 39, -24, -16, 24, -2, -18, 58, 9, 61, 43, -16, 34, -11, -28, 59, 24, -16, 30, 47, -24, 56, -19, 47, 42, -14, 60, 5, -23, -5};

UWORD uwBossStageData[] = {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0, 0,0,21,0,22,0,23,24,21,25,0,0, 0,26,27,28,29,30,31,32,33,34,35,0, 0,36,37,38,39,40,41,42,43,44,45,0, 0,46,47,48,49,50,51,52,53,54,46,0, 0,1,55,56,57,58,59,60,61,62,8,0, 0,63,64,65,66,67,68,69,70,71,72,0, 0,73,74,75,76,77,78,79,80,81,82,0, 0,83,84,85,86,87,88,89,90,91,92,0, 0,93,85,94,85,95,96,94,85,94,97,0, 0,85,98,99,100,101,100,101,100,101,85,0, 0,102,103,104,105,106,107,108,109,106,101,0, 0,110,111,112,113,114,115,116,117,114,118,0, 0,85,119,118,119,118,110,120,121,118,85,0, 0,94,85,94,85,94,94,94,94,122,94,0, 0,101,94,90,94,94,94,85,94,94,100,0, 0,123,124,125,126,125,127,125,128,125,129,0, 0,130,131,131,131,131,131,131,131,131,132,0, 0,130,131,131,131,131,131,131,131,131,132,0, 0,133,131,131,131,131,131,131,131,131,134,0, 0,131,131,131,135,136,136,137,131,131,131,0, 0,138,136,139,138,136,140,141,138,140,141,0, 0,0,142,0,138,143,136,141,0,142,0,0, 0,0,0,144,0,142,145,146,0,0,147,0, 0,148,149,150,151,0,152,153,0,0,154,0, 0,148,155,156,157,0,0,0,0,158,0,0, 0,0,159,160,161,158,0,0,158,162,0,0, 0,0,0,0,158,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,144,0,0,0, 0,0,0,0,0,0,0,114,163,164,148,0, 0,0,0,0,0,158,0,106,165,166,148,0, 0,0,158,158,0,158,0,167,142,0,0,0, 0,0,0,144,168,0,158,0,0,158,0,0, 0,148,169,163,170,158,171,0,172,0,0,0, 0,148,173,174,175,158,176,0,177,0,0,0, 0,0,0,142,176,0,0,167,0,0,0,0, 0,0,172,178,0,158,0,0,176,0,0,0, 0,179,180,181,0,162,0,158,144,0,0,0, 0,0,177,0,176,0,158,182,163,164,148,0, 0,0,0,183,0,179,184,106,165,185,148,0, 0,0,0,177,158,0,162,0,186,187,181,0, 0,113,0,0,0,168,0,0,0,177,114,0, 0,188,0,0,189,190,191,192,0,0,193,0, 0,194,0,0,0,195,196,197,0,0,198,0, 0,188,0,172,0,199,200,201,0,158,202,0, 0,119,203,204,205,206,206,206,206,206,118,0, 0,85,85,122,85,85,85,94,94,85,85,0, 0,101,100,207,101,100,208,101,100,209,101,0, 0,198,188,0,198,188,179,210,188,0,198,0, 0,106,211,0,106,211,0,106,211,167,106,0, 0,0,0,176,0,0,0,0,0,0,0,0, 0,0,0,0,0,182,113,144,0,0,0,0, 0,148,148,212,213,118,119,214,148,148,148,0, 0,148,148,215,216,101,100,217,218,148,148,0, 0,0,0,0,142,106,219,179,181,158,144,0, 0,0,0,0,0,176,220,0,0,0,221,0, 0,0,114,113,144,114,113,176,114,113,222,0, 0,138,223,224,136,225,188,0,198,188,226,0, 0,163,118,119,227,118,119,206,118,119,228,0, 0,208,101,100,208,101,100,207,101,100,229,0, 0,0,198,194,0,198,188,114,230,231,232,0, 0,206,233,234,235,118,236,237,85,85,85,0, 0,238,238,238,238,239,85,240,125,125,125,0, 0,90,85,85,85,241,239,242,131,131,131,0, 0,85,94,85,85,243,244,242,245,246,246,0};
#endif // _GAME_H_