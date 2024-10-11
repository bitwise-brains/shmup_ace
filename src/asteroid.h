#include "game.h"

#define NUM_FRAMES_ASTEROID 16

//const ULONG asteroidAnimOffset[] = {0, 0, 640, 640, 1280, 1280, 1920, 1920, 2560, 2560, 3200, 3200, 3840, 3840, 4480, 4480};

typedef struct _tAsteroid {
    tBob sBob;
    WORD wX;
    BYTE bHealth;
    UBYTE ubMovementSpeed;
    UBYTE ubFrameIndex;
    UBYTE ubOffsetIndex;
} tAsteroid;