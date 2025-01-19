#ifndef SCORES_H
#define SCORES_H
#include "constants.h"

typedef struct {
    char name[NAME_LENGTH+1];
    int score;
} Score;

typedef struct {
    Score* scores;
    int length;
} Scores;

Scores read_scores();
void write_scores(const Scores* scores);

#endif //SCORES_H
