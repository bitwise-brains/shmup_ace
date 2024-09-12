#include "game.h"
#include "asteroid.h"
#include "projectile.h"
#include "enemy.h"
#include <ace/generic/screen.h>
#include <ace/managers/game.h>
#include <ace/managers/viewport/tilebuffer.h>
#include <ace/managers/key.h>
#include <ace/managers/system.h>
#include <ace/managers/rand.h>
#include <ace/managers/blit.h>
#include <ace/managers/bob.h>
#include <ace/managers/sprite.h>
#include <ace/managers/copper.h>
#include <ace/utils/palette.h>

#define GAME_BPP 5
#define MAP_WIDTH_IN_TILES 20
#define MAP_HEIGHT_IN_TILES 32
#define ANIMATION_PERIOD 2
#define PLAYER_MOVE_SPEED 2
#define PROJECTILE_MOVE_SPEED 8
#define MAX_PROJECTILES 5
#define MAX_ASTEROIDS 1
#define MAX_ENEMEYS 2
#define FIRE_DELAY 6
#define VIEWPORT_HEIGHT 256
#define NUM_FRAMES_PLAYER 4

static void reinstanceSprite(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight);

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVPort; // Viewport for playfield
static tCameraManager *s_pCamera;
static tTileBufferManager *s_pTileBuffer;
static tRandManager s_sRandManager;

static tBitMap *s_pTiles;

static tUwCoordYX playerPosition;

static tProjectile s_tProjectiles[MAX_PROJECTILES];
// static tAsteroid s_tAsteroids[MAX_ASTEROIDS];
static tEnemy s_tEnemys[MAX_ENEMEYS];

static tBitMap *s_pPlayerProjectileImage;
static tSprite *s_pSprite0;

static tBob s_sPlayerShipBob;
static tBitMap *s_pPlayerImage;
static tBitMap *s_pPlayerMask;

// static tBitMap *s_pAsteroidImage;
// static tBitMap *s_pAsteroidMask;

static tBitMap *s_pEnemyImage;
static tBitMap *s_pEnemyMask;

static UWORD fireDelay = 0;
static UWORD animationTimer = 0;

static UBYTE playerShipAnimFrame = 0;
const ULONG playerShipAnimOffset[] = {0, 640, 1280, 1920};
const UBYTE mapData[] = {1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16,
            1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
            2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14,
            3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15,
            4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16};

