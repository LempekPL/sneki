#ifndef GAME_H
#define GAME_H
#include "game_ball.h"
#include "snake.h"
#include "time.h"

typedef enum gamestate_t {
    GameState_Playing,
    GameState_Dead,
    GameState_Win,
} GameState;


typedef struct game_data_t {
    SnakeHead snake;
    unsigned int last_texture_tick, speed_up_time, last_bonus_time, bonus_wait_time, bonus_speed_time;
    unsigned int points;
    float bonus_speed;
    GameState state;
    bool ready_for_next_direction;
    BlueBall* blue_ball;
    RedBall* red_ball;
} GameData;

GameData initGame();
void update_game(GameData* game, TimeData* time);
void reset_game(GameData* game);

#endif //GAME_H
