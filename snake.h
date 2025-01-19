#ifndef SNAKE_H
#define SNAKE_H
#include <stddef.h>

typedef enum snake_direction_t {
    NONE = -1,
    UP = 0,
    LEFT,
    RIGHT,
    DOWN,
} SnakeDirection;

typedef struct snake_part {
    int x,y;
    SnakeDirection direction;
} SnakePart;

typedef struct snake_t {
    SnakePart* parts;
    int length;
    int capacity;
} SnakeBody;

typedef struct snake_head_t {
    SnakeDirection next_direction;
    SnakePart head;
    SnakeBody body;
    unsigned int last_logic_tick, speed;
} SnakeHead;

SnakePart* snake_segment_get(const SnakeBody* snake_body, size_t index);
void snake_segment_add(SnakeBody* snake_body);
void snake_segment_add_at(SnakeBody* snake_body, int x, int y, SnakeDirection direction);
void snake_segment_remove(SnakeBody* snake_body);
bool is_snake_at(const SnakeBody* snake_body, int x, int y);
SnakeHead create_snake();
void destroy_snake_body(SnakeBody* body);

#endif //SNAKE_H
