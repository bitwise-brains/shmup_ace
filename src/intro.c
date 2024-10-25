#include "game.h"
#include <ace/managers/key.h>
#include <ace/managers/joy.h>
#include <ace/managers/state.h>

static tView *s_pView;
static tVPort *s_pIntroViewport;
static tSimpleBufferManager *s_pIntroBuffer;

static tBitMap *s_tAceLogo;

void introGsCreate(void) {
    s_pView = viewCreate(0,
        TAG_VIEW_GLOBAL_PALETTE, 1,
    TAG_DONE);

    s_pIntroViewport = vPortCreate(0,
        TAG_VPORT_VIEW, s_pView,
        TAG_VPORT_BPP, GAME_BPP,
    TAG_DONE);

    paletteLoad("data/acelogo.plt", s_pIntroViewport->pPalette, 1 << GAME_BPP);

    s_pIntroBuffer = simpleBufferCreate(0,
        TAG_SIMPLEBUFFER_VPORT, s_pIntroViewport,
        TAG_SIMPLEBUFFER_USE_X_SCROLLING, 0,
        TAG_SIMPLEBUFFER_IS_DBLBUF, 0,
        TAG_SIMPLEBUFFER_BITMAP_FLAGS, BMF_CLEAR,
    TAG_DONE);

    paletteLoad("data/splash_ace.plt", s_pIntroViewport->pPalette, 1 << GAME_BPP);
    s_tAceLogo = bitmapCreateFromFile("data/splash_ace.bm", 0);
    blitCopy(s_tAceLogo, 0, 0, s_pIntroBuffer->pBack, 53, 73, 224, 112, MINTERM_COOKIE);
    viewLoad(s_pView);
    systemUnuse();
}

void introGsLoop(void) {
    if(keyCheck(KEY_ESCAPE)) {
        gameExit();
    }

	if (keyUse(KEY_SPACE)) {
		stateChange(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
        //statePush(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
	}

    viewProcessManagers(s_pView);
    copProcessBlocks();
    systemIdleBegin();
    vPortWaitForEnd(s_pIntroViewport);
    systemIdleEnd();
}

void introGsDestroy(void) {
    systemUse();
    bitmapDestroy(s_tAceLogo);
    viewDestroy(s_pView);
}