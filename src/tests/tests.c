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

    printf("Select test to run (1-4):\n");
    printf("  1: Play Phase Initialization\n");
    printf("  2: Load Function\n");
    printf("  3: Invalid Self-Move\n");
    printf("  4: Undo/Redo Functionality\n");
    printf("Enter test number: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting.\n");
        exit(1);
    }

    switch (choice) {
        case 1:
            printf("Running Test 1: Play Phase Initialization\n");
        LD(NULL);
        RunStartupPhase("P");
        printf("Expected: currentPhase == PLAY_PHASE, Actual: %d\n", currentPhase);
        printf("Expected: columnArr[0] not empty, Actual: %s\n", columnArr[0]->next != NULL ? "PASS" : "FAIL");
        break;

        case 2:
            printf("Running Test 2: LD\n");

            printf("\nAttempting LD(test1337) - should return error\n");
            LD("test1337");

            printf("\nAttempting LD( ) - empty filename should return error\n");
            LD("");

            printf("\nAttempting LD(testfile) - No errors\n");
            LD("testfile");

            printf("\nAttempting: LD(cards51) - should return 2 different errors\n");
            LD("cards51");

        break;


        case 3:
            printf("Running Test 3: Invalid Self-Move\n");
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

        case 4:
            printf("Running Test 4: Undo/Redo Functionality\n");
        RunStartupPhase("LD");
        RunStartupPhase("P");
        RunPlayPhase("C3:7H->C5");
        RunPlayPhase("C6->C5");
        RunPlayPhase("U");
        RunPlayPhase("R");
        RunPlayPhase("C2:8C->C5");

        // checks for log entries
        printf("MoveLog[0]: Expected: 0C3:7H->C5, Actual: %s\n", moveLog[0]);
        printf("MoveLog[1]: Expected: 0C6->C5, Actual: %s\n", moveLog[1]);
        printf("MoveLog[2]: Expected: 1C2:8C->C5, Actual: %s\n", moveLog[2]);

        // Check that top card in C6 is now 8C (after redo)
        Card* topC6 = columnArr[5]->prev;
        printf("Top card in C5 ID: Expected: 6S, Actual: %s\n", topC6->ID);
            break;

        case 5:
            RunStartupPhase("LD");
        RunStartupPhase("P");
            RunPlayPhase("C3:7H->C5");
        printf("movelog 0: %s\n", moveLog[0]);
        printf("movelog 1: %s\n", moveLog[1]);
        printf("movelog 2: %s\n", moveLog[2]);
        RunPlayPhase("U");
        RunPlayPhase("R");
        RunPlayPhase("C1->F1");
        RunPlayPhase("U");
        RunPlayPhase("R");


        break;

        default:
            printf("No such test implemented: %d\n", choice);
            break;
    }

    exit(0);
}