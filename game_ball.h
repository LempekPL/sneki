#ifndef GAME_BALL_H
#define GAME_BALL_H

typedef struct {
    int x,y;
    int frame;
} BlueBall;

typedef struct {
    int x,y;
    int frame;
    unsigned int last_bonus_time;
    int load_save;
} RedBall;

BlueBall* create_blue_ball(int x, int y);
RedBall* create_red_ball(int x, int y);

#endif //GAME_BALL_H
