#include "main.h"

static tView *s_pView;
static tVPort *s_pViewport;
static tSimpleBufferManager *s_pBuffer;

static tBitMap *s_pTitlescreenImage;
static tTextBitMap *s_pWinText;
static tFont *s_pFont;
static tPtplayerMod *s_pWinMusic;

static UBYTE s_ubDimLevel = 0;
static UBYTE s_ubFadeInComplete = FALSE;
static UBYTE s_ubWaitTimer = 50;
static UWORD s_uwFadePalette[16][32] = {0};

static const char s_cWinText[] = "END OF DEMO\n\nPRESS SPACE TO CONTINUE.";

void winGsCreate(void) {
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

    spriteManagerCreate(s_pView, 0);
    systemSetDmaBit(DMAB_SPRITE, 0);

    // Load music
    ptplayerCreate(1);
    ptplayerSetChannelsForPlayer(0b1111);
    ptplayerSetMasterVolume(56);
    
    s_pWinMusic = ptplayerModCreateFromFd(pakFileGetFile(g_pPakFile, "intro.mod"));
    ptplayerLoadMod(s_pWinMusic, 0, 0);
    ptplayerEnableMusic(1);

    // Load assets
    s_pFont = fontCreateFromFd(pakFileGetFile(g_pPakFile, "hudfont.fnt"));
    s_pWinText = fontCreateTextBitMap(288, 128);
    s_pTitlescreenImage = bitmapCreateFromFd(pakFileGetFile(g_pPakFile, "text_title.bm"), 0);

    // Generate fade palette lookup
    paletteLoadFromFd(pakFileGetFile(g_pPakFile, "game.plt"), s_uwFadePalette[15], 32);
    for (UBYTE i=0; i<16; i++) {
        paletteDim(s_uwFadePalette[15], s_uwFadePalette[i], 32, i);
    }

    // Blit titlescreen
    blitCopy(s_pTitlescreenImage, 0, 0, s_pBuffer->pBack, 48, 16, 224, 80, MINTERM_COOKIE);
    fontDrawStr(s_pFont, s_pBuffer->pBack, 28, 128, s_cWinText, 19, FONT_SHADOW | FONT_COOKIE, s_pWinText);
    viewLoad(s_pView);
    systemUnuse();
}

void winGsLoop(void) {
    ptplayerProcess();
    
    if(keyCheck(KEY_ESCAPE)) {
        ptplayerSetMasterVolume(0);
        ptplayerStop();        
        gameExit();
    }

    // Fade in.
    if (s_ubFadeInComplete == FALSE && s_ubDimLevel != 15) {
        s_ubDimLevel++;
        if (s_ubDimLevel == 15) { s_ubFadeInComplete = TRUE; }

        for (UBYTE i=0; i<32; i++) {
            s_pViewport->pPalette[i] = s_uwFadePalette[s_ubDimLevel][i];
        }

        viewUpdateGlobalPalette(s_pView);
        viewProcessManagers(s_pView);
        copProcessBlocks();
        systemIdleBegin();
        vPortWaitForEnd(s_pViewport);
        systemIdleEnd();          
        return;
    }

    if (s_ubWaitTimer == 0) {
        if (keyUse(KEY_SPACE) || joyCheck(JOY1_FIRE)) {
            ptplayerSetMasterVolume(0);
            ptplayerStop();
            stateChange(g_pGameStateManager, &g_pGameStates[STATE_SCORE]);
            return;
        }
    }

    if (s_ubWaitTimer > 0) { s_ubWaitTimer--; }
    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pViewport);
    systemIdleEnd();
}

void winGsDestroy(void) {
    systemUse();
    fontDestroyTextBitMap(s_pWinText);
    fontDestroy(s_pFont);
    ptplayerModDestroy(s_pWinMusic);
    ptplayerDestroy();
    bitmapDestroy(s_pTitlescreenImage);
    spriteManagerDestroy();
    viewDestroy(s_pView);   
}