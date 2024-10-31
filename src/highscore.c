#include "highscore.h"

static tView *s_pView;
static tVPort *s_pViewport;
static tSimpleBufferManager *s_pBuffer;

static tFont *s_pFont;
static tPtplayerMod *s_pMusic;
static tBitMap *s_tTitleImage;
static tTextBitMap *s_pScoreText;

static UWORD s_uwFadePalette[16][32] = {0};
static UBYTE s_ubDimLevel = 0;

static tHighScore s_tHighScores[8] = {0};

void highscoreGsCreate(void) {
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

    // Load highscore data.
	tFile *pFile = fileOpen("data/highscore.dat", "rb");
	fileRead(pFile, s_tHighScores, sizeof(s_tHighScores));
    fileClose(pFile);

    logWrite("Player score: %07ld", g_ulPlayerScore);
    
    // Load music.
    ptplayerCreate(1);
    ptplayerSetChannelsForPlayer(0b1111);
    ptplayerSetMasterVolume(24);
    s_pMusic = ptplayerModCreate("data/intro.mod");
    ptplayerLoadMod(s_pMusic, 0, 0);
    ptplayerEnableMusic(1);

    // Load font and create text bitmap.
    s_pFont = fontCreate("data/hudfont.fnt");
    s_pScoreText = fontCreateTextBitMap(128, 128);

    // Load palette.
    paletteLoad("data/game.plt", s_uwFadePalette[15], 32);
    for (UBYTE i=0; i<16; i++) {
        paletteDim(s_uwFadePalette[15], s_uwFadePalette[i], 32, i);
    }

    // Blit title image.
    s_tTitleImage = bitmapCreateFromFile("data/text_title.bm", 0);
    blitCopy(s_tTitleImage, 0, 0, s_pBuffer->pBack, 48, 16, 224, 80, MINTERM_COOKIE);
    renderText();

    s_ubDimLevel = 0;

    viewLoad(s_pView);
    systemUnuse();    
}

void highscoreGsLoop(void) {
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

    if (s_ubDimLevel < 15) {
        s_ubDimLevel++;
        updatePalette(s_ubDimLevel);
    }

    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pViewport);
    systemIdleEnd();  
}

void highscoreGsDestroy(void) {
    systemUse();
    fontDestroy(s_pFont);
    bitmapDestroy(s_tTitleImage);
    fontDestroyTextBitMap(s_pScoreText);
    ptplayerModDestroy(s_pMusic);
    ptplayerDestroy();
    spriteManagerDestroy();
    viewDestroy(s_pView);
}

static void updatePalette(UBYTE ubIndex) {
    for (UBYTE i=0; i<32; i++) {
        s_pViewport->pPalette[i] = s_uwFadePalette[ubIndex][i];
    }

    viewUpdateGlobalPalette(s_pView);
}

static void writeHighscore(void) {
	systemUse();
	tFile *pFile = fileOpen("data/highscore.dat", "wb");
	fileWrite(pFile, s_tHighScores, sizeof(s_tHighScores));
    fileClose(pFile);
	systemUnuse();
}

static void renderText(void) {
    UWORD uwYOffset = 108;
    for (UBYTE i=0; i<8; i++) {
        char cHighscoreText[16];
        sprintf(cHighscoreText, "%c%c%c    %07ld", s_tHighScores[i].cInitials[0], s_tHighScores[i].cInitials[1], s_tHighScores[i].cInitials[2], s_tHighScores[i].ulScore);
        fontDrawStr(s_pFont, s_pBuffer->pBack, 99, uwYOffset, cHighscoreText, 27, 0, s_pScoreText);
        uwYOffset += 12;
    }
}