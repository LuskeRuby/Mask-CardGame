#ifndef STARTUPPHASE_H
#include "view/PRINT.H"
#define STARTUPPHASE_H

// Enum to define the phases of the game
typedef enum {
    STARTUP_PHASE,
    PLAY_PHASE
} Phase;

extern Phase currentPhase;  // Declaration of the current phase

//Startupphase Functions
void RunStartupPhase(const char* input); //Run startupPhase by taking an input and call startupPhase command matching input


#endif //STARTUPPHASE_H
