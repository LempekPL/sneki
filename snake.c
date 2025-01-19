#include "snake.h"

#include <stdlib.h>

#include "constants.h"

SnakeBody create_snake_body() {
    SnakeBody body;
    body.capacity = 2;
    body.length = 0;
    body.parts = malloc(sizeof(SnakePart) * body.capacity);
    for (int i = 0; i < SNAKE_DEFAULT_LENGTH; i++) {
        snake_segment_add_at(&body, BOARD_SIZE/2+i <= BOARD_SIZE-1 ? BOARD_SIZE/2+i : BOARD_SIZE-1, BOARD_SIZE/2, LEFT);
    }
    return body;
}

SnakePart* snake_segment_get(const SnakeBody* snake_body, size_t index) {
    if (index >= snake_body->length) {
        exit(1010101);
    }
    return snake_body->parts + index;
}

void snake_segment_push(SnakeBody* snake_body, const SnakePart* snake_part) {
    if (snake_body->length == snake_body->capacity) {
        snake_body->capacity *= 2;
        snake_body->parts = realloc(snake_body->parts, snake_body->capacity * sizeof(SnakeBody));
    }
    snake_body->parts[snake_body->length] = *snake_part;
    snake_body->length++;
}

void snake_segment_add(SnakeBody* snake_body) {
    if (snake_body->length == snake_body->capacity) {
        snake_body->capacity *= 2;
        snake_body->parts = realloc(snake_body->parts, snake_body->capacity * sizeof(SnakeBody));
    }
    snake_body->parts[snake_body->length] = (SnakePart){BOARD_SIZE/2, BOARD_SIZE/2, NONE};
    snake_body->length++;
}

void snake_segment_add_at(SnakeBody* snake_body, int x, int y, SnakeDirection direction) {
    if (snake_body->length == snake_body->capacity) {
        snake_body->capacity *= 2;
        snake_body->parts = realloc(snake_body->parts, snake_body->capacity * sizeof(SnakeBody));
    }
    snake_body->parts[snake_body->length] = (SnakePart){x, y, direction};
    snake_body->length++;
}

void snake_segment_remove(SnakeBody* snake_body) {
    snake_body->length--;
}

bool is_snake_at(const SnakeBody* snake_body, const int x, const int y) {
    for (int i = 0; i < snake_body->length; i++) {
        SnakePart sp = snake_body->parts[i];
        if (sp.x == x && sp.y == y) {
            return true;
        }
    }
    return false;
}

void destroy_snake_body(SnakeBody* body) {
    free(body->parts);
}

SnakeHead create_snake() {
    SnakeHead snake_head;
    snake_head.next_direction = LEFT;
    snake_head.head.direction = LEFT;
    snake_head.head.x = BOARD_SIZE/2-1;
    snake_head.head.y = BOARD_SIZE/2;
    snake_head.body = create_snake_body();
    snake_head.last_logic_tick = 0;
    snake_head.speed = SNAKE_DEFAULT_SPEED;
    return snake_head;
}
