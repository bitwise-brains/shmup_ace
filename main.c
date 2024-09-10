#include <ace/generic/main.h>
#include <ace/managers/key.h>
#include <ace/managers/state.h>
#include "game.h"

tStateManager *g_pGameStateManager = 0;
tState *g_pGameState = 0;

void genericCreate(void) {
  keyCreate();
  g_pGameStateManager = stateManagerCreate();
  g_pGameState = stateCreate(gameGsCreate, gameGsLoop, gameGsDestroy, 0, 0);
  statePush(g_pGameStateManager, g_pGameState);
}

void genericProcess(void) {
  keyProcess();
  stateProcess(g_pGameStateManager);
}

void genericDestroy(void) {
  stateManagerDestroy(g_pGameStateManager);
  stateDestroy(g_pGameState);
  keyDestroy();
}