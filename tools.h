#ifndef GENETICALGORITHM_TOOLS_H
#define GENETICALGORITHM_TOOLS_H

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

struct Coord_t{
    int x;
    int y;
};

struct Array_t{
    int** array;
    int rows;
    int columns;

    struct Coord_t* start;
    struct Coord_t* end;
};

struct Param_t{
    int height;
    int width;
    int processes;
    int creatures;
    int delPerc;
    int mutation;
    int moves;
    struct Array_t* env;
};

typedef struct Coord_t Coord;
typedef struct Array_t Array;
typedef struct Param_t Param;

void displayBoard(int** array, int row, int column);
Coord* initCoord(int x, int y);
bool checkNotInEnv(int x, int y);
Array* create2DArray(size_t column, size_t row);
void free2DArray(Array* array);
Array* readFile(char* filename);
Param* parseArguments(int argc, char **argv);
int parseInputMenu();
void generateRandomMove(int* moves, int size);
void mutateMove(int* moves, int size, int mut);
void createScore(int** score, int row, int column);
int doMoves(int* moves, Array* env);


#endif //GENETICALGORITHM_TOOLS_H
