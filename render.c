#include "render.h"

#include <SDL_timer.h>

#include "constants.h"
#include "time.h"


void print_string(const SDL_Data* sdl, int x, const int y, const char* text) {
    SDL_Rect src, dest;
    src.w = src.h = dest.w = dest.h = CHAR_SIZE;
    while (*text) {
        const int c = *text & 255;
        const int px = (c % 16) * CHAR_SIZE;
        const int py = (c / 16) * CHAR_SIZE;
        src.x = px;
        src.y = py;
        dest.x = x;
        dest.y = y;
        SDL_BlitSurface(sdl->charset, &src, sdl->screen, &dest);
        x += CHAR_SIZE;
        text++;
    }
}

void print_box(const SDL_Data* sdl, SDL_Rect rect, int border_width, Uint32 color, Uint32 border_color) {
    SDL_Rect border = {
        rect.x - border_width,
        rect.y - border_width,
        rect.w + border_width * 2,
        rect.h + border_width * 2,
    };
    SDL_FillRect(sdl->screen, &border, border_color);
    SDL_FillRect(sdl->screen, &rect, color);
}

typedef enum {
    RenderSnakeHead,
    RenderSnakeBodySkinny,
    RenderSnakeBodyFat,
    RenderSnakeTail,
} RenderSnakeType;

void get_snake_rect_simple(SDL_Rect* source, const SnakePart* snake_part, const RenderSnakeType type) {
    int offset = 0;
    switch (type) {
        case RenderSnakeTail:
            offset = 2 * SNAKE_SIZE;
        case RenderSnakeHead:
            source->x = (int) snake_part->direction * SNAKE_SIZE;
            source->y = offset;
            break;
        case RenderSnakeBodySkinny:
            offset = 2 * SNAKE_SIZE;
        case RenderSnakeBodyFat:
            if (snake_part->direction == LEFT || snake_part->direction == RIGHT) {
                source->x = SNAKE_SIZE + offset;
                source->y = SNAKE_SIZE;
            } else {
                source->x = offset;
                source->y = SNAKE_SIZE;
            }
            if (snake_part->direction == NONE) {
                source->y = -SNAKE_SIZE;
                source->x = -SNAKE_SIZE;
            }
            break;
    }
}

void get_snake_rect_advanced(
    SDL_Rect* source,
    const SnakePart* snake_part,
    const RenderSnakeType type,
    SnakeDirection next_direction
) {
    source->y = type == RenderSnakeBodyFat ? 3 * SNAKE_SIZE : 4 * SNAKE_SIZE;
    switch (snake_part->direction) {
        case UP:
            source->x = next_direction == LEFT ? 3 * SNAKE_SIZE : 2 * SNAKE_SIZE;
            break;
        case LEFT:
            source->x = next_direction == UP ? 1 * SNAKE_SIZE : 2 * SNAKE_SIZE;
            break;
        case RIGHT:
            source->x = next_direction == UP ? 0 : 3 * SNAKE_SIZE;
            break;
        case DOWN:
            source->x = next_direction == LEFT ? 0 : 1 * SNAKE_SIZE;
            break;
        default: ;
    }
}

void draw_snake_segment(
    const SDL_Data* sdl,
    const SnakePart* snake_part,
    const RenderSnakeType type,
    SnakeDirection next_part_direction
) {
    SDL_Rect source, destination;
    source.w = source.h = destination.w = destination.h = SNAKE_SIZE;
    if (next_part_direction == -1 || snake_part->direction == next_part_direction) {
        get_snake_rect_simple(&source, snake_part, type);
    } else {
        get_snake_rect_advanced(&source, snake_part, type, next_part_direction);
    }
    destination.x = BOARD_OFFSET + snake_part->x * SNAKE_SIZE;
    destination.y = TOP_BAR_OFFSET + BOARD_OFFSET + snake_part->y * SNAKE_SIZE;
    SDL_BlitSurface(sdl->snake_bmp, &source, sdl->screen, &destination);
}

void render_snake(const SDL_Data* sdl, const SnakeHead* snake) {
    for (int i = 0; i < snake->body.length; i++) {
        SnakePart* snake_part = snake_segment_get(&snake->body, i);
        if (i == snake->body.length - 1) {
            draw_snake_segment(sdl, snake_part, RenderSnakeTail, -1);
            continue;
        }
        draw_snake_segment(
            sdl,
            snake_part,
            i % 2 == 0 ? RenderSnakeBodyFat : RenderSnakeBodySkinny,
            snake_segment_get(&snake->body, i + 1)->direction
        );
    }
    draw_snake_segment(sdl, &snake->head, RenderSnakeHead, -1);
}

void render_red_bar(const SDL_Data* sdl, const RedBall* red_ball) {
    int bar_height = 8;
    SDL_Rect bar = {
        BOARD_OFFSET,
        BOARD_OFFSET + TOP_BAR_OFFSET - bar_height - 4,
        (SCREEN_WIDTH - BOARD_OFFSET * 2) *
        (1 - (float) (SDL_GetTicks() - red_ball->last_bonus_time) / (float) RED_BONUS_WAIT),
        bar_height
    };
    SDL_FillRect(sdl->screen, &bar, SDL_MapRGB(sdl->screen->format, 255, 0, 0));
    char text[] = "bonus point time!!!";
    print_string(sdl, bar.x, bar.y, text);
}

void render_green_bar(const SDL_Data* sdl, const GameData* game) {
    int bar_height = 8;
    SDL_Rect bar = {
        BOARD_OFFSET,
        BOARD_OFFSET + TOP_BAR_OFFSET - bar_height - 12,
        (SCREEN_WIDTH - BOARD_OFFSET * 2) *
        (1 - (float) (SDL_GetTicks() - game->bonus_speed_time) / (float) SPEED_BONUS_TIME),
        bar_height
    };
    SDL_FillRect(sdl->screen, &bar, SDL_MapRGB(sdl->screen->format, 0, 255, 0));
    char text[] = "bonus speed time";
    print_string(sdl, bar.x, bar.y, text);
}

