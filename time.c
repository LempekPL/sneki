#include "time.h"
#include <SDL_timer.h>

TimeData initTime() {
    TimeData time = {0};
    time.delta = 0;
    time.time = 0;
    time.fps = 0;
    time.fps_timer = 0;
    time.frames = 0;
    time.last_tick = 0;
    return time;
}

void time_update(TimeData* time) {
    unsigned int current_tick = SDL_GetTicks();
    time->delta = (current_tick - time->last_tick) * 0.001;
    time->last_tick = current_tick;
    time->time += time->delta;
    time->fps_timer += time->delta;
    if(time->fps_timer > 0.5) {
        time->fps = time->frames * 2;
        time->frames = 0;
        time->fps_timer -= 0.5;
    };
}

void format_time(char text[10], int time) {
    const int seconds = time % 60;
    const int minutes = (seconds / 60) % 60;
    const int hours = (seconds / 3600) % 24;
    sprintf(text, "%02d:%02d:%02d", hours, minutes, seconds);
}