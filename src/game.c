#include "game.h"
#include "gamemath.h"

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
#include <ace/managers/memory.h>

#define GAME_BPP 5
#define MAP_WIDTH_IN_TILES 20
#define MAP_HEIGHT_IN_TILES 32
#define VIEWPORT_HEIGHT 256
#define MAP_BOTTOM 512

#define FIRE_DELAY 6
#define MAX_ENEMY_PROJECTILES 16
#define ENEMY_PROJECTILE_SPEED 3
#define ENEMY_PROJECTILE_HEIGHT 7
#define ENEMY_SPRITE_CHANNELS 4

typedef struct _tEnemyProjectile {
    tCopBlock *pProjectileBlock;
    fix16_t fX;
    fix16_t fY;
    fix16_t fDeltaX;
    fix16_t fDeltaY;
    UBYTE ubAlive;
    UBYTE ubChannel;
} tEnemyProjectile;

static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel);

static tView *s_pView; // View containing all the viewports
static tVPort *s_pVPort; // Viewport for playfield
static tCameraManager *s_pCamera;
static tTileBufferManager *s_pTileBuffer;
static tBitMap *s_pTiles;

static tBitMap *s_pEnemyProjectileImage;
static tBitMap *s_pEnemyProjectileImageA;
static tBitMap *s_pEnemyProjectileImageB;
static tBitMap *s_pEnemyProjectileImageC;
static tBitMap *s_pEnemyProjectileImageD;

static tSprite *s_pEnemyProjectileSprite0;
static tSprite *s_pEnemyProjectileSprite[ENEMY_SPRITE_CHANNELS];
static tEnemyProjectile s_tEnemyProjectiles[MAX_ENEMY_PROJECTILES];
static fix16_t s_fChannelPos[ENEMY_SPRITE_CHANNELS] = {0};

static tUwCoordYX s_tPlayerPosition;
static UWORD s_uwFireDelay = 0;
static UBYTE s_ubMoveDirection = 0;

const UBYTE c_ubMapData[] = {1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13,
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

    // Load palette
    paletteLoad("data/game.plt", s_pVPort->pPalette, 1 << GAME_BPP);

    // Load images
    s_pTiles = bitmapCreateFromFile("data/bgtiles.bm", 0);

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
            s_pTileBuffer->pTileData[x][y] = c_ubMapData[idx] - 1;
            idx++;
        }
    }
    
    s_pCamera = s_pTileBuffer->pCamera;

    // Sprite stuff
    spriteManagerCreate(s_pView, 0);
    systemSetDmaBit(DMAB_SPRITE, 1);

    s_pEnemyProjectileImage = bitmapCreateFromFile("data/bullet_enemy.bm", 0);
    s_pEnemyProjectileImageA = bitmapCreateFromFile("data/bullet_enemyA.bm", 0);
    s_pEnemyProjectileImageB = bitmapCreateFromFile("data/bullet_enemyB.bm", 0);
    s_pEnemyProjectileImageC = bitmapCreateFromFile("data/bullet_enemyC.bm", 0);
    s_pEnemyProjectileImageD = bitmapCreateFromFile("data/bullet_enemyD.bm", 0);

    s_pEnemyProjectileSprite0 = spriteAdd(0, s_pEnemyProjectileImage);
    s_pEnemyProjectileSprite[0] = spriteAdd(2, s_pEnemyProjectileImageA);
    s_pEnemyProjectileSprite[1] = spriteAdd(3, s_pEnemyProjectileImageB);
    s_pEnemyProjectileSprite[2] = spriteAdd(4, s_pEnemyProjectileImageC);
    s_pEnemyProjectileSprite[3] = spriteAdd(5, s_pEnemyProjectileImageD);

    s_fChannelPos[0] = fix16_from_int(0);
    s_fChannelPos[1] = fix16_from_int(0);
    s_fChannelPos[2] = fix16_from_int(0);
    s_fChannelPos[3] = fix16_from_int(0);

    for (UBYTE i=0; i<MAX_ENEMY_PROJECTILES; i++) {
        s_tEnemyProjectiles[i].pProjectileBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
        s_tEnemyProjectiles[i].fX = 0;
        s_tEnemyProjectiles[i].fY = 0;
        s_tEnemyProjectiles[i].fDeltaX = 0;
        s_tEnemyProjectiles[i].fDeltaY = 0;
        s_tEnemyProjectiles[i].ubAlive = 0;
        s_tEnemyProjectiles[i].ubChannel = 255;
    }

    // Finish up
    gameMathInit();
    systemUnuse();
    viewLoad(s_pView);
    tileBufferRedrawAll(s_pTileBuffer);

    s_tPlayerPosition.uwX = 16; s_tPlayerPosition.uwY = 240;
    cameraMoveBy(s_pCamera, 0, 32);
}

