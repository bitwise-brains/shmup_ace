#include <ace/generic/main.h>
#include "main.h"

tStateManager *g_pGameStateManager = 0;
tState g_pGameStates[GAME_STATES] = {
  [STATE_INTRO] = {.cbCreate = introGsCreate, .cbLoop = introGsLoop, .cbDestroy = introGsDestroy},
  [STATE_GAME] = {.cbCreate = gameGsCreate, .cbLoop = gameGsLoop, .cbDestroy = gameGsDestroy},
  [STATE_SCORE] = {.cbCreate = highscoreGsCreate, .cbLoop = highscoreGsLoop, .cbDestroy = highscoreGsDestroy},
};

ULONG g_ulPlayerScore = 0;

void genericCreate(void) {
  keyCreate();
  joyOpen();
  g_pGameStateManager = stateManagerCreate();
  stateChange(g_pGameStateManager, &g_pGameStates[STATE_INTRO]);
}

void genericProcess(void) {
  keyProcess();
  joyProcess();
  stateProcess(g_pGameStateManager);
}

void genericDestroy(void) {
  stateManagerDestroy(g_pGameStateManager);
  keyDestroy();
  joyClose();
}