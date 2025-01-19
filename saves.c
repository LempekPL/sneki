#include "saves.h"

#include <SDL_timer.h>
#include <stdio.h>

#include "constants.h"

// points, time (in seconds)
// blue_ball_x, blue_ball_y
// red_ball_x, red_ball_y, time_to_dissapear or -1
// time_of_bonus_speed or -1
// snake_head_x, snake_head_y, snake_head_direction, snake_speed, snake_length
// snake_part_x, snake_part_y, snake_part_direction
// ... (for snake_length)
// snake_part_x, snake_part_y, snake_part_direction
// portal amount
// portal_one_x, portal_one_y, portal_two_x, portal_two_y

void save_file(GameData* game, TimeData* time) {
    FILE* save = fopen("./save.txt", "w");
    fprintf(save, "%d, %d\n", game->points, (int) time->since_start);
    fprintf(save, "%d, %d\n", game->blue_ball->x, game->blue_ball->y);
    if (game->red_ball != NULL) {
        fprintf(save, "%d, %d, %d\n", game->red_ball->x, game->red_ball->y, time->last_tick - game->last_bonus_time);
    } else {
        fprintf(save, "-1\n");
    }
    if (game->bonus_speed != 0) {
        fprintf(save, "%d\n", time->last_tick - game->bonus_speed_time);
    } else {
        fprintf(save, "-1\n");
    }
    SnakeHead* snake = &game->snake;
    fprintf(save, "%d, %d, %d, %d, %d\n", snake->head.x, snake->head.y, snake->head.direction, snake->speed,
            snake->body.length);
    for (int i = 0; i < snake->body.length; i++) {
        SnakePart* snake_part = snake_segment_get(&snake->body, i);
        fprintf(save, "%d, %d, %d\n", snake_part->x, snake_part->y, snake_part->direction);
    }
    fprintf(save, "%d\n", game->portals.length);
    for (int i = 0; i < game->portals.length; i++) {
        Portal* portal = game->portals.portals + i;
        fprintf(save, "%d, %d, %d, %d\n", portal->one_x, portal->one_y, portal->two_x, portal->two_y);
    }
    fclose(save);
}

void load_file(GameData* game, TimeData* time) {
    free(game->portals.portals);
    FILE* save = fopen("./save.txt", "r");
    if (save == NULL) {
        fclose(save);
        return;
    }
    int time_amount;
    fscanf(save, "%d, %d\n", &game->points, &time_amount);
    time->since_start = time_amount;
    fscanf(save, "%d, %d\n", &game->blue_ball->x, &game->blue_ball->y);
    int value;
    fscanf(save, "%d", &value);
    if (value > -1) {
        int value2 = 0;
        int red_time = 0;
        fscanf(save, ", %d, %d", &value2, &red_time);
        game->red_ball = create_red_ball(value, value2);
        game->last_bonus_time = SDL_GetTicks() - red_time;
        game->red_ball->load_save = red_time;
    } else {
        if (game->red_ball != NULL) {
            free(game->red_ball);
            game->red_ball = nullptr;
        }
        fscanf(save, "\n");
    }
    fscanf(save, "%d\n", &value);
    if (value > -1) {
        game->bonus_speed_time = SDL_GetTicks() - value;
        game->load_bonus_speed = value;
        game->bonus_speed = SPEED_BONUS;
    } else {
        game->bonus_speed_time = SDL_GetTicks() - SPEED_BONUS_TIME;
    }
    SnakeHead* snake = &game->snake;
    int amount = 0;
    fscanf(
        save,
        "%d, %d, %d, %d, %d\n",
        &snake->head.x, &snake->head.y, &snake->head.direction, &snake->speed, &amount
    );
    if (snake->body.length - amount > 0) {
        for (int i = 0; i < snake->body.length - amount; i++) {
            snake_segment_remove(&snake->body);
        }
    } else {
        for (int i = 0; i < amount - snake->body.length; i++) {
            snake_segment_add(&snake->body);
        }
    }
    for (int i = 0; i < amount; i++) {
        SnakePart* snake_part = snake_segment_get(&snake->body, i);
        fscanf(save, "%d, %d, %d\n", &snake_part->x, &snake_part->y, &snake_part->direction);
    }
    fscanf(save, "%d\n", &amount);
    game->portals.portals = malloc(sizeof(Portal) * amount);
    for (int i = 0; i < amount; i++) {
        game->portals.length = amount;
        Portal* portal = game->portals.portals + i;
        fscanf(save, "%d, %d, %d, %d\n", &portal->one_x, &portal->one_y, &portal->two_x, &portal->two_y);
    }
    game->snake.next_direction = snake->head.direction;
    game->state = GameState_Playing;
    fclose(save);
}