void gameGsLoop(void) {
    // Check input
    if(keyCheck(KEY_ESCAPE)) {
        gameExit();
    }

    if (keyCheck(KEY_SPACE)) {
        if (s_uwFireDelay == 0) {
            for (UBYTE projectileIdx=0; projectileIdx<MAX_ENEMY_PROJECTILES; projectileIdx++) {
                if (s_tEnemyProjectiles[projectileIdx].ubAlive == 0) {
                    UBYTE ubAngle = getAngleBetweenPoints(160, 128, s_tPlayerPosition.uwX, s_tPlayerPosition.uwY);
                    s_tEnemyProjectiles[projectileIdx].fX = fix16_from_int(160);
                    s_tEnemyProjectiles[projectileIdx].fY = fix16_from_int(128);
                    s_tEnemyProjectiles[projectileIdx].fDeltaX = ccos(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    s_tEnemyProjectiles[projectileIdx].fDeltaY = csin(ubAngle) * ENEMY_PROJECTILE_SPEED;                    
                    s_tEnemyProjectiles[projectileIdx].ubAlive = 255;
                    s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
                    s_uwFireDelay = FIRE_DELAY;
                    break;
                }
            }
        }
    }

    // Move player position
    if (s_ubMoveDirection == 0) {
        s_tPlayerPosition.uwX++;
    } else {
        s_tPlayerPosition.uwX--;
    }

    if (s_tPlayerPosition.uwX <= 16) { s_ubMoveDirection = 0; s_tPlayerPosition.uwX = 16; }
    if (s_tPlayerPosition.uwX >= 284) { s_ubMoveDirection = 1; s_tPlayerPosition.uwX = 284; }
    
    // Bobs
    tileBufferQueueProcess(s_pTileBuffer);

    // Sprites
    for (int projectileIdx=0; projectileIdx<MAX_ENEMY_PROJECTILES; projectileIdx++) {
        if (s_tEnemyProjectiles[projectileIdx].ubAlive != 0) 
        {
            s_tEnemyProjectiles[projectileIdx].fX = fix16_add(s_tEnemyProjectiles[projectileIdx].fX, s_tEnemyProjectiles[projectileIdx].fDeltaX);
            s_tEnemyProjectiles[projectileIdx].fY = fix16_add(s_tEnemyProjectiles[projectileIdx].fY, s_tEnemyProjectiles[projectileIdx].fDeltaY);                
            s_tEnemyProjectiles[projectileIdx].ubAlive--;

            // Check X bounds
            if (s_tEnemyProjectiles[projectileIdx].fX < 0 || s_tEnemyProjectiles[projectileIdx].fX > fix16_from_int(320))
            {
                s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
            }

            // Check Y bounds
            if (s_tEnemyProjectiles[projectileIdx].fY < fix16_from_int(s_pCamera->uPos.uwY) || s_tEnemyProjectiles[projectileIdx].fY > fix16_from_int(s_pCamera->uPos.uwY+256))
            {
                s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
            }

            if (s_tEnemyProjectiles[projectileIdx].ubAlive == 0) {
                UBYTE channelIdx = s_tEnemyProjectiles[projectileIdx].ubChannel;
                moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, channelIdx);
                s_fChannelPos[channelIdx] = fix16_from_int(0);
                s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
                continue;
            }

            s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
            int spriteYPos = fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY);

            for (int channelIdx=0; channelIdx<ENEMY_SPRITE_CHANNELS; channelIdx++)
            {
                int channelYPos = fix16_to_int(s_fChannelPos[channelIdx]);
                if (spriteYPos > (channelYPos + ENEMY_PROJECTILE_HEIGHT+1) || (spriteYPos + ENEMY_PROJECTILE_HEIGHT+1) < channelYPos)
                {
                    s_tEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                    s_fChannelPos[channelIdx] = fix16_from_int(spriteYPos);
                    break;
                }
            }

            if (s_tEnemyProjectiles[projectileIdx].ubChannel == 255) {
                moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, 0);
                s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
                continue;
            }

            moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX), fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY) - s_pCamera->uPos.uwY, ENEMY_PROJECTILE_HEIGHT, s_tEnemyProjectiles[projectileIdx].ubChannel);
        }
    }

    if (s_uwFireDelay > 0) { s_uwFireDelay--; }

    // Finish up.
    tileBufferQueueProcess(s_pTileBuffer);
    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pVPort);
    systemIdleEnd();
}

void gameGsDestroy(void) {
    systemUse();
    bitmapDestroy(s_pTiles);
    viewDestroy(s_pView);
}

static void moveEnemyProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel) {
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
        (UBYTE*)s_pEnemyProjectileSprite[ubChannel]->pBitmap->Planes[0] +
        sizeof(g_pCustom->spr[ubChannel + 2].pos) +
        sizeof(g_pCustom->spr[ubChannel + 2].ctl)
    ); // skip the control words

    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel + 2].uwHi, ulAddr >> 16);
    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel + 2].uwLo, ulAddr & 0xFFFF);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel + 2].pos, uwPos);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel + 2].ctl, uwCtl);
}