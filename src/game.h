#ifndef _GAME_H_
#define _GAME_H_

#include <ace/types.h>
#include <ace/generic/screen.h>
#include <ace/managers/game.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/viewport/tilebuffer.h>
#include <ace/managers/blit.h>
#include <ace/managers/bob.h>
#include <ace/managers/copper.h>
#include <ace/managers/memory.h>
#include <ace/managers/key.h>
#include <ace/managers/joy.h>
#include <ace/managers/ptplayer.h>
#include <ace/managers/rand.h>
#include <ace/managers/sprite.h>
#include <ace/managers/state.h>
#include <ace/managers/system.h>
#include <ace/utils/font.h>
#include <ace/utils/palette.h>
#include <ace/utils/string.h>
#include <fixmath/fixmath.h>
#include "../ace_audio_mixer/include/ace/contrib/managers/audio_mixer.h"

#define GAME_BPP 5
#define GAME_STATES 2
#define HUD_VIEWPORT_WIDTH 240
#define HUD_VIEWPORT_HEIGHT 12
#define HUD_TEXT_WIDTH 32
#define HUD_SCORE_WIDTH 64
#define HUD_TEXT_HEIGHT 8
#define HUD_TEXT_COLOR 27
#define TILE_VIEWPORT_WIDTH 160
#define TILE_VIEWPORT_HEIGHT 244
#define TILE_VIEWPORT_YMIN 4
#define TILE_VIEWPORT_XMIN 32
#define TILE_VIEWPORT_XMAX 192
#define MAP_WIDTH_IN_TILES 14
#define MAP_HEIGHT_IN_TILES 256
//#define MAP_TILES_COUNT 56
#define MAP_TILES_COUNT 3584
#define CAMERA_START_YPOS 3872
#define CAMERA_Y_MIN 32
#define CAMERA_MOVE_RATE 2
#define CAMERA_MOVE_AMOUNT -1
#define BOUNDS_MIN 8
#define BOUNDS_MAX 65527
#define NUMBER_OF_LEVELS 1

#define CHANNEL_FOR_PLAYER 1
#define CHANNEL_FOR_EXPLOSION 2

#define POWERUP_EXTRALIFE 1
#define POWERUP_SPECIAL 2
#define POWERUP_WEAPON 3
#define POWERUP_FALL_SPEED 1
#define POWERUP_WIDTH 16
#define POWERUP_HEIGHT 16
#define POWERUP_SCORE 5000

#define FLASH_TIME 4
#define FLASH_TIME_SHORT 2

// Debug
#define DEBUG_COMMAND_DELAY 20
//#define COLLISIONS_DISABLED

typedef enum tGameState {
    STATE_INTRO,
    STATE_GAME,
} tGameState;

typedef enum tIntroStage {
    INTRO_BRAINS,
    INTRO_ACE,
    INTRO_TITLE,
    INTRO_TEXT,
    INTRO_FINISHED,
} tIntroStage;

extern tStateManager *g_pGameStateManager;
extern tState g_pGameStates[GAME_STATES];

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);

void introGsCreate(void);
void introGsLoop(void);
void introGsDestroy(void);

static void initGame();
static void initViews();
static void initAudio();
static void initHud();
static void initBobs();
static void initSprites();

static void processLevelStart();
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

//static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType, UBYTE ubSecondary);
static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType);
static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType);
static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2);
static void createExplosionAtPosition(tUwCoordYX tPosition);
static void createPowerupAtPosition(tUwCoordYX tPosition, UBYTE ubPowerupType);

#endif // _GAME_H_