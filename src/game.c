#include "game.h"
#include "game_math.h"
#include "projectile.h"
#include "player.h"
#include "enemy.h"
#include "explosion.h"

// System
static tRandManager s_tRandManager;

// Display
static tView *s_pView; // View containing all the viewports
static tVPort *s_pGameViewport; // Viewport for playfield
static tVPort *s_pHudViewport; // Viewport for HUD
static tTileBufferManager *s_pTileBuffer;
static tSimpleBufferManager *s_pHudBuffer;
static tCameraManager *s_pCamera;
static UBYTE s_ubFrameCounter = 0;
static UBYTE s_ubFlashTimer = 0;
static UBYTE s_ubFlashOn = FALSE;
static UBYTE s_ubLevelStart = TRUE;
static UBYTE s_ubLevelStartTimer = 128;

// Audio
static tPtplayerMod *s_pGameMusic;
static tPtplayerSfx *s_pSfxLaser;
static tPtplayerSfx *s_pSfxExplosion;

// Level
static tBitMap *s_pTiles;
static UBYTE s_ubCameraCanMove = FALSE;
static UBYTE s_ubMoveCameraCounter = 0;
static UBYTE s_ubCurrentLevel = 0;
static UBYTE s_ubWavesInLevel[NUMBER_OF_LEVELS] = {2};

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
static UBYTE s_ubPlayerLives = 3;
static UBYTE s_ubPlayerSpecial = 2;
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
static UBYTE s_ubAudioDelay = 0;
static UBYTE s_ubEquippedProjectileType = 0;
static UBYTE s_ubPlayerAlive = FALSE;
static UBYTE s_ubPlayerMovedOnY = FALSE;
static UBYTE s_ubDisplayPlayer = TRUE;
static UBYTE s_ubPlayerIsInvincible = FALSE;
static UBYTE s_ubPlayerInvincibleTimer = 0;

// Enemies
static tEnemy s_tEnemy[ENEMY_MAX] = {0};
static tBitMap *s_pEnemyImage;
static tBitMap *s_pEnemyMask;
static UBYTE s_ubWaveIndex = 0;
static UBYTE s_ubActiveEnemies = 0;
static tEnemyType s_tEnemyTypes[ENEMY_TYPES];
static tEnemyWave s_tEnemyWaves[ENEMY_WAVES];

// Explosions
static tExplosion s_tExplosions[EXPLOSIONS_MAX] = {0};
static UBYTE s_ubActiveExplosions = 0;
static tBitMap *s_pExplosionImage;
static tBitMap *s_pExplosionMask;

// Big Explosion
static tBob s_tBigExplosionBob;
static UBYTE s_ubBigExplosionActive = 0;
static UBYTE s_ubBigExplosionFrame = 0;
static tBitMap *s_pBigExplosionImage;
static tBitMap *s_pBigExplosionMask;

// Powerups
static tBitMap *s_pLifePowImage;
static tBitMap *s_pLifePowMask;
static tBob s_tLifePowBob;
static UBYTE s_ubLifePowActive = FALSE;

static tBitMap *s_pSpecialPowImage;
static tBitMap *s_pSpecialPowMask;
static tBob s_tSpecialPowBob;
static UBYTE s_ubSpecialPowActive = FALSE;

static tBitMap *s_pWeaponPowImage;
static tBitMap *s_pWeaponPowMask;
static tBob s_tWeaponPowBob;
static UBYTE s_ubWeaponPowActive = FALSE;

// Player Projectiles
static tPlayerProjectileType s_tPlayerProjectileTypes[PLAYER_PROJECTILE_TYPES] = {0};
static tPlayerProjectile s_tPlayerProjectiles[PLAYER_PROJECTILE_MAX] = {0};
static tSprite *s_pPlayerProjectileSprites[PLAYER_PROJECTILE_TYPES] = {0};
static tBitMap *s_pPlayerProjectileImages[PLAYER_PROJECTILE_TYPES] = {0};
// static tPlayerSecondaryProjectile s_tPlayerSecondaryProjectiles[PLAYER_PROJECTILE_MAX] = {0};
// static tSprite *s_pPlayerSecondaryProjectileSprites[PLAYER_SECONDARY_PROJECTILE_TYPES];
// static tBitMap *s_pPlayerSecondaryProjectileImages[PLAYER_SECONDARY_PROJECTILE_TYPES] = {0};

// Enemy Projectiles
static tSprite *s_pEnemyProjectileSprite[ENEMY_SPRITE_CHANNELS] = {0};
static tBob s_tEnemyProjectileBob[ENEMY_BOB_CHANNELS] = {0};
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
static UBYTE s_ubShowTextGameOver = FALSE;

static tBob s_tTextReadyBob;
static tBitMap *s_pTextReadyImage;
static tBitMap *s_pTextReadyMask;
static UBYTE s_ubShowTextReady = FALSE;
static UBYTE s_ubTextTimerReady = 0;

static tBob s_tTextGoBob;
static tBitMap *s_pTextGoImage;
static tBitMap *s_pTextGoMask;
static UBYTE s_ubShowTextGo = FALSE;
static UBYTE s_ubTextTimerGo = 0;

