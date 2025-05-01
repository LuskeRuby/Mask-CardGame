#include "gui.h"
#include <SDL3/SDL.h>
#include <stdio.h>  // For printf

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int InitGUI() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_WindowFlags SDL_WINDOW_SHOWN;
    // Create the window with 4 arguments: title, width, height, flags
    window = SDL_CreateWindow("Mask Card Game", 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return -1;
    }

    // Create the renderer with 2 arguments: window and driver name (NULL for default)
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return -1;
    }

    // Event loop to keep window open and responsive
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        // Poll for events
        while (SDL_PollEvent(&event)) {
            Uint32 SDL_QUIT_renamed_SDL_EVENT_QUIT;
            if (event.type == SDL_QUIT) {
                quit = 1; // Close the window when quit event is triggered
            }
        }

        // Clear the screen (optional)
        SDL_RenderClear(renderer);

        // You can draw things here (e.g., cards, background)

        // Present the renderer to the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; // Success
}


void HandleMouseClick(int x, int y) {
    int column1X = 100, column1Y = 200;  // Example values
    int columnWidth = 100, columnHeight = 150;  // Example values

    // You need to map the x, y coordinates to game actions.
    // Assuming columns and cards are mapped on the screen.

    // Translate x, y to column/card positions
    // For example: C1 is at position (100, 200) on the screen.
    // You can check if the mouse click is within the bounds of a column or card.

    printf("Mouse click at (%d, %d)\n", x, y);

    // Determine which card was clicked, e.g., based on coordinates:
    // Example:
    if (x > column1X && x < column1X + columnWidth && y > column1Y && y < column1Y + columnHeight) {
        // You clicked on column 1, you can now determine which card is selected
        printf("Card selected from column 1\n");
        // Trigger appropriate game action like moving cards or starting a move command.
    }

    // Call game logic to make a move, e.g., `C1->C2`
    // You need to have a mechanism to track the currently selected card or column.
    // For example, if a column is clicked, you might need to select a card or column for the next step.
}

void HandleMouseMotion(int x, int y) {
    // If you want to allow dragging, you can track where the mouse is moving and update the card position
    // Example of a drag move (if implemented):
    printf("Mouse motion at (%d, %d)\n", x, y);
    // If you're dragging a card, update its position or highlight the destination.
}

void UpdateGUI() {
    // If you want to perform any updates after a move, you can call this.
    // You mentioned no drawing, but this would be where you'd refresh the window and update the UI.
    SDL_RenderClear(renderer);
    // Draw cards, if needed (no drawing, so maybe this is left out).
    SDL_RenderPresent(renderer);
}

void CleanupGUI() {
    // Cleanup SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}