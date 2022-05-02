// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include "tetris_figure_factory.h"
#include <stdlib.h>

#define ONE_ROTATION 1
#define TWO_ROTATIONS 2
#define FOUR_ROTATIONS 4
#define INVALID_ROTATIONS -1

figure_t I[TWO_ROTATIONS] = {{1,  4,  (char *[]){"1111"}},
                       {4,  1,  (char *[]){"1",  "1",  "1",  "1"}}};

figure_t Z[TWO_ROTATIONS] = {{2,  3,  (char *[]){"220",  "022"}},
                       {3,  2,  (char *[]){"02",  "22",  "20"}}};

figure_t S[TWO_ROTATIONS] = {{2,  3,  (char *[]){"033",  "330"}},
                       {3,  2,  (char *[]){"30",  "33",  "03"}}};

figure_t L[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"004",  "444"}},
    {3,  2,  (char *[]){"40",  "40",  "44"}},
    {2,  3,  (char *[]){"444",  "400"}},
    {3,  2,  (char *[]){"44",  "04",  "04"}}
};

figure_t J[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"500",  "555"}},
    {3,  2,  (char *[]){"55",  "50",  "50"}},
    {2,  3,  (char *[]){"555",  "005"}},
    {3,  2,  (char *[]){"05",  "05",  "55"}}
};

figure_t O[ONE_ROTATION] = {{2,  2,  (char *[]){"66",  "66"}}};

figure_t T[FOUR_ROTATIONS] = {
    {2,  3,  (char *[]){"070",  "777"}},
    {3,  2,  (char *[]){"70",  "77",  "70"}},
    {2,  3,  (char *[]){"777",  "070"}},
    {3,  2,  (char *[]){"07",  "77",  "07"}}
};

int get_tetris_figure_num_rotations(char figure) {
    int num_rotations;
    switch (figure) {
        case 'I':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'Z':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'S':
            num_rotations = TWO_ROTATIONS;
            break;
        case 'L':
            num_rotations = FOUR_ROTATIONS;
            break;
        case 'J':
            num_rotations = FOUR_ROTATIONS;
            break;
        case 'O':
            num_rotations = ONE_ROTATION;
            break;
        case 'T':
            num_rotations = FOUR_ROTATIONS;
            break;
        default:
            num_rotations = INVALID_ROTATIONS;
            break;
    }

    return num_rotations;
}

figure_t* get_tetris_figure(char figure, int num_rotations) {
    figure_t* tetris_figure;
    int max_num_rotations = get_tetris_figure_num_rotations(figure);

    if (num_rotations < 0 || num_rotations >= max_num_rotations) {
        return NULL;
    }

    switch (figure) {
        case 'I':
            tetris_figure = &I[num_rotations];
            break;
        case 'Z':
            tetris_figure = &Z[num_rotations];
            break;
        case 'S':
            tetris_figure = &S[num_rotations];
            break;
        case 'L':
            tetris_figure = &L[num_rotations];
            break;
        case 'J':
            tetris_figure = &J[num_rotations];
            break;
        case 'O':
            tetris_figure = &O[num_rotations];
            break;
        case 'T':
            tetris_figure = &T[num_rotations];
            break;
        default:
            tetris_figure = NULL;
            break;
    }

    return tetris_figure;
}
