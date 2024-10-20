#include "game.h"
#include "game_math.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"

// System
static tView *s_pView; // View containing all the viewports
static tVPort *s_pGameViewport; // Viewport for playfield
static tVPort *s_pHudViewport; // Viewport for HUD
static tTileBufferManager *s_pTileBuffer;
static tScrollBufferManager *s_pHudBuffer;
static tCameraManager *s_pCamera;
static tRandManager s_tRandManager;
static UBYTE s_ubFrameCounter = 0;

// Level
static tBitMap *s_pTiles;
static UBYTE s_ubCameraCanMove = FALSE;
static UBYTE s_ubMoveCameraCounter = 0;

// HUD
static tFont *s_pHudFont;
static tTextBitMap *s_pHudScoreText;
static tTextBitMap *s_pHudLivesText;
static tTextBitMap *s_pHudSpecialText;
static tTextBitMap *s_pHudPowerText;
static tBitMap *s_pHudLivesIconImage;
static tBitMap *s_pHudLivesIconMask;
static tBitMap *s_pHudSpecialIconImage;
static tBitMap *s_pHudSpecialIconMask;
static tBitMap *s_pHudPowerIconImage;
static tBitMap *s_pHudPowerIconMask;

static ULONG s_ulPlayerScore = 0; 
static UBYTE s_ubPlayerLives = PLAYER_LIVES_MAX;
static UBYTE s_ubPlayerSpecial = PLAYER_SPECIAL_MAX;
static UBYTE s_ubUpdateScore = TRUE;
static UBYTE s_ubUpdatePower = TRUE;
static UBYTE s_ubUpdateLives = TRUE;
static UBYTE s_ubUpdateSpecial = TRUE;

// Player
static tBob s_tPlayerShipBob;
static tBitMap *s_pPlayerImage;
static tBitMap *s_pPlayerMask;
static tUwCoordYX s_tPlayerPosition;
static UBYTE s_ubPlayerShipAnimFrame = 0;
static UBYTE s_ubFireDelay = 0;
static UBYTE s_ubEquippedProjectileType = 0;
static UBYTE s_ubPlayerAlive = FALSE;

// Enemies
static tEnemy s_tEnemy[ENEMY_MAX];
static tBitMap *s_pEnemyImage;
static tBitMap *s_pEnemyMask;
static UBYTE s_ubActiveEnemies = 0;

// Explosions
static tExplosion s_tExplosions[EXPLOSIONS_MAX];
static UBYTE s_ubActiveExplosions = 0;
static tBitMap *s_pExplosionImage;
static tBitMap *s_pExplosionMask;

// Big Explosion
static tBob s_tBigExplosionBob;
static UBYTE s_ubBigExplosionActive = 0;
static UBYTE s_ubBigExplosionFrame = 0;
static tBitMap *s_pBigExplosionImage;
static tBitMap *s_pBigExplosionMask;

// Player Projectiles
static tPlayerProjectileType s_tPlayerProjectileTypes[PLAYER_PROJECTILE_TYPES] = {0};

static tPlayerProjectile s_tPlayerProjectiles[PLAYER_PROJECTILE_MAX] = {0};
static tSprite *s_pPlayerProjectileSprites[PLAYER_PROJECTILE_TYPES];
static tBitMap *s_pPlayerProjectileImages[PLAYER_PROJECTILE_TYPES];

static tPlayerSecondaryProjectile s_tPlayerSecondaryProjectiles[PLAYER_PROJECTILE_MAX] = {0};
static tSprite *s_pPlayerSecondaryProjectileSprites[PLAYER_SECONDARY_PROJECTILE_TYPES];
static tBitMap *s_pPlayerSecondaryProjectileImages[PLAYER_SECONDARY_PROJECTILE_TYPES];

// Enemy Projectiles
static tSprite *s_pEnemyProjectileSprite[ENEMY_SPRITE_CHANNELS];
static tBob s_tEnemyProjectileBob[ENEMY_BOB_CHANNELS];
static UBYTE s_ubProjectileBobIndex = 0;

  // fix16
static tEnemyProjectile s_tEnemyProjectiles[ENEMY_PROJECTILE_MAX] = {0};
static tChannelBounds s_fChannelBounds[ENEMY_SPRITE_CHANNELS] = {0};
static fix16_t s_fEnemyProjectileHeight;

  // UWORD
static tSimpleEnemyProjectile s_tSimpleEnemyProjectiles[ENEMY_PROJECTILE_MAX] = {0};
static tSimpleChannelBounds s_tSimpleChannelBounds[ENEMY_SPRITE_CHANNELS] = {0};

static tBitMap *s_pEnemyProjectileBobImage;
static tBitMap *s_pEnemyProjectileBobMask;
static tBitMap *s_pEnemyProjectileSpriteImage;

// Text
static tBob s_tTextGameOverBob;
static tBitMap *s_pTextGameOverImage;
static tBitMap *s_pTextGameOverMask;

// Lookups
static fix16_t s_fViewportXMax;
static UWORD s_uwPlayerShipAnimOffset[] = {0, 0, 160, 160, 320, 320, 480, 480};
static UWORD s_uwExplosionAnimOffset[] = {0, 0, 640, 640, 1280, 1280, 1920, 1920, 2560, 2560, 3200, 3200, 3840, 3840, 4480, 4480};
static UWORD s_uwBigExplosionAnimOffset[] = {0, 0, 0, 1440, 1440, 1440, 2880, 2880, 2880, 4320, 4320, 4320, 5760, 5760, 5760, 7200, 7200, 7200, 8640, 8640, 8640, 10080, 10080, 10080, 11520, 11520, 11520, 12960, 12960, 12960, 14400, 14400, 14400, 15840, 15840, 15840};
static WORD s_wSpineX[] = {112, 113, 113, 114, 114, 115, 116, 116, 117, 117, 118, 119, 119, 120, 121, 121, 122, 123, 123, 124, 125, 126, 126, 127, 128, 129, 129, 130, 131, 132, 132, 133, 134, 135, 135, 136, 137, 138, 138, 139, 140, 141, 141, 142, 143, 144, 145, 145, 146, 147, 148, 148, 149, 150, 151, 151, 152, 153, 154, 154, 155, 156, 156, 157, 158, 158, 159, 160, 160, 161, 162, 162, 163, 164, 164, 165, 165, 166, 167, 167, 168, 168, 169, 169, 170, 170, 171, 171, 172, 172, 173, 173, 173, 174, 174, 174, 175, 175, 175, 176, 176, 176, 177, 177, 177, 177, 177, 177, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 177, 177, 177, 177, 177, 176, 176, 176, 175, 175, 174, 174, 173, 173, 172, 172, 171, 170, 170, 169, 168, 167, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 153, 152, 151, 150, 149, 147, 146, 145, 143, 142, 141, 139, 138, 137, 135, 134, 132, 131, 130, 128, 127, 125, 124, 122, 121, 119, 118, 116, 115, 113, 112, 110, 109, 107, 106, 104, 103, 101, 100, 98, 97, 95, 93, 92, 90, 89, 88, 86, 85, 83, 82, 80, 79, 77, 76, 75, 73, 72, 70, 69, 68, 66, 65, 64, 63, 61, 60, 59, 58, 57, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 43, 42, 41, 40, 40, 39, 38, 38, 37, 37, 36, 36, 35, 35, 35, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 43, 43, 44, 44, 45, 46, 46, 47, 48, 48, 49, 50, 51, 51, 52, 53, 54, 54, 55, 56, 57, 58, 58, 59, 60, 61, 62, 63, 64, 64, 65, 66, 67, 68, 69, 70, 71, 71, 72, 73, 74, 75, 76, 77, 78, 79, 79, 80, 81, 82, 83, 84, 85, 86, 87, 87, 88, 89, 90, 91, 92, 93, 93, 94, 95, 96, 97, 98, 98, 99, 100, 101, 101, 102, 103, 104, 104, 105, 106, 106, 107, 108, 108, 109, 110, 110, 111, 111, 112};
static WORD s_wSpineY[] = {32, 32, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 36, 36, 37, 38, 38, 39, 40, 41, 42, 43, 44, 44, 45, 46, 48, 49, 50, 51, 52, 53, 54, 56, 57, 58, 60, 61, 62, 64, 65, 67, 68, 70, 71, 73, 74, 76, 77, 79, 80, 82, 84, 85, 87, 88, 90, 92, 93, 95, 97, 99, 100, 102, 104, 105, 107, 109, 111, 112, 114, 116, 118, 119, 121, 123, 125, 126, 128, 130, 131, 133, 135, 136, 138, 140, 141, 143, 145, 146, 148, 150, 151, 153, 154, 156, 157, 159, 160, 162, 163, 165, 166, 167, 169, 170, 171, 173, 174, 175, 176, 177, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 188, 189, 190, 191, 191, 192, 193, 193, 194, 194, 195, 196, 196, 197, 197, 198, 198, 199, 199, 200, 200, 201, 201, 202, 202, 203, 203, 203, 204, 204, 205, 205, 205, 206, 206, 206, 207, 207, 207, 208, 208, 208, 208, 209, 209, 209, 209, 210, 210, 210, 210, 210, 211, 211, 211, 211, 211, 211, 211, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 212, 211, 211, 211, 211, 211, 211, 211, 210, 210, 210, 210, 210, 209, 209, 209, 209, 208, 208, 208, 208, 207, 207, 207, 206, 206, 206, 205, 205, 205, 204, 204, 203, 203, 203, 202, 202, 201, 201, 200, 200, 199, 199, 198, 198, 197, 197, 196, 196, 195, 194, 194, 193, 193, 192, 191, 191, 190, 189, 188, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 177, 176, 175, 174, 173, 171, 170, 169, 167, 166, 165, 163, 162, 160, 159, 157, 156, 154, 153, 151, 150, 148, 146, 145, 143, 141, 140, 138, 136, 135, 133, 131, 130, 128, 126, 125, 123, 121, 119, 118, 116, 114, 112, 111, 109, 107, 105, 104, 102, 100, 99, 97, 95, 93, 92, 90, 88, 87, 85, 84, 82, 80, 79, 77, 76, 74, 73, 71, 70, 68, 67, 65, 64, 62, 61, 60, 58, 57, 56, 54, 53, 52, 51, 50, 49, 48, 46, 45, 44, 44, 43, 42, 41, 40, 39, 38, 38, 37, 36, 36, 35, 35, 34, 34, 34, 33, 33, 33, 32, 32, 32, 32, 32};

