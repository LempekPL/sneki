#include "game_ball.h"

#include <SDL_timer.h>
#include <stdlib.h>

BlueBall* create_blue_ball(const int x, const int y) {
    BlueBall* blue_ball = malloc(sizeof(BlueBall));
    blue_ball->frame = 0;
    blue_ball->x = x;
    blue_ball->y = y;
    return blue_ball;
}

RedBall* create_red_ball(const int x, const int y) {
    RedBall* red_ball = malloc(sizeof(RedBall));
    red_ball->frame = 0;
    red_ball->x = x;
    red_ball->y = y;
    red_ball->last_bonus_time = SDL_GetTicks();
    return red_ball;
}