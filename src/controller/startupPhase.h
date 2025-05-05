#ifndef STARTUPPHASE_H
#define STARTUPPHASE_H

#include "model/deck.h"
#include "view/PRINT.H"

// Enum to define the phases of the game
typedef enum {
    STARTUP_PHASE,
    PLAY_PHASE
} Phase;

extern Phase currentPhase;  // Declaration of the current phase

//Startupphase Functions
void RunStartupPhase(const char* input);




#endif //STARTUPPHASE_H