static UBYTE s_ubMapData[] = {0, 0, 1, 5,  9, 13, 17, 21, 25, 29, 1, 5, 0, 0, 
                              0, 0, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 0, 0, 
                              0, 0, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 0, 0, 
                              0, 0, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 0, 0};

// Debug
static UBYTE s_ubEnemyCanFire;

// Game states.
void gameGsCreate(void) {
    randInit(&s_tRandManager, 1337, 2600);
    initGame();
    initViews();
    initHud();
    initBobs();
    initSprites();

    gameMathInit();
    viewLoad(s_pView);
    tileBufferRedrawAll(s_pTileBuffer);
    memLogPeak();
    systemUnuse();
}

void gameGsLoop(void) {
    //g_pCustom->color[0] = 0xF00;
    s_ubFrameCounter = !s_ubFrameCounter;
    
    // Process game logic.
    processCamera();
    processInput();
    processPlayerProjectiles();
    processEnemies();
    processSimpleEnemyProjectiles();
    processBobs();
    processTimers();
    processHud();

    // Wrap up processing frame.    
    tileBufferQueueProcess(s_pTileBuffer);
    viewProcessManagers(s_pView);
    copProcessBlocks();
    //g_pCustom->color[0] = 0x000;
    systemIdleBegin();
    vPortWaitForEnd(s_pGameViewport);
    systemIdleEnd();

}

void gameGsDestroy(void) {
    systemUse();
    bitmapDestroy(s_pTiles);
    viewDestroy(s_pView);
}

