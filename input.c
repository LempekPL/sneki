#include "input.h"

#include <SDL_events.h>

#include "constants.h"
#include "saves.h"

void handle_snake(SDL_Event* event, GameData* game) {
    if (game->ready_for_next_direction) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
                if (game->snake.head.direction != DOWN && game->snake.head.y != 0)
                    game->snake.next_direction = UP;
            break;
            case SDLK_DOWN:
                if (game->snake.head.direction != UP && game->snake.head.y != BOARD_SIZE-1)
                    game->snake.next_direction = DOWN;
            break;
            case SDLK_LEFT:
                if (game->snake.head.direction != RIGHT && game->snake.head.x != 0)
                    game->snake.next_direction = LEFT;
            break;
            case SDLK_RIGHT:
                if (game->snake.head.direction != LEFT && game->snake.head.x != BOARD_SIZE-1)
                    game->snake.next_direction = RIGHT;
            break;
        }
        game->ready_for_next_direction = false;
    }
}

int handle_input(GameData* game, TimeData* time) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 0;
        if (event.type == SDL_KEYDOWN) {
            if (game->state == GameState_Playing) {
                handle_snake(&event, game);
            }
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    return 0;
                case SDLK_n:
                    time->since_start = 0;
                    reset_game(game);
                break;
                case SDLK_s:
                    save_file(game, time);
                break;
                case SDLK_l:
                    load_file(game, time);
                break;
            }
        }
    }
    return 1;
}