// Lookups
static fix16_t s_fViewportXMax;
static UWORD s_uwPlayerShipAnimOffset[] = {0, 0, 160, 160, 320, 320, 480, 480};
static UWORD s_uwExplosionAnimOffset[] = {0, 0, 640, 640, 1280, 1280, 1920, 1920, 2560, 2560, 3200, 3200, 3840, 3840, 4480, 4480};
static UWORD s_uwBigExplosionAnimOffset[] = {0, 0, 0, 1440, 1440, 1440, 2880, 2880, 2880, 4320, 4320, 4320, 5760, 5760, 5760, 7200, 7200, 7200, 8640, 8640, 8640, 10080, 10080, 10080, 11520, 11520, 11520, 12960, 12960, 12960, 14400, 14400, 14400, 15840, 15840, 15840};
static UWORD s_uwPathData[] = {64,0,64,1,64,1,64,2,64,3,64,4,64,4,64,5,64,6,64,7,64,7,64,8,64,9,64,10,64,11,64,11,64,12,64,13,64,14,64,15,64,16,64,16,64,17,64,18,64,19,64,20,64,21,64,22,64,23,64,24,64,24,64,25,64,26,64,27,64,28,64,29,64,30,64,31,64,32,64,33,64,34,64,35,64,36,64,37,64,38,64,39,64,40,64,41,64,42,64,43,64,44,64,45,64,46,64,47,64,48,64,49,64,50,64,51,64,52,64,53,64,54,64,55,64,56,64,57,64,58,64,59,64,61,64,62,64,63,64,64,64,65,64,66,64,67,64,68,64,69,64,70,64,71,64,72,64,74,64,75,64,76,64,77,64,78,64,79,64,80,64,81,64,82,64,83,64,84,64,86,64,87,64,88,64,89,64,90,64,91,64,92,64,93,64,94,64,96,64,97,64,98,64,99,64,100,64,101,64,102,64,103,64,104,64,105,64,106,64,108,64,109,64,110,64,111,64,112,64,113,64,114,64,115,64,116,64,117,64,118,64,120,64,121,64,122,64,123,64,124,64,125,64,126,64,127,64,128,64,129,64,130,64,131,64,132,64,133,64,134,64,135,64,136,64,137,64,138,64,139,63,140,63,140,63,141,63,142,63,143,63,144,63,145,63,145,63,146,62,147,62,148,62,149,62,149,62,150,62,151,62,151,61,152,61,153,61,153,61,154,61,155,61,155,61,156,60,157,60,157,60,158,60,159,60,159,59,160,59,160,59,161,59,161,59,162,59,162,58,163,58,163,58,164,58,165,58,165,58,165,57,166,57,166,57,167,57,167,57,168,57,168,56,169,56,169,56,170,56,170,56,170,56,171,56,171,55,172,55,172,55,172,55,173,55,173,55,174,55,174,55,174,55,175,55,175,54,175,54,176,54,176,54,176,54,177,54,177,54,177,54,178,54,178,54,178,54,179,54,179,54,179,54,180,54,180,54,180,54,180,54,181,54,181,54,181,54,182,55,182,55,182,55,183,55,183,55,183,55,183,55,184,56,184,56,184,56,185,56,185,56,185,57,186,57,186,57,186,57,186,58,187,58,187,58,187,58,188,59,188,59,188,60,189,60,189,60,189,61,190,61,190,62,190,62,191,62,191,63,191,63,192,64,192,65,192,65,193,66,193,66,193,67,194,67,194,68,194,69,194,69,195,70,195,70,195,71,195,72,195,72,195,73,196,74,196,74,196,75,196,76,196,76,196,77,196,78,196,79,196,79,196,80,196,81,196,81,196,82,196,83,196,84,196,85,196,85,196,86,196,87,196,88,196,88,196,89,196,90,196,91,196,92,196,92,196,93,195,94,195,95,195,96,195,97,195,97,195,98,195,99,194,100,194,101,194,102,194,103,194,103,194,104,194,105,193,106,193,107,193,108,193,109,193,109,193,110,192,111,192,112,192,113,192,114,192,115,191,115,191,116,191,117,191,118,191,119,191,120,190,121,190,121,190,122,190,123,190,124,190,125,190,126,189,127,189,127,189,128,189,129,189,130,189,131,189,132,188,132,188,133,188,134,188,135,188,136,188,136,188,137,188,138,188,139,188,139,188,140,188,141,188,142,188,143,188,143,188,144,188,145,188,145,188,146,188,147,188,148,188,148,188,149,188,150,188,150,188,151,188,152,189,152,189,153,189,154,189,154,189,155,189,155,190,156,190,157,190,157,190,158,191,158,191,159,191,159,192,160,192,161,192,161,193,162,193,162,193,163,194,163,194,163,195,164,195,164,195,165,196,165,196,166,197,166,197,167,198,167,198,167,198,168,199,168,199,168,200,169,200,169,201,170,201,170,202,170,202,171,203,171,203,171,204,171,204,172,205,172,205,172,206,173,206,173,207,173,207,173,208,174,208,174,209,174,209,174,210,174,210,175,211,175,212,175,212,175,213,175,213,175,214,176,214,176,215,176,215,176,216,176,217,176,217,176,218,176,218,176,219,177,219,177,220,177,221,177,221,177,222,177,222,177,223,177,223,177,224,177,225,177,225,177,226,177,226,177,227,177,227,177,228,177,229,176,229,176,230,176,230,176,231,176,231,176,232,176,233,176,233,176,234,175,234,175,235,175,235,175,236,175,236,175,237,174,238,174,238,174,239,174,239,174,240,173,240,173,241,173,241,173,242,172,242,172,243,172,243,171,244,171,244,171,245,171,245,170,246,170,246,170,247,169,247,169,248,168,248,168,249,168,249,167,250,167,250,167,250,166,251,166,251,165,252,165,252,164,253,164,253,163,253,163,254,163,254,162,255,162,255,161,255,161,256,160,256,159,256,159,257,158,257,158,257,157,258,157,258,156,258,155,258,155,259,154,259,154,259,153,259,152,259,152,259,151,260,150,260,150,260,149,260,148,260,148,260,147,260,146,260,145,260,145,260,144,260,143,260,143,260,142,260,141,260,140,260,139,260,139,260,138,260,137,260,136,260,136,260,135,260,134,260,133,260,132,260,132,260,131,259,130,259,129,259,128,259,127,259,127,259,126,259,125,258,124,258,123,258,122,258,121,258,121,258,120,258,119,257,118,257,117,257,116,257,115,257,115,257,114,256,113,256,112,256,111,256,110,256,109,255,109,255,108,255,107,255,106,255,105,255,104,254,103,254,103,254,102,254,101,254,100,254,99,254,98,253,97,253,97,253,96,253,95,253,94,253,93,253,92,252,92,252,91,252,90,252,89,252,88,252,88,252,87,252,86,252,85,252,85,252,84,252,83,252,82,252,81,252,81,252,80,252,79,252,79,252,78,252,77,252,76,252,76,252,75,252,74,252,74,252,73,252,72,253,72,253,71,253,70,253,70,253,69,253,69,254,68,254,67,254,67,254,66,255,66,255,65,255,65,256,64,256,63,256,63,257,62,257,62,257,61,258,61,258,60,259,60,259,59,259,59,260,58,260,58,261,58,261,57,262,57,262,56,262,56,263,55,263,55,264,54,264,54,265,54,265,53,266,53,266,52,267,52,267,52,268,51,268,51,269,51,269,50,270,50,270,49,271,49,271,49,272,49,272,48,273,48,273,48,274,47,274,47,275,47,276,46,276,46,277,46,277,46,278,46,278,45,279,45,279,45,280,45,281,44,281,44,282,44,282,44,283,44,283,44,284,44,285,43,285,43,286,43,286,43,287,43,287,43,288,43,289,43,289,43,290,43,290,43,291,43,291,43,292,43,293,43,293,43,294,43,294,43,295,43,295,43,296,43,297,43,297,43,298,44,298,44,299,44,299,44,300,44,300,44,301,45,302,45,302,45,303,45,303,46,304,46,304,46,305,46,305,47,306,47,306,47,307,48,307,48,308,48,308,49,309,49,309,50,310,50,310,50,311,51,311,51,312,52,312,52,313,53,313,53,314,54,314,55,314,55,315,56,315,56,316,57,316,58,317,58,317,59,317,60,318,60,318,61,319,62,319,62,319,63,320,64,320,65,320,66,321,66,321,67,321,68,322,69,322,70,322,71,322,72,323,73,323,74,323,75,323,75,324,76,324,77,324,78,324,79,324,80,325,82,325,83,325,84,325,85,325,86,325,87,326,88,326,89,326,90,326,91,326,92,326,94,326,95,326,96,326,97,326,98,326,99,326,101,327,102,327,103,327,104,327,106,327,107,327,108,327,109,327,110,327,112,327,113,327,114,327,115,327,117,327,118,326,119,326,121,326,122,326,123,326,124,326,126,326,127,326,128,326,130,326,131,326,132,326,133,326,135,326,136,326,137,326,139,325,140,325,141,325,142,325,144,325,145,325,146,325,148,325,149,325,150,325,151,324,153,324,154,324,155,324,157,324,158,324,159,324,160,324,162,323,163,323,164,323,165,323,166,323,168,323,169,323,170,323,171,323,173,322,174,322,175,322,176,322,177,322,178,322,180,322,181,322,182,322,183,321,184,321,185,321,186,321,187,321,188,321,189,321,190,321,192,321,193,321,194,321,195,321,196,320,197,320,197,320,198,320,199,320,200,320,201,320,202,320,203,320,204,320,205,320,206,320,206,320,207,320,208,320,160,0,160,0,160,1,160,1,160,2,160,2,160,3,160,3,160,4,160,5,160,5,160,6,160,6,160,7,160,8,160,9,160,9,160,10,160,11,160,12,160,13,160,13,160,14,160,15,160,16,160,17,160,18,160,19,160,20,160,21,160,22,160,23,160,24,160,25,160,26,160,27,160,28,160,29,160,30,160,31,160,32,160,33,160,34,160,35,160,37,160,38,160,39,160,40,160,41,160,42,160,44,160,45,160,46,160,47,160,48,160,50,160,51,160,52,160,53,160,55,160,56,160,57,160,58,160,60,160,61,160,62,160,63,160,65,160,66,160,67,160,68,160,70,160,71,160,72,160,73,160,75,160,76,160,77,160,78,160,80,160,81,160,82,160,83,160,84,160,86,160,87,160,88,160,89,160,90,160,92,160,93,160,94,160,95,160,96,160,97,160,98,160,100,160,101,160,102,160,103,160,104,160,105,160,106,160,107,160,108,160,109,160,110,160,111,160,112,160,113,160,114,160,115,160,116,160,117,160,118,160,118,160,119,160,120,160,121,160,122,160,123,160,123,160,124,160,125,160,125,160,126,160,127,160,127,160,128,160,129,160,129,160,130,160,130,160,131,160,132,160,132,160,133,160,133,160,134,160,135,160,135,160,136,160,137,160,137,161,138,161,138,161,139,161,140,161,140,161,141,161,141,161,142,161,143,161,143,161,144,161,145,161,145,162,146,162,146,162,147,162,148,162,148,162,149,162,149,162,150,162,151,162,151,162,152,163,152,163,153,163,154,163,154,163,155,163,155,163,156,163,157,163,157,163,158,164,158,164,159,164,160,164,160,164,161,164,161,164,162,164,162,164,163,164,164,164,164,165,165,165,165,165,166,165,166,165,167,165,167,165,168,165,169,165,169,165,170,165,170,165,171,165,171,165,172,165,172,165,173,166,173,166,174,166,174,166,175,166,175,166,176,166,176,166,177,166,177,166,178,166,178,166,179,166,179,166,179,166,180,166,180,166,181,166,181,165,182,165,182,165,182,165,183,165,183,165,184,165,184,165,184,165,185,165,185,165,185,165,186,164,186,164,187,164,187,164,187,164,188,164,188,164,188,163,188,163,189,163,189,163,189,163,190,162,190,162,190,162,190,162,191,161,191,161,191,161,191,161,192,160,192,160,192,160,192,159,192,159,193,159,193,158,193,158,193,157,193,157,193,156,193,156,194,155,194,155,194,154,194,154,194,153,194,153,194,152,194,152,194,151,194,150,194,150,194,149,194,148,194,147,194,147,194,146,194,145,194,145,194,144,194,143,194,142,194,141,194,141,194,140,194,139,194,138,194,137,194,136,194,136,194,135,194,134,194,133,194,132,194,131,194,130,194,129,194,128,194,127,194,126,193,125,193,125,193,124,193,123,193,122,193,121,193,120,193,119,193,118,193,117,192,116,192,115,192,114,192,113,192,112,192,111,192,110,192,109,192,108,192,107,192,106,191,105,191,104,191,103,191,102,191,101,191,100,191,99,191,99,191,98,191,97,191,96,190,95,190,94,190,93,190,92,190,91,190,90,190,89,190,88,190,88,190,87,190,86,190,85,190,84,190,83,190,83,190,82,190,81,190,80,190,79,190,79,190,78,190,77,190,77,190,76,190,75,190,74,190,74,190,73,190,72,190,72,190,71,190,71,190,70,190,70,190,69,190,69,190,68,190,68,191,67,191,67,191,66,191,66,191,65,191,65,191,65,192,64,192,64,192,64,192,63,192,63,193,63,193,63,193,62,193,62,194,62,194,61,194,61,195,61,195,61,195,60,196,60,196,60,196,60,197,60,197,59,198,59,198,59,199,59,199,59,199,58,200,58,200,58,201,58,201,58,202,57,202,57,203,57,203,57,204,57,204,57,205,57,205,56,206,56,207,56,207,56,208,56,208,56,209,56,209,56,210,55,211,55,211,55,212,55,213,55,213,55,214,55,214,55,215,55,216,55,216,55,217,55,218,55,218,55,219,55,219,54,220,54,221,54,221,54,222,54,223,54,223,54,224,54,225,54,225,54,226,54,227,54,227,54,228,55,229,55,229,55,230,55,230,55,231,55,232,55,232,55,233,55,234,55,234,55,235,55,235,55,236,55,237,55,237,56,238,56,239,56,239,56,240,56,240,56,241,56,241,56,242,57,243,57,243,57,244,57,244,57,245,57,245,57,246,58,246,58,247,58,247,58,248,58,248,59,249,59,249,59,249,59,250,59,250,60,251,60,251,60,252,60,252,60,252,61,253,61,253,61,253,61,254,62,254,62,254,62,255,63,255,63,255,63,255,63,256,64,256,64,256,64,256,65,256,65,257,65,257,66,257,66,257,67,257,67,257,68,257,68,258,69,258,69,258,70,258,70,258,71,258,71,258,72,258,72,258,73,258,74,258,74,258,75,258,76,258,77,258,77,258,78,258,79,258,79,258,80,258,81,258,82,258,83,258,83,258,84,258,85,258,86,258,87,258,88,258,88,258,89,258,90,258,91,258,92,258,93,258,94,258,95,258,96,258,97,258,98,257,99,257,99,257,100,257,101,257,102,257,103,257,104,257,105,257,106,257,107,256,108,256,109,256,110,256,111,256,112,256,113,256,114,256,115,256,116,256,117,256,118,255,119,255,120,255,121,255,122,255,123,255,124,255,125,255,125,255,126,255,127,255,128,254,129,254,130,254,131,254,132,254,133,254,134,254,135,254,136,254,136,254,137,254,138,254,139,254,140,254,141,254,141,254,142,254,143,254,144,254,145,254,145,254,146,254,147,254,147,254,148,254,149,254,150,254,150,254,151,254,152,254,152,254,153,254,153,254,154,254,154,254,155,254,155,254,156,254,156,255,157,255,157,255,158,255,158,255,159,255,159,255,159,256,160,256,160,256,160,256,161,256,161,257,161,257,161,257,162,257,162,258,162,258,163,258,163,259,163,259,163,259,164,260,164,260,164,260,164,261,165,261,165,262,165,262,165,263,166,263,166,263,166,264,166,264,167,265,167,265,167,266,167,266,167,267,168,267,168,268,168,268,168,269,168,269,169,270,169,271,169,271,169,272,169,272,170,273,170,273,170,274,170,275,170,275,170,276,170,277,171,277,171,278,171,278,171,279,171,280,171,280,171,281,171,282,171,282,172,283,172,283,172,284,172,285,172,285,172,286,172,287,172,287,172,288,172,289,172,289,172,290,172,291,172,291,172,292,172,293,172,293,172,294,172,294,172,295,172,296,172,296,172,297,172,298,172,298,172,299,172,299,172,300,172,301,171,301,171,302,171,303,171,303,171,304,171,304,171,305,171,305,170,306,170,307,170,307,170,308,170,308,170,309,169,309,169,310,169,310,169,311,168,311,168,312,168,312,168,313,167,313,167,313,167,314,167,314,166,315,166,315,166,316,165,316,165,316,165,317,164,317,164,317,164,318,163,318,163,318,163,319,162,319,162,319,161,319,161,320,160,320,160,320,160,320,159,320,159,321,158,321,157,321,157,321,156,321,155,321,155,322,154,322,153,322,152,322,152,322,151,322,150,322,149,322,148,323,147,323,146,323,145,323,144,323,143,323,142,323,141,323,140,323,139,323,138,323,137,323,136,323,134,324,133,324,132,324,131,324,130,324,128,324,127,324,126,324,125,324,123,324,122,324,121,324,119,324,118,324,117,324,115,324,114,324,112,324,111,324,110,324,108,324,107,324,105,324,104,324,103,324,101,324,100,324,98,324,97,323,95,323,94,323,92,323,91,323,89,323,88,323,87,323,85,323,84,323,82,323,81,323,79,323,78,323,76,323,75,323,73,323,72,323,71,323,69,322,68,322,66,322,65,322,64,322,62,322,61,322,59,322,58,322,57,322,55,322,54,322,53,322,51,322,50,322,49,321,48,321,46,321,45,321,44,321,43,321,42,321,40,321,39,321,38,321,37,321,36,321,35,321,34,321,33,321,32,321,31,321,30,320,29,320,28,320,27,320,26,320,25,320,24,320,24,320,23,320,22,320,21,320,21,320,20,320,19,320,19,320,18,320,17,320,17,320,16,320,16,320};
static UBYTE s_ubMapData[] = {0, 0, 1, 5,  9, 13, 17, 21, 25, 29, 1, 5, 0, 0, 
                              0, 0, 2, 6, 10, 14, 18, 22, 26, 30, 2, 6, 0, 0, 
                              0, 0, 3, 7, 11, 15, 19, 23, 27, 31, 3, 7, 0, 0, 
                              0, 0, 4, 8, 12, 16, 20, 24, 28, 32, 4, 8, 0, 0};

