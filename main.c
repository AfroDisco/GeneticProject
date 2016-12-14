#include "tools.h"

#include <stdio.h>

int main(int argc, char** argv){
    if(argc != 8 && argc != 9){
        printf("programName Height Width NbProcesses NbCreatures DeletionPercentage MutationRate NbMoves [Area.file]");
        return -1;
    }

    Param* parameters = parseArguments(argc, argv);
    if(!parameters){
        printf("Error creating parameters");
        return -1;
    }

    displayBoard(parameters->env->array, parameters->env->columns, parameters->env->rows);

//    readFile("Flat.txt");
}


