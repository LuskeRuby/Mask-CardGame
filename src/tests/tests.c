//
// Created by rubym on 06/05/2025.
//

#include "tests.h"

#include <stdlib.h>
#include <string.h>
#include <controller/playPhase.h>
#include <controller/startupPhase.h>

#include "model/playPhaseCommands.h"
#include "model/playPhaseValidation.h"
#include "model/StartPhaseCommands.h"
#include "tests/tests.h"

void RunTests() {
    int choice;

    printf("Select test to run (0-3):\n");
    printf("  0: Play Phase Initialization\n");
    printf("  1: Load Function\n");
    printf("  2: Invalid Self-Move\n");
    printf("  3: Undo/Redo Functionality\n");
    printf("Enter test number: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    switch (choice) {
        case 0:
            printf("Running Test 0: Play Phase Initialization\n");
        LD(NULL);
        RunStartupPhase("P");
        printf("Expected: currentPhase == PLAY_PHASE, Actual: %d\n", currentPhase);
        printf("Expected: columnArr[0] not empty, Actual: %s\n", columnArr[0]->next != NULL ? "PASS" : "FAIL");
        break;

        case 1:
            printf("Running Test 1: LD\n");

            printf("\nAttempting LD(test1337) - should return error\n");
            LD("test1337");

            printf("\nAttempting LD( ) - empty filename should return error\n");
            LD("");

            printf("\nAttempting LD(testfile) - No errors\n");
            LD("testfile");

            printf("\nAttempting: LD(cards51) - should return 2 different errors\n");
            LD("cards51");

        break;


        case 2:
            printf("Running Test 1: Invalid Self-Move\n");
            LD(NULL);
            RunStartupPhase("P");

            printf("Attempting invalid move: C1:AC->F1\n");
            RunPlayPhase("C1:AC->F1");
            printf("Attempting invalid move: C2:KD->F1\n");
            RunPlayPhase("C2:KD->F1");
            printf("Attempting correct move: C3->C7\n");
            RunPlayPhase("C3->C7");

        // Checks
        printf("Expected: Invalid: can only move top card to/from foundation, Actual: %s\n",
            foundationArr[0] == NULL ? "FAIL (card moved)" : "PASS (move rejected)");

        printf("Expected: Invalid move to foundation (KD not valid for F1), Actual: %s\n",
            columnArr[0]->prev != NULL ? "PASS (move rejected)" : "FAIL (move accepted)");

        Card* topC7 = columnArr[6]->prev;
        printf("Expected: PASS, Actual: %s\n",
            (topC7 && strcmp(topC7->ID, "QH") == 0) ?
            "PASS (card moved)" : "FAIL (card not moved)");
            break;

        case 3:
            printf("Running Test 3: Undo/Redo Functionality\n");
        RunStartupPhase("LD");
        RunStartupPhase("P");
        RunPlayPhase("C1:AC->F1");
        RunPlayPhase("C6:Th->C6");
        RunPlayPhase("C3->C3");
        exit(0);
            break;

        default:
            printf("No such test implemented: %d\n", choice);
            break;
    }

    exit(0);
}