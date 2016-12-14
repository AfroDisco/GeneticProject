#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ISCHAR 1

/**
 * Display the environment can be modified to add creature and other things
 * @param array the array to display
 * @param row The number of rows
 * @param column The number of columns
 */
void displayBoard(int** array, int row, int column){
    for(int j = row-1; j >= 0; --j) {
        for(int i = 0; i < column; ++i) {
#if ISCHAR
            if(array[i][j] == 0)
                printf(" ");
            if(array[i][j] == 1)
                printf("#");
            if(array[i][j] == 2)
                printf("S");
            if(array[i][j] == 3)
                printf("E");
#else
            printf("%d ", array[i][j]);
#endif
        }
        printf("\n");
    }
}

/**
 * Display the menu in console
 */
void displayMenu(){
    printf("1 - Request the next generation\n");
    printf("2 - Request the next X generations, X in [1, 100]\n");
    printf("3 - Display the best creature and his performance\n");
}

/**
 * Allocate and initialize a Coord
 * @param x X-coordinate
 * @param y Y-coordinate
 * @return The newly allocated Coord, NULL if an error occurs
 */
Coord* initCoord(int x, int y){
    Coord* tmp = malloc(sizeof(Coord));
    if(!tmp){
        printf("Cannot allocate memory for Coord\n");
        return NULL;
    }

    tmp->x = x;
    tmp->y = y;

    return tmp;
}

/**
* Check if the x and y-coordinates are in the correct boundaries
* @param x X-coordinate
* @param y Y-coordinate
* @return True if there is at least an error, false otherwise
*/
bool checkNotInEnv(int x, int y){
    if(x < 5 || x > 10){
        printf("The width is not in [5,10]: %d\n", x);
        return true;
    }

    if(y < 3 || y > 5){
        printf("The height is not in [3,5]: %d\n", y);
        return true;
    }

    return false;
}

/**
 * Allocate a 2D array of int
 * @param column Number of columns
 * @param row Number of rows
 * @return The newly allocated and initialized (0) array, NULL if an error occurs
 */
Array* create2DArray(size_t column, size_t row){
    Array* arr = malloc(sizeof(Array));
    if(!arr){
        printf("Malloc error for environment.\n");
        return NULL;
    }

    arr->start = NULL;
    arr->end = NULL;

    arr->array = malloc(sizeof(int*) * column);
    if(!arr->array){
        printf("Malloc error for environment\n");
        return NULL;
    }

    for(unsigned int k = 0; k < column ; k++){
        arr->array[k] = malloc(sizeof(int) * row);

        if(!arr->array[k]){
            while(k > 0){
                free(arr->array[k-1]);
                k--;
            }

            free(arr->array);
            printf("Malloc error for environment\n");
            return NULL;
        }
    }

    for(unsigned int i = 0; i < column; ++i) {
        for(unsigned int j = 0; j < row; ++j) {
            arr->array[i][j] = 0;
        }
    }

    return arr;
}

/**
 * Deallocate the array
 * @param array The array to deallocate
 */
void free2DArray(Array* array){
    for(int i = 0; i < array->rows; ++i) {
        free(array->array[i]);
    }

    free(array->array);
    free(array);
}

/**
 * Read the environment from a file
 * @param filename Name of the file
 * @return An allocated array containning the environment and the start/end coordinates, NULL if an error occurs
 */
