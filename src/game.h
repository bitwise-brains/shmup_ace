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
#include <ace/managers/rand.h>
#include <ace/managers/sprite.h>
#include <ace/managers/system.h>
#include <ace/utils/font.h>
#include <ace/utils/palette.h>
#include <ace/utils/string.h>
#include <fixmath/fixmath.h>

#define GAME_BPP 5
#define HUD_VIEWPORT_WIDTH 320
#define HUD_VIEWPORT_HEIGHT 12
#define HUD_TEXT_WIDTH 32
#define HUD_SCORE_WIDTH 64
#define HUD_TEXT_HEIGHT 8
#define TILE_VIEWPORT_WIDTH 320
#define TILE_VIEWPORT_HEIGHT 244
#define TILE_VIEWPORT_YMIN 4
#define TILE_VIEWPORT_XMIN 2
#define TILE_VIEWPORT_XMAX 160
#define MAP_WIDTH_IN_TILES 10
#define MAP_HEIGHT_IN_TILES 32
#define MAP_TILES_COUNT 40
#define CAMERA_MOVE_RATE 2
#define CAMERA_MOVE_AMOUNT -1

// Debug
//#define COLLISIONS_DISABLED

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);

static void initGame();
static void initViews();
static void initHud();
static void initBobs();
static void initSprites();

static void processCamera();
static void processHud();
static void processInput();
static void processBobs();
static void processTimers();
static void processEnemies();
static void processPlayerProjectiles();
static void processEnemyProjectiles();

static void playerShoot();
static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType, UBYTE ubSecondary);
static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel);
static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2);
static void createExplosionAtPosition(tUwCoordYX tPosition);

#endif // _GAME_H_