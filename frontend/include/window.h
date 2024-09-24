#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "frame.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    int running;
} WIN_Window;

WIN_Window* WIN_Create(const char* title, int width, int height);
void WIN_PollEvents(WIN_Window* window);
void WIN_PrepareRender(WIN_Window* window);

void WIN_FillTexture(WIN_Window* window, FLV_Context* context);
void WIN_DoRender(WIN_Window* window);
void WIN_Destroy(WIN_Window* window);

#endif