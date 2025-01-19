#define SDL_MAIN_HANDLED
#include <time.h>

#include "game.h"
#include "init.h"
#include "input.h"
#include "render.h"
#include "time.h"

int main(int argc, char* argv[]) {
    srand(time(nullptr));
    SDL_Data sdl = initSDL();
    TimeData time = initTime();
    GameData game = initGame();

    int running = 1;
    while (running) {
        time_update(&time);
        if (game.state == GameState_Playing) {
            time.since_start += time.delta;
        }

        running = handle_input(&game, &time);

        update_game(&game, &time);

        render_game(&sdl, &game, &time);
        time.frames++;
    }

    destroySDL(&sdl);
    return 0;
}
