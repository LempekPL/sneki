#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BOARD_SIZE              13    // board size (per snake segment)
#define SNAKE_DEFAULT_LENGTH    4     // default length of snake at start of every game
#define SNAKE_DEFAULT_SPEED     500   // default speed of snake at start of every game (1 move per X ms)
#define ANIMATION_SPEED         500   // texture changing speed in miliseconds
#define SPEEDUP_TIME            10000 // speed up time in miliseconds
#define SPEEDUP_AMOUNT          .1    // amount of % to reduce for next snake update
#define RED_BONUS_MIN           2000  // min amount of time to wait in ms for red bonus
#define RED_BONUS_MAX           2000  // max amount of time to wait in ms for red bonus
#define RED_BONUS_WAIT          10000 // how much ms of time before red bonus dissapears
#define RED_BONUS_CHANCE        .5    // chance of speedup bonus relative to snake remove segments
#define SPEED_BONUS             .25   // amount of % to reduce from snake updates
#define SPEED_BONUS_TIME        5000  // how much ms of time before speed bonus runs out
#define REMOVE_SNAKE_SEGMENTS   1     // amount of segments to remove from snake
#define PORTAL_PAIR_AMOUNT      2     // amount of pairs of portals on board to spawn
#define NAME_LENGTH             10    // length of allowed name

#define CHAR_SIZE               8   // size of char in cs8x8.bmp in pixels
#define SNAKE_SIZE              32  // size of texture atlass in snake.bmp in pixels
#define BOARD_OFFSET            32  // board offset in pixels
#define TOP_BAR_OFFSET          32  // amount of pixels for topbar

#define SCREEN_WIDTH    3*BOARD_OFFSET+(BOARD_SIZE-1)*SNAKE_SIZE
#define SCREEN_HEIGHT   TOP_BAR_OFFSET+3*BOARD_OFFSET+(BOARD_SIZE-1)*SNAKE_SIZE

#endif //CONSTANTS_H
