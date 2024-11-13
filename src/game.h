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
#define MAP_TILES_COUNT 3072
#define CAMERA_START_YPOS 3852
#define CAMERA_Y_MIN 32
#define CAMERA_MOVE_RATE 2
#define CAMERA_MOVE_AMOUNT -1
#define BOUNDS_MIN 8
#define BOUNDS_MAX 65527

#define CHANNEL_FOR_PLAYER 1
#define CHANNEL_FOR_EXPLOSION 2

#define POWERUP_EXTRALIFE 1
#define POWERUP_SPECIAL 2
#define POWERUP_WEAPON 3
#define POWERUP_FALL_SPEED 1
#define POWERUP_WIDTH 16
#define POWERUP_HEIGHT 16
#define POWERUP_SCORE 5000

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
static void createExplosionAtPosition(tUwCoordYX tPosition);
static void createPowerupAtPosition(tUwCoordYX tPosition, UBYTE ubPowerupType);

UWORD uwTilesInStage[] = {261, 350, 352};

#endif // _GAME_H_