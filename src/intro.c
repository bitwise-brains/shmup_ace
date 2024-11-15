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
static UBYTE s_ubFrameCounter = 0;
static UWORD s_uwWaitTimer = 100;
static UBYTE s_ubDimLevel = 0;
static UBYTE s_ubFadeInComplete = FALSE;
static UBYTE s_ubFadeOutComplete = FALSE;

static UWORD s_uwAcePalette[32] = {0};
static UWORD s_uwTitlescreenPalette[32] = {0};
static UWORD s_uwFadePalette[16][32] = {0};
static UWORD s_uwGradient[] = {0xff3,0xfd3,0xea2,0xd82,0xc61,0xc31,0xb10};

static const char s_cIntroText[] = " ONE HUNDRED AND TWENTY EIGHT\n  YEARS AFTER THEIR INVASION\nFLEET WAS DESTROYED A LONE\nBATTLESHIP OF THE BARRIX EMPIRE\n APPEARS IN OUR SOLAR SYSTEM,\n  THE RESULT OF AN FTL DRIVE\nMALFUNCTION THAT DELAYED THEIR\n          ARRIVAL.\n  HEAVILY DAMAGED BY ORBITAL\nPLATFORMS THE BATTLESHIP RACES\n       TOWARDS EARTH,\n IT IS UP TO YOU TO BOARD THE\n EXPERIMENTAL TTE1337 FIGHTER\n AND DEFEAT THE BARRIX EMPIRE\n      ONCE AND FOR ALL!";

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
    ptplayerSetMasterVolume(56);
    s_pIntroMusic = ptplayerModCreate("data/intro.mod");
    ptplayerLoadMod(s_pIntroMusic, 0, 0);
    ptplayerEnableMusic(1);

    // Load assets
    s_pFont = fontCreate("data/hudfont.fnt");
    s_pIntroText = fontCreateTextBitMap(288, 128);
    s_tBrainsImage = bitmapCreateFromFile("data/splash_brains.bm", 0);
    s_tAceImage = bitmapCreateFromFile("data/splash_ace.bm", 0);
    s_tTitlescreenImage = bitmapCreateFromFile("data/text_title.bm", 0);
    
    // Generate fade palette lookup
    paletteLoad("data/highscore.plt", s_uwTitlescreenPalette, 32);
    paletteLoad("data/splash_ace.plt", s_uwAcePalette, 32);
    paletteLoad("data/splash_brains.plt", s_uwFadePalette[15], 32);
    for (UBYTE i=0; i<16; i++) {
        paletteDim(s_uwFadePalette[15], s_uwFadePalette[i], 32, i);
    }

    // Setup score text gradient
    UWORD uwGradientYPos = 128;
    tCopBlock *colorCopBlock = copBlockCreate(s_pView->pCopList, 1, 0, uwGradientYPos);
    while (uwGradientYPos < 276) {
        UWORD uwYPos = uwGradientYPos;
        
        for (UBYTE i=0; i<7; i++) {
            copMove(s_pView->pCopList, colorCopBlock, &g_pCustom->color[31], s_uwGradient[i]);
            uwYPos++;
            colorCopBlock = copBlockCreate(s_pView->pCopList, 1, 0, uwYPos);
        }

        uwGradientYPos += 8;
    }
    colorCopBlock = copBlockCreate(s_pView->pCopList, 1, 0, uwGradientYPos);
    copMove(s_pView->pCopList, colorCopBlock, &g_pCustom->color[31], 0xfff);

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
    if (s_ubFrameCounter >=5) {
        s_ubFrameCounter = 0;
        updatePalette(s_ubDimLevel);
        
        // Fade in finished?
        if (s_ubFadeInComplete == FALSE && s_ubDimLevel == 15) {
            s_ubFadeInComplete = TRUE;
        }

        // Fade out finished?
        if (s_ubFadeInComplete == TRUE && s_ubFadeOutComplete == FALSE && s_ubDimLevel == 0) {
            s_ubFadeOutComplete = TRUE;
            s_uwWaitTimer = 100;
        }

        if (s_ubFadeInComplete == FALSE && s_ubDimLevel < 15) {
            s_ubDimLevel++;
        }

        if (s_ubFadeInComplete == TRUE && s_ubDimLevel > 0 && s_uwWaitTimer == 0) {
            s_ubDimLevel--;
        }
        
        if (s_ubDimLevel == 15 && s_uwWaitTimer > 0) {
            s_uwWaitTimer--;
        }
    }

    if (s_ubFadeInComplete == TRUE && s_ubFadeOutComplete == TRUE) {
        s_ubFadeInComplete = FALSE;
        s_ubFadeOutComplete = FALSE;

        switch (s_eIntroStage) {
            case INTRO_BRAINS:
                blitRect(s_pBuffer->pBack, 113, 58, 96, 142, 0); // Clear BitwiseBrains logo
                for (UBYTE i=0; i<16; i++) {
                    paletteDim(s_uwAcePalette, s_uwFadePalette[i], 32, i);
                }                
                blitCopy(s_tAceImage, 0, 0, s_pBuffer->pBack, 53, 73, 224, 114, MINTERM_COOKIE);
                s_eIntroStage = INTRO_ACE;
                break;
            case INTRO_ACE:
                blitRect(s_pBuffer->pBack, 53, 73, 224, 114, 0); // Clear ACE logo
                for (UBYTE i=0; i<16; i++) {
                    paletteDim(s_uwTitlescreenPalette, s_uwFadePalette[i], 32, i);
                }
                blitCopy(s_tTitlescreenImage, 0, 0, s_pBuffer->pBack, 48, 16, 224, 80, MINTERM_COOKIE);            
                s_eIntroStage = INTRO_TITLE;
                break;
            case INTRO_TITLE:
                fontDrawStr(s_pFont, s_pBuffer->pBack, 29, 109, s_cIntroText, 1, 0, s_pIntroText);
                fontDrawStr(s_pFont, s_pBuffer->pBack, 28, 108, s_cIntroText, 31, FONT_COOKIE, s_pIntroText);
                s_eIntroStage = INTRO_TEXT;
                s_uwWaitTimer = 525;
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

    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pViewport);
    systemIdleEnd();
    s_ubFrameCounter++;
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