#include "scores.h"
#include <stdio.h>
#include <stdlib.h>

// amount
// name1 score1
// name2 score2
// name3 score3

void write_score(FILE* score_file, Score* score) {
    fprintf(score_file, "%s %d\n", score->name, score->score);
}

void write_scores(const Scores* scores) {
    FILE* score = fopen("./scores.txt", "w");
    fprintf(score, "%d\n", scores->length);
    for (int i = 0; i < scores->length; i++) {
        write_score(score, scores->scores + i);
    }
    fclose(score);
}

Scores read_scores() {
    FILE* score = fopen("./scores.txt", "r");
    if (score == NULL) {
        fclose(score);
        return (Scores) {malloc(3 * sizeof(Score)), 0};
    }
    Scores scores;
    int amount = 0;
    fscanf(score, "%d\n", &amount);
    scores.length = amount;
    scores.scores = malloc(4 * sizeof(Score));
    for (int i = 0; i < amount; i++) {
        fscanf(score, "%s %d\n", &scores.scores[i].name, &scores.scores[i].score);
    }
    fclose(score);
    return scores;
}
