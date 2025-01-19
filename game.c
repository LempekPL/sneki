#include "game.h"
#include <SDL_timer.h>
#include "constants.h"
#include "portal.h"

void random_xy(const GameData* game, int* x, int* y) {
    do {
        *x = rand() % BOARD_SIZE;
        *y = rand() % BOARD_SIZE;
    } while (
        is_snake_at(&game->snake.body, *x, *y) ||
        is_near_portal(&game->portals, *x, *y) ||
        (*x == game->snake.head.x && *y == game->snake.head.y) ||
        (*x == game->blue_ball->x && *y == game->blue_ball->y) ||
        (game->red_ball != NULL && *x == game->red_ball->x && *y == game->red_ball->y)
    );
}

GameData initGame() {
    GameData game = {0};
    game.snake = create_snake();
    game.state = GameState_Playing;
    game.ready_for_next_direction = true;
    game.red_ball = nullptr;
    game.last_bonus_time = SDL_GetTicks();
    game.bonus_speed = 0;
    game.load_bonus_speed = 0;
    game.bonus_speed_time = SDL_GetTicks();
    game.speed_up_time = SDL_GetTicks();
    game.bonus_wait_time = rand() % RED_BONUS_MAX + RED_BONUS_MIN;
    int ball_y;
    do {
        ball_y = rand() % BOARD_SIZE;
    } while (ball_y == BOARD_SIZE / 2);
    int ball_x = rand() % BOARD_SIZE;
    game.blue_ball = create_blue_ball(ball_x, ball_y);
    game.portals = create_portals(PORTAL_PAIR_AMOUNT);
    for (int i = 0; i < PORTAL_PAIR_AMOUNT; i++) {
        int one_x, one_y, two_x, two_y;
        random_xy(&game, &one_x, &one_y);
        random_xy(&game, &two_x, &two_y);
        game.portals.portals[i] = (Portal){one_x, one_y, two_x, two_y, 0};
    }
    return game;
}

void update_snake(SnakeHead* snake) {
    if (snake->next_direction != snake->head.direction) {
        snake->head.direction = snake->next_direction;
    }
    for (int i = snake->body.length - 1; i >= 0; i--) {
        SnakePart* snake_part = snake_segment_get(&snake->body, i);
        if (i == 0) {
            snake_part->direction = snake->head.direction;
            snake_part->x = snake->head.x;
            snake_part->y = snake->head.y;
            break;
        }
        SnakePart* snake_part_next = snake_segment_get(&snake->body, i - 1);
        snake_part->direction = snake_part_next->direction;
        snake_part->x = snake_part_next->x;
        snake_part->y = snake_part_next->y;
    }
    switch (snake->head.direction) {
        case UP:
            snake->head.y -= 1;
            break;
        case LEFT:
            snake->head.x -= 1;
            break;
        case RIGHT:
            snake->head.x += 1;
            break;
        case DOWN:
            snake->head.y += 1;
            break;
    }
}

bool check_snake_correct_pos(SnakeHead* snake) {
    int board_size = BOARD_SIZE - 1;
    if (snake->head.x <= 0 && snake->head.direction == LEFT && snake->next_direction != DOWN) {
        snake->next_direction = UP;
        if (snake->head.y <= 0 || is_snake_at(&snake->body, snake->head.x, snake->head.y - 1)) {
            if (snake->head.y == board_size) { return true; }
            snake->next_direction = DOWN;
        }
    } else if (snake->head.y <= 0 && snake->head.direction == UP && snake->next_direction != LEFT) {
        snake->next_direction = RIGHT;
        if (snake->head.x >= board_size || is_snake_at(&snake->body, snake->head.x + 1, snake->head.y)) {
            if (snake->head.x == 0) { return true; }
            snake->next_direction = LEFT;
        }
    } else if (snake->head.x >= board_size && snake->head.direction == RIGHT && snake->next_direction != UP) {
        snake->next_direction = DOWN;
        if (snake->head.y >= board_size || is_snake_at(&snake->body, snake->head.x, snake->head.y + 1)) {
            if (snake->head.y == 0) { return true; }
            snake->next_direction = UP;
        }
    } else if (snake->head.y >= board_size && snake->head.direction == DOWN && snake->next_direction != RIGHT) {
        snake->next_direction = LEFT;
        if (snake->head.x <= 0 || is_snake_at(&snake->body, snake->head.x - 1, snake->head.y)) {
            if (snake->head.x == board_size) { return true; }
            snake->next_direction = RIGHT;
        }
    }
    return false;
}

void handle_teleportation(Portal* portal, SnakeHead* snake) {
    if ((snake->head.x == portal->one_x && snake->head.y == portal->one_y) ||
        (snake->head.x == portal->two_x && snake->head.y == portal->two_y)) {
        bool isAtPortalOne = (snake->head.x == portal->one_x && snake->head.y == portal->one_y);
        snake->head.x = isAtPortalOne ? portal->two_x : portal->one_x;
        snake->head.y = isAtPortalOne ? portal->two_y : portal->one_y;

        switch (snake->head.direction) {
            case UP: snake->head.y--;
                break;
            case LEFT: snake->head.x--;
                break;
            case RIGHT: snake->head.x++;
                break;
            case DOWN: snake->head.y++;
                break;
        }
    }
}

