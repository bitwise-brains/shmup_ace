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
#define HUD_SCORE_WIDTH 96
#define HUD_SCORE_HEIGHT 8
#define TILE_VIEWPORT_WIDTH 320
#define TILE_VIEWPORT_HEIGHT 244
#define TILE_VIEWPORT_XMIN 0
#define TILE_VIEWPORT_YMIN 4
#define TILE_VIEWPORT_EDGE_MIN 32
#define TILE_VIEWPORT_EDGE_MAX 352
#define MAP_WIDTH_IN_TILES 24
#define MAP_HEIGHT_IN_TILES 36
#define MAP_BOTTOM 512
#define CAMERA_MOVE_RATE 4
#define CAMERA_MOVE_AMOUNT -1

// Debug
//#define COLLISIONS_DISABLED

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);
static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel);
static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel);
static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2);
static void createExplosionAtPosition(tUwCoordYX tPosition);
static void processCamera();
static void processHud();
static void processInput();
static void processBobs();
static void processTimers();
static void processEnemies();
static void processPlayerProjectiles();
static void processEnemyProjectiles();

#endif // _GAME_H_