// Initialize game.
static void initGame() {
    // Setup projectile types
    s_tPlayerProjectileTypes[0] = (tPlayerProjectileType){ .ubDamage = 10, .ubDelay =  5, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -6, .ubXOffset =  2, .ubXOffset2 =  0, .ubWidth = 11, .ubHeight = 10, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[1] = (tPlayerProjectileType){ .ubDamage = 12, .ubDelay =  8, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -6, .ubXOffset =  1, .ubXOffset2 =  0, .ubWidth = 11, .ubHeight = 12, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[2] = (tPlayerProjectileType){ .ubDamage = 14, .ubDelay = 10, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -3, .ubXOffset =  0, .ubXOffset2 =  0, .ubWidth = 13, .ubHeight = 11, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[3] = (tPlayerProjectileType){ .ubDamage =  5, .ubDelay =  4, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -8, .ubXOffset =  2, .ubXOffset2 =  0, .ubWidth = 12, .ubHeight = 12, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    // s_tPlayerProjectileTypes[4] = (tPlayerProjectileType){ .ubDamage = 30, .ubDelay = 8, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -4, .ubXOffset = 10, .ubXOffset2 =  0, .ubWidth = 11, .ubHeight = 20, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    // s_tPlayerProjectileTypes[5] = (tPlayerProjectileType){ .ubDamage = 5,  .ubDelay = 8, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -8, .ubXOffset =  0, .ubXOffset2 = 16, .ubWidth = 31, .ubHeight = 20, .ubDieOnCollision = TRUE, .ubWideSprite = TRUE,  .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 }; // Wideshot
    // s_tPlayerProjectileTypes[6] = (tPlayerProjectileType){ .ubDamage = 25, .ubDelay = 8, .bDeltaX = -5, .bDeltaX2 =  5, .bDeltaY = -5, .ubXOffset =  9, .ubXOffset2 = 16, .ubWidth =  7, .ubHeight =  8, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = TRUE,  .ubSecondarySpriteIndex = 1 }; // SpreadShot
}

static void initViews() {
    // Setup screen and viewports.
    s_pView = viewCreate(0,
        TAG_VIEW_GLOBAL_PALETTE, 1,
        TAG_VIEW_WINDOW_WIDTH, 160,
        TAG_VIEW_WINDOW_START_X, SCREEN_XOFFSET + 80,
    TAG_DONE);

    s_pGameViewport = vPortCreate(0,
        TAG_VPORT_VIEW, s_pView,
        TAG_VPORT_BPP, GAME_BPP,
        TAG_VPORT_HEIGHT, TILE_VIEWPORT_HEIGHT,
    TAG_DONE);

    s_pHudViewport = vPortCreate(0,
        TAG_VPORT_VIEW, s_pView,
        TAG_VPORT_BPP, GAME_BPP,
        TAG_VPORT_HEIGHT, HUD_VIEWPORT_HEIGHT,
    TAG_DONE);

    // Load palette.
    paletteLoad("data/game.plt", s_pGameViewport->pPalette, 1 << GAME_BPP);
  
    // Load tiles bitmap.
    s_pTiles = bitmapCreateFromFile("data/bgtiles.bm", 0);

    // Init tilemap buffer.
    s_pTileBuffer = tileBufferCreate(0,
        TAG_TILEBUFFER_VPORT, s_pGameViewport,
        TAG_TILEBUFFER_BITMAP_FLAGS, BMF_CLEAR | BMF_INTERLEAVED,
        TAG_TILEBUFFER_BOUND_TILE_X, MAP_WIDTH_IN_TILES,
        TAG_TILEBUFFER_BOUND_TILE_Y, MAP_HEIGHT_IN_TILES,
        TAG_TILEBUFFER_IS_DBLBUF, 1,
        TAG_TILEBUFFER_TILE_SHIFT, 4,
        TAG_TILEBUFFER_REDRAW_QUEUE_LENGTH, 100,
        TAG_TILEBUFFER_TILESET, s_pTiles,
    TAG_DONE);

    // Init simple buffer.
    // s_pHudBuffer = simpleBufferCreate(0,
    //     TAG_SIMPLEBUFFER_VPORT, s_pHudViewport,
    //     TAG_SIMPLEBUFFER_USE_X_SCROLLING, 1,
    //     TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
    //     TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    // TAG_DONE);

    s_pHudBuffer = scrollBufferCreate(0,
        TAG_SCROLLBUFFER_VPORT, s_pHudViewport,
        TAG_SCROLLBUFFER_IS_DBLBUF, 0,
        TAG_SCROLLBUFFER_MARGIN_WIDTH, 4,
        TAG_SCROLLBUFFER_BOUND_WIDTH, 160,
        TAG_SCROLLBUFFER_BOUND_HEIGHT, 12,        
        TAG_SCROLLBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    TAG_DONE);

    // Populate tilemap.
    UWORD idx = 0;
    for (UWORD y=0; y<MAP_HEIGHT_IN_TILES; y++) {
        for (UWORD x=0; x<MAP_WIDTH_IN_TILES; x++) {
            if (idx == MAP_TILES_COUNT) { idx = 0; }
            s_pTileBuffer->pTileData[x][y] = s_ubMapData[idx];
            idx++;
        }
    }

    // Setup camera.
    s_pCamera = s_pTileBuffer->pCamera;
    s_fViewportXMax = fix16_from_int(TILE_VIEWPORT_XMAX);
    cameraMoveBy(s_pCamera, 32, 32);
}

static void initHud() {
    // Load font
    s_pHudFont = fontCreate("data/hudfont.fnt");
    s_pHudScoreText = fontCreateTextBitMap(HUD_SCORE_WIDTH, HUD_TEXT_HEIGHT);
    s_pHudLivesText = fontCreateTextBitMap(HUD_TEXT_WIDTH, HUD_TEXT_HEIGHT);
    s_pHudSpecialText = fontCreateTextBitMap(HUD_TEXT_WIDTH, HUD_TEXT_HEIGHT);
    s_pHudPowerText = fontCreateTextBitMap(HUD_TEXT_WIDTH, HUD_TEXT_HEIGHT);

    // Load HUD lives icon
    s_pHudLivesIconImage = bitmapCreateFromFile("data/hud_lives_icon.bm", 0);
    s_pHudLivesIconMask = bitmapCreateFromFile("data/hud_lives_icon_mask.bm", 0);

    // Load HUD special icon
    s_pHudSpecialIconImage = bitmapCreateFromFile("data/hud_special_icon.bm", 0);
    s_pHudSpecialIconMask = bitmapCreateFromFile("data/hud_special_icon_mask.bm", 0);

    // Load HUD power level icon
    s_pHudPowerIconImage = bitmapCreateFromFile("data/hud_power_icon.bm", 0);
    s_pHudPowerIconMask = bitmapCreateFromFile("data/hud_power_icon_mask.bm", 0);
}

static void initBobs() {
    // Init tilemap bob manager.
    bobManagerCreate(s_pTileBuffer->pScroll->pFront, s_pTileBuffer->pScroll->pBack, s_pTileBuffer->pScroll->uBfrBounds.uwY);

    // Player ship bob.
    s_pPlayerImage = bitmapCreateFromFile("data/player.bm", 0);
    s_pPlayerMask = bitmapCreateFromFile("data/player_mask.bm", 0);
    bobInit(&s_tPlayerShipBob, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, 1, s_pPlayerImage->Planes[0], s_pPlayerMask->Planes[0], 0, 0);

    // Enemy bobs
    s_pEnemyImage = bitmapCreateFromFile("data/enemy.bm", 0);
    s_pEnemyMask = bitmapCreateFromFile("data/enemy_mask.bm", 0);

    // Init enemy structs.
    for (UBYTE i=0; i<ENEMY_MAX; i++) {
        bobInit( &s_tEnemy[i].sBob, ENEMY_WIDTH, ENEMY_HEIGHT, 1, s_pEnemyImage->Planes[0], s_pEnemyMask->Planes[0], 0, 0 );
        s_tEnemy[i].tPosition.uwX = 0;
        s_tEnemy[i].tPosition.uwY = 0;
        s_tEnemy[i].bHealth = 0;
        s_tEnemy[i].ubCooldownTimer = 0;
        s_tEnemy[i].ubEnemyType = 0;
        s_tEnemy[i].ubPathType = 0;
        s_tEnemy[i].uwPathIdx = 0;
        s_tEnemy[i].uwScoreValue = ENEMY_SCORE_VALUE;
    }

    // Small explosion bobs
    s_pExplosionImage = bitmapCreateFromFile("data/explosion.bm", 0);
    s_pExplosionMask = bitmapCreateFromFile("data/explosion_mask.bm", 0);

    for (UBYTE i=0; i<EXPLOSIONS_MAX; i++) {
        bobInit(&s_tExplosions[i].sBob, 32, 32, 1, s_pExplosionImage->Planes[0], s_pExplosionMask->Planes[0], 0, 0);
    }

    // Large explosion bob
    s_pBigExplosionImage = bitmapCreateFromFile("data/bigexplosion.bm", 0);
    s_pBigExplosionMask = bitmapCreateFromFile("data/bigexplosion_mask.bm", 0);

    bobInit(&s_tBigExplosionBob, 48, 48, 1, s_pBigExplosionImage->Planes[0], s_pBigExplosionMask->Planes[0], 0, 0);

    // Enemy projectiles.
    s_pEnemyProjectileBobImage = bitmapCreateFromFile("data/enemy_bullet_bob_2.bm", 0);
    s_pEnemyProjectileBobMask = bitmapCreateFromFile("data/enemy_bullet_bob_2_mask.bm", 0);

    for (UBYTE i=0; i<ENEMY_BOB_CHANNELS; i++) {
        bobInit(&s_tEnemyProjectileBob[i], 16, 6, 1, s_pEnemyProjectileBobImage->Planes[0], s_pEnemyProjectileBobMask->Planes[0], 0, 0);
    }

    // Game Over text
    s_pTextGameOverImage = bitmapCreateFromFile("data/text_gameover.bm", 0);
    s_pTextGameOverMask = bitmapCreateFromFile("data/text_gameover_mask.bm", 0);
    bobInit(&s_tTextGameOverBob, 112, 60, 0, s_pTextGameOverImage->Planes[0], s_pTextGameOverMask->Planes[0], 0, 0);

    // Finish tilebuffer bob init.
    bobReallocateBgBuffers();
}

static void initSprites() {
    // Init sprites.
    spriteManagerCreate(s_pView, 0);
    systemSetDmaBit(DMAB_SPRITE, 1);

    // Enemy projectile sprites.
    s_fEnemyProjectileHeight = fix16_from_int(ENEMY_PROJECTILE_HEIGHT+1);    
    s_pEnemyProjectileSpriteImage = bitmapCreateFromFile("data/enemy_bullet_sprite_2.bm", 0);
    s_pEnemyProjectileSprite[0] = spriteAdd(0, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[1] = spriteAdd(1, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[2] = spriteAdd(2, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[3] = spriteAdd(3, s_pEnemyProjectileSpriteImage);

    // s_fChannelBounds[0] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    // s_fChannelBounds[1] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    // s_fChannelBounds[2] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    // s_fChannelBounds[3] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};

    s_tSimpleChannelBounds[0] = (tSimpleChannelBounds){.uwMin = 8, .uwMax = 65527};
    s_tSimpleChannelBounds[1] = (tSimpleChannelBounds){.uwMin = 8, .uwMax = 65527};
    s_tSimpleChannelBounds[2] = (tSimpleChannelBounds){.uwMin = 8, .uwMax = 65527};
    s_tSimpleChannelBounds[3] = (tSimpleChannelBounds){.uwMin = 8, .uwMax = 65527};

    for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
        //s_tEnemyProjectiles[i].pCopBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
        s_tSimpleEnemyProjectiles[i].pCopBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
    }

    // Player projectile sprite.   
    s_pPlayerProjectileImages[0] = bitmapCreateFromFile("data/player_bullet_sprite_1.bm", 0);
    s_pPlayerProjectileImages[1] = bitmapCreateFromFile("data/player_bullet_sprite_2.bm", 0);
    s_pPlayerProjectileImages[2] = bitmapCreateFromFile("data/player_bullet_sprite_3.bm", 0);
    s_pPlayerProjectileImages[3] = bitmapCreateFromFile("data/player_bullet_sprite_4.bm", 0);
    
    s_pPlayerProjectileSprites[0] = spriteAdd(4, s_pPlayerProjectileImages[0]);
    s_pPlayerProjectileSprites[1] = spriteAdd(4, s_pPlayerProjectileImages[1]);
    s_pPlayerProjectileSprites[2] = spriteAdd(4, s_pPlayerProjectileImages[2]);
    s_pPlayerProjectileSprites[3] = spriteAdd(4, s_pPlayerProjectileImages[3]);

    // s_pPlayerSecondaryProjectileImages[0] = bitmapCreateFromFile("data/bullet6b_sprite_player.bm", 0); // Wide shot
    // s_pPlayerSecondaryProjectileImages[1] = bitmapCreateFromFile("data/bullet7r_sprite_player.bm", 0); // Spread shot

    // s_pPlayerSecondaryProjectileSprites[0] = spriteAdd(5, s_pPlayerSecondaryProjectileImages[0]);
    // s_pPlayerSecondaryProjectileSprites[1] = spriteAdd(5, s_pPlayerSecondaryProjectileImages[1]);

    for (UBYTE i=0; i<PLAYER_PROJECTILE_MAX; i++) {
        s_tPlayerProjectiles[i].pCopBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
        //s_tPlayerSecondaryProjectiles[i].pCopBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
    }
}

// Process game logic.
static void processCamera() {
    if (s_ubCameraCanMove == TRUE) {
        if (s_ubMoveCameraCounter == CAMERA_MOVE_RATE) { 
            if (s_pCamera->uPos.uwY > 0) { cameraMoveBy(s_pCamera, 0, CAMERA_MOVE_AMOUNT); }
            s_ubMoveCameraCounter = 0;
        }
        s_ubMoveCameraCounter++;

        // Keep player inside camera area.
        UWORD uwCameraYMax = s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT;
        if (s_tPlayerPosition.uwY+PLAYER_SHIP_HEIGHT > uwCameraYMax) {
            s_tPlayerPosition.uwY = uwCameraYMax - PLAYER_SHIP_HEIGHT;
        }        
    }
}

static void processInput() {
    // Quit
    if(keyCheck(KEY_ESCAPE)) {
        gameExit();
    }

    // Move player
    if(keyCheck(KEY_W)) {
        s_tPlayerPosition.uwY = s_tPlayerPosition.uwY - PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwY < s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN) { s_tPlayerPosition.uwY = s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN; }
    }
    if(keyCheck(KEY_S)) {
        s_tPlayerPosition.uwY = s_tPlayerPosition.uwY + PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwY > s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT)) { s_tPlayerPosition.uwY = s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT); }
    }
    if(keyCheck(KEY_A)) {
        s_tPlayerPosition.uwX = s_tPlayerPosition.uwX - PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwX < TILE_VIEWPORT_XMIN) { s_tPlayerPosition.uwX = TILE_VIEWPORT_XMIN; }
    }
    if(keyCheck(KEY_D)) {
        s_tPlayerPosition.uwX = s_tPlayerPosition.uwX + PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwX > (TILE_VIEWPORT_XMAX-PLAYER_SHIP_WIDTH)) { s_tPlayerPosition.uwX = (TILE_VIEWPORT_XMAX-PLAYER_SHIP_WIDTH); }
    }

    // Shoot
    if (keyCheck(KEY_SPACE)) {
        processPlayerShoot();
    }
   
    // DEBUG: toggle enemy fire
    if (keyCheck(KEY_UP)) { cameraMoveBy(s_pCamera, 0, -1); }
    if (keyCheck(KEY_DOWN)) { cameraMoveBy(s_pCamera, 0, 1); }

    if (keyCheck(KEY_1)) {
        if (s_ubFireDelay == 0) {
            s_ubEnemyCanFire = !s_ubEnemyCanFire;
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }        
    }

    // DEBUG: spawn enemy
    if (keyCheck(KEY_2)) {
        if (s_ubFireDelay == 0) {
            if (s_ubActiveEnemies < ENEMY_MAX) {
                for (UBYTE i=0; i<ENEMY_MAX; i++) {
                    if (s_tEnemy[i].bHealth > 0) { continue; }
                    s_tEnemy[i].tPosition.uwX = s_wSpineX[0];
                    s_tEnemy[i].tPosition.uwY = s_wSpineY[0];
                    s_tEnemy[i].bHealth = 20;
                    s_tEnemy[i].ubCooldownTimer = 0;
                    s_tEnemy[i].uwPathIdx = 0;
                    s_tEnemy[i].uwScoreValue = ENEMY_SCORE_VALUE;
                    s_ubActiveEnemies++;
                    break;
                }
            }
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }

    // DEBUG: increment player lives
    if (keyCheck(KEY_3)) {
        if (s_ubFireDelay == 0) {
            if (s_ubPlayerLives < PLAYER_LIVES_MAX) { s_ubPlayerLives++; }
            s_ubUpdateLives = TRUE;
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }

    // DEBUG: cycle projectile type
    if (keyCheck(KEY_4)) {
        if (s_ubFireDelay == 0) {
            s_ubEquippedProjectileType++;
            if (s_ubEquippedProjectileType >= PLAYER_PROJECTILE_TYPES) { s_ubEquippedProjectileType = 0; }
            s_ubUpdatePower = TRUE;
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }

    // DEBUG: respawn player
    if (keyCheck(KEY_R)) {
        if (s_ubPlayerAlive == 0 && s_ubPlayerLives > 0) {
            s_tPlayerPosition.uwX = s_pCamera->uPos.uwX+(80 - (PLAYER_SHIP_WIDTH/2));
            s_tPlayerPosition.uwY = s_pCamera->uPos.uwY+128;
            s_ubPlayerAlive = 1;
        }
    }
}

static void processHud() {
    // Lives
    if (s_ubUpdateLives == TRUE) {
        char cLives[4];
        sprintf(cLives, "%01d", s_ubPlayerLives);
        fontFillTextBitMap(s_pHudFont, s_pHudLivesText, cLives);
        fontDrawTextBitMap(s_pHudBuffer->pBack, s_pHudLivesText, 14, 3, 19, 0);
        blitCopyMask(s_pHudLivesIconImage, 0, 0, s_pHudBuffer->pBack, 4, 3, 7, 7, s_pHudLivesIconMask->Planes[0]);
        s_ubUpdateLives = FALSE;
    }

    // Special
    if (s_ubUpdateSpecial == TRUE) {
        char cSpecial[4];
        sprintf(cSpecial, "%01d", s_ubPlayerSpecial);
        fontFillTextBitMap(s_pHudFont, s_pHudSpecialText, cSpecial);
        fontDrawTextBitMap(s_pHudBuffer->pBack, s_pHudSpecialText, 42, 3, 19, 0);
        blitCopyMask(s_pHudSpecialIconImage, 0, 0, s_pHudBuffer->pBack, 32, 2, 7, 9, s_pHudSpecialIconMask->Planes[0]);
        s_ubUpdateSpecial = FALSE;
    }    

    // Power
    if (s_ubUpdatePower == TRUE) {
        char cPlayerPower[4];
        sprintf(cPlayerPower, "%01d", s_ubEquippedProjectileType);
        fontFillTextBitMap(s_pHudFont, s_pHudPowerText, cPlayerPower);
        fontDrawTextBitMap(s_pHudBuffer->pBack, s_pHudPowerText, 69, 3, 19, 0);
        blitCopyMask(s_pHudPowerIconImage, 0, 0, s_pHudBuffer->pBack, 58, 4, 7, 5, s_pHudPowerIconMask->Planes[0]);
        s_ubUpdatePower = FALSE;
    }

    // Score
    if (s_ubUpdateScore == TRUE) {
        char cPlayerScore[12];
        sprintf(cPlayerScore, "%07ld", s_ulPlayerScore);
        fontFillTextBitMap(s_pHudFont, s_pHudScoreText, cPlayerScore);
        fontDrawTextBitMap(s_pHudBuffer->pBack, s_pHudScoreText, 94, 3, 19, 0);
        s_ubUpdateScore = FALSE;
    }
}

static void processBobs() {
    systemSetDmaBit(DMAB_BLITHOG, 1);
    bobBegin(s_pTileBuffer->pScroll->pBack);

    // Player ship
    if (s_ubPlayerAlive == 1) {
        s_tPlayerShipBob.sPos = s_tPlayerPosition;
        bobSetFrame(
            &s_tPlayerShipBob,
            &s_pPlayerImage->Planes[0][s_uwPlayerShipAnimOffset[s_ubPlayerShipAnimFrame]],
            &s_pPlayerMask->Planes[0][s_uwPlayerShipAnimOffset[s_ubPlayerShipAnimFrame]]
        );

        bobPush(&s_tPlayerShipBob);
    }

    // Enemies
    if (s_ubActiveEnemies > 0) {
        for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
            if (s_tEnemy[enemyIdx].bHealth == 0) { continue; }
            bobPush(&s_tEnemy[enemyIdx].sBob);
        }
    }

    // Explosions
    if (s_ubActiveExplosions > 0) {
        for (UBYTE explosionIdx=0; explosionIdx < EXPLOSIONS_MAX; explosionIdx++) {
            if (s_tExplosions[explosionIdx].ubAlive == 0) { continue; }
            UBYTE ubFrameIdx = s_tExplosions[explosionIdx].ubFrameIndex;
            UWORD uwAnimOffset = s_uwExplosionAnimOffset[ubFrameIdx];

            bobSetFrame(
                &s_tExplosions[explosionIdx].sBob,
                &s_pExplosionImage->Planes[0][uwAnimOffset],
                &s_pExplosionMask->Planes[0][uwAnimOffset]
            );

            bobPush(&s_tExplosions[explosionIdx].sBob);
        }        
    }

    // Big Explosion
    if (s_ubBigExplosionActive == 1) {
        bobSetFrame(
            &s_tBigExplosionBob,
            &s_pBigExplosionImage->Planes[0][s_uwBigExplosionAnimOffset[s_ubBigExplosionFrame]],
            &s_pBigExplosionMask->Planes[0][s_uwBigExplosionAnimOffset[s_ubBigExplosionFrame]]
        );

        bobPush(&s_tBigExplosionBob);        
    }

    // Enemy projectiles
    if (s_ubProjectileBobIndex > 0) {
        for (UBYTE projectileIdx=0; projectileIdx<s_ubProjectileBobIndex; projectileIdx++) {
            bobPush(&s_tEnemyProjectileBob[projectileIdx]);
        }
    }
    
    // Game Over text
    if (s_ubPlayerLives == 0 && s_ubBigExplosionActive == 0) {
        s_tTextGameOverBob.sPos.uwX = 104; s_tTextGameOverBob.sPos.uwY = 92;
        bobPush(&s_tTextGameOverBob);        
    }

    bobEnd();
    systemSetDmaBit(DMAB_BLITHOG, 0);
}

static void processTimers() {
    // Player
    s_ubPlayerShipAnimFrame++; s_ubPlayerShipAnimFrame = s_ubPlayerShipAnimFrame % PLAYER_SHIP_ANIM_FRAMES;
    if (s_ubFireDelay > 0) { s_ubFireDelay--; }

    // Enemies
    for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
        if (s_tEnemy[enemyIdx].ubCooldownTimer > 0) { s_tEnemy[enemyIdx].ubCooldownTimer--; }
    }

    // Explosions
    if (s_ubActiveExplosions > 0) {
        for (UBYTE explosionIdx=0; explosionIdx < EXPLOSIONS_MAX; explosionIdx++) {
            if (s_tExplosions[explosionIdx].ubAlive == 0) { continue; }
            s_tExplosions[explosionIdx].ubFrameIndex++;
            if (s_tExplosions[explosionIdx].ubFrameIndex == EXPLOSION_ANIM_FRAMES) {
                s_tExplosions[explosionIdx].ubAlive = 0;
                s_ubActiveExplosions--;
            }
        }        
    }

    // Big Explosion
    if (s_ubBigExplosionActive == 1) {
        s_ubBigExplosionFrame++;
        if (s_ubBigExplosionFrame == BIGEXPLOSION_ANIM_FRAMES) {
            s_ubBigExplosionActive = 0;
            s_ubBigExplosionFrame = 0;
        }
    }
}

static void processEnemies() {
    if (s_ubActiveEnemies == 0) { return; }

    UWORD uwCameraYMin = s_pCamera->uPos.uwY;
    UWORD uwCameraYMax = s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT;
    UWORD uwCameraXMin = TILE_VIEWPORT_XMIN;
    UWORD uwCameraXMax = TILE_VIEWPORT_XMAX;

    for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
        if (s_tEnemy[enemyIdx].bHealth == 0) { continue; }
        
        // Move
        s_tEnemy[enemyIdx].uwPathIdx++;
        if (s_tEnemy[enemyIdx].uwPathIdx >= ENEMY_PATH_LENGTH) {
            s_tEnemy[enemyIdx].uwPathIdx = 0;
        }

        s_tEnemy[enemyIdx].tPosition.uwX = s_wSpineX[s_tEnemy[enemyIdx].uwPathIdx];
        s_tEnemy[enemyIdx].tPosition.uwY = s_wSpineY[s_tEnemy[enemyIdx].uwPathIdx];
        s_tEnemy[enemyIdx].sBob.sPos = s_tEnemy[enemyIdx].tPosition;

        // Check X bounds
        if (s_tEnemy[enemyIdx].tPosition.uwX < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.uwX > uwCameraXMax)
        {
            // s_tEnemy[enemyIdx].bHealth = 0;
            // s_tEnemy[enemyIdx].tPosition.wX = 0;
            // s_tEnemy[enemyIdx].tPosition.wY = 0;
            continue;
        }

        // Check Y bounds
        if (s_tEnemy[enemyIdx].tPosition.uwY < uwCameraYMin || s_tEnemy[enemyIdx].tPosition.uwY > uwCameraYMax)
        {
            // s_tEnemy[enemyIdx].bHealth = 0;
            // s_tEnemy[enemyIdx].tPosition.wX = 0;
            // s_tEnemy[enemyIdx].tPosition.wY = 0;
            continue;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        if (s_ubPlayerAlive == TRUE) {
            UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX+PLAYER_HITBOX_OFFSET_X, s_tPlayerPosition.uwY+PLAYER_HITBOX_OFFSET_Y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT, 
                                               s_tEnemy[enemyIdx].tPosition.uwX, s_tEnemy[enemyIdx].tPosition.uwY, ENEMY_WIDTH, ENEMY_HEIGHT);
            if (ubCollision == TRUE) { 
                processPlayerDie();
            }
        }
        #endif

        // Shoot
        if (s_ubEnemyCanFire == 0) { continue; }

        if (s_tEnemy[enemyIdx].ubCooldownTimer == 0) {
            for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
                if (s_tSimpleEnemyProjectiles[i].ubAlive == 0) {
                    // UWORD uwPlayerCenterX = s_tPlayerPosition.uwX + PLAYER_CENTER_OFFSET_X;
                    // UWORD uwPlayerCenterY = s_tPlayerPosition.uwY + PLAYER_CENTER_OFFSET_Y;
                    UWORD uwEnemyGunX = s_tEnemy[enemyIdx].tPosition.uwX + ENEMY_GUN_OFFSET_X;
                    UWORD uwEnemyGunY = s_tEnemy[enemyIdx].tPosition.uwY + ENEMY_GUN_OFFSET_Y;

                    // UBYTE ubAngle = getAngleBetweenPoints(uwEnemyGunX, uwEnemyGunY, uwPlayerCenterX, uwPlayerCenterY);
                    // s_tEnemyProjectiles[i].fX = fix16_from_int(uwEnemyGunX - ENEMY_PROJECTILE_OFFSET_X);
                    // s_tEnemyProjectiles[i].fY = fix16_from_int(uwEnemyGunY);
                    // s_tEnemyProjectiles[i].fDeltaX = ccos(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    // s_tEnemyProjectiles[i].fDeltaY = csin(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    // s_tEnemyProjectiles[i].fDeltaX = 0;
                    // s_tEnemyProjectiles[i].fDeltaY = fix16_from_int(ENEMY_PROJECTILE_SPEED);
                    // s_tEnemyProjectiles[i].ubAlive = 255;
                    // s_tEnemyProjectiles[i].ubChannel = 255;

                    s_tSimpleEnemyProjectiles[i].uwX = uwEnemyGunX;
                    s_tSimpleEnemyProjectiles[i].uwY = uwEnemyGunY;
                    s_tSimpleEnemyProjectiles[i].bDeltaX = 0;
                    s_tSimpleEnemyProjectiles[i].bDeltaY = 2;
                    s_tSimpleEnemyProjectiles[i].ubAlive = 255;
                    s_tSimpleEnemyProjectiles[i].ubChannel = 255;
                    s_tEnemy[enemyIdx].ubCooldownTimer = ENEMY_FIRE_DELAY;
                    break;
                }
            }
        }        
    }
}

