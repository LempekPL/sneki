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
                if (game->snake.head.direction != UP && game->snake.head.y != BOARD_SIZE - 1)
                    game->snake.next_direction = DOWN;
                break;
            case SDLK_LEFT:
                if (game->snake.head.direction != RIGHT && game->snake.head.x != 0)
                    game->snake.next_direction = LEFT;
                break;
            case SDLK_RIGHT:
                if (game->snake.head.direction != LEFT && game->snake.head.x != BOARD_SIZE - 1)
                    game->snake.next_direction = RIGHT;
                break;
        }
        game->ready_for_next_direction = false;
    }
}

void handle_highscore(SDL_Event event, GameData* game) {
    if (event.type == SDL_TEXTINPUT) {
        if (strlen(game->temp_text) + strlen(event.text.text) < NAME_LENGTH + 1) {
            strcat(game->temp_text, event.text.text);
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(game->temp_text) > 0) {
            game->temp_text[strlen(game->temp_text) - 1] = '\0';
        }
        if (event.key.keysym.sym == SDLK_RETURN) {
            if (game->scores.length == 0) {
                game->scores.length = 1;
                strcpy(game->scores.scores[0].name, game->temp_text);
                game->scores.scores[0].score = (int) game->points;
            } else {
                bool added = false;
                for (int i = 0; i < game->scores.length; i++) {
                    if (game->scores.scores[i].score < game->points) {
                        for (int j = game->scores.length; j > i; j--) {
                            strcpy(game->scores.scores[j].name, game->scores.scores[j-1].name);
                            game->scores.scores[j].score = game->scores.scores[j-1].score;
                        }
                        strcpy(game->scores.scores[i].name, game->temp_text);
                        game->scores.scores[i].score = (int) game->points;
                        added = true;
                        break;
                    }
                }
                if (!added) {
                    strcpy(game->scores.scores[game->scores.length].name, game->temp_text);
                    game->scores.scores[game->scores.length].score = (int) game->points;
                }
                if (game->scores.length >= 3) {
                    game->scores.length = 3;
                } else {
                    game->scores.length++;
                }
            }
            write_scores(&game->scores);
            game->high_score = -1;
        }
    }
}

int handle_input(GameData* game, TimeData* time) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return 0;

        if (game->high_score == 1) {
            handle_highscore(event, game);
            break;
        }

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
