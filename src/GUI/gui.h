#ifndef GUI_H
#define GUI_H

#include <SDL3/SDL.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;

int InitGUI();
void CloseGUI();
void HandleMouseClick(int x, int y);
void HandleMouseMotion(int x, int y);
void CleanupGUI(void);

#endif