// Utility functions

static void processPlayerDie() {
    s_tBigExplosionBob.sPos.uwX = s_tPlayerPosition.uwX - 16;
    s_tBigExplosionBob.sPos.uwY = s_tPlayerPosition.uwY - 16;
    s_ubBigExplosionActive = 1;
    s_ubPlayerAlive = FALSE;
    if (s_ubPlayerLives != 0) { s_ubPlayerLives--; }
    s_ubUpdateLives = TRUE;    
}

static void processPlayerShoot() {
    if (s_ubFireDelay == 0 && s_ubPlayerAlive == TRUE) {
        for (UBYTE i=0; i<PLAYER_PROJECTILE_MAX; i++) {
            if (s_tPlayerProjectiles[i].ubAlive == 0) {
                s_ubFireDelay = s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubDelay;

                s_tPlayerProjectiles[i].uwX = (s_tPlayerPosition.uwX + s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubXOffset);
                s_tPlayerProjectiles[i].uwY = (s_tPlayerPosition.uwY - s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubHeight);
                s_tPlayerProjectiles[i].ubAlive = 1;
                s_tPlayerProjectiles[i].ubType = s_ubEquippedProjectileType;

                if (s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubWideSprite == TRUE || s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubSpreadShot == TRUE) {
                    s_tPlayerSecondaryProjectiles[i].uwX = (s_tPlayerPosition.uwX + s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubXOffset2);
                    s_tPlayerSecondaryProjectiles[i].ubType = s_ubEquippedProjectileType;
                }

                // if (fireAudioDelay == 0) {
                //     //ptplayerSfxPlay(s_pSfxLaser, 2, 64, 16);
                //     audioMixerPlaySfx(s_pSfxLaser, CHANNEL_FOR_LASER, 8, 0);
                //     fireAudioDelay = FIRE_AUDIO_DELAY;
                // }
                break;
            }
        }
    }    
}

