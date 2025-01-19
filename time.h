#ifndef TIME_H
#define TIME_H

typedef struct time_data_t {
    double delta, time, since_start, fps, fps_timer;
    int frames;
    unsigned int last_tick;
} TimeData;

TimeData initTime();
void time_update(TimeData* time);
void format_time(char text[10], int time);

#endif //TIME_H
