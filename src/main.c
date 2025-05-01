#include "controller/startupPhase.h"
#include "model/playPhaseCommands.h"
#include "gui.h"  // GUI related functions

#include "model/StartPhaseCommands.h"

int main(void) {

    // Initialize the game data (deck, columns, etc.)
    LD(NULL);
    InitArray();

    // Initialize the GUI (SDL)
    if (InitGUI() != 0) {
        return -1;  // Exit if initialization fails
    }

    // Event loop for GUI interaction
    SDL_Event e;
    int isRunning = 1;

    // Initialize the startup phase GUI
    RunStartupPhase();

    while (isRunning) {
        // Handle SDL events (mouse, keyboard, etc.)
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                // handle quit event
            } else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                // handle mouse button down
            } else if (e.type == SDL_EVENT_MOUSE_MOTION) {
                // handle mouse motion
            }



            // You can add more event handling as needed for keyboard inputs, etc.
        }

        // The game state has been updated, now you can keep handling input, no need to redraw unless necessary.
    }

    // Clean up before exit
    CleanupGUI();
    return 0;
}
