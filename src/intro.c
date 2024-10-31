#include "intro.h"

static tView *s_pView;
static tVPort *s_pViewport;
static tSimpleBufferManager *s_pBuffer;

static tBitMap *s_tBrainsImage;
static tBitMap *s_tAceImage;
static tBitMap *s_tTitlescreenImage;
static tTextBitMap *s_pIntroText;
static tFont *s_pFont;
static tPtplayerMod *s_pIntroMusic;

static tIntroStage s_eIntroStage = INTRO_BRAINS;
static UBYTE ubFrameCounter = 0;
static UBYTE ubWaitTimer = 50;
static UBYTE ubDimLevel = 0;
static UBYTE ubFadeInComplete = FALSE;
static UBYTE ubFadeOutComplete = FALSE;
static UBYTE ubMusicVolume = 24;

static UWORD s_uwAcePalette[32] = {0};
static UWORD s_uwTitlescreenPalette[32] = {0};
static UWORD s_uwFadePalette[16][32] = {0};

static const char s_cIntroText[] = " ONE HUNDRED AND TWENTY EIGHT\n  YEARS AFTER THEIR INVASION\n  FLEET WAS DESTROYED A LONE\nBATTLESHIP OF THE BARRIX EMPIRE\n  APPEARS IN OUR SOLAR SYSTEM\n  THE RESULT OF AN FTL DRIVE\nMALFUNCTION THAT DELAYED THEIR\n           ARRIVAL\n  HEAVILY DAMAGED BY ORBITAL\nPLATFORMS THE BATTLESHIP RACES\n        TOWARDS EARTH\n IT IS UP TO YOU TO BOARD THE\n EXPERIMENTAL TTE1337 FIGHTER\n AND DEFEAT THE BARRIX EMPIRE\n       ONCE AND FOR ALL";

void introGsCreate(void) {
    s_pView = viewCreate(0,
        TAG_VIEW_GLOBAL_PALETTE, 1,
    TAG_DONE);

    s_pViewport = vPortCreate(0,
        TAG_VPORT_VIEW, s_pView,
        TAG_VPORT_BPP, GAME_BPP,
    TAG_DONE);

    s_pBuffer = simpleBufferCreate(0,
        TAG_SIMPLEBUFFER_VPORT, s_pViewport,
        TAG_SIMPLEBUFFER_USE_X_SCROLLING, 0,
        TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
        TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    TAG_DONE);

    // Load music
    ptplayerCreate(1);
    ptplayerSetChannelsForPlayer(0b1111);
    ptplayerSetMasterVolume(ubMusicVolume);
    s_pIntroMusic = ptplayerModCreate("data/intro.mod");
    ptplayerLoadMod(s_pIntroMusic, 0, 0);
    ptplayerEnableMusic(1);

    // Load assets
    s_pFont = fontCreate("data/hudfont.fnt");
    s_pIntroText = fontCreateTextBitMap(288, 128);
    s_tBrainsImage = bitmapCreateFromFile("data/splash_brains.bm", 0);
    s_tAceImage = bitmapCreateFromFile("data/splash_ace.bm", 0);
    s_tTitlescreenImage = bitmapCreateFromFile("data/titlescreen.bm", 0);

    // Generate fade palette lookup
    paletteLoad("data/titlescreen.plt", s_uwTitlescreenPalette, 32);
    paletteLoad("data/splash_ace.plt", s_uwAcePalette, 32);
    paletteLoad("data/splash_brains.plt", s_uwFadePalette[15], 32);
    for (UBYTE i=0; i<16; i++) {
        paletteDim(s_uwFadePalette[15], s_uwFadePalette[i], 32, i);
    }

    // Blit first logo
    blitCopy(s_tBrainsImage, 0, 0, s_pBuffer->pBack, 113, 58, 96, 142, MINTERM_COOKIE);
    viewLoad(s_pView);
    systemUnuse();
}

void introGsLoop(void) {
    if(keyCheck(KEY_ESCAPE)) {
        ptplayerSetMasterVolume(0);
        ptplayerStop();        
        gameExit();
    }

	if (keyUse(KEY_SPACE) || joyCheck(JOY1_FIRE)) {
        ptplayerSetMasterVolume(0);
        ptplayerStop();
		stateChange(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
        return;
	}

    // Only process every 5 frames.
    if (ubFrameCounter >=5) {
        ubFrameCounter = 0;
        updatePalette(ubDimLevel);

        // Fade in finished?
        if (ubFadeInComplete == FALSE && ubDimLevel == 15) {
            ubFadeInComplete = TRUE;
        }

        // Fade out finished?
        if (ubFadeInComplete == TRUE && ubFadeOutComplete == FALSE && ubDimLevel == 0) {
            ubFadeOutComplete = TRUE;
            ubWaitTimer = 50;
        }

        if (ubFadeInComplete == FALSE && ubDimLevel < 15) {
            ubDimLevel++;
        }

        if (ubFadeInComplete == TRUE && ubDimLevel > 0 && ubWaitTimer == 0) {
            ubDimLevel--;
        }
        
        if (ubDimLevel == 15 && ubWaitTimer > 0) {
            ubWaitTimer--;
        }
    }

    if (ubFadeInComplete == TRUE && ubFadeOutComplete == TRUE) {
        ubFadeInComplete = FALSE;
        ubFadeOutComplete = FALSE;

        switch (s_eIntroStage) {
            case INTRO_BRAINS:
                blitRect(s_pBuffer->pBack, 113, 58, 96, 142, 0); // Clear
                for (UBYTE i=0; i<16; i++) {
                    paletteDim(s_uwAcePalette, s_uwFadePalette[i], 32, i);
                }                
                blitCopy(s_tAceImage, 0, 0, s_pBuffer->pBack, 53, 73, 224, 114, MINTERM_COOKIE);
                s_eIntroStage = INTRO_ACE;
                break;
            case INTRO_ACE:
                for (UBYTE i=0; i<16; i++) {
                    paletteDim(s_uwTitlescreenPalette, s_uwFadePalette[i], 32, i);
                }             
                blitCopyAligned(s_tTitlescreenImage, 0, 0, s_pBuffer->pBack, 0, 0, 320, 256);
                s_eIntroStage = INTRO_TITLE;
                break;
            case INTRO_TITLE:
                fontDrawStr(s_pFont, s_pBuffer->pBack, 28, 128, s_cIntroText, 19, FONT_SHADOW | FONT_COOKIE, s_pIntroText);
                s_eIntroStage = INTRO_TEXT;
                ubWaitTimer = 255;
                break;
            case INTRO_TEXT:
                ptplayerSetMasterVolume(0);
                ptplayerEnableMusic(0);
                stateChange(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
                return;
            case INTRO_FINISHED:
                break;
        }
    }

    //viewUpdateGlobalPalette(s_pView);
    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pViewport);
    systemIdleEnd();
    ubFrameCounter++;
}

void introGsDestroy(void) {
    systemUse();
    bitmapDestroy(s_tBrainsImage);
    bitmapDestroy(s_tAceImage);
    bitmapDestroy(s_tTitlescreenImage);
    fontDestroy(s_pFont);
    ptplayerModDestroy(s_pIntroMusic);
    ptplayerDestroy();
    viewDestroy(s_pView);
}

static void updatePalette(UBYTE ubIndex) {
    for (UBYTE i=0; i<32; i++) {
        s_pViewport->pPalette[i] = s_uwFadePalette[ubIndex][i];
    }

    viewUpdateGlobalPalette(s_pView);
}