static void processPlayerProjectiles() {
    UWORD uwCameraYMin = s_pCamera->uPos.uwY;
    UWORD uwCameraYMax = s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT;
    UWORD uwCameraXMin = TILE_VIEWPORT_XMIN;
    UWORD uwCameraXMax = TILE_VIEWPORT_XMAX;

    for (UBYTE projectileIdx=0; projectileIdx<PLAYER_PROJECTILE_MAX; projectileIdx++) {
        if (s_tPlayerProjectiles[projectileIdx].ubAlive != 0) {
            UWORD uwX = s_tPlayerProjectiles[projectileIdx].uwX;
            UWORD uwX2 = 0;
            UWORD uwY = s_tPlayerProjectiles[projectileIdx].uwY;
            UBYTE ubType = s_tPlayerProjectiles[projectileIdx].ubType;

            BYTE bDeltaX = s_tPlayerProjectileTypes[ubType].bDeltaX;
            BYTE bDeltaX2 = s_tPlayerProjectileTypes[ubType].bDeltaX2;
            BYTE bDeltaY = s_tPlayerProjectileTypes[ubType].bDeltaY;

            UBYTE ubWidth = s_tPlayerProjectileTypes[ubType].ubWidth;
            UBYTE ubHeight = s_tPlayerProjectileTypes[ubType].ubHeight;
            UBYTE ubDamage = s_tPlayerProjectileTypes[ubType].ubDamage;
            UBYTE ubWideSprite = s_tPlayerProjectileTypes[ubType].ubWideSprite;
            UBYTE ubSpreadShot = s_tPlayerProjectileTypes[ubType].ubSpreadShot;
            UBYTE ubDieOnCollision = s_tPlayerProjectileTypes[ubType].ubDieOnCollision;
            UBYTE ubSecondarySpriteIndex = s_tPlayerProjectileTypes[ubType].ubSecondarySpriteIndex;

            UBYTE ubHasSecondary = FALSE;

            if (ubWideSprite || ubSpreadShot) {
                ubHasSecondary = TRUE;
                uwX2 = s_tPlayerSecondaryProjectiles[projectileIdx].uwX;
            }

            // Check if projectile if off screen.
            //if (uwY < (uwCameraYMin-ubHeight) || uwX < (uwCameraXMin-ubWidth) || uwX > uwCameraXMax) {
            if (uwY < (uwCameraYMin-ubHeight)) {
                s_tPlayerProjectiles[projectileIdx].ubAlive = 0;
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);

                if (ubHasSecondary) {
                    movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE); 
                }

                continue;
            }

            // Check collisions
            #ifndef COLLISIONS_DISABLED
            for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
                if (s_tEnemy[enemyIdx].bHealth <= 0) { continue; }

                // Check if enemy is on screen.
                if (s_tEnemy[enemyIdx].tPosition.uwX+ENEMY_WIDTH < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.uwX > uwCameraXMax) { continue; }
                if (s_tEnemy[enemyIdx].tPosition.uwY+ENEMY_HEIGHT < uwCameraYMin || s_tEnemy[enemyIdx].tPosition.uwY > uwCameraYMax) { continue; }

                // TODO: Rethink this approach, ends up basically doing collision detection twice. Unroll into individual checks? Include checks in checkCollision()?

                // Check if projectile is to far to the left of the enemy
                if (s_tEnemy[enemyIdx].tPosition.uwX+ENEMY_WIDTH < uwX) { continue; }

                // Check if projectile is to far to the right of the enemy
                if (s_tEnemy[enemyIdx].tPosition.uwX > uwX+ubWidth) { continue; }

                // Check if projectile is above enemy
                if (s_tEnemy[enemyIdx].tPosition.uwY > uwY+(ubHeight+bDeltaY)) { continue; }

                // Test collision
                UBYTE ubCollision = checkCollision(uwX, uwY+bDeltaY, ubWidth, ubHeight, s_tEnemy[enemyIdx].tPosition.uwX, s_tEnemy[enemyIdx].tPosition.uwY, ENEMY_WIDTH, ENEMY_HEIGHT);

                // Check spread shot collision
                // if (ubSpreadShot == TRUE) {
                //     UBYTE ubSpreadCollision = checkCollision(uwX, uwY+bDeltaY, ubWidth, ubHeight, s_tEnemy[enemyIdx].tPosition.wX, s_tEnemy[enemyIdx].tPosition.wY, ENEMY_WIDTH, ENEMY_HEIGHT);
                // }

                if (ubCollision == FALSE) { continue; }

                // Damage enemy
                s_tEnemy[enemyIdx].bHealth -= ubDamage;

                // Remove projectile?
                if (ubDieOnCollision == TRUE) {
                    s_tPlayerProjectiles[projectileIdx].ubAlive = 0;
                }

                // Enemy is dead.
                if (s_tEnemy[enemyIdx].bHealth <= 0) {
                    tUwCoordYX tExplosionPosition = (tUwCoordYX){.uwX = s_tEnemy[enemyIdx].tPosition.uwX-8, .uwY = s_tEnemy[enemyIdx].tPosition.uwY-8};
                    createExplosionAtPosition(tExplosionPosition);
                    s_tEnemy[enemyIdx].bHealth = 0;
                    s_ulPlayerScore += s_tEnemy[enemyIdx].uwScoreValue;
                    s_ubUpdateScore = TRUE;
                    s_ubActiveEnemies--;
                }
            }
            #endif

            if (s_tPlayerProjectiles[projectileIdx].ubAlive == 0) {
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);
                if (ubHasSecondary) { 
                    movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE); 
                }
                continue;
            }

            UWORD uwScreenY = (uwY - s_pCamera->uPos.uwY);
            UWORD uwScreenX = (uwX - TILE_VIEWPORT_XMIN) + 80;

            movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, uwScreenX, uwScreenY, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);

            // Secondary projectile
            if (ubHasSecondary) {
                movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, uwX2, uwScreenY, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE);
            }

            // Move projectile
            if (bDeltaX != 0) {
                s_tPlayerProjectiles[projectileIdx].uwX += bDeltaX;
                s_tPlayerSecondaryProjectiles[projectileIdx].uwX += bDeltaX2;
            }
            
            s_tPlayerProjectiles[projectileIdx].uwY += bDeltaY;
        }
    }
}

