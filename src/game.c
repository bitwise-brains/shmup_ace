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
static tSimpleBufferManager *s_pHudBuffer;
static tCameraManager *s_pCamera;
static tRandManager s_tRandManager;
static UBYTE s_ubFrameCounter = 0;

// Font
static tFont *s_pHudFont;
static tTextBitMap *s_pHudText;

// HUD
// static UWORD s_uwPlayerScore = 0;
// static char s_cScoreText[10] = "";
// static UBYTE s_ubPlayerLives = 0;

// Level
static tBitMap *s_pTiles;
static UBYTE s_ubCameraCanMove = FALSE;
static UBYTE s_ubMoveCameraCounter = 0;

// HUD
static tBitMap *s_pHudLivesIconImage;
static tBitMap *s_pHudLivesIconMask;
static tBitMap *s_pHudLivesMarkImage;
static tBitMap *s_pHudLivesMarkMask;
static tBitMap *s_pHudSpecialIconImage;
static tBitMap *s_pHudSpecialIconMask;
static tBitMap *s_pHudSpecialMarkImage;
static tBitMap *s_pHudSpecialMarkMask;

// Player
static tBob s_tPlayerShipBob;
static tBitMap *s_pPlayerImage;
static tBitMap *s_pPlayerMask;
static tWCoordYX s_tPlayerPosition;
static ULONG s_ulPlayerScore = 0; 
static UBYTE s_ubPlayerLives = PLAYER_LIVES_MAX;
static UBYTE s_ubPlayerSpecial = PLAYER_SPECIAL_MAX;
static UBYTE s_ubPlayerShipAnimFrame = 0;
static UBYTE s_ubFireDelay = 0;
static UBYTE s_ubPlayerAlive = TRUE;
static UBYTE s_ubUpdateScore = TRUE;
static UBYTE s_ubUpdateLives = FALSE;
static UBYTE s_ubUpdateSpecial = FALSE;

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
static tPlayerProjectile s_tPlayerProjectiles[PLAYER_PROJECTILE_MAX];
static tSprite *s_pPlayerProjectileSprite;
static tBitMap *s_pPlayerProjectileImage;
static fix16_t s_fPlayerProjectileHeight;
static fix16_t s_fPlayerProjectileSpeed;
static fix16_t s_fPlayerProjectileOffsetX;

// Enemy Projectiles
static tSprite *s_pEnemyProjectileSprite[ENEMY_SPRITE_CHANNELS];
static tBob s_tEnemyProjectileBob[ENEMY_SPRITE_CHANNELS];
static tEnemyProjectile s_tEnemyProjectiles[ENEMY_PROJECTILE_MAX];
static tChannelBounds s_fChannelBounds[ENEMY_SPRITE_CHANNELS] = {0};
static fix16_t s_fEnemyProjectileHeight;
static UBYTE s_ubProjectileBobIndex = 0;

static tBitMap *s_pEnemyProjectileBobImage;
static tBitMap *s_pEnemyProjectileBobMask;
static tBitMap *s_pEnemyProjectileSpriteImage;

// Text
static tBob s_tTextGameOverBob;
static tBitMap *s_pTextGameOverImage;
static tBitMap *s_pTextGameOverMask;

