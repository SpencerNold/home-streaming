#include "window.h"

#include <stdlib.h>

WIN_Window* WIN_Create(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return NULL;
    }
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Quit();
        return NULL;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (texture == NULL) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return NULL;
    }
    WIN_Window* win_window = (WIN_Window*) malloc(sizeof(WIN_Window));
    if (win_window == NULL) {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    win_window->window = window;
    win_window->renderer = renderer;
    win_window->texture = texture;
    win_window->running = 1;
    return win_window;
}

void WIN_PollEvents(WIN_Window* window) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            window->running = 0;
        }
    }
}

void WIN_PrepareRender(WIN_Window* window) {
    SDL_RenderClear(window->renderer);
}

void WIN_FillTexture(WIN_Window* window, FLV_Context* flv) {
    SDL_UpdateTexture(window->texture, NULL, flv->display->data[0], flv->display->linesize[0]);
}

void WIN_DoRender(WIN_Window* window) {
    SDL_RenderCopy(window->renderer, window->texture, NULL, NULL);
    SDL_RenderPresent(window->renderer);
    SDL_Delay(16); // TODO Remove
}

void WIN_Destroy(WIN_Window* window) {
    SDL_DestroyTexture(window->texture);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    free(window);
}