static void processEnemyProjectiles() {
    fix16_t fCameraYMin = fix16_from_int(s_pCamera->uPos.uwY);
    fix16_t fCameraYMax = fix16_from_int(s_pCamera->uPos.uwY+(TILE_VIEWPORT_HEIGHT-ENEMY_PROJECTILE_HEIGHT));
    fix16_t fCameraXMax = s_fViewportXMax;

    s_ubProjectileBobIndex = 0;
    s_fChannelBounds[0] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[1] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[2] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[3] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};

    for (UBYTE projectileIdx=0; projectileIdx<ENEMY_PROJECTILE_MAX; projectileIdx++) {
        if (s_tEnemyProjectiles[projectileIdx].ubAlive == 0) { continue; }

        s_tEnemyProjectiles[projectileIdx].fX = fix16_add(s_tEnemyProjectiles[projectileIdx].fX, s_tEnemyProjectiles[projectileIdx].fDeltaX);
        s_tEnemyProjectiles[projectileIdx].fY = fix16_add(s_tEnemyProjectiles[projectileIdx].fY, s_tEnemyProjectiles[projectileIdx].fDeltaY);
        s_tEnemyProjectiles[projectileIdx].ubAlive--;

        // Check X bounds
        if (s_tEnemyProjectiles[projectileIdx].fX < 0 || s_tEnemyProjectiles[projectileIdx].fX > fCameraXMax)
        {
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
        }

        // Check Y bounds
        if (s_tEnemyProjectiles[projectileIdx].fY < fCameraYMin || s_tEnemyProjectiles[projectileIdx].fY > fCameraYMax)
        {
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX+PLAYER_HITBOX_OFFSET_X, s_tPlayerPosition.uwY+PLAYER_HITBOX_OFFSET_Y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT, 
                                           fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX), fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY), ENEMY_PROJECTILE_WIDTH, ENEMY_PROJECTILE_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
            processPlayerDie();
        }
        #endif

        if (s_tEnemyProjectiles[projectileIdx].ubAlive == 0) {
            moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pCopBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, 0);
            s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
            continue;
        }

        s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
        fix16_t fSpriteYMin = s_tEnemyProjectiles[projectileIdx].fY;
        fix16_t fSpriteYMax = fix16_add(fSpriteYMin, s_fEnemyProjectileHeight);

        for (UBYTE channelIdx=0; channelIdx<ENEMY_SPRITE_CHANNELS; channelIdx++)
        {
            if(fSpriteYMax < s_fChannelBounds[channelIdx].fMin)
            {
                // logWrite("before %d: %ld < %ld", channelIdx, fSpriteYMax, fChannelY);
                s_tEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                s_fChannelBounds[channelIdx].fMin = fSpriteYMin;
                if(s_fChannelBounds[channelIdx].fMax == fix16_minimum) {
                    s_fChannelBounds[channelIdx].fMax = fSpriteYMax;
                }
                break;
            }
            else if (fSpriteYMin > s_fChannelBounds[channelIdx].fMax)
            {
                // logWrite("after %d: %ld > %ld", channelIdx, fSpriteY, fChannelYMax);
                s_tEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                if(s_fChannelBounds[channelIdx].fMin == fix16_maximum) {
                    s_fChannelBounds[channelIdx].fMin = fSpriteYMin;
                }
                s_fChannelBounds[channelIdx].fMax = fSpriteYMax;
                break;
            }
        }

        if (s_tEnemyProjectiles[projectileIdx].ubChannel == 255) {
            moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pCopBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, 0);

            if (s_ubProjectileBobIndex < ENEMY_SPRITE_CHANNELS) {
                s_tEnemyProjectileBob[s_ubProjectileBobIndex].sPos.uwX = fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX);
                s_tEnemyProjectileBob[s_ubProjectileBobIndex].sPos.uwY = fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY);
                s_ubProjectileBobIndex++;
                continue;
            } else {
                s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
                continue;
            }
        }

        moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pCopBlock, ((fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX) - TILE_VIEWPORT_XMIN)+80), (fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY) - s_pCamera->uPos.uwY), ENEMY_PROJECTILE_HEIGHT, s_tEnemyProjectiles[projectileIdx].ubChannel);
    }    
}

