#include "init.h"
#include <SDL.h>

#include "constants.h"

SDL_Surface* load_image(const SDL_Data* sdl_data, const char* filename) {
    SDL_Surface* image = SDL_LoadBMP(filename);
    if(image == NULL) {
        fprintf(stderr, "SDL_LoadBMP(%s) error: %s\n", filename, SDL_GetError());
        destroySDL(sdl_data);
    };
    SDL_SetColorKey(image, true, 0x000000);
    return image;
}

SDL_Data initSDL() {
    SDL_Data sdl_data;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        destroySDL(&sdl_data);
    }
    sdl_data.window = SDL_CreateWindow(
        "Snake",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!sdl_data.window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        destroySDL(&sdl_data);
    }
    sdl_data.renderer = SDL_CreateRenderer(sdl_data.window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(sdl_data.renderer, SDL_BLENDMODE_BLEND);
    if (!sdl_data.renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        destroySDL(&sdl_data);
    }
    sdl_data.screen = SDL_GetWindowSurface(sdl_data.window);
    sdl_data.texture = SDL_CreateTextureFromSurface(sdl_data.renderer, sdl_data.screen);
	sdl_data.charset = load_image(&sdl_data, "./assets/cs8x8.bmp");
	sdl_data.snake_bmp = load_image(&sdl_data, "./assets/snake.bmp");

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(sdl_data.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(sdl_data.renderer, 0, 0, 0, 255);
    SDL_ShowCursor(SDL_DISABLE);

    return sdl_data;
}

void destroySDL(const SDL_Data* sdl_data) {
    if (sdl_data) {
        if (sdl_data->screen) SDL_FreeSurface(sdl_data->screen);
        if (sdl_data->charset) SDL_FreeSurface(sdl_data->charset);
        if (sdl_data->texture) SDL_DestroyTexture(sdl_data->texture);
        if (sdl_data->renderer) SDL_DestroyRenderer(sdl_data->renderer);
        if (sdl_data->window) SDL_DestroyWindow(sdl_data->window);
        SDL_Quit();
        exit(1);
    }
}