// Game states.

void gameGsCreate(void) {
    randInit(&s_tRandManager, 1337, 2600);
    initGame();
    initViews();
    initAudio();
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
    ptplayerProcess();
    
    // Process game logic.
    // if (s_ubLevelStart == TRUE) {
    //     processLevelStart();
    // } else {
    //     processInput();
    // }

    processInput();
    processCamera();
    //processWaves();
    //processPlayer();
    processPlayerProjectiles();
    processEnemies();
    processSimpleEnemyProjectiles();
    processPowerups();
    processBobs();
    processHud();
    processTimers();

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
    // Setup enemy types.
    s_tEnemyTypes[0] = (tEnemyType){ 
        .bHealth = 20,
        .ubWidth = 16, .ubHeight = 16,
        .ubGunOffsetX = 3, .ubGunOffsetY = 15,
        .ubMoveSpeed = 4,
        .ubCanShoot = FALSE,
        .ubCooldownTime = 0,
        .ubProjectileType = 0,
        .ubPowerupType = 0,
        .uwScoreValue = 1337
    };

    s_tEnemyTypes[1] = (tEnemyType){ 
        .bHealth = 20,
        .ubWidth = 16, .ubHeight = 16,
        .ubGunOffsetX = 3, .ubGunOffsetY = 15,
        .ubMoveSpeed = 4,
        .ubCanShoot = TRUE,
        .ubCooldownTime = 50,
        .ubProjectileType = 0,
        .ubPowerupType = 0,
        .uwScoreValue = 1337
    };

    s_tEnemyTypes[2] = (tEnemyType){ 
        .bHealth = 20,
        .ubWidth = 16, .ubHeight = 16,
        .ubGunOffsetX = 3, .ubGunOffsetY = 15,
        .ubMoveSpeed = 4,
        .ubCanShoot = TRUE,
        .ubCooldownTime = 50,
        .ubProjectileType = 0,
        .ubPowerupType = 3,
        .uwScoreValue = 1337
    };

    // Setup enemy waves.
    s_tEnemyWaves[0] = (tEnemyWave){
        .ubEnemies = {0, 0, 0, 2},
        .ubNumberOfEnemies = 4,
        .ubSpacing = 64,
        .uwSpawnYPosition = 3760,
        .uwPathYOffset = 3648,
        .uwPathArrayOffset = 0,
        .uwPathLength = 1794,
        .ubPathLoops = FALSE
    };

    s_tEnemyWaves[1] = (tEnemyWave){
        .ubEnemies = {2, 0, 1, 0},
        .ubNumberOfEnemies = 4,
        .ubSpacing = 64,
        .uwSpawnYPosition = 3632,
        .uwPathYOffset = 3520,
        .uwPathArrayOffset = 1794,
        .uwPathLength = 1794,
        .ubPathLoops = FALSE
    };


    // Setup player projectile types.
    s_tPlayerProjectileTypes[0] = (tPlayerProjectileType){ .ubDamage = 10, .ubDelay =  5, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -6, .ubXOffset =  2, .ubXOffset2 =  0, .ubWidth = 11, .ubHeight = 10, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[1] = (tPlayerProjectileType){ .ubDamage = 12, .ubDelay =  8, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -6, .ubXOffset =  1, .ubXOffset2 =  0, .ubWidth = 11, .ubHeight = 12, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[2] = (tPlayerProjectileType){ .ubDamage = 14, .ubDelay = 10, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -3, .ubXOffset =  0, .ubXOffset2 =  0, .ubWidth = 13, .ubHeight = 11, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    s_tPlayerProjectileTypes[3] = (tPlayerProjectileType){ .ubDamage =  5, .ubDelay =  4, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -8, .ubXOffset =  2, .ubXOffset2 =  0, .ubWidth = 12, .ubHeight = 12, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 };
    // s_tPlayerProjectileTypes[] = (tPlayerProjectileType){ .ubDamage = 5,  .ubDelay = 8, .bDeltaX =  0, .bDeltaX2 =  0, .bDeltaY = -8, .ubXOffset =  0, .ubXOffset2 = 16, .ubWidth = 31, .ubHeight = 20, .ubDieOnCollision = TRUE, .ubWideSprite = TRUE,  .ubSpreadShot = FALSE, .ubSecondarySpriteIndex = 0 }; // Wideshot
    // s_tPlayerProjectileTypes[] = (tPlayerProjectileType){ .ubDamage = 25, .ubDelay = 8, .bDeltaX = -5, .bDeltaX2 =  5, .bDeltaY = -5, .ubXOffset =  9, .ubXOffset2 = 16, .ubWidth =  7, .ubHeight =  8, .ubDieOnCollision = TRUE, .ubWideSprite = FALSE, .ubSpreadShot = TRUE,  .ubSecondarySpriteIndex = 1 }; // SpreadShot
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
    //s_pTiles = bitmapCreateFromFile("data/bgtiles.bm", 0);
    s_pTiles = bitmapCreateFromFile("data/hextiles512.bm", 0);

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
        TAG_SIMPLEBUFFER_USE_X_SCROLLING, 1,
        TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
        TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    TAG_DONE);

    // Populate tilemap.
    UWORD idx = 0;
    for (UWORD y=0; y<MAP_HEIGHT_IN_TILES; y++) {
        for (UWORD x=0; x<MAP_WIDTH_IN_TILES; x++) {
            //if (idx == MAP_TILES_COUNT) { idx = 0; }
            //s_pTileBuffer->pTileData[x][y] = s_ubMapData[idx];
            if (idx == 512) { idx = 0; }
            s_pTileBuffer->pTileData[x][y] = idx;
            idx++;
        }
    }

    // Setup camera.
    s_pCamera = s_pTileBuffer->pCamera;
    s_fViewportXMax = fix16_from_int(TILE_VIEWPORT_XMAX);
    //cameraMoveBy(s_pCamera, 32, CAMERA_START_YPOS);
    cameraMoveBy(s_pCamera, 32, 0);
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

static void initAudio() {
    s_pSfxLaser = ptplayerSfxCreateFromFile("data/laser.sfx", 1);
    s_pSfxExplosion = ptplayerSfxCreateFromFile("data/explosion.sfx", 1);    
    ptplayerCreate(1);
    ptplayerSetChannelsForPlayer(0b0111);
    ptplayerSetMasterVolume(24);
    audioMixerCreate();
    //s_pGameMusic = ptplayerModCreate("data/chips.mod");
    //ptplayerLoadMod(s_pGameMusic, 0, 0);
    //ptplayerEnableMusic(1);
}

static void initBobs() {
    // Init tilemap bob manager.
    bobManagerCreate(s_pTileBuffer->pScroll->pFront, s_pTileBuffer->pScroll->pBack, s_pTileBuffer->pScroll->uwBmAvailHeight);

    // Player ship bob.
    s_pPlayerImage = bitmapCreateFromFile("data/player.bm", 0);
    s_pPlayerMask = bitmapCreateFromFile("data/player_mask.bm", 0);
    bobInit(&s_tPlayerShipBob, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, 1, s_pPlayerImage->Planes[0], s_pPlayerMask->Planes[0], 0, 0);

    // Enemy bobs
    s_pEnemyImage = bitmapCreateFromFile("data/enemy.bm", 0);
    s_pEnemyMask = bitmapCreateFromFile("data/enemy_mask.bm", 0);

    // Init enemy structs.
    for (UBYTE i=0; i<ENEMY_MAX; i++) {
        bobInit( &s_tEnemy[i].sBob, 16, 16, 1, s_pEnemyImage->Planes[0], s_pEnemyMask->Planes[0], 0, 0 );
        s_tEnemy[i].bHealth = 0;
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

    // Extra Life Powerup
    s_pLifePowImage = bitmapCreateFromFile("data/powerup_1up.bm", 0);
    s_pLifePowMask = bitmapCreateFromFile("data/powerup_1up_mask.bm", 0);
    bobInit(&s_tLifePowBob, 16, 16, 1, s_pLifePowImage->Planes[0], s_pLifePowMask->Planes[0], 0, 0);

    // Special Powerup
    s_pSpecialPowImage = bitmapCreateFromFile("data/powerup_special.bm", 0);
    s_pSpecialPowMask = bitmapCreateFromFile("data/powerup_special_mask.bm", 0);
    bobInit(&s_tSpecialPowBob, 16, 16, 1, s_pSpecialPowImage->Planes[0], s_pSpecialPowMask->Planes[0], 0, 0);

    // Weapon Powerup
    s_pWeaponPowImage = bitmapCreateFromFile("data/powerup_weapon.bm", 0);
    s_pWeaponPowMask = bitmapCreateFromFile("data/powerup_weapon_mask.bm", 0);
    bobInit(&s_tWeaponPowBob, 16, 16, 1, s_pWeaponPowImage->Planes[0], s_pWeaponPowMask->Planes[0], 0, 0);

    // Game Over text
    s_pTextGameOverImage = bitmapCreateFromFile("data/text_gameover.bm", 0);
    s_pTextGameOverMask = bitmapCreateFromFile("data/text_gameover_mask.bm", 0);
    bobInit(&s_tTextGameOverBob, 112, 60, 0, s_pTextGameOverImage->Planes[0], s_pTextGameOverMask->Planes[0], 0, 0);

    // Ready text
    s_pTextReadyImage = bitmapCreateFromFile("data/text_ready.bm", 0);
    s_pTextReadyMask = bitmapCreateFromFile("data/text_ready_mask.bm", 0);
    bobInit(&s_tTextReadyBob, 144, 27, 1, s_pTextReadyImage->Planes[0], s_pTextReadyMask->Planes[0], 0, 0);

    // Go text
    s_pTextGoImage = bitmapCreateFromFile("data/text_go.bm", 0);
    s_pTextGoMask = bitmapCreateFromFile("data/text_go_mask.bm", 0);
    bobInit(&s_tTextGoBob, 64, 38, 1, s_pTextGoImage->Planes[0], s_pTextGoMask->Planes[0], 0, 0);

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

    s_tSimpleChannelBounds[0] = (tSimpleChannelBounds){.uwMin = BOUNDS_MAX, .uwMax = BOUNDS_MIN};
    s_tSimpleChannelBounds[1] = (tSimpleChannelBounds){.uwMin = BOUNDS_MAX, .uwMax = BOUNDS_MIN};
    s_tSimpleChannelBounds[2] = (tSimpleChannelBounds){.uwMin = BOUNDS_MAX, .uwMax = BOUNDS_MIN};
    s_tSimpleChannelBounds[3] = (tSimpleChannelBounds){.uwMin = BOUNDS_MAX, .uwMax = BOUNDS_MIN};

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

static void processLevelStart() {
    s_ubLevelStartTimer--; 
    if (s_ubLevelStartTimer > 64) {
        s_ubShowTextReady = TRUE;
    }

    if (s_ubLevelStartTimer > 0 && s_ubLevelStartTimer <= 64) {
        s_ubShowTextReady = FALSE;
        s_ubShowTextGo = TRUE;
        s_ubCameraCanMove = TRUE; 
    }

    if (s_ubLevelStartTimer == 0) { 
        s_ubShowTextReady = FALSE;
        s_ubShowTextGo = FALSE;
        s_ubCameraCanMove = TRUE;
        s_ubLevelStart = FALSE;          
    }
}

static void processCamera() {
    if (s_ubCameraCanMove == TRUE) {
        s_ubMoveCameraCounter++;

        if (s_ubMoveCameraCounter >= CAMERA_MOVE_RATE) { 
            if (s_pCamera->uPos.uwY > CAMERA_Y_MIN) { 
                cameraMoveBy(s_pCamera, 0, CAMERA_MOVE_AMOUNT);

                if (s_ubPlayerMovedOnY == FALSE) {
                    s_tPlayerPosition.uwY += CAMERA_MOVE_AMOUNT;
                }                 
            }
            s_ubMoveCameraCounter = 0;
        }

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

    // Move
    s_ubPlayerMovedOnY = FALSE;
    if(keyCheck(KEY_W) || joyCheck(JOY1_UP)) {
        s_tPlayerPosition.uwY = s_tPlayerPosition.uwY - PLAYER_MOVE_SPEED;
        s_ubPlayerMovedOnY = TRUE;
        if (s_tPlayerPosition.uwY < s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN) { s_tPlayerPosition.uwY = s_pCamera->uPos.uwY + TILE_VIEWPORT_YMIN; }
    }
    if(keyCheck(KEY_S) || joyCheck(JOY1_DOWN)) {
        s_tPlayerPosition.uwY = s_tPlayerPosition.uwY + (PLAYER_MOVE_SPEED-1);
        s_ubPlayerMovedOnY = TRUE;
        if (s_tPlayerPosition.uwY > s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT)) { s_tPlayerPosition.uwY = s_pCamera->uPos.uwY + (TILE_VIEWPORT_HEIGHT-PLAYER_SHIP_HEIGHT); }
    }
    if(keyCheck(KEY_A) || joyCheck(JOY1_LEFT)) {
        s_tPlayerPosition.uwX = s_tPlayerPosition.uwX - PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwX < TILE_VIEWPORT_XMIN) { s_tPlayerPosition.uwX = TILE_VIEWPORT_XMIN; }
    }
    if(keyCheck(KEY_D) || joyCheck(JOY1_RIGHT)) {
        s_tPlayerPosition.uwX = s_tPlayerPosition.uwX + PLAYER_MOVE_SPEED;
        if (s_tPlayerPosition.uwX > (TILE_VIEWPORT_XMAX-PLAYER_SHIP_WIDTH)) { s_tPlayerPosition.uwX = (TILE_VIEWPORT_XMAX-PLAYER_SHIP_WIDTH); }
    }

    // Shoot
    if (keyCheck(KEY_SPACE) || joyCheck(JOY1_FIRE)) {
        processPlayerShoot();
    }
   
    // DEBUG: Move camera
    if (keyCheck(KEY_UP)) { cameraMoveBy(s_pCamera, 0, -1); }
    if (keyCheck(KEY_DOWN)) { cameraMoveBy(s_pCamera, 0, 1); }
    if (keyCheck(KEY_LEFT)) { cameraMoveBy(s_pCamera, -1, 0); }
    if (keyCheck(KEY_RIGHT)) { cameraMoveBy(s_pCamera, 1, 0); }

    // DEBUG: increment player lives
    if (keyCheck(KEY_1)) {
        if (s_ubFireDelay == 0) {
            if (s_ubPlayerLives < PLAYER_LIVES_MAX) { s_ubPlayerLives++; }
            s_ubUpdateLives = TRUE;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }
    }

    // DEBUG: cycle projectile type
    if (keyCheck(KEY_2)) {
        if (s_ubFireDelay == 0) {
            s_ubEquippedProjectileType++;
            if (s_ubEquippedProjectileType >= PLAYER_PROJECTILE_TYPES) { s_ubEquippedProjectileType = 0; }
            s_ubUpdatePower = TRUE;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }
    }

    // DEBUG: toggle camera move
    if (keyCheck(KEY_3)) {
        if (s_ubFireDelay == 0) {
            s_ubCameraCanMove = !s_ubCameraCanMove;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }        
    }
    
    // DEBUG: toggle invinciblity
    if (keyCheck(KEY_4)) {
        if (s_ubFireDelay == 0) {
            s_ubPlayerIsInvincible = TRUE;
            s_ubPlayerInvincibleTimer = PLAYER_INVINCIBLE_TIME;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }           
    }

    if (keyCheck(KEY_8)) {
        if (s_ubFireDelay == 0) {
            s_ubShowTextGameOver = !s_ubShowTextGameOver;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }        
    }

    if (keyCheck(KEY_9)) {
        if (s_ubFireDelay == 0) {
            s_ubShowTextReady = !s_ubShowTextReady;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }        
    }

    if (keyCheck(KEY_0)) {
        if (s_ubFireDelay == 0) {
            s_ubShowTextGo = !s_ubShowTextGo;
            s_ubFireDelay = DEBUG_COMMAND_DELAY;
        }        
    }

    // DEBUG: respawn player
    if (keyCheck(KEY_R)) {
        if (s_ubPlayerAlive == 0 && s_ubPlayerLives > 0) {
            s_tPlayerPosition.uwX = s_pCamera->uPos.uwX+(80 - (PLAYER_SHIP_WIDTH/2));
            s_tPlayerPosition.uwY = s_pCamera->uPos.uwY+128;
            s_ubPlayerAlive = TRUE;
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

static void processWaves() {
    if (s_ubWaveIndex != 255) {
        if (s_pCamera->uPos.uwY <= s_tEnemyWaves[s_ubWaveIndex].uwSpawnYPosition) {
            if (s_ubActiveEnemies + s_tEnemyWaves[s_ubWaveIndex].ubNumberOfEnemies < ENEMY_MAX) {
                UBYTE ubPathIdxSpacingOffset = 0;
                for (UBYTE spawnIdx=0; spawnIdx<s_tEnemyWaves[s_ubWaveIndex].ubNumberOfEnemies; spawnIdx++) {
                    UBYTE ubEnemyTypeToSpawn = s_tEnemyWaves[s_ubWaveIndex].ubEnemies[spawnIdx];
                    logWrite("Wave[%d] Spawn Enemy: %d => Type: %d", s_ubWaveIndex, spawnIdx, ubEnemyTypeToSpawn);
                    for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
                        if (s_tEnemy[enemyIdx].bHealth > 0) { continue; }
                        s_tEnemy[enemyIdx].bHealth = s_tEnemyTypes[ubEnemyTypeToSpawn].bHealth;
                        s_tEnemy[enemyIdx].ubOnScreen = FALSE;
                        s_tEnemy[enemyIdx].ubCanShoot = s_tEnemyTypes[ubEnemyTypeToSpawn].ubCanShoot;
                        s_tEnemy[enemyIdx].ubCooldownTimer = s_tEnemyTypes[ubEnemyTypeToSpawn].ubCooldownTime;
                        s_tEnemy[enemyIdx].ubEnemyType = ubEnemyTypeToSpawn;
                        s_tEnemy[enemyIdx].ubPowerupType = s_tEnemyTypes[ubEnemyTypeToSpawn].ubPowerupType;
                        s_tEnemy[enemyIdx].ubWaveIdx = s_ubWaveIndex;
                        s_tEnemy[enemyIdx].uwScoreValue = s_tEnemyTypes[ubEnemyTypeToSpawn].uwScoreValue;
                        s_tEnemy[enemyIdx].ubMoveSpeed = s_tEnemyTypes[ubEnemyTypeToSpawn].ubMoveSpeed;
                        s_tEnemy[enemyIdx].ubPathLoops = s_tEnemyWaves[s_ubWaveIndex].ubPathLoops;
                        s_tEnemy[enemyIdx].uwPathLength = s_tEnemyWaves[s_ubWaveIndex].uwPathLength;
                        s_tEnemy[enemyIdx].uwPathArrayOffset = s_tEnemyWaves[s_ubWaveIndex].uwPathArrayOffset;
                        s_tEnemy[enemyIdx].uwPathYOffset = s_tEnemyWaves[s_ubWaveIndex].uwPathYOffset;
                        s_tEnemy[enemyIdx].uwPathIdx = ubPathIdxSpacingOffset;
                        ubPathIdxSpacingOffset += s_tEnemyWaves[s_ubWaveIndex].ubSpacing;
                        s_ubActiveEnemies++;
                        break;
                    }
                }
            }

            s_ubWaveIndex++;
            if (s_ubWaveIndex >= s_ubWavesInLevel[s_ubCurrentLevel]) {
                s_ubWaveIndex = 255;
            }
        }
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

        if (s_ubDisplayPlayer == TRUE) {
            bobPush(&s_tPlayerShipBob);
        }        
    }

    // Extra Life Powerup
    if (s_ubLifePowActive == TRUE) { bobPush(&s_tLifePowBob); }

    // Special Powerup
    if (s_ubSpecialPowActive == TRUE) { bobPush(&s_tSpecialPowBob); }

    // Weapon Powerup
    if (s_ubWeaponPowActive == TRUE) { bobPush(&s_tWeaponPowBob); }

    // Enemies
    if (s_ubActiveEnemies > 0) {
        for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
            if (s_tEnemy[enemyIdx].bHealth == 0 || s_tEnemy[enemyIdx].ubOnScreen == FALSE) { continue; }
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
    if (s_ubShowTextGameOver == TRUE) {
        s_tTextGameOverBob.sPos.uwX = 57; 
        s_tTextGameOverBob.sPos.uwY = s_pCamera->uPos.uwY+99;
        bobPush(&s_tTextGameOverBob);
    }

    // Ready text
    if (s_ubShowTextReady == TRUE) {
        s_tTextReadyBob.sPos.uwX = 41; 
        s_tTextReadyBob.sPos.uwY = s_pCamera->uPos.uwY+115;
        bobPush(&s_tTextReadyBob);
    }

    // Go text
    if (s_ubShowTextGo == TRUE) {
        s_tTextGoBob.sPos.uwX = 81; 
        s_tTextGoBob.sPos.uwY = s_pCamera->uPos.uwY+110;
        bobPush(&s_tTextGoBob);
    }

    bobEnd();
    systemSetDmaBit(DMAB_BLITHOG, 0);
}

static void processTimers() {
    // System
    s_ubFrameCounter = !s_ubFrameCounter;
    s_ubFlashTimer++; 
    if (s_ubFlashTimer >= FLASH_TIME) { 
        s_ubFlashTimer = 0; 
        s_ubFlashOn = !s_ubFlashOn;
    }

    // Player
    s_ubPlayerShipAnimFrame++; s_ubPlayerShipAnimFrame = s_ubPlayerShipAnimFrame % PLAYER_SHIP_ANIM_FRAMES;
    if (s_ubFireDelay > 0) { s_ubFireDelay--; }
    if (s_ubAudioDelay > 0) { s_ubAudioDelay--; }
    if (s_ubPlayerInvincibleTimer > 0) { s_ubPlayerInvincibleTimer--; }

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

static void processPowerups() {
    if (s_ubLifePowActive == TRUE) {
        s_tLifePowBob.sPos.uwY += POWERUP_FALL_SPEED;

        // Check if offscreen.
        if (s_tLifePowBob.sPos.uwY > s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT) {
            s_ubLifePowActive = FALSE;
            return;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX, s_tPlayerPosition.uwY, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, 
                                           s_tLifePowBob.sPos.uwX, s_tLifePowBob.sPos.uwY, POWERUP_WIDTH, POWERUP_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            if (s_ubPlayerLives < PLAYER_LIVES_MAX) {
                s_ubPlayerLives++;
                s_ubUpdateLives = TRUE;
            }
            s_ubLifePowActive = FALSE;
        }
        #endif

        return;
    }

    if (s_ubSpecialPowActive == TRUE) {
        s_tSpecialPowBob.sPos.uwY += POWERUP_FALL_SPEED;

        // Check if offscreen.
        if (s_tSpecialPowBob.sPos.uwY > s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT) {
            s_ubSpecialPowActive = FALSE;
            return;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX, s_tPlayerPosition.uwY, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, 
                                           s_tSpecialPowBob.sPos.uwX, s_tSpecialPowBob.sPos.uwY, POWERUP_WIDTH, POWERUP_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            if (s_ubPlayerSpecial < PLAYER_SPECIAL_MAX) {
                s_ubPlayerSpecial++;
                s_ubUpdateSpecial = TRUE;
            }
            s_ubSpecialPowActive = FALSE;
        }
        #endif

        return;
    }

    if (s_ubWeaponPowActive == TRUE) {
        s_tWeaponPowBob.sPos.uwY += POWERUP_FALL_SPEED;

        // Check if offscreen.
        if (s_tWeaponPowBob.sPos.uwY > s_pCamera->uPos.uwY+TILE_VIEWPORT_HEIGHT) {
            s_ubWeaponPowActive = FALSE;
            return;
        }

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX, s_tPlayerPosition.uwY, PLAYER_SHIP_WIDTH, PLAYER_SHIP_HEIGHT, 
                                           s_tWeaponPowBob.sPos.uwX, s_tWeaponPowBob.sPos.uwY, POWERUP_WIDTH, POWERUP_HEIGHT);
        if (ubCollision == TRUE && s_ubPlayerAlive == TRUE) { 
            if (s_ubEquippedProjectileType < (PLAYER_PROJECTILE_TYPES-1)) {
                s_ubEquippedProjectileType++;
                s_ubUpdatePower = TRUE;
            }
            s_ubWeaponPowActive = FALSE;
        }
        #endif

        return;
    }
}

static void processPlayer() {
    if (s_ubPlayerIsInvincible == TRUE) {
        if (s_ubPlayerInvincibleTimer == 0) {
            s_ubPlayerIsInvincible = FALSE;
            s_ubDisplayPlayer = TRUE;
        } else {
            if (s_ubFlashOn == TRUE) {
                s_ubDisplayPlayer = TRUE;
            } else {
                s_ubDisplayPlayer = FALSE;
            }
        }
    }

    // Respawn player.
    if (s_ubPlayerAlive == FALSE && s_ubPlayerLives > 0 && s_ubBigExplosionActive == FALSE) {
        s_tPlayerPosition.uwX = s_pCamera->uPos.uwX+(80 - (PLAYER_SHIP_WIDTH/2));
        s_tPlayerPosition.uwY = s_pCamera->uPos.uwY+192;
        s_ubPlayerAlive = TRUE;
        s_ubPlayerIsInvincible = TRUE;
        s_ubPlayerInvincibleTimer = PLAYER_INVINCIBLE_TIME;
    }

    if (s_ubPlayerLives == 0) {
        s_ubCameraCanMove = FALSE;
        s_ubShowTextGameOver = TRUE;
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
        
        UBYTE ubEnemyType = s_tEnemy[enemyIdx].ubEnemyType;

        // Move
        s_tEnemy[enemyIdx].uwPathIdx += s_tEnemy[enemyIdx].ubMoveSpeed;

        if (s_tEnemy[enemyIdx].uwPathIdx >= s_tEnemy[enemyIdx].uwPathLength-1) {
            if (s_tEnemy[enemyIdx].ubPathLoops == TRUE) {
                s_tEnemy[enemyIdx].uwPathIdx = 0;
            } else {
                s_tEnemy[enemyIdx].bHealth = 0;
                s_ubActiveEnemies--;
            }
        }

        UWORD uwPathIdx = s_tEnemy[enemyIdx].uwPathArrayOffset + s_tEnemy[enemyIdx].uwPathIdx;
        s_tEnemy[enemyIdx].tPosition.uwX = s_uwPathData[uwPathIdx];
        s_tEnemy[enemyIdx].tPosition.uwY = s_uwPathData[uwPathIdx+1] + s_tEnemy[enemyIdx].uwPathYOffset;
        s_tEnemy[enemyIdx].sBob.sPos = s_tEnemy[enemyIdx].tPosition;

        // Check X bounds
        if ((s_tEnemy[enemyIdx].tPosition.uwX+s_tEnemyTypes[ubEnemyType].ubWidth) < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.uwX > uwCameraXMax)
        {
            s_tEnemy[enemyIdx].ubOnScreen = FALSE;
            continue;
        }

        // Check Y bounds
        if ((s_tEnemy[enemyIdx].tPosition.uwY+s_tEnemyTypes[ubEnemyType].ubHeight) < uwCameraYMin)
        {
            s_tEnemy[enemyIdx].ubOnScreen = FALSE;
            continue;
        }

        if (s_tEnemy[enemyIdx].tPosition.uwY > uwCameraYMax) {
            s_tEnemy[enemyIdx].ubOnScreen = FALSE;
            s_tEnemy[enemyIdx].bHealth = 0;
            s_ubActiveEnemies--;
        }

        // Enemy is on screen.
        s_tEnemy[enemyIdx].ubOnScreen = TRUE;

        #ifndef COLLISIONS_DISABLED
        // Check collision with player.
        if (s_ubPlayerAlive == TRUE) {
            UBYTE ubCollision = checkCollision(s_tPlayerPosition.uwX+PLAYER_HITBOX_OFFSET_X, s_tPlayerPosition.uwY+PLAYER_HITBOX_OFFSET_Y, PLAYER_HITBOX_WIDTH, PLAYER_HITBOX_HEIGHT, 
                                               s_tEnemy[enemyIdx].tPosition.uwX, s_tEnemy[enemyIdx].tPosition.uwY, s_tEnemyTypes[ubEnemyType].ubWidth, s_tEnemyTypes[ubEnemyType].ubHeight);
            if (ubCollision == TRUE) { 
                processPlayerDie();
            }
        }
        #endif

        // Fire projectile.
        if (s_tEnemy[enemyIdx].ubCanShoot == FALSE) { continue; }

        if (s_tEnemy[enemyIdx].ubCooldownTimer == 0) {
            for (UBYTE i=0; i<ENEMY_PROJECTILE_MAX; i++) {
                if (s_tSimpleEnemyProjectiles[i].ubAlive == 0) {
                    UWORD uwEnemyGunX = s_tEnemy[enemyIdx].tPosition.uwX + s_tEnemyTypes[ubEnemyType].ubGunOffsetX;
                    UWORD uwEnemyGunY = s_tEnemy[enemyIdx].tPosition.uwY + s_tEnemyTypes[ubEnemyType].ubGunOffsetY;
                    s_tSimpleEnemyProjectiles[i].uwX = uwEnemyGunX;
                    s_tSimpleEnemyProjectiles[i].uwY = uwEnemyGunY;
                    s_tSimpleEnemyProjectiles[i].bDeltaX = 0;
                    s_tSimpleEnemyProjectiles[i].bDeltaY = ENEMY_PROJECTILE_SPEED;
                    s_tSimpleEnemyProjectiles[i].ubAlive = 255;
                    s_tSimpleEnemyProjectiles[i].ubChannel = 255;
                    s_tEnemy[enemyIdx].ubCooldownTimer = s_tEnemyTypes[ubEnemyType].ubCooldownTime;
                    break;

                    // UWORD uwPlayerCenterX = s_tPlayerPosition.uwX + PLAYER_CENTER_OFFSET_X;
                    // UWORD uwPlayerCenterY = s_tPlayerPosition.uwY + PLAYER_CENTER_OFFSET_Y;
                    // UBYTE ubAngle = getAngleBetweenPoints(uwEnemyGunX, uwEnemyGunY, uwPlayerCenterX, uwPlayerCenterY);
                    // s_tEnemyProjectiles[i].fX = fix16_from_int(uwEnemyGunX - ENEMY_PROJECTILE_OFFSET_X);
                    // s_tEnemyProjectiles[i].fY = fix16_from_int(uwEnemyGunY);
                    // s_tEnemyProjectiles[i].fDeltaX = ccos(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    // s_tEnemyProjectiles[i].fDeltaY = csin(ubAngle) * ENEMY_PROJECTILE_SPEED;
                    // s_tEnemyProjectiles[i].fDeltaX = 0;
                    // s_tEnemyProjectiles[i].fDeltaY = fix16_from_int(ENEMY_PROJECTILE_SPEED);
                    // s_tEnemyProjectiles[i].ubAlive = 255;
                    // s_tEnemyProjectiles[i].ubChannel = 255;
                }
            }
        }        
    }
}

static void processPlayerDie() {
    if (s_ubPlayerIsInvincible == TRUE) { return; }
    s_tBigExplosionBob.sPos.uwX = s_tPlayerPosition.uwX - 16;
    s_tBigExplosionBob.sPos.uwY = s_tPlayerPosition.uwY - 16;
    s_ubBigExplosionActive = 1;
    audioMixerPlaySfx(s_pSfxExplosion, CHANNEL_FOR_EXPLOSION, 10, 0);
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

                // if (s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubWideSprite == TRUE || s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubSpreadShot == TRUE) {
                //     s_tPlayerSecondaryProjectiles[i].uwX = (s_tPlayerPosition.uwX + s_tPlayerProjectileTypes[s_ubEquippedProjectileType].ubXOffset2);
                //     s_tPlayerSecondaryProjectiles[i].ubType = s_ubEquippedProjectileType;
                // }

                if (s_ubAudioDelay == 0) {
                    audioMixerPlaySfx(s_pSfxLaser, CHANNEL_FOR_PLAYER, 8, 0);
                    s_ubAudioDelay = PLAYER_AUDIO_DELAY;
                }

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
            UWORD uwY = s_tPlayerProjectiles[projectileIdx].uwY;
            UBYTE ubType = s_tPlayerProjectiles[projectileIdx].ubType;
            BYTE bDeltaX = s_tPlayerProjectileTypes[ubType].bDeltaX;
            BYTE bDeltaY = s_tPlayerProjectileTypes[ubType].bDeltaY;
            UBYTE ubWidth = s_tPlayerProjectileTypes[ubType].ubWidth;
            UBYTE ubHeight = s_tPlayerProjectileTypes[ubType].ubHeight;
            UBYTE ubDamage = s_tPlayerProjectileTypes[ubType].ubDamage;
            UBYTE ubDieOnCollision = s_tPlayerProjectileTypes[ubType].ubDieOnCollision;
            
            //UWORD uwX2 = 0;
            //BYTE bDeltaX2 = s_tPlayerProjectileTypes[ubType].bDeltaX2;            
            //UBYTE ubSecondarySpriteIndex = s_tPlayerProjectileTypes[ubType].ubSecondarySpriteIndex;
            //UBYTE ubWideSprite = s_tPlayerProjectileTypes[ubType].ubWideSprite;
            //UBYTE ubSpreadShot = s_tPlayerProjectileTypes[ubType].ubSpreadShot;
            //UBYTE ubHasSecondary = FALSE;
            // if (ubWideSprite || ubSpreadShot) {
            //     ubHasSecondary = TRUE;
            //     uwX2 = s_tPlayerSecondaryProjectiles[projectileIdx].uwX;
            // }

            // Check if projectile if off screen.
            //if (uwY < (uwCameraYMin-ubHeight) || uwX < (uwCameraXMin-ubWidth) || uwX > uwCameraXMax) {
            if (uwY < (uwCameraYMin-ubHeight)) {
                s_tPlayerProjectiles[projectileIdx].ubAlive = 0;
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType);

                //movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);
                // if (ubHasSecondary) {
                //     movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE); 
                // }

                continue;
            }

            // Check collisions
            #ifndef COLLISIONS_DISABLED
            for (UBYTE enemyIdx=0; enemyIdx<ENEMY_MAX; enemyIdx++) {
                if (s_tEnemy[enemyIdx].bHealth <= 0) { continue; }

                // Get EnemyType
                UBYTE ubEnemyType = s_tEnemy[enemyIdx].ubEnemyType;

                // Check if enemy is on screen.
                if (s_tEnemy[enemyIdx].tPosition.uwX+s_tEnemyTypes[ubEnemyType].ubWidth < uwCameraXMin || s_tEnemy[enemyIdx].tPosition.uwX > uwCameraXMax) { continue; }
                if (s_tEnemy[enemyIdx].tPosition.uwY+s_tEnemyTypes[ubEnemyType].ubHeight < uwCameraYMin || s_tEnemy[enemyIdx].tPosition.uwY > uwCameraYMax) { continue; }

                // TODO: Rethink this approach, ends up basically doing collision detection twice. Unroll into individual checks? Include checks in checkCollision()?

                // Check if projectile is to far to the left of the enemy
                if (s_tEnemy[enemyIdx].tPosition.uwX+s_tEnemyTypes[ubEnemyType].ubWidth < uwX) { continue; }

                // Check if projectile is to far to the right of the enemy
                if (s_tEnemy[enemyIdx].tPosition.uwX > uwX+ubWidth) { continue; }

                // Check if projectile is above enemy
                if (s_tEnemy[enemyIdx].tPosition.uwY > uwY+(ubHeight+bDeltaY)) { continue; }

                // Test collision
                UBYTE ubCollision = checkCollision(uwX, uwY+bDeltaY, ubWidth, ubHeight, s_tEnemy[enemyIdx].tPosition.uwX, s_tEnemy[enemyIdx].tPosition.uwY, s_tEnemyTypes[ubEnemyType].ubWidth, s_tEnemyTypes[ubEnemyType].ubHeight);

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
                    s_tEnemy[enemyIdx].bHealth = 0;

                    tUwCoordYX tExplosionPosition = (tUwCoordYX){.uwX = s_tEnemy[enemyIdx].tPosition.uwX-8, .uwY = s_tEnemy[enemyIdx].tPosition.uwY-8};
                    createExplosionAtPosition(tExplosionPosition);

                    if (s_tEnemy[enemyIdx].ubPowerupType > 0) {
                        createPowerupAtPosition(s_tEnemy[enemyIdx].tPosition, s_tEnemy[enemyIdx].ubPowerupType);
                    }
                    
                    s_ulPlayerScore += s_tEnemy[enemyIdx].uwScoreValue;
                    s_ubUpdateScore = TRUE;
                    s_ubActiveEnemies--;
                }
            }
            #endif

            if (s_tPlayerProjectiles[projectileIdx].ubAlive == 0) {
                movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType);
                //movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);
                // if (ubHasSecondary) { 
                //     movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, -16, -16, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE); 
                // }
                continue;
            }

            UWORD uwScreenY = (uwY - s_pCamera->uPos.uwY);
            UWORD uwScreenX = (uwX - TILE_VIEWPORT_XMIN);

            movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, uwScreenX, uwScreenY, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType);
            //movePlayerProjectile(s_tPlayerProjectiles[projectileIdx].pCopBlock, uwScreenX, uwScreenY, ubHeight, PLAYER_SPRITE_CHANNEL_A, ubType, FALSE);

            // Secondary projectile
            // if (ubHasSecondary) {
            //     movePlayerProjectile(s_tPlayerSecondaryProjectiles[projectileIdx].pCopBlock, uwX2, uwScreenY, ubHeight, PLAYER_SPRITE_CHANNEL_B, ubSecondarySpriteIndex, TRUE);
            // }

            // Move projectile
            if (bDeltaX != 0) {
                s_tPlayerProjectiles[projectileIdx].uwX += bDeltaX;
                //s_tPlayerSecondaryProjectiles[projectileIdx].uwX += bDeltaX2;
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

// Utility functions.

//static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType, UBYTE ubSecondary) {
static void movePlayerProjectile(tCopBlock *pBlock, WORD wX, WORD wY, UWORD uwHeight, UBYTE ubChannel, UBYTE ubType) {
    UWORD uwVStart = s_pView->ubPosY + wY;
    UWORD uwVStop = uwVStart + uwHeight;
    UWORD uwHStart = (s_pView->ubPosX - 1) + wX;

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
    ulAddr = (ULONG)(
        (UBYTE*)s_pPlayerProjectileSprites[ubType]->pBitmap->Planes[0] +
        sizeof(g_pCustom->spr[ubChannel].pos) +
        sizeof(g_pCustom->spr[ubChannel].ctl)
    ); // skip the control words
        
    // if (ubSecondary == FALSE) {
    //     ulAddr = (ULONG)(
    //         (UBYTE*)s_pPlayerProjectileSprites[ubType]->pBitmap->Planes[0] +
    //         sizeof(g_pCustom->spr[ubChannel].pos) +
    //         sizeof(g_pCustom->spr[ubChannel].ctl)
    //     ); // skip the control words
    // } else {
    //     ulAddr = (ULONG)(
    //         (UBYTE*)s_pPlayerSecondaryProjectileSprites[ubType]->pBitmap->Planes[0] +
    //         sizeof(g_pCustom->spr[ubChannel].pos) +
    //         sizeof(g_pCustom->spr[ubChannel].ctl)
    //     );
    // }

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

static void createPowerupAtPosition(tUwCoordYX tPosition, UBYTE ubPowerupType) {
    if (s_ubLifePowActive == TRUE || s_ubSpecialPowActive == TRUE || s_ubWeaponPowActive == TRUE) { return; } // Only one active powerups at a time.

    switch (ubPowerupType) {
        case POWERUP_EXTRALIFE: {
            s_tLifePowBob.sPos = tPosition;
            s_ubLifePowActive = TRUE;
            break;
        }
        case POWERUP_SPECIAL: {
            s_tSpecialPowBob.sPos = tPosition;
            s_ubSpecialPowActive = TRUE;
            break;
        }
        case POWERUP_WEAPON: {
            s_tWeaponPowBob.sPos = tPosition;
            s_ubWeaponPowActive = TRUE;
            break;
        }                
    }
}

static void createExplosionAtPosition(tUwCoordYX tPosition) {
    if (s_ubActiveExplosions < EXPLOSIONS_MAX) {
        for (UBYTE explosionIdx=0; explosionIdx < EXPLOSIONS_MAX; explosionIdx++) {
            if (s_tExplosions[explosionIdx].ubAlive != 0) { continue; }
            s_tExplosions[explosionIdx].sBob.sPos = tPosition;
            s_tExplosions[explosionIdx].ubAlive = 1;
            s_tExplosions[explosionIdx].ubFrameIndex = 0;
            s_ubActiveExplosions++;
            audioMixerPlaySfx(s_pSfxExplosion, CHANNEL_FOR_EXPLOSION, 8, 0);
            break;
        }
    }
}

static UBYTE checkCollision(WORD x1, WORD y1, UBYTE w1, UBYTE h1, WORD x2, WORD y2, UBYTE w2, UBYTE h2) {
    return x1 < x2+w2 && x1+w1 > x2 && y1 < y2+h2 && y1+h1 > y2;
}