// Constants
static fix16_t s_fViewportXMax;
static UWORD s_uwPlayerShipAnimOffset[] = {0, 0, 640, 640, 1280, 1280, 1920, 1920};
static UWORD s_uwExplosionAnimOffset[] = {0, 0, 640, 640, 1280, 1280, 1920, 1920, 2560, 2560, 3200, 3200, 3840, 3840, 4480, 4480};
static UWORD s_uwBigExplosionAnimOffset[] = {0, 0, 0, 1440, 1440, 1440, 2880, 2880, 2880, 4320, 4320, 4320, 5760, 5760, 5760, 7200, 7200, 7200, 8640, 8640, 8640, 10080, 10080, 10080, 11520, 11520, 11520, 12960, 12960, 12960, 14400, 14400, 14400, 15840, 15840, 15840};
static WORD s_wSpineX[] = {64, 64, 64, 64, 64, 64, 65, 65, 66, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 83, 84, 86, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 129, 131, 133, 135, 137, 139, 142, 144, 146, 148, 151, 153, 155, 157, 160, 162, 164, 166, 168, 171, 173, 175, 177, 180, 182, 184, 186, 188, 190, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 218, 220, 222, 224, 226, 227, 229, 230, 232, 233, 235, 236, 238, 239, 240, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 256, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 240, 239, 238, 236, 235, 233, 232, 230, 229, 227, 226, 224, 222, 220, 218, 217, 215, 213, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 190, 188, 186, 184, 182, 180, 177, 175, 173, 171, 168, 166, 164, 162, 160, 157, 155, 153, 151, 148, 146, 144, 142, 139, 137, 135, 133, 131, 129, 126, 124, 122, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 101, 99, 97, 95, 94, 92, 90, 89, 87, 86, 84, 83, 81, 80, 79, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 68, 67, 66, 66, 65, 65, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 65, 65, 66, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 79, 80, 81, 83, 84, 86, 87, 89, 90, 92, 94, 95, 97, 99, 101, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 129, 131, 133, 135, 137, 139, 142, 144, 146, 148, 151, 153, 155, 157, 160, 162, 164, 166, 168, 171, 173, 175, 177, 180, 182, 184, 186, 188, 190, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 218, 220, 222, 224, 226, 227, 229, 230, 232, 233, 235, 236, 238, 239, 240, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 256, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 240, 239, 238, 236, 235, 233, 232, 230, 229, 227, 226, 224, 222, 220, 218, 217, 215, 213, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 190, 188, 186, 184, 182, 180, 177, 175, 173, 171, 168, 166, 164, 162, 160, 157, 155, 153, 151, 148, 146, 144, 142, 139, 137, 135, 133, 131, 129, 126, 124, 122, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 101, 99, 97, 95, 94, 92, 90, 89, 87, 86, 84, 83, 81, 80, 79, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 68, 67, 66, 66, 65, 65, 64, 64, 64, 64, 64};
static WORD s_wSpineY[] = {64, 63, 63, 62, 62, 61, 61, 60, 60, 59, 59, 58, 58, 58, 57, 57, 57, 56, 56, 55, 55, 55, 54, 54, 54, 53, 53, 53, 53, 52, 52, 52, 52, 51, 51, 51, 51, 50, 50, 50, 50, 50, 49, 49, 49, 49, 49, 49, 49, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 49, 49, 49, 49, 49, 49, 49, 50, 50, 50, 50, 50, 51, 51, 51, 51, 52, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 55, 55, 55, 56, 56, 57, 57, 57, 58, 58, 58, 59, 59, 60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 65, 65, 66, 66, 67, 68, 68, 69, 70, 70, 71, 72, 73, 73, 74, 75, 76, 77, 78, 79, 80, 80, 81, 82, 83, 84, 85, 86, 87, 88, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 101, 102, 103, 104, 105, 107, 108, 109, 110, 111, 113, 114, 115, 116, 118, 119, 120, 121, 123, 124, 125, 126, 128, 129, 130, 131, 132, 134, 135, 136, 137, 139, 140, 141, 142, 144, 145, 146, 147, 148, 150, 151, 152, 153, 154, 156, 157, 158, 159, 160, 161, 162, 163, 164, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 185, 186, 187, 187, 188, 189, 189, 190, 190, 191, 192, 192, 192, 193, 193, 194, 194, 195, 195, 196, 196, 197, 197, 197, 198, 198, 199, 199, 199, 200, 200, 200, 201, 201, 201, 202, 202, 202, 202, 203, 203, 203, 204, 204, 204, 204, 204, 205, 205, 205, 205, 205, 206, 206, 206, 206, 206, 206, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 208, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 207, 206, 206, 206, 206, 206, 206, 205, 205, 205, 205, 205, 204, 204, 204, 204, 204, 203, 203, 203, 202, 202, 202, 202, 201, 201, 201, 200, 200, 200, 199, 199, 199, 198, 198, 197, 197, 197, 196, 196, 195, 195, 194, 194, 193, 193, 192, 192, 192, 191, 190, 190, 189, 189, 188, 187, 187, 186, 185, 185, 184, 183, 182, 182, 181, 180, 179, 178, 177, 176, 175, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 164, 163, 162, 161, 160, 159, 158, 157, 156, 154, 153, 152, 151, 150, 148, 147, 146, 145, 144, 142, 141, 140, 139, 137, 136, 135, 134, 132, 131, 130, 129, 128, 126, 125, 124, 123, 121, 120, 119, 118, 116, 115, 114, 113, 111, 110, 109, 108, 107, 105, 104, 103, 102, 101, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 88, 87, 86, 85, 84, 83, 82, 81, 80, 80, 79, 78, 77, 76, 75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 68, 67, 66, 66, 65, 65, 64};
static UBYTE s_ubMapData[] = {1, 1, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 17, 21, 25, 29, 1, 5, 9, 13, 1, 1,
                              1, 1, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 10, 14, 1, 1,
                              1, 1, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 11, 15, 1, 1,
                              1, 1, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 12, 16, 1, 1};

// Debug
static UBYTE s_ubEnemyCanFire;

