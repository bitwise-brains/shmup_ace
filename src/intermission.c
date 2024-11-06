#include "main.h"

void intermissionGsCreate(void) {
    systemUnuse();
}

void intermissionGsLoop(void) {
    // if (g_ubCurrentStage >= GAME_STAGES) {
    //     logWrite("LOADING BOSS STAGE");
    //     stateChange(g_pGameStateManager, &g_pGameStates[STATE_BOSS]);
    //     return;
    // }

    logWrite("LOADING STAGE: %d", g_ubCurrentStage);
    stateChange(g_pGameStateManager, &g_pGameStates[STATE_GAME]);
    return;
}

void intermissionGsDestroy(void) {
    systemUse();
    // Nothing to destroy...
}