#include <ace/managers/bob.h>
#include <ace/types.h>
#include <ace/managers/copper.h>
#include <fixmath/fixmath.h>

typedef struct _tEnemy {
    tBob sBob;
    UBYTE ubHealth;
    UBYTE ubSpeed;
} tEnemy;