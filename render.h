#ifndef RENDER_H
#define RENDER_H
#include "game.h"
#include "init.h"
#include "time.h"

typedef enum {
    BorderSingle,
    BorderDouble
} BorderType;

void render_game(const SDL_Data* sdl, const GameData* game, const TimeData* time);
void print_string(const SDL_Data *sdl, int x, int y, const char *text);

#endif //RENDER_H
