#ifndef _MAIN_H_
#define _MAIN_H_

#include <ace/types.h>
#include <ace/generic/screen.h>
#include <ace/managers/game.h>
#include <ace/managers/viewport/simplebuffer.h>
#include <ace/managers/viewport/tilebuffer.h>
#include <ace/managers/blit.h>
#include <ace/managers/bob.h>
#include <ace/managers/copper.h>
#include <ace/managers/memory.h>
#include <ace/managers/key.h>
#include <ace/managers/joy.h>
#include <ace/managers/ptplayer.h>
#include <ace/managers/rand.h>
#include <ace/managers/sprite.h>
#include <ace/managers/state.h>
#include <ace/managers/system.h>
#include <ace/utils/font.h>
#include <ace/utils/palette.h>
#include <ace/utils/string.h>
#include <fixmath/fixmath.h>
#include "../ace_audio_mixer/include/ace/contrib/managers/audio_mixer.h"

#define GAME_BPP 5
#define GAME_STATES 3

void introGsCreate(void);
void introGsLoop(void);
void introGsDestroy(void);

void gameGsCreate(void);
void gameGsLoop(void);
void gameGsDestroy(void);

void highscoreGsCreate(void);
void highscoreGsLoop(void);
void highscoreGsDestroy(void);

typedef enum tGameState {
    STATE_INTRO,
    STATE_GAME,
    STATE_SCORE,
} tGameState;

extern tStateManager *g_pGameStateManager;
extern tState g_pGameStates[GAME_STATES];
extern ULONG g_ulPlayerScore;

#endif // _MAIN_H_