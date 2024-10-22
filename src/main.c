#include <ace/generic/main.h>
#include <ace/managers/key.h>
#include <ace/managers/joy.h>
#include <ace/managers/state.h>
// Without it compiler will yell about undeclared gameGsCreate etc
#include "game.h"

tStateManager *g_pGameStateManager = 0;
tState *g_pGameState = 0;

void genericCreate(void) {
  // Here goes your startup code
  keyCreate(); // We'll use keyboard
  joyOpen();
  // Initialize gamestate
  g_pGameStateManager = stateManagerCreate();
  g_pGameState = stateCreate(gameGsCreate, gameGsLoop, gameGsDestroy, 0, 0);

  statePush(g_pGameStateManager, g_pGameState);
}

void genericProcess(void) {
  keyProcess();
  joyProcess();
  stateProcess(g_pGameStateManager); // Process current gamestate's loop
}

void genericDestroy(void) {
  stateManagerDestroy(g_pGameStateManager);
  stateDestroy(g_pGameState);
  keyDestroy();
  joyClose();
}