void gameGsCreate(void) {
	s_pView = viewCreate(0,
		TAG_VIEW_GLOBAL_PALETTE, 1,
    TAG_DONE);

	s_pVPort = vPortCreate(0,
		TAG_VPORT_VIEW, s_pView,
		TAG_VPORT_BPP, GAME_BPP,
    TAG_DONE);

    randInit(&s_sRandManager, 1337, 2600);

    paletteLoad("data/shmup32.plt", s_pVPort->pPalette, 1 << GAME_BPP);

    s_pTiles = bitmapCreateFromFile("data/bgtiles.bm", 0);
    s_pPlayerImage = bitmapCreateFromFile("data/player.bm", 0);
    s_pPlayerMask = bitmapCreateFromFile("data/player-mask.bm", 0);
    // s_pAsteroidImage = bitmapCreateFromFile("data/asteroid.bm", 0);
    // s_pAsteroidMask = bitmapCreateFromFile("data/asteroid-mask.bm", 0);
    s_pEnemyImage = bitmapCreateFromFile("data/enemy_small.bm", 0);
    s_pEnemyMask = bitmapCreateFromFile("data/enemy_small-mask.bm", 0);

    // Tilemap stuff
	s_pTileBuffer = tileBufferCreate(0,
		TAG_TILEBUFFER_VPORT, s_pVPort,
		TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
		TAG_TILEBUFFER_BOUND_TILE_X, 20,
		TAG_TILEBUFFER_BOUND_TILE_Y, 32,
        TAG_TILEBUFFER_IS_DBLBUF, 1,
        TAG_TILEBUFFER_TILE_SHIFT, 4,
        TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 100,
        TAG_TILEBUFFER_TILESET, s_pTiles,
    TAG_DONE);

    UWORD idx = 0;
    for (UWORD y=0; y<MAP_HEIGHT_IN_TILES; y++) {
        for (UWORD x=0; x<MAP_WIDTH_IN_TILES; x++) {
            s_pTileBuffer->pTileData[x][y] = mapData[idx] - 1;
            idx++;
        }
    }
    
    s_pCamera = s_pTileBuffer->pCamera;

    // Bob stuff
    bobManagerCreate(s_pTileBuffer->pScroll->pFront, s_pTileBuffer->pScroll->pBack, s_pTileBuffer->pScroll->uBfrBounds.uwY);
    
    bobInit(
        &s_sPlayerShipBob,
        32, 32,
        1,
        s_pPlayerImage->Planes[0],
        s_pPlayerMask->Planes[0],
        0, 0
    );

    UBYTE xPos = 32;
    for (UBYTE i=0; i<MAX_ENEMEYS; i++) {
        bobInit(
            &s_tEnemys[i].sBob,
            32, 32,               
            1,
            s_pEnemyImage->Planes[0],
            s_pEnemyMask->Planes[0],
            0, 0
        );

        s_tEnemys[i].sBob.sPos.uwX = xPos;
        s_tEnemys[i].sBob.sPos.uwY = 64;
        xPos += 48;
    }

    // for (UBYTE i=0; i<MAX_ASTEROIDS; i++) {
    //     bobInit(
    //         &s_tAsteroids[i].sBob,
    //         32, 32,
    //         1,
    //         s_pAsteroidImage->Planes[0],
    //         s_pAsteroidMask->Planes[0],
    //         0, 0
    //     );
    //     s_tAsteroids[i].sBob.sPos.uwX = xPos;
    //     s_tAsteroids[i].sBob.sPos.uwY = 64;
    //     xPos += 64;
    // }
  
    bobReallocateBgBuffers();

    // Sprite stuff
    spriteManagerCreate(s_pView, 0);
    systemSetDmaBit(DMAB_SPRITE, 1);
    s_pPlayerProjectileImage = bitmapCreateFromFile("data/bulletsprite.bm", 0);
    s_pSprite0 = spriteAdd(0, s_pPlayerProjectileImage);
    // spriteProcess(s_pSprite0);
    // spriteProcessChannel(0);

    UWORD offset = 100;
    for (UBYTE i=0; i<MAX_PROJECTILES; i++) {
        s_tProjectiles[i].pProjectileBlock = copBlockCreate(s_pView->pCopList, 4, 0, s_pView->ubPosY+offset);
        s_tProjectiles[i].wX = 0;
        s_tProjectiles[i].wY = 0;
        s_tProjectiles[i].ubTimeToLive = 0;
        offset += 20;
    }

    // Finish up
    systemUnuse();
    viewLoad(s_pView);
    tileBufferRedrawAll(s_pTileBuffer);

    playerPosition.uwX = 144; playerPosition.uwY = 240;
    cameraMoveBy(s_pCamera, 0, 32);
}

