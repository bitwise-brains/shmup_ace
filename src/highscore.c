#include "highscore.h"

static tView *s_pView;
static tVPort *s_pViewport;
static tSimpleBufferManager *s_pBuffer;

static tFont *s_pFont;
static tPtplayerMod *s_pMusic;
static tBitMap *s_tTitleImage;
static tBitMap *s_tCreditsImage;
static tTextBitMap *s_pScoreText;
static tTextBitMap *s_pGreetsText;

static UWORD s_uwFadePalette[16][32] = {0};
static UBYTE s_ubDimLevel = 0;
static UBYTE s_ubFadeIn = TRUE;
static UBYTE s_ubDisplayCredits = FALSE;
static UWORD s_uwHighScoreTimer = 0;
static UBYTE s_ubEnterInitials = FALSE;
static UBYTE s_ubSaveHighscore = FALSE;
static UBYTE s_ubInitialsYPos = 0;
static char s_cInitialsInput[4] = {0};
static char s_cGreets[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ01\nABCDEFGHIJKLMNOPQRSTUVWXYZ02\nABCDEFGHIJKLMNOPQRSTUVWXYZ03\nABCDEFGHIJKLMNOPQRSTUVWXYZ04\nABCDEFGHIJKLMNOPQRSTUVWXYZ05\nABCDEFGHIJKLMNOPQRSTUVWXYZ06\nABCDEFGHIJKLMNOPQRSTUVWXYZ07\nABCDEFGHIJKLMNOPQRSTUVWXYZ08";
static UBYTE s_ubInitialsIndex = 0;
static UBYTE s_ubTableIndex = 0;
static tHighScore s_tHighScores[HIGHSCORE_MAX] = {0};

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
    s_pGreetsText = fontCreateTextBitMap(256, 64);
    fontFillTextBitMap(s_pFont, s_pGreetsText, s_cGreets);

    // Load palette.
    paletteLoad("data/highscore.plt", s_uwFadePalette[15], 32);
    for (UBYTE i=0; i<16; i++) {
        paletteDim(s_uwFadePalette[15], s_uwFadePalette[i], 32, i);
    }

    // Blit title image.
    s_tTitleImage = bitmapCreateFromFile("data/text_title.bm", 0);
    blitCopy(s_tTitleImage, 0, 0, s_pBuffer->pBack, 48, 16, 224, 80, MINTERM_COOKIE);

    // Load credits image
    s_tCreditsImage = bitmapCreateFromFile("data/text_credits.bm", 0);

    // Reset values
    s_ubDimLevel = 0;
    s_ubFadeIn = TRUE;
    s_ubDisplayCredits = FALSE;
    s_uwHighScoreTimer = 0;
    s_ubEnterInitials = FALSE;
    s_ubSaveHighscore = FALSE;
    s_ubInitialsYPos = 0;
    s_ubInitialsIndex = 0;
    s_ubTableIndex = 0;

    // Load highscore data.
	tFile *pFile = fileOpen("data/highscore.dat", "rb");
	fileRead(pFile, s_tHighScores, sizeof(s_tHighScores));
    fileClose(pFile);

    logWrite("Player score: %ld", g_ulPlayerScore);
    if (g_ulPlayerScore > HIGHSCORE_VALUE_MAX) { g_ulPlayerScore = HIGHSCORE_VALUE_MAX; }

    // Check player score against high score table
    for (UBYTE tableIdx=0; tableIdx<HIGHSCORE_MAX; tableIdx++) {
        if (g_ulPlayerScore > s_tHighScores[tableIdx].ulScore) {
            // Shift down
            for (UBYTE shiftIdx=HIGHSCORE_MAX-1; shiftIdx > tableIdx; shiftIdx--) {
                s_tHighScores[shiftIdx] = s_tHighScores[shiftIdx-1];
            }

            s_ubEnterInitials = TRUE; // Player has a high score.
            s_ubInitialsYPos = HIGHSCORE_YPOS + (HIGHSCORE_SPACING * tableIdx); // Text offset.
            s_ubTableIndex = tableIdx;
            memset(s_tHighScores[tableIdx].cInitials, 0, sizeof(s_tHighScores[tableIdx].cInitials));
            s_tHighScores[tableIdx].ulScore = g_ulPlayerScore;
            s_ubSaveHighscore = TRUE;
            break;
        }
    }

    renderText();
    systemUnuse();

    viewLoad(s_pView);
}

