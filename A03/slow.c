/*----------------------------------------------
 * Author: Selin Butun
 * Date: February 8, 2023
 * Description: Program that turns a phrase into ent-speech.
 * Ent-phrases contain '.' between each letter in the input.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void entify(char *response, int pause, int length){
    for(int i = 0; i < length-1; i++){
        printf("%c", response[i]);
        for(int j = 0; j < pause; j++){
            printf(".");
        }
    }
    printf("\n");
}

int main() {
    printf("Enter a word: ");
    char *response = malloc(sizeof(char) * 32);
    fgets(response, 32, stdin); // get length of string bc of /0
    int length = strlen(response);
    printf("enter pause length: ");
    int pause;
    scanf("%d", &pause);
    entify(response, pause, length);
    free(response);
    return 0;
}
