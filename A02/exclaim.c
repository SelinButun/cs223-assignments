#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//exclaim method that converts lowercase letter to given chars
void exclaim(char *response, int size){
    for (int i = 0; i < size; i++){
	//range of lowercase characters
        if (response[i] >= 97 && response[i] <= 122){
            int forChar = rand() % 4;
            if (forChar == 0){
                response[i] = '@';
            }
            else if (forChar == 1){
                response[i] = '!';
            }
            else if (forChar == 3){
                response[i] = '#';
            }
            else{
                response[i] = '*';
            }
        }
    }

    printf("OMG! %s", response);
}

//main method scans in the user input
int main(){
    printf("Enter a word: ");
    char *response = malloc(sizeof(char) * 32);
    fgets(response, 32, stdin); // get length of string bc of /0
    int size = strlen(response);
    exclaim(response, size);
    free(response);
    return 0;
}
