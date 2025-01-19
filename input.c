#include "input.h"

#include <SDL_events.h>

#include "constants.h"

void handle_snake(SDL_Event* event, GameData* game) {
    if (game->ready_for_next_direction) {
        switch (event->key.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                if (game->snake.head.direction != DOWN && game->snake.head.y != 0)
                    game->snake.next_direction = UP;
            break;
            case SDLK_DOWN:
            case SDLK_s:
                if (game->snake.head.direction != UP && game->snake.head.y != BOARD_SIZE-1)
                    game->snake.next_direction = DOWN;
            break;
            case SDLK_LEFT:
            case SDLK_a:
                if (game->snake.head.direction != RIGHT && game->snake.head.x != 0)
                    game->snake.next_direction = LEFT;
            break;
            case SDLK_RIGHT:
            case SDLK_d:
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
            }
        }
    }
    return 1;
}
