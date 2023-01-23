/***************************************************
 * mathgame.c
 * Author:Selin Butun 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
	printf("Welcome to Math Game!\n How many rounds do you want to play?\n");
	int response = 0;
	scanf("%d",&response);
	int correct = 0;

	for(int i = 0; i < response; i++){
		int a = rand() % 10;
		int b = rand() % 10;
		int answer = a + b;
		int userAnswer;
		printf("%d + %d = ?\n", a, b);
		scanf("%d", &userAnswer);
		if(userAnswer == answer){
			printf("Correct answer!\n");
			correct++;
		}
		else{
			printf("Incorrect :(\n");
		}
	}
	printf("You answered %d out of %d correct! \n", correct, response);	
	
  return 0;
}
