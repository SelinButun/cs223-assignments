/*----------------------------------------------
 * Author: Selin Butun 
 * Date: Feb 22, 2023
 * Description: Program that makes a crossword with 
 * two inputted words.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <word1> <word2>\n", argv[0]);
        exit(0);
    }
    int length1 = strlen(argv[1]);
    int length2 = strlen(argv[2]);
    char match = '@';
    int rowIndex;
    int colIndex;
    char **cross;
    cross = malloc(sizeof(char *) * length1);

    for (int i = 0; i < length1; i++) {
        cross[i] = malloc(sizeof(char) * length2);
    }

    for(int r = 0; r < length1; r++){
        for(int j = 0; j < length2; j++){
            if(argv[1][r] == argv[2][j]){
                match = argv[2][j];
                rowIndex = r;
                colIndex = j;
		break;
            }
        }
	if(match != '@'){
	    break;
	}
    }

    if(match == '@'){
        printf("No match");
        exit(1);
    }
    
    for(int k = 0; k < length1; k++){
        for(int p = 0; p < length2; p++){
            if(p != colIndex && k != rowIndex){
                cross[k][p] = '.';
            }
            else if(k == rowIndex){
                cross[k][p] = argv[2][p];
            }
	    else if(p == colIndex){
		cross[k][p] = argv[1][k];
	    }
        }
    }

    for(int y = 0; y < length1; y++){
	for(int d = 0; d <length2; d++){
	   printf("%c", cross[y][d]);
	}
	printf("\n");
    }

    free(cross);
    return 0;
}
   