static void processSimpleEnemyProjectiles() {
    UWORD uwCameraYMin = s_pCamera->uPos.uwY;
    UWORD uwCameraYMax = s_pCamera->uPos.uwY+(TILE_VIEWPORT_HEIGHT-ENEMY_PROJECTILE_HEIGHT);
    UWORD uwCameraXMin = TILE_VIEWPORT_XMIN;
    UWORD uwCameraXMax = TILE_VIEWPORT_XMAX;
    s_ubProjectileBobIndex = 0;

    s_tSimpleChannelBounds[0] = (tSimpleChannelBounds){.uwMin = 65527, .uwMax = 8};
    s_tSimpleChannelBounds[1] = (tSimpleChannelBounds){.uwMin = 65527, .uwMax = 8};
    s_tSimpleChannelBounds[2] = (tSimpleChannelBounds){.uwMin = 65527, .uwMax = 8};
    s_tSimpleChannelBounds[3] = (tSimpleChannelBounds){.uwMin = 65527, .uwMax = 8};

    for (UBYTE projectileIdx=0; projectileIdx<ENEMY_PROJECTILE_MAX; projectileIdx++) {
        if (s_tSimpleEnemyProjectiles[projectileIdx].ubAlive == 0) { continue; }

        s_tSimpleEnemyProjectiles[projectileIdx].uwY += s_tSimpleEnemyProjectiles[projectileIdx].bDeltaY;
        s_tSimpleEnemyProjectiles[projectileIdx].ubAlive--;

        if (s_tSimpleEnemyProjectiles[projectileIdx].uwX < uwCameraXMin || s_tSimpleEnemyProjectiles[projectileIdx].uwX > uwCameraXMax) {
            s_tSimpleEnemyProjectiles[projectileIdx].ubAlive = 0;
        }

        if (s_tSimpleEnemyProjectiles[projectileIdx].uwY < uwCameraYMin || s_tSimpleEnemyProjectiles[projectileIdx].uwY > uwCameraYMax) {
            s_tSimpleEnemyProjectiles[projectileIdx].ubAlive = 0;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX+PLAYER_HITBOX_OFFSET_X, s_tPlayerPosition.uwY+PLAYER_HITBOX_OFFSET_Y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT, 
                                           s_tSimpleEnemyProjectiles[projectileIdx].uwX, s_tSimpleEnemyProjectiles[projectileIdx].uwY, ENEMY_PROJECTILE_WIDTH, ENEMY_PROJECTILE_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
            processPlayerDie();
        }
        #endif

        if (s_tSimpleEnemyProjectiles[projectileIdx].ubAlive == 0) {
            moveEnemyProjectile(s_tSimpleEnemyProjectiles[projectileIdx].pCopBlock, -16, -16, ENEMY_PROJECTILE_HEIGHT, 0);
            s_tSimpleEnemyProjectiles[projectileIdx].ubChannel = 255;
            continue;
        }

        s_tSimpleEnemyProjectiles[projectileIdx].ubChannel = 255;
        UWORD uwSpriteYMin = s_tSimpleEnemyProjectiles[projectileIdx].uwY;
        UWORD uwSpriteYMax = s_tSimpleEnemyProjectiles[projectileIdx].uwY + ENEMY_PROJECTILE_HEIGHT;

        for (UBYTE channelIdx=0; channelIdx<ENEMY_SPRITE_CHANNELS; channelIdx++)
        {
            if(uwSpriteYMax < s_tSimpleChannelBounds[channelIdx].uwMin)
            {
                s_tSimpleEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                s_tSimpleChannelBounds[channelIdx].uwMin = uwSpriteYMin;
                if(s_tSimpleChannelBounds[channelIdx].uwMax <= BOUNDS_MIN) {
                    s_tSimpleChannelBounds[channelIdx].uwMax = uwSpriteYMax;
                }
                break;
            }
            else if (uwSpriteYMin > s_tSimpleChannelBounds[channelIdx].uwMax)
            {
                s_tSimpleEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                if(s_tSimpleChannelBounds[channelIdx].uwMin >= BOUNDS_MAX) {
                    s_tSimpleChannelBounds[channelIdx].uwMin = uwSpriteYMin;
                }
                s_tSimpleChannelBounds[channelIdx].uwMax = uwSpriteYMax;
                break;
            }
        }

        if (s_tSimpleEnemyProjectiles[projectileIdx].ubChannel == 255) {
            moveEnemyProjectile(s_tSimpleEnemyProjectiles[projectileIdx].pCopBlock, -16, -16, ENEMY_PROJECTILE_HEIGHT, 0);

            if (s_ubProjectileBobIndex < ENEMY_SPRITE_CHANNELS) {
                s_tEnemyProjectileBob[s_ubProjectileBobIndex].sPos.uwX = s_tSimpleEnemyProjectiles[projectileIdx].uwX;
                s_tEnemyProjectileBob[s_ubProjectileBobIndex].sPos.uwY = s_tSimpleEnemyProjectiles[projectileIdx].uwY;
                s_ubProjectileBobIndex++;
                continue;
            } else {
                s_tSimpleEnemyProjectiles[projectileIdx].ubAlive = 0;
                continue;
            }
        }

        moveEnemyProjectile(s_tSimpleEnemyProjectiles[projectileIdx].pCopBlock, ((s_tSimpleEnemyProjectiles[projectileIdx].uwX-TILE_VIEWPORT_XMIN)+80), (s_tSimpleEnemyProjectiles[projectileIdx].uwY - s_pCamera->uPos.uwY), ENEMY_PROJECTILE_HEIGHT, s_tSimpleEnemyProjectiles[projectileIdx].ubChannel);       
    }
}