void render_balls(const SDL_Data* sdl, const GameData* game) {
    // render blue ball
    SDL_Rect source, destination;
    source.w = source.h = destination.w = destination.h = SNAKE_SIZE;
    source.x = 7 * SNAKE_SIZE;
    source.y = game->blue_ball->frame * SNAKE_SIZE;
    destination.x = BOARD_OFFSET + game->blue_ball->x * SNAKE_SIZE;
    destination.y = TOP_BAR_OFFSET + BOARD_OFFSET + game->blue_ball->y * SNAKE_SIZE;
    SDL_BlitSurface(sdl->snake_bmp, &source, sdl->screen, &destination);
    // render red ball
    if (game->red_ball != NULL) {
        render_red_bar(sdl, game->red_ball);
        source.w = source.h = destination.w = destination.h = SNAKE_SIZE;
        source.x = 6 * SNAKE_SIZE;
        source.y = game->red_ball->frame * SNAKE_SIZE;
        destination.x = BOARD_OFFSET + game->red_ball->x * SNAKE_SIZE;
        destination.y = TOP_BAR_OFFSET + BOARD_OFFSET + game->red_ball->y * SNAKE_SIZE;
        SDL_BlitSurface(sdl->snake_bmp, &source, sdl->screen, &destination);
    }
}

void render_end(const SDL_Data* sdl, const GameData* game, const TimeData* time) {
    SDL_Rect box = {128, 96, 224, 128};
    print_box(
        sdl, box, 2,
        SDL_MapRGB(sdl->screen->format, 0, 0, 0),
        SDL_MapRGB(sdl->screen->format, 255, 255, 255)
    );
    char text[100];
    sprintf(text, "GAME OVER!!!");
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 2, text);

    sprintf(text, "You got %d points", game->points);
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 5, text);
    char time_text[10];
    format_time(time_text, time->since_start);
    sprintf(text, "You survied for %s", time_text);
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 6 + 2, text);

    sprintf(text, "[n] - New Game");
    print_string(sdl, box.x + CHAR_SIZE * 2, box.y + box.h - CHAR_SIZE * 3, text);
    sprintf(text, "[esc] - Quit Game");
    print_string(sdl, box.x + CHAR_SIZE * 2, box.y + box.h - CHAR_SIZE * 2, text);
}

void render_win(const SDL_Data* sdl, const GameData* game, const TimeData* time) {
    SDL_Rect box = {128, 96, 224, 128};
    print_box(
        sdl, box, 2,
        SDL_MapRGB(sdl->screen->format, 0, 0, 0),
        SDL_MapRGB(sdl->screen->format, 255, 255, 255)
    );
    char text[100];
    sprintf(text, "YOU WIN!!!");
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 2, text);

    sprintf(text, "You got %d points", game->points);
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 5, text);
    char time_text[10];
    format_time(time_text, time->since_start);
    sprintf(text, "You survied for %s", time_text);
    print_string(sdl, box.x + box.w / 2 - CHAR_SIZE * (strlen(text) / 2), box.y + CHAR_SIZE * 6 + 2, text);

    sprintf(text, "[n] - New Game");
    print_string(sdl, box.x + CHAR_SIZE * 2, box.y + box.h - CHAR_SIZE * 3, text);
    sprintf(text, "[esc] - Quit Game");
    print_string(sdl, box.x + CHAR_SIZE * 2, box.y + box.h - CHAR_SIZE * 2, text);
}

void render_game(const SDL_Data* sdl, const GameData* game, const TimeData* time) {
    SDL_FillRect(sdl->screen, nullptr, SDL_MapRGB(sdl->screen->format, 0x00, 0x00, 0x00));

    char text[100];
    sprintf(text, "FPS: %.f", time->fps, game->snake.speed * (1 - (float) game->bonus_speed));
    print_string(sdl, 2, 2, text);

    char time_text[10];
    format_time(time_text, time->time);
    sprintf(text, "Time:  %s | Implemented: 1-4, A,B,C,D", time_text);
    print_string(sdl, 2, 12, text);

    format_time(time_text, time->since_start);
    sprintf(
        text, "Alive: %s | Points: %05d | Speed: %.2f", time_text, game->points,
        1 / ((float) game->snake.speed * (1 - (float) game->bonus_speed)) * 1000);
    print_string(sdl, 2, 22, text);

    SDL_Rect box = {
        BOARD_OFFSET,BOARD_OFFSET + TOP_BAR_OFFSET,
        SCREEN_WIDTH - BOARD_OFFSET * 2,
        SCREEN_HEIGHT - BOARD_OFFSET * 2 - TOP_BAR_OFFSET,
    };
    print_box(
        sdl, box, 4,
        SDL_MapRGB(sdl->screen->format, 64, 64, 64),
        SDL_MapRGB(sdl->screen->format, 255, 255, 255)
    );

    if (game->bonus_speed != 0) {
        render_green_bar(sdl, game);
    }
    render_balls(sdl, game);
    render_snake(sdl, &game->snake);

    switch (game->state) {
        case GameState_Dead:
            render_end(sdl, game, time);
            break;
        case GameState_Win:
            render_win(sdl, game, time);
            break;
    }

    SDL_UpdateTexture(sdl->texture, nullptr, sdl->screen->pixels, sdl->screen->pitch);
    SDL_RenderCopy(sdl->renderer, sdl->texture, nullptr, nullptr);
    SDL_RenderPresent(sdl->renderer);
}