Array* readFile(char* filename){
    //filename == null
    if(!filename) {
        printf("Filename is NULL\n");
        return NULL;
    }

    //Cannot open file
    FILE* env = fopen(filename, "r");
    if(!env) {
        printf("Cannot open file\n");
        return NULL;
    }

    int x, y;
    Coord *start;
    Coord *end;

    fscanf(env, "%d %d", &x, &y);
    start = initCoord(x, y);
    if(!start){
        printf("Memory error with start.\n");
        return NULL;
    }

    fscanf(env, "%d %d", &x, &y);
    end = initCoord(x, y);
    if(!end){
        printf("Memory error with end.\n");
        free(start);
        return NULL;
    }

    int xMax = 0, yMax = 0;

    while(fscanf(env, "%d %d", &x, &y) == 2){
        xMax = (xMax < x+1) ? x+1: xMax;
        yMax = (yMax < y+1) ? y+1: yMax;
    }

    if(checkNotInEnv(xMax, yMax)){
        printf("File incorrect\n");
        free(start);
        free(end);
        fclose(env);
        return NULL;
    }

    Array* environment = create2DArray(xMax, yMax);
    if(!environment){
        printf("Error creating environment\n");
        free(start);
        free(end);
        fclose(env);
        return NULL;
    }

    fseek(env, 0, SEEK_SET);

    while(fscanf(env, "%d %d", &x, &y) == 2){
        environment->array[x][y] = 1;
    }

    environment->array[start->x][start->y] = 2;    //set start
    environment->array[end->x][end->y] = 3;    //set end
    environment->start = start;
    environment->end = end;

    fclose(env);
    displayBoard(environment->array, yMax, xMax);

    return environment;
}

/**
 * Parse arguments given to the program
 * @param argc Number of arguments
 * @param argv All arguments
 * @return An allocated Struct Param_t, NULL if an error occurs
 */
Param* parseArguments(int argc, char **argv) {
    Param* param = malloc(sizeof(Param));
    if(!param){
        printf("Memory error creating parameters");
        return NULL;
    }

    if(argc == 9){
        param->env = readFile(argv[8]);
    }

    param->height = atoi(argv[1]);
    param->width = atoi(argv[2]);
    param->processes = atoi(argv[3]);
    param->creatures = atoi(argv[4]);
    param->delPerc = atoi(argv[5]);
    param->mutation = atoi(argv[6]);
    param->moves = atoi(argv[7]);

    if(param->height < 3 || param->height > 5 || param->width < 5 || param->width > 10){
        free2DArray(param->env);
        free(param);
        printf("Error Height and/or Width is/are not conform\n");
        return NULL;
    }

    if(param->processes < 1 || param->processes > 10 || param->creatures < 100 || param->creatures > 1000){
        free2DArray(param->env);
        free(param);
        printf("Error number of processes and/or creatures is/are not conform\n");
        return NULL;
    }

    if(param->delPerc < 20 || param->delPerc > 80 || param->mutation < 1 || param->mutation > 10){
        free2DArray(param->env);
        free(param);
        printf("Error Percentage of deletion and/or mutation is/are not conform\n");
        return NULL;
    }

    if(param->moves < 5 || param->moves > 20){
        free2DArray(param->env);
        free(param);
        printf("Error number of moves is not conform\n");
        return NULL;
    }

    return param;
}

/**
 * Parse the input of the console
 * @return  0 for help/reminder
 *          1 for first option
 *          2 for second option
 *          3 for third option
 */
int parseInputMenu(){
    int in;
    scanf("%d", &in);
    switch (in){
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        default:
            return 0;
    }
}

/**
 * Generate random moves
 * @param moves The arrays of moves
 * @param size Number of moves
 */
void generateRandomMove(int* moves, int size){
    srand(time(NULL));
    for (int i = 0; i < size; ++i) {
        moves[i] = rand()%8;
    }
}

/**
 * Generate random moves based on the existing one
 * @param moves The arrays of moves
 * @param size Number of moves
 * @param mut Mutation percentage
 */
void mutateMove(int* moves, int size, int mut){
    srand(time(NULL));
    int tmp;
    for (int i = 0; i < size; ++i) {
        if(rand()%100 < mut) {
            tmp = rand()%8;
            if(tmp != moves[i])
                moves[i] = tmp;
            else
                moves[i] = (tmp+1)%8;
        }
    }
}

/**
 * Pre-calculated scores
 * @param score The array where score will be stored
 * @param row Number of rows in the array
 * @param column Number of columns in the array
 */
void createScore(int** score, int row, int column){
    //todo
}

/**
 * Compute result of a creature's moves
 * @param moves Moves to perform
 * @param env Environment where the creature is in
 * @return The score of the creature after his moves
 */
int doMoves(int* moves, Array* env){
    //todo
}
