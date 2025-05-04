#include "controller/startupPhase.h"
#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

//USED FOR TEST (DELETE BEFORE SUBMISSION)





int main(void) {
    LD(NULL);
    InitArray();
    Card* k1 = columnArr[0];
    Card* k2 = columnArr[1];
    Card* k3 = columnArr[2];
    Card* k4 = columnArr[3];
    Card* k5 = columnArr[4];
    Card* k6 = columnArr[5];
    Card* k7 = columnArr[6];

    RunStartupPhase();

    //MoveTopCards(&columnArr[1],&columnArr[0],5);
    PrintPlayPhase("ok", "ok");
}