void check_snake_collision(GameData* game, SnakeHead* snake) {
    SnakePart snake_head = snake->head;
    if (is_snake_at(&snake->body, snake_head.x, snake_head.y)) {
        game->state = GameState_Dead;
    }
    if (snake->head.x == game->blue_ball->x && snake->head.y == game->blue_ball->y) {
        game->points++;
        int ball_x, ball_y;
        random_xy(game, &ball_x, &ball_y);
        game->blue_ball->x = ball_x;
        game->blue_ball->y = ball_y;
        snake_segment_add(&snake->body);
    }
    if (game->red_ball != NULL && snake->head.x == game->red_ball->x && snake->head.y == game->red_ball->y) {
        game->points++;
        free(game->red_ball);
        game->red_ball = nullptr;
        game->bonus_wait_time = rand() % RED_BONUS_MAX + RED_BONUS_MIN;
        game->last_bonus_time = SDL_GetTicks();
        if ((float) rand() / (float) RAND_MAX <= RED_BONUS_CHANCE) {
            game->bonus_speed = SPEED_BONUS;
            game->bonus_speed_time = SDL_GetTicks();
        } else {
            if (snake->body.length > 2 + REMOVE_SNAKE_SEGMENTS) {
                for (int i = 0; i < REMOVE_SNAKE_SEGMENTS; i++) {
                    snake_segment_remove(&snake->body);
                }
            }
        }
    }
    for (int i = 0; i < game->portals.length; i++) {
        Portal* portal = game->portals.portals + i;
        handle_teleportation(portal, snake);
    }
    if (snake->head.x < 0 || snake->head.y < 0 || snake->head.x >= BOARD_SIZE || snake->head.y >= BOARD_SIZE) {
        game->state = GameState_Dead;
    }
}

void change_frame_texture(GameData* game) {
    // blue ball
    game->blue_ball->frame++;
    if (game->blue_ball->frame > 4) {
        game->blue_ball->frame = 0;
    }
    // red ball
    if (game->red_ball != NULL) {
        game->red_ball->frame++;
        if (game->red_ball->frame > 2) {
            game->red_ball->frame = 0;
        }
    }
    // portals
    for (int i = 0; i < game->portals.length; i++) {
        Portal* portal = game->portals.portals + i;
        portal->frame++;
        if (portal->frame > 1) {
            portal->frame = 0;
        }
    }
}

void update_game(GameData* game, TimeData* time) {
    if (game->state == GameState_Playing) {
        if (time->last_tick - game->last_texture_tick >= ANIMATION_SPEED) {
            change_frame_texture(game);
            game->last_texture_tick = SDL_GetTicks();
        }
        SnakeHead* snake = &game->snake;
        if (game->red_ball == NULL) {
            if (time->last_tick - game->last_bonus_time >= game->bonus_wait_time) {
                int ball_x, ball_y;
                random_xy(game, &ball_x, &ball_y);
                game->red_ball = create_red_ball(ball_x, ball_y);
                game->last_bonus_time = SDL_GetTicks();
            }
        } else {
            if (time->last_tick - game->last_bonus_time >= RED_BONUS_WAIT) {
                free(game->red_ball);
                game->red_ball = nullptr;
                game->last_bonus_time = SDL_GetTicks();
            }
        }
        if (time->last_tick - game->bonus_speed_time >= SPEED_BONUS_TIME) {
            game->bonus_speed = 0;
            game->bonus_speed_time = SDL_GetTicks();
        }
        if (time->last_tick - game->speed_up_time >= SPEEDUP_TIME) {
            snake->speed *= 1 - (float) SPEEDUP_AMOUNT;
            game->speed_up_time = SDL_GetTicks();
        }
        int speed =
                (float) snake->speed * (1.0 - game->bonus_speed) < 10
                    ? 10
                    : (float) snake->speed * (1.0 - game->bonus_speed);
        if (time->last_tick - snake->last_logic_tick >= speed) {
            update_snake(snake);
            check_snake_collision(game, snake);
            game->ready_for_next_direction = true;
            if (snake->body.length > BOARD_SIZE * BOARD_SIZE - 2) {
                game->state = GameState_Win;
            }
            snake->last_logic_tick = SDL_GetTicks();
        }
        if (check_snake_correct_pos(snake)) {
            game->state = GameState_Dead;
        }
    }
}

void reset_game(GameData* game) {
    destroy_snake_body(&game->snake.body);
    free(game->blue_ball);
    free(game->red_ball);
    free(game->portals.portals);
    *game = initGame();
}
