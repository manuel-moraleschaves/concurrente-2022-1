#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tetris_figure_factory.h"

typedef struct {
    size_t id;
    int depth;
    int rows;
    int columns;
    int** matrix;
    char* figure_sequence;
} tetris_t;

void** create_matrix(size_t row_count, size_t col_count, size_t element_size);
void free_matrix(const size_t row_count, void** matrix);


int main(int argc, char** argv) {    
    char file_name[100];

    if (argc == 2) {
        if (sscanf(argv[1], "%s", file_name) != 1 || errno) {
            fprintf(stderr, "Error: invalid file name.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Error: please provide a initial file.\n");
        return 2;
    }

    printf("File Name: %s\n", file_name); 
    FILE* file = fopen(file_name, "r"); // read only  
    
    if (file) {
        tetris_t* tetris = (tetris_t*) calloc(1, sizeof(tetris_t));
        size_t st_line; 
        int i_line;

        fscanf(file, "%zu", &st_line);
        if (st_line <= 0) {
            fprintf(stderr, "Error: invalid identifier in the file.\n");
            return 3;    
        }
        tetris->id = st_line;
        printf("ID: %zu\n", tetris->id);
        

        fscanf(file, "%d", &i_line);
        if (i_line < 0) {
            fprintf(stderr, "Error: invalid depth in the file.\n");
            return 4;    
        }
        tetris->depth = i_line;
        printf("Depth: %d\n", tetris->depth); 

        fscanf(file, "%d", &i_line);
        if (i_line <= 0) {
            fprintf(stderr, "Error: invalid rows in the file.\n");
            return 5;    
        }
        tetris->rows = i_line;
        printf("Rows: %d\n", tetris->rows); 

        fscanf(file, "%d", &i_line);
        if (i_line <= 0) {
            fprintf(stderr, "Error: invalid columns in the file.\n");
            return 6;    
        }
        tetris->columns = i_line;
        printf("Columns: %d\n", tetris->columns); 

        tetris->matrix = (int**)create_matrix(tetris->rows, tetris->columns, sizeof(int));

        int value = fgetc(file);
        for (int x = 0; x < tetris->rows; ++x) {
            for (int y = 0; y < tetris->columns; ++y) {
                value = fgetc(file);
                printf( "%c", value );
                tetris->matrix[x][y] = value;
            }
            fgetc(file);
            printf("\n");
        }

        fscanf(file, "%d", &i_line);
        if (i_line <= 0 || i_line < tetris->depth+1) {
            fprintf(stderr, "Error: invalid figure count in the file.\n");
            return 7;    
        }
        int k = i_line;
        printf("K: %d\n", k); 

        char letter;
        tetris->figure_sequence = (char*) calloc(k, sizeof(char));

        for (int i = 0; i < k; ++i) {
            fscanf(file, "%s", &letter);
            tetris->figure_sequence[i] = letter;
        }
        printf("Figure sequence: %s\n", tetris->figure_sequence);
  
        free_matrix(tetris->rows, (void**)tetris->matrix);
        fclose(file);
        
    } else {
        fprintf(stderr, "Error: could not open the file %s\n", file_name);
        return 3;
    }
    

    return EXIT_SUCCESS;
}

void** create_matrix(size_t row_count, size_t col_count, size_t element_size) {
    void** matrix = (void**) calloc(row_count, sizeof(void*));
    if (matrix == NULL) {
        return NULL;
    }

    for (size_t row = 0; row < row_count; ++row) {
        if ((matrix[row] = calloc(col_count, element_size)) == NULL) {
            free_matrix(row_count, matrix);
            return NULL;
        }
    }

    return matrix;
}

void free_matrix(const size_t row_count, void** matrix) {
    if (matrix) {
        for (size_t row = 0; row < row_count; ++row) {
            free(matrix[row]);
        }
    }

    free(matrix);
}