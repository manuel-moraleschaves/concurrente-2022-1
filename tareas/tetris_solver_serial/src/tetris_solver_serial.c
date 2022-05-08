// Copyright 2022 Manuel Morales Chaves <manuel.moraleschaves@ucr.ac.cr>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tetris_state.h"


int main(int argc, char** argv) {
    char file_name[100] = "./test/Test1.txt";

    if (argc == 2) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 1;
        }
    }  // else {
    //    fprintf(stderr, "Error: please provide a initial file.\n");
    //    return 2;
    //}

    printf("File Name: %s\n", file_name);
    FILE* file = fopen(file_name, "r");  // read only

    if (file) {
        tetris_t* tetris = read_tetris(file);

        if (!tetris) {
            fprintf(stderr, "Error: invalid file content.\n");
            return 3;
        }

        // TODO(manum): BORRAR
        FILE* file2 = fopen("Out.txt", "w");

        solve_tetris_dfs(tetris, 0, file2);

        destroy_tetris(tetris);
        fclose(file);
        // TODO(manum): BORRAR
        fclose(file2);

    } else {
        fprintf(stderr, "Error: could not open the file %s.\n", file_name);
        return 4;
    }


    return EXIT_SUCCESS;
}