void highscoreGsLoop(void) {
    // Fade in
    if (s_ubFadeIn == TRUE && s_ubDimLevel < 15) {
        s_ubDimLevel++;
        if (s_ubDimLevel == 15) { s_ubFadeIn = FALSE; }

        updatePalette(s_ubDimLevel);
        viewProcessManagers(s_pView);
        copProcessBlocks();
        systemIdleBegin();
        vPortWaitForEnd(s_pViewport);
        systemIdleEnd();          
        return;
    }

    // Input
    if(keyCheck(KEY_ESCAPE)) {
        ptplayerSetMasterVolume(0);
        ptplayerStop();
        if (s_ubSaveHighscore == TRUE) {
            writeHighscore();
        }        
        gameExit();
    }

    // Restart
    if (s_ubEnterInitials == FALSE && s_ubFadeIn == FALSE) {
        if (keyUse(KEY_SPACE) || joyCheck(JOY1_FIRE)) {
            if (s_ubSaveHighscore == TRUE) {
                writeHighscore();
            }
            ptplayerSetMasterVolume(0);
            ptplayerStop();
            stateChange(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
            return;
        }
    }

    // Enter high score initials
    if (s_ubEnterInitials == TRUE && s_ubInitialsIndex < 3 && s_ubFadeIn == FALSE) {
        if (keyUse(KEY_BACKSPACE)) {
            if (s_ubInitialsIndex > 0) { s_ubInitialsIndex--; }
        }

        if(keyUse(g_sKeyManager.ubLastKey)) {
            UBYTE ubInput = g_pToAscii[g_sKeyManager.ubLastKey];
            ubInput -= 'a' - 'A';
            if (ubInput >= 'A' && ubInput <= 'Z') {
                s_cInitialsInput[s_ubInitialsIndex] = ubInput;
                logWrite("INPUT: %c / %c%c%c", ubInput, s_cInitialsInput[0], s_cInitialsInput[1], s_cInitialsInput[2]);

                char cHighscoreText[16];
                sprintf(cHighscoreText, "%c%c%c", s_cInitialsInput[0], s_cInitialsInput[1], s_cInitialsInput[2]);
                fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_L, s_ubInitialsYPos, cHighscoreText, HIGHSCORE_COLOR, 0, s_pScoreText);

                s_ubInitialsIndex++;
                if (s_ubInitialsIndex == 3) {
                    memcpy(s_tHighScores[s_ubTableIndex].cInitials, s_cInitialsInput, sizeof(s_tHighScores[s_ubTableIndex].cInitials));
                    fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_L+10, HIGHSCORE_YPOS+((HIGHSCORE_SPACING*HIGHSCORE_MAX)+(HIGHSCORE_SPACING+8)), "  PRESS FIRE  ", HIGHSCORE_COLOR, 0, s_pScoreText);
                    s_ubEnterInitials = FALSE;
                }                
            } else {
                logWrite("Invalid input!");
            }
        }
    }

    // Cycle between high scores and credits.
    if (s_ubEnterInitials == FALSE) {
        s_uwHighScoreTimer++;
        if (s_uwHighScoreTimer >= HIGHSCORE_TIMER) {
            s_uwHighScoreTimer = 0;
            s_ubDisplayCredits = !s_ubDisplayCredits;

            if (s_ubDisplayCredits == TRUE) {
                blitRect(s_pBuffer->pBack, HIGHSCORE_OFFSET_L, (CREDITS_YPOS+CREDITS_HEIGHT), 208, 76, 0); // Clear scores.
                blitCopy(s_tCreditsImage, 0, 0, s_pBuffer->pBack, CREDITS_XPOS, CREDITS_YPOS, CREDITS_WIDTH, CREDITS_HEIGHT, MINTERM_COOKIE);
                fontDrawTextBitMap(s_pBuffer->pBack, s_pGreetsText, GREETS_XPOS, GREETS_YPOS, HIGHSCORE_COLOR, 0);
                
            } else {
                blitRect(s_pBuffer->pBack, CREDITS_XPOS, CREDITS_YPOS, CREDITS_WIDTH, CREDITS_HEIGHT, 0); // Clear credits.
                blitRect(s_pBuffer->pBack, GREETS_XPOS, GREETS_YPOS, GREETS_WIDTH, GREETS_HEIGHT, 0); // Clear greets.
                renderText();
            }            
        }
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
    bitmapDestroy(s_tCreditsImage);
    fontDestroyTextBitMap(s_pScoreText);
    fontDestroyTextBitMap(s_pGreetsText);
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
    // TODO: Render to bitmap instead of fontDrawStr.
    UWORD uwYOffset = HIGHSCORE_YPOS;
    for (UBYTE i=0; i<HIGHSCORE_MAX; i++) {
        char cHighscoreText[10] = {0};
        sprintf(cHighscoreText, "%09ld", s_tHighScores[i].ulScore);
        fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_L, uwYOffset, s_tHighScores[i].cInitials, HIGHSCORE_COLOR, 0, s_pScoreText);
        fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_R, uwYOffset, cHighscoreText, HIGHSCORE_COLOR, 0, s_pScoreText);
        uwYOffset += HIGHSCORE_SPACING;
    }

    uwYOffset += HIGHSCORE_SPACING+8;
    if (s_ubEnterInitials == TRUE) {
        fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_L+10, uwYOffset, "ENTER INITIALS", HIGHSCORE_COLOR, 0, s_pScoreText);
    } else {
        fontDrawStr(s_pFont, s_pBuffer->pBack, HIGHSCORE_OFFSET_L+10, uwYOffset, "  PRESS FIRE", HIGHSCORE_COLOR, 0, s_pScoreText);
    }
    
}