static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType, UBYTE ubSecondary) {
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
    ULONG ulAddr = 0;
    if (ubSecondary == FALSE) {
        ulAddr = (ULONG)(
            (UBYTE*)s_pPlayerProjectileSprites[ubType]->pBitmap->Planes[0] +
            sizeof(g_pCustom->spr[ubChannel].pos) +
            sizeof(g_pCustom->spr[ubChannel].ctl)
        ); // skip the control words
    } else {
        ulAddr = (ULONG)(
            (UBYTE*)s_pPlayerSecondaryProjectileSprites[ubType]->pBitmap->Planes[0] +
            sizeof(g_pCustom->spr[ubChannel].pos) +
            sizeof(g_pCustom->spr[ubChannel].ctl)
        );
    }

    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel].uwHi, ulAddr >> 16);
    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel].uwLo, ulAddr & 0xFFFF);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel].pos, uwPos);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel].ctl, uwCtl);
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
        sizeof(g_pCustom->spr[ubChannel].pos) +
        sizeof(g_pCustom->spr[ubChannel].ctl)
    ); // skip the control words

    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel].uwHi, ulAddr >> 16);
    copMove(s_pView->pCopList, pBlock, &g_pSprFetch[ubChannel].uwLo, ulAddr & 0xFFFF);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel].pos, uwPos);
    copMove(s_pView->pCopList, pBlock, &g_pCustom->spr[ubChannel].ctl, uwCtl);
}

static void createExplosionAtPosition(tUwCoordYX tPosition) {
    if (s_ubActiveExplosions < EXPLOSIONS_MAX) {
        for (UBYTE explosionIdx=0; explosionIdx < EXPLOSIONS_MAX; explosionIdx++) {
            if (s_tExplosions[explosionIdx].ubAlive != 0) { continue; }
            s_tExplosions[explosionIdx].sBob.sPos = tPosition;
            s_tExplosions[explosionIdx].ubAlive = 1;
            s_tExplosions[explosionIdx].ubFrameIndex = 0;
            s_ubActiveExplosions++;
            break;
        }
    }
}

static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2) {
    return x1 < x2+w2 && x1+w1 > x2 && y1 < y2+h2 && y1+h1 > y2;
}
