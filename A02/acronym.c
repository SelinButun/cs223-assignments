#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//acronym function gets the first letter of each word in phrase and stores it
//ONLY if it's an upper case letter. If it's lower case, it will not get stored
//as a part of the final acronym
void acronym(char *response, int size){
    int index = 0;
    char *acro = malloc(sizeof(char) * 1024);
    for (int i = 0; i < strlen(response); i++) {
        if (i == 0 && response[i] != ' ' && response[i] >= 65 && response[i] <= 90) {
            acro[index] = response[i];
            index++;
        }
        else if (i > 0 && response[i - 1] == ' ' && response[i] >= 65 && response[i] <= 90) {
            acro[index] = response[i];
            index++;
        }
    }
    acro[index] = '\0';
    printf("Your acronym is: %s\n", acro);
    free(acro);
}

//main method takes in user phrase and calls on acronym method with it
int main(){
    printf("Enter a phrase: ");
    char *response = malloc(sizeof(char) * 1024);
    fgets(response, 1024, stdin);
    int size = strlen(response);
    acronym(response, size);
    free(response);
    return 0;
}


