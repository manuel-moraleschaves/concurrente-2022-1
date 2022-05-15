// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#ifndef TETRIS_SOLVER_SERIAL_TETRIS_FIGURE_FACTORY_H_
#define TETRIS_SOLVER_SERIAL_TETRIS_FIGURE_FACTORY_H_

typedef struct figure {
    int height;
    int width;
    char** value;
} figure_t;

/**
 * @brief Gets the number of possible rotations for a given tetris figure.
 * @details Given a tetris figure it returns the number of possible rotations depending on the shape
 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @return Integer value representing the number of rotations (1, 2 or 4). If figure is not valid it returns -1.
 *
 */
int get_tetris_figure_num_rotations(char figure);

/**
 * @brief Gets a figure_t ptr for the given symbol and number of rotations.
 * @details Returns a figure_t ptr representing the given symbol figure rotated clockwise based on the given num_rotations
 * @param figure Character symbol representing a tetris figure. Accepted symbols: 'I', 'Z', 'S', 'L', 'J', 'O', 'T'
 * @param num_rotations Number of rotations (clockwise) to apply to the figure. Accepted values: 1, 2, 4
 * @return figure_t ptr. If figure symbol is not valid or num_rotations is not valid for the given figure, it returns NULL
 *
 */
figure_t* get_tetris_figure(char figure, int num_rotations);

#endif  // TETRIS_SOLVER_SERIAL_TETRIS_FIGURE_FACTORY_H_
