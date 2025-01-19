#ifndef INIT_H
#define INIT_H
#include <SDL_events.h>
#include <SDL_render.h>

typedef struct SDL_data_t {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screen;
    SDL_Texture* texture;
    SDL_Surface* charset;
    SDL_Surface* snake_bmp;
} SDL_Data;

SDL_Data initSDL();
void destroySDL(const SDL_Data* sdl_data);

#endif //INIT_H