void gameGsLoop(void) {
    // Check input
    if(keyCheck(KEY_ESCAPE)) {
        gameExit();
    }

	if(keyCheck(KEY_W)) {
		playerPosition.uwY = playerPosition.uwY - PLAYER_MOVE_SPEED;
	}
	if(keyCheck(KEY_S)) {
		playerPosition.uwY = playerPosition.uwY + PLAYER_MOVE_SPEED;
	}
	if(keyCheck(KEY_A)) {
		playerPosition.uwX = playerPosition.uwX - PLAYER_MOVE_SPEED;
	}
	if(keyCheck(KEY_D)) {
		playerPosition.uwX = playerPosition.uwX + PLAYER_MOVE_SPEED;
	}

    if (keyCheck(KEY_SPACE)) {
        if (fireDelay == 0) {
            for (UBYTE i=0; i<MAX_PROJECTILES; i++) {
                if (s_tProjectiles[i].ubTimeToLive == 0) {
                    s_tProjectiles[i].wX = playerPosition.uwX + 7;
                    s_tProjectiles[i].wY = playerPosition.uwY - 12;
                    s_tProjectiles[i].ubTimeToLive = 255;
                    fireDelay = FIRE_DELAY;
                    break;
                }
            }
        }
    }

    UWORD uwY = 0;
	if(keyCheck(KEY_UP)) {
		uwY = -1;
	}
	if(keyCheck(KEY_DOWN)) {
		uwY = 1;
	}

    // Keep player inside viewport area.
    if (playerPosition.uwX <= 4) {
        playerPosition.uwX = 4;
    }

    if (playerPosition.uwX > 284) {
        playerPosition.uwX = 284;
    }

    if (playerPosition.uwY > s_pCamera->uPos.uwY+VIEWPORT_HEIGHT-32) {
        playerPosition.uwY = s_pCamera->uPos.uwY+VIEWPORT_HEIGHT-32;
    }

    if (playerPosition.uwY < s_pCamera->uPos.uwY) {
        playerPosition.uwY = s_pCamera->uPos.uwY;
    }

    // Bobs
    tileBufferQueueProcess(s_pTileBuffer);
    bobBegin(s_pTileBuffer->pScroll->pBack);

    s_sPlayerShipBob.sPos = playerPosition;
    bobSetFrame(
        &s_sPlayerShipBob,
        &s_pPlayerImage->Planes[0][playerShipAnimOffset[playerShipAnimFrame]],
        &s_pPlayerMask->Planes[0][playerShipAnimOffset[playerShipAnimFrame]]
    );
    bobPush(&s_sPlayerShipBob);

    for (UBYTE i=0; i<MAX_ENEMEYS; i++) {
        bobPush(&s_tEnemys[i].sBob);        
    }

    // for (UBYTE i=0; i<MAX_ASTEROIDS; i++) {
    //     bobSetFrame(
    //         &s_tAsteroids[i].sBob,
    //         &s_pAsteroidImage->Planes[0][asteroidAnimOffset[s_tAsteroids[i].ubFrameIndex]],
    //         &s_pAsteroidMask->Planes[0][asteroidAnimOffset[s_tAsteroids[i].ubFrameIndex]]
    //     );
    //     bobPush(&s_tAsteroids[i].sBob);        
    // }
    bobEnd();

    // Sprites
    //logWrite("C:(%d, %d) => S:(%d, %d)", s_pCamera->uPos.uwX, s_pCamera->uPos.uwY, s_tProjectiles[0].uwX, s_tProjectiles[0].uwY);

    for (UBYTE i=0; i<MAX_PROJECTILES; i++) {
        if (s_tProjectiles[i].ubTimeToLive > 0) {
            reinstanceSprite(s_tProjectiles[i].pProjectileBlock, s_tProjectiles[i].wX, s_tProjectiles[i].wY - s_pCamera->uPos.uwY, s_pSprite0->uwHeight);
            s_tProjectiles[i].wY -= PROJECTILE_MOVE_SPEED;

            if (s_tProjectiles[i].wY < (s_pCamera->uPos.uwY-32)) {
                s_tProjectiles[i].ubTimeToLive = 0;
                //reinstanceSprite(s_tProjectiles[i].pProjectileBlock, 0, 0, s_pSprite0->uwHeight);
            } else {
                s_tProjectiles[i].ubTimeToLive--;
            }
        }
    }

    // Timers
    animationTimer++;
    if (animationTimer > ANIMATION_PERIOD) {
        animationTimer = 0;

        // Increment all frame indexes.
        playerShipAnimFrame++;
        playerShipAnimFrame = playerShipAnimFrame % NUM_FRAMES_PLAYER;

        // for (UBYTE i=0; i<MAX_ASTEROIDS; i++) {
        //     s_tAsteroids[i].ubFrameIndex++;
        //     s_tAsteroids[i].ubFrameIndex = s_tAsteroids[i].ubFrameIndex % NUM_FRAMES_ASTEROID;
        // }
    }

    if (fireDelay > 0) { fireDelay--; }

    // Finish up.
    cameraMoveBy(s_pCamera, 0, uwY);
    tileBufferQueueProcess(s_pTileBuffer);
    viewProcessManagers(s_pView);
    copProcessBlocks();
    vPortWaitForEnd(s_pVPort);
}

void gameGsDestroy(void) {
    systemUse();
    bitmapDestroy(s_pTiles);
    viewDestroy(s_pView);
}

static void reinstanceSprite(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight) {
    UWORD uwVStart = s_pView->ubPosY + wY;
    UWORD uwVStop = uwVStart + uwHeight;
    UWORD uwHStart = 128 + wX;

    UWORD uwPos = ((uwVStart << 8) | ((uwHStart) >> 1));
    UWORD uwCtl = (UWORD) (
        (uwVStop << 8) |
        (0 << 7) |
        (BTST(uwVStart, 8) << 2) |
        (BTST(uwVStop, 8) << 1) |
        BTST(uwHStart, 0)
    );

    copBlockWait(s_pView->pCopList, pBlock, 0, s_pView->ubPosY + wY);

    pBlock->uwCurrCount = 0;
    ULONG ulAddr = (ULONG)(
        (UBYTE*)s_pSprite0->pBitmap->Planes[0] +
        sizeof(g_pCustom->spr[0].pos) +
        sizeof(g_pCustom->spr[0].ctl)
    ); // skip the control words

    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[0].uwHi, ulAddr >> 16);
    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[0].uwLo, ulAddr & 0xFFFF);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[0].pos, uwPos);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[0].ctl, uwCtl);
}