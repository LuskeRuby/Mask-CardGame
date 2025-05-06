//
// Created by rubym on 06/05/2025.
//

#include "tests.h"

#include <stdlib.h>

#include "model/playPhaseCommands.h"
#include "model/StartPhaseCommands.h"

void RunTests() {

    // Initializing
    LD(NULL);
    InitArray();

    printf("------LD() TESTS------\n");

    printf("\nCalling LD(test1337) - should return error\n");
    LD("test1337");

    printf("\nCalling LD( ) - empty filename should return error\n");
    LD("");

    printf("\nCalling LD(testfile) - No errors\n");
    LD("testfile");

    printf("\nCalling: LD(cards51) - should return 2 different errors\n");
    LD("cards51");


    //SI TESTS
    printf("------SI() TESTS-------\n");

    printf("\n Assuming a deck is loaded"); LD(NULL);

    printf("\nCalling: SI(NULL) - no errors\n");
    SI(NULL);
    printf("\nCalling: SI(52) - Should return error\n");
    SI(1);
    printf("\nCalling: SI(16) - no errors\n");
    SI(16);

    //SD(NULL);
    //SI(5);
    //SR();
    //SW();

    exit(0);


}