#include <stdlib.h>
#include <stdio.h>

typedef struct {
    
}

int main(int argc, char *argv[]) {
    
    if (argv != 4) {
        printf("There must be 4 command line arguments");
    }

    // Get command line arguments
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];
    int option = atoi(argv[3]);
    
    FILE *fp = fopen(inputFileName, "r");
    if (!fp) { 
        perror("Can't find the file pal!")
        return 1;
    }

    fseek



    return 1;

}