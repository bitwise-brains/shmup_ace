#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include "main.h"

typedef struct _tHighScore {
    char cInitials[4];
    ULONG ulScore;
} tHighScore;

static void updatePalette(UBYTE ubIndex);
static void writeHighscore(void);
static void readHighscore(void);
static void renderText(void);

#endif // _HIGHSCORE_H_