void gameGsCreate(void) {
    // Init random.
    randInit(&s_tRandManager, 1337, 2600);

    // Setup screen and viewports.
    s_pView = viewCreate(0,
        TAG_VIEW_GLOBAL_PALETTE, 1,
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

    // Load tilemap.
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
    s_pHudBuffer = simpleBufferCreate(0,
        TAG_SIMPLEBUFFER_VPORT, s_pHudViewport,
        TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    TAG_DONE);

    // Populate tilemap.
    UWORD idx = 0;
    for (UWORD y=0; y<MAP_HEIGHT_IN_TILES; y++) {
        for (UWORD x=0; x<MAP_WIDTH_IN_TILES; x++) {
            if (idx == 96) { idx = 0; }
            s_pTileBuffer->pTileData[x][y] = s_ubMapData[idx] - 1;
            idx++;
        }
    }

    // Setup camera.
    s_pCamera = s_pTileBuffer->pCamera;
    s_fViewportXMax = fix16_from_int(320);

    // Load font
    s_pHudFont = fontCreate("data/hudfont.fnt");
    s_pHudText = fontCreateTextBitMap(HUD_SCORE_WIDTH, HUD_SCORE_HEIGHT);

    // Load HUD lives icon
    s_pHudLivesIconImage = bitmapCreateFromFile("data/hud_lives_icon.bm", 0);
    s_pHudLivesIconMask = bitmapCreateFromFile("data/hud_lives_icon_mask.bm", 0);

    // Load HUD lives mark
    s_pHudLivesMarkImage = bitmapCreateFromFile("data/hud_lives_mark.bm", 0);
    s_pHudLivesMarkMask = bitmapCreateFromFile("data/hud_lives_mark_mask.bm", 0);

    // Load HUD special icon
    s_pHudSpecialIconImage = bitmapCreateFromFile("data/hud_special_icon.bm", 0);
    s_pHudSpecialIconMask = bitmapCreateFromFile("data/hud_special_icon_mask.bm", 0);

    // Load HUD special mark
    s_pHudSpecialMarkImage = bitmapCreateFromFile("data/hud_special_mark.bm", 0);
    s_pHudSpecialMarkMask = bitmapCreateFromFile("data/hud_special_mark_mask.bm", 0);

    // Init tilemap bob manager.
    bobManagerCreate(s_pTileBuffer->pScroll->pFront, s_pTileBuffer->pScroll->pBack, s_pTileBuffer->pScroll->uBfrBounds.uwY);

    // Player ship bob.
    s_pPlayerImage = bitmapCreateFromFile("data/playership.bm", 0);
    s_pPlayerMask = bitmapCreateFromFile("data/playership_mask.bm", 0);
    bobInit(&s_tPlayerShipBob, 32, 32, 1, s_pPlayerImage->Planes[0], s_pPlayerMask->Planes[0], 0, 0);

    // Enemy bobs
    s_pEnemyImage = bitmapCreateFromFile("data/enemy.bm", 0);
    s_pEnemyMask = bitmapCreateFromFile("data/enemy_mask.bm", 0);

    // Init enemy structs.
    for (UBYTE i=0; i<ENEMY_MAX; i++) {
        bobInit( &s_tEnemy[i].sBob, ENEMY_WIDTH, ENEMY_HEIGHT, 1, s_pEnemyImage->Planes[0], s_pEnemyMask->Planes[0], 0, 0 );
        s_tEnemy[i].tPosition.wX = 0;
        s_tEnemy[i].tPosition.wY = 0;
        s_tEnemy[i].bHealth = 0;
        s_tEnemy[i].ubCooldownTimer = 0;
        s_tEnemy[i].ubSpeed = 2;        
        s_tEnemy[i].uwPositionOffsetIdx = 0;
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
    s_pEnemyProjectileBobImage = bitmapCreateFromFile("data/bullet_bob_enemy.bm", 0);
    s_pEnemyProjectileBobMask = bitmapCreateFromFile("data/bullet_bob_enemy_mask.bm", 0);

    for (UBYTE i=0; i<ENEMY_BOB_CHANNELS; i++) {
        bobInit(&s_tEnemyProjectileBob[i], 16, 6, 1, s_pEnemyProjectileBobImage->Planes[0], s_pEnemyProjectileBobMask->Planes[0], 0, 0);
    }

    s_pTextGameOverImage = bitmapCreateFromFile("data/text_gameover.bm", 0);
    s_pTextGameOverMask = bitmapCreateFromFile("data/text_gameover_mask.bm", 0);
    bobInit(&s_tTextGameOverBob, 112, 60, 0, s_pTextGameOverImage->Planes[0], s_pTextGameOverMask->Planes[0], 0, 0);

    // Finish tilebuffer bob init.
    bobReallocateBgBuffers();
    
    // Init sprites.
    spriteManagerCreate(s_pView, 0);
    systemSetDmaBit(DMAB_SPRITE, 1);

    // Enemy projectile sprites.
    s_fEnemyProjectileHeight = fix16_from_int(ENEMY_PROJECTILE_HEIGHT+1);    
    s_pEnemyProjectileSpriteImage = bitmapCreateFromFile("data/bullet_sprite_enemy.bm", 0);
    s_pEnemyProjectileSprite[0] = spriteAdd(0, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[1] = spriteAdd(1, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[2] = spriteAdd(2, s_pEnemyProjectileSpriteImage);
    s_pEnemyProjectileSprite[3] = spriteAdd(3, s_pEnemyProjectileSpriteImage);

    s_fChannelBounds[0] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[1] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[2] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};
    s_fChannelBounds[3] = (tChannelBounds){.fMin = fix16_maximum, .fMax = fix16_minimum};

    for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
        s_tEnemyProjectiles[i].pProjectileBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
        s_tEnemyProjectiles[i].fX = 0;
        s_tEnemyProjectiles[i].fY = 0;
        s_tEnemyProjectiles[i].fDeltaX = 0;
        s_tEnemyProjectiles[i].fDeltaY = 0;
        s_tEnemyProjectiles[i].ubAlive = 0;
        s_tEnemyProjectiles[i].ubChannel = 255;
    }

    // Player projectile sprite.
    s_fPlayerProjectileHeight = fix16_from_int(PLAYER_PROJECTILE_HEIGHT+1);
    s_fPlayerProjectileSpeed = fix16_from_int(PLAYER_PROJECTILE_SPEED);
    s_fPlayerProjectileOffsetX = fix16_from_int(PLAYER_PROJECTILE_OFFSET_X);
    s_pPlayerProjectileImage = bitmapCreateFromFile("data/bullet_sprite_player.bm", 0);
    s_pPlayerProjectileSprite = spriteAdd(4, s_pPlayerProjectileImage);

    for (UBYTE i=0; i<PLAYER_PROJECTILE_MAX; i++) {
        s_tPlayerProjectiles[i].pProjectileBlock = copBlockCreate(s_pView->pCopList, 4, 0, 0);
        s_tPlayerProjectiles[i].fX = 0;
        s_tPlayerProjectiles[i].fY = 0;
        s_tPlayerProjectiles[i].fDeltaX = 0;
        s_tPlayerProjectiles[i].fDeltaY = 0;
        s_tPlayerProjectiles[i].ubDamage = 10;
        s_tPlayerProjectiles[i].ubAlive = 0;
        s_tPlayerProjectiles[i].ubChannel = 0;
    }

    // Draw lives on HUD
    blitCopyMask(s_pHudLivesIconImage, 0, 0, s_pHudBuffer->pBack, 6, 3, 7, 7, s_pHudLivesIconMask->Planes[0]);
    UBYTE ubLivesOffset = 16;
    for (UBYTE i=0; i<s_ubPlayerLives; i++) {
        blitCopyMask(s_pHudLivesMarkImage, 0, 0, s_pHudBuffer->pBack, ubLivesOffset, 3, 7, 7, s_pHudLivesMarkMask->Planes[0]);
        ubLivesOffset += 10;
    }

    // Draw special on HUD
    blitCopyMask(s_pHudSpecialIconImage, 0, 0, s_pHudBuffer->pBack, 72, 2, 7, 9, s_pHudSpecialIconMask->Planes[0]);
    UBYTE ubSpecialOffset = 84;
    for (UBYTE i=0; i<s_ubPlayerSpecial; i++) {
        blitCopyMask(s_pHudSpecialMarkImage, 0, 0, s_pHudBuffer->pBack, ubSpecialOffset, 3, 7, 7, s_pHudSpecialMarkMask->Planes[0]);
        ubSpecialOffset += 10;
    }

    // Set player position
    s_tPlayerPosition.wX = 176; s_tPlayerPosition.wY = 128;

    //cameraMoveBy(s_pCamera, 32, ((MAP_HEIGHT_IN_TILES*16)-256));
    cameraMoveBy(s_pCamera, 32, 0);

    // Finish up
    gameMathInit();
    viewLoad(s_pView);
    tileBufferRedrawAll(s_pTileBuffer);
    memLogPeak();
    systemUnuse();    
}

void gameGsLoop(void) {
    //g_pCustom->color[0] = 0xF00;
    processCamera();    
    processInput();
    processPlayerProjectiles();
    processEnemies();
    processEnemyProjectiles();
    //tileBufferQueueProcess(s_pTileBuffer);
    processBobs();
    processTimers();
    processHud();
    
    tileBufferQueueProcess(s_pTileBuffer);
    viewProcessManagers(s_pView);
    copProcessBlocks();

    s_ubFrameCounter = !s_ubFrameCounter;
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

static void processCamera() {
    if (s_ubCameraCanMove == TRUE) {
        if (s_ubMoveCameraCounter == CAMERA_MOVE_RATE) { 
            if (s_pCamera->uPos.uwY > 0) { cameraMoveBy(s_pCamera, 0, CAMERA_MOVE_AMOUNT); }
            s_ubMoveCameraCounter = 0;
        }
        s_ubMoveCameraCounter++;
    }    
}

static void processInput() {
    // Quit
    if(keyCheck(KEY_ESCAPE)) {
        gameExit();
    }

    // Move player
    if(keyCheck(KEY_W)) {
        s_tPlayerPosition.wY = s_tPlayerPosition.wY - PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.wY < s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN) { s_tPlayerPosition.wY = s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN; }
    }
    if(keyCheck(KEY_S)) {
        s_tPlayerPosition.wY = s_tPlayerPosition.wY + PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.wY > s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT)) { s_tPlayerPosition.wY = s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT); }
    }
    if(keyCheck(KEY_A)) {
        s_tPlayerPosition.wX = s_tPlayerPosition.wX - PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.wX < TILE_VIEWPORT_EDGE_MIN) { s_tPlayerPosition.wX = TILE_VIEWPORT_EDGE_MIN; }
    }
    if(keyCheck(KEY_D)) {
        s_tPlayerPosition.wX = s_tPlayerPosition.wX + PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.wX > (TILE_VIEWPORT_EDGE_MAX-PLAYER_SHIP_WIDTH)) { s_tPlayerPosition.wX = (TILE_VIEWPORT_EDGE_MAX-PLAYER_SHIP_WIDTH); }
    }

    // Shoot
    if (keyCheck(KEY_SPACE)) {
        if (s_ubFireDelay == 0 && s_ubPlayerAlive == TRUE) {
            for (UBYTE i=0; i<PLAYER_PROJECTILE_MAX; i++) {
                if (s_tPlayerProjectiles[i].ubAlive == 0) {
                    s_tPlayerProjectiles[i].fX = fix16_add(fix16_from_int(s_tPlayerPosition.wX), s_fPlayerProjectileOffsetX);
                    s_tPlayerProjectiles[i].fY = fix16_from_int(s_tPlayerPosition.wY);
                    s_tPlayerProjectiles[i].fDeltaX = 0;
                    s_tPlayerProjectiles[i].fDeltaY = fix16_mul(s_fPlayerProjectileSpeed, fix16_from_int(-1));
                    s_tPlayerProjectiles[i].ubAlive = 1;
                    s_ubFireDelay = PLAYER_FIRE_DELAY;
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

    // Debug stuff.

    if (keyCheck(KEY_UP)) { cameraMoveBy(s_pCamera, 0, -1); logWrite("CameraY: %d", s_pCamera->uPos.uwY); }
    if (keyCheck(KEY_DOWN)) { cameraMoveBy(s_pCamera, 0, 1); logWrite("CameraY: %d", s_pCamera->uPos.uwY); }
    // if (keyCheck(KEY_LEFT)) { cameraMoveBy(s_pCamera, -1, 0); }
    // if (keyCheck(KEY_RIGHT)) { cameraMoveBy(s_pCamera, 1, 0); }

    if (keyCheck(KEY_1)) {
        s_ubEnemyCanFire = 0;
    }

    if (keyCheck(KEY_2)) {
        for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
            s_tEnemy[enemyIdx].ubCooldownTimer = randUwMinMax(&s_tRandManager, 0, 255);
        }

        s_ubEnemyCanFire = 1;
    }

    if (keyCheck(KEY_3)) {
        if (s_ubFireDelay == 0) {
            if (s_ubPlayerLives < PLAYER_LIVES_MAX) { s_ubPlayerLives++; }
            s_ubUpdateLives = TRUE;
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }

    if (keyCheck(KEY_4)) {
        if (s_ubFireDelay == 0) {
            if (s_ubPlayerSpecial < PLAYER_SPECIAL_MAX) { s_ubPlayerSpecial++; }
            s_ubUpdateSpecial = TRUE;
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }            

    if (keyCheck(KEY_5)) {
        if (s_ubFireDelay == 0) {
            if (s_ubActiveEnemies < ENEMY_MAX) {
                for (UBYTE i=0; i<ENEMY_MAX; i++) {
                    if (s_tEnemy[i].bHealth > 0) { continue; }
                    s_tEnemy[i].tPosition.wX = s_wSpineX[0];
                    s_tEnemy[i].tPosition.wY = s_wSpineY[0];
                    s_tEnemy[i].bHealth = 20;
                    s_tEnemy[i].ubCooldownTimer = 0;
                    s_tEnemy[i].ubSpeed = 2;        
                    s_tEnemy[i].uwPositionOffsetIdx = 0;
                    s_tEnemy[i].uwScoreValue = ENEMY_SCORE_VALUE;
                    s_ubActiveEnemies++;
                    break;
                }
            }
            s_ubFireDelay = PLAYER_FIRE_DELAY;
        }
    }

    if (keyCheck(KEY_6)) {
        s_ubCameraCanMove = TRUE;
    }

    if (keyCheck(KEY_O)) {
        if (s_ubPlayerAlive == 0 && s_ubPlayerLives > 0) {
            s_tPlayerPosition.wX = 144; s_tPlayerPosition.wY = 128;
            s_ubPlayerAlive = 1;
        }
    }

    if (keyCheck(KEY_P)) {
        if (s_ubFireDelay == 0) {
            for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
                if (s_tEnemyProjectiles[i].ubAlive == 0) {
                    UWORD uwPlayerCenterX = s_tPlayerPosition.wX + PLAYER_CENTER_OFFSET_X;
                    UWORD uwPlayerCenterY = s_tPlayerPosition.wY + PLAYER_CENTER_OFFSET_Y;
                    UBYTE ubAngle = getAngleBetweenPoints(160, 128, uwPlayerCenterX, uwPlayerCenterY);
                    s_tEnemyProjectiles[i].fX = fix16_from_int(160);
                    s_tEnemyProjectiles[i].fY = fix16_from_int(128);
                    s_tEnemyProjectiles[i].fDeltaX = ccos(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    s_tEnemyProjectiles[i].fDeltaY = csin(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    s_tEnemyProjectiles[i].ubAlive = 255;
                    s_tEnemyProjectiles[i].ubChannel = 255;
                    s_ubFireDelay = ENEMY_FIRE_DELAY;
                    break;
                }
            }
        }        
    }  
}

static void processHud() {
    // Score
    if (s_ubUpdateScore == TRUE) {
        char cPlayerScore[11];
        sprintf(cPlayerScore, "%010ld", s_ulPlayerScore);
        fontFillTextBitMap(s_pHudFont, s_pHudText, cPlayerScore);
        fontDrawTextBitMap(s_pHudBuffer->pBack, s_pHudText, 0, 4, 19, FONT_RIGHT);
        s_ubUpdateScore = FALSE;
    }

    // Lives
    if (s_ubUpdateLives == TRUE) {
        // Clear area
        blitRect(s_pHudBuffer->pBack, 16, 0, 48, 12, 0);

        // Draw lives
        UBYTE ubLivesOffset = 16;
        for (UBYTE i=0; i<s_ubPlayerLives; i++) {
            blitCopyMask(s_pHudLivesMarkImage, 0, 0, s_pHudBuffer->pBack, ubLivesOffset, 3, 7, 7, s_pHudLivesMarkMask->Planes[0]);
            ubLivesOffset += 10;
        }             
    }

    // Special
    if (s_ubUpdateSpecial == TRUE) {
        // Clear area
        blitRect(s_pHudBuffer->pBack, 84, 0, 48, 12, 0);

        // Draw lives
        UBYTE ubSpecialOffset = 84;
        for (UBYTE i=0; i<s_ubPlayerSpecial; i++) {
            blitCopyMask(s_pHudSpecialMarkImage, 0, 0, s_pHudBuffer->pBack, ubSpecialOffset, 3, 7, 7, s_pHudSpecialMarkMask->Planes[0]);
            ubSpecialOffset += 10;
        }   
    }
}

static void processBobs() {
    bobBegin(s_pTileBuffer->pScroll->pBack);

    // Player ship
    if (s_ubPlayerAlive == 1) {
        s_tPlayerShipBob.sPos.uwX = s_tPlayerPosition.wX;
        s_tPlayerShipBob.sPos.uwY = s_tPlayerPosition.wY;
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
    UWORD uwCameraXMin = TILE_VIEWPORT_EDGE_MIN;
    UWORD uwCameraXMax = TILE_VIEWPORT_EDGE_MAX;

    for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
        if (s_tEnemy[enemyIdx].bHealth == 0) { continue; }
        
        // Move
        s_tEnemy[enemyIdx].uwPositionOffsetIdx++;
        if (s_tEnemy[enemyIdx].uwPositionOffsetIdx >= ENEMY_PATH_LENGTH) {
            s_tEnemy[enemyIdx].uwPositionOffsetIdx = 0;
        }

        s_tEnemy[enemyIdx].tPosition.wX = s_wSpineX[s_tEnemy[enemyIdx].uwPositionOffsetIdx];
        s_tEnemy[enemyIdx].tPosition.wY = s_wSpineY[s_tEnemy[enemyIdx].uwPositionOffsetIdx];
        s_tEnemy[enemyIdx].sBob.sPos.uwX = s_tEnemy[enemyIdx].tPosition.wX;
        s_tEnemy[enemyIdx].sBob.sPos.uwY = s_tEnemy[enemyIdx].tPosition.wY;

        // Check X bounds
        if (s_tEnemy[enemyIdx].tPosition.wX < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.wX > uwCameraXMax)
        {
            s_tEnemy[enemyIdx].bHealth = 0;
            s_tEnemy[enemyIdx].tPosition.wX = 0;
            s_tEnemy[enemyIdx].tPosition.wY = 0;
            continue;
        }

        // Check Y bounds
        if (s_tEnemy[enemyIdx].tPosition.wY < uwCameraYMin || s_tEnemy[enemyIdx].tPosition.wY > uwCameraYMax)
        {
            s_tEnemy[enemyIdx].bHealth = 0;
            s_tEnemy[enemyIdx].tPosition.wX = 0;
            s_tEnemy[enemyIdx].tPosition.wY = 0;
            continue;
        }

        // Shoot
        if (s_ubEnemyCanFire == 0) { continue; }

        if (s_tEnemy[enemyIdx].ubCooldownTimer == 0) {
            for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
                if (s_tEnemyProjectiles[i].ubAlive == 0) {
                    UWORD uwPlayerCenterX = s_tPlayerPosition.wX + PLAYER_CENTER_OFFSET_X;
                    UWORD uwPlayerCenterY = s_tPlayerPosition.wY + PLAYER_CENTER_OFFSET_Y;
                    UWORD uwEnemyGunX = s_tEnemy[enemyIdx].tPosition.wX + ENEMY_GUN_OFFSET_X;
                    UWORD uwEnemyGunY = s_tEnemy[enemyIdx].tPosition.wY + ENEMY_GUN_OFFSET_Y;

                    UBYTE ubAngle = getAngleBetweenPoints(uwEnemyGunX, uwEnemyGunY, uwPlayerCenterX, uwPlayerCenterY);
                    s_tEnemyProjectiles[i].fX = fix16_from_int(uwEnemyGunX - ENEMY_PROJECTILE_OFFSET_X);
                    s_tEnemyProjectiles[i].fY = fix16_from_int(uwEnemyGunY);
                    s_tEnemyProjectiles[i].fDeltaX = ccos(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    s_tEnemyProjectiles[i].fDeltaY = csin(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    s_tEnemyProjectiles[i].ubAlive = 255;
                    s_tEnemyProjectiles[i].ubChannel = 255;
                    s_tEnemy[enemyIdx].ubCooldownTimer = ENEMY_FIRE_DELAY;
                    break;
                }
            }
        }        
    }
}

static void processPlayerProjectiles() {
    UWORD uwCameraYMin = s_pCamera->uPos.uwY;
    UWORD uwCameraYMax = s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT;
    UWORD uwCameraXMin = TILE_VIEWPORT_EDGE_MIN;
    UWORD uwCameraXMax = TILE_VIEWPORT_EDGE_MAX;

    for (UBYTE projectileIdx=0; projectileIdx<PLAYER_PROJECTILE_MAX; projectileIdx++) {
        if (s_tPlayerProjectiles[projectileIdx].ubAlive != 0) {
            WORD wX = fix16_to_int(s_tPlayerProjectiles[projectileIdx].fX);
            WORD wY = fix16_to_int(s_tPlayerProjectiles[projectileIdx].fY);
            //logWrite("[%d] (%d, %d)", projectileIdx, wX, wY);

            // Check if projectile if off screen before processing collisions.
            if (wY < (uwCameraYMin-PLAYER_PROJECTILE_HEIGHT)) {
                s_tPlayerProjectiles[projectileIdx].ubAlive = 0;
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pProjectileBlock, 0, -16, PLAYER_PROJECTILE_HEIGHT, 4);
                continue;
            }

            // Check collisions
            #ifndef COLLISIONS_DISABLED
            for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
                if (s_tEnemy[enemyIdx].bHealth <= 0) { continue; }

                // Check if enemy is on screen.
                if (s_tEnemy[enemyIdx].tPosition.wX+ENEMY_WIDTH < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.wX > uwCameraXMax) { continue; }
                if (s_tEnemy[enemyIdx].tPosition.wY+ENEMY_HEIGHT < uwCameraYMin || s_tEnemy[enemyIdx].tPosition.wY > uwCameraYMax) { continue; }
               
                // Check if projectile is past enemy
                if (s_tEnemy[enemyIdx].tPosition.wY > wY+(PLAYER_PROJECTILE_HEIGHT+PLAYER_PROJECTILE_SPEED)) { 
                    continue;
                }

                // Test collision
                UBYTE ubCollision = checkCollision(wX, wY+PLAYER_PROJECTILE_SPEED, PLAYER_PROJECTILE_WIDTH, PLAYER_PROJECTILE_HEIGHT, s_tEnemy[enemyIdx].tPosition.wX, s_tEnemy[enemyIdx].tPosition.wY, ENEMY_WIDTH, ENEMY_HEIGHT);

                if (ubCollision == FALSE) { continue; }

                // Damage enemy
                s_tPlayerProjectiles[projectileIdx].ubAlive = 0;
                s_tEnemy[enemyIdx].bHealth -= s_tPlayerProjectiles[projectileIdx].ubDamage;

                // Enemy is dead.
                if (s_tEnemy[enemyIdx].bHealth <= 0) {
                    tUwCoordYX tExplosionPosition = (tUwCoordYX){.uwX = s_tEnemy[enemyIdx].tPosition.wX, .uwY = s_tEnemy[enemyIdx].tPosition.wY};
                    createExplosionAtPosition(tExplosionPosition);
                    s_tEnemy[enemyIdx].bHealth = 0;
                    s_ulPlayerScore += s_tEnemy[enemyIdx].uwScoreValue;
                    s_ubUpdateScore = TRUE;
                    s_ubActiveEnemies--;
                }
            }
            #endif

            if (s_tPlayerProjectiles[projectileIdx].ubAlive == 0) {
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pProjectileBlock, 0, -16, PLAYER_PROJECTILE_HEIGHT, 4);
                continue;
            }

            movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pProjectileBlock, (wX - s_pCamera->uPos.uwX), (wY - s_pCamera->uPos.uwY), PLAYER_PROJECTILE_HEIGHT, 4);

            // Move projectile
            if (s_tPlayerProjectiles[projectileIdx].fDeltaX != 0) {
                s_tPlayerProjectiles[projectileIdx].fX = fix16_add(s_tPlayerProjectiles[projectileIdx].fX, s_tPlayerProjectiles[projectileIdx].fDeltaX);
            }
            
            s_tPlayerProjectiles[projectileIdx].fY = fix16_add(s_tPlayerProjectiles[projectileIdx].fY, s_tPlayerProjectiles[projectileIdx].fDeltaY);
        }
    }
}

static void processEnemyProjectiles() {
    fix16_t fCameraY = fix16_from_int(s_pCamera->uPos.uwY);
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
        if (s_tEnemyProjectiles[projectileIdx].fY < fCameraY || s_tEnemyProjectiles[projectileIdx].fY > fCameraYMax)
        {
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.wX+PLAYER_HITBOX_OFFSET_X, s_tPlayerPosition.wY+PLAYER_HITBOX_OFFSET_Y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT, 
                                           fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX), fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY), ENEMY_PROJECTILE_WIDTH, ENEMY_PROJECTILE_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            s_tEnemyProjectiles[projectileIdx].ubAlive = 0;
            s_tBigExplosionBob.sPos.uwX = s_tPlayerPosition.wX - 8; // Visual glitch when player is near viewport sides, will get fixed when there's an offscreen buffer on X.
            s_tBigExplosionBob.sPos.uwY = s_tPlayerPosition.wY - 8;
            s_ubBigExplosionActive = 1;
            s_ubPlayerAlive = FALSE;
            if (s_ubPlayerLives != 0) { s_ubPlayerLives--; }
            s_ubUpdateLives = TRUE;
        }
        #endif

        if (s_tEnemyProjectiles[projectileIdx].ubAlive == 0) {
            moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, 0);
            s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
            continue;
        }

        s_tEnemyProjectiles[projectileIdx].ubChannel = 255;
        fix16_t fSpriteY = s_tEnemyProjectiles[projectileIdx].fY;
        fix16_t fSpriteYMax = fix16_add(fSpriteY, s_fEnemyProjectileHeight);

        for (UBYTE channelIdx=0; channelIdx<ENEMY_SPRITE_CHANNELS; channelIdx++)
        {
            if(fSpriteYMax < s_fChannelBounds[channelIdx].fMin)
            {
                // logWrite("before %d: %ld < %ld", channelIdx, fSpriteYMax, fChannelY);
                s_tEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                s_fChannelBounds[channelIdx].fMin = fSpriteY;
                if(s_fChannelBounds[channelIdx].fMax == fix16_minimum) {
                    s_fChannelBounds[channelIdx].fMax = fSpriteYMax;
                }
                break;
            }
            else if (fSpriteY > s_fChannelBounds[channelIdx].fMax)
            {
                // logWrite("after %d: %ld > %ld", channelIdx, fSpriteY, fChannelYMax);
                s_tEnemyProjectiles[projectileIdx].ubChannel = channelIdx;
                if(s_fChannelBounds[channelIdx].fMin == fix16_maximum) {
                    s_fChannelBounds[channelIdx].fMin = fSpriteY;
                }
                s_fChannelBounds[channelIdx].fMax = fSpriteYMax;
                break;
            }
        }

        if (s_tEnemyProjectiles[projectileIdx].ubChannel == 255) {
            moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, 0, -16, ENEMY_PROJECTILE_HEIGHT, 0);

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

        moveEnemyProjectile(s_tEnemyProjectiles[projectileIdx].pProjectileBlock, fix16_to_int(s_tEnemyProjectiles[projectileIdx].fX), (fix16_to_int(s_tEnemyProjectiles[projectileIdx].fY) - s_pCamera->uPos.uwY), ENEMY_PROJECTILE_HEIGHT, s_tEnemyProjectiles[projectileIdx].ubChannel);
    }    
}

static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel) {
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
        (UBYTE*)s_pPlayerProjectileSprite->pBitmap->Planes[0] +
        sizeof(g_pCustom->spr[ubChannel].pos) +
        sizeof(g_pCustom->spr[ubChannel].ctl)
    ); // skip the control words

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
