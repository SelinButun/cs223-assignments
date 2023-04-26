#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void search(int * array, int begin, int end, int input, int pid, int *value){
  for(int i = begin; i < end; i++){
    if(array[i] == input){
      if(pid == 0){
        printf("Child found %d at index %d\n", input, i);
        *value = 50;
        break;
      }
      else{
        printf("Parent found %d at index %d\n", input, i);
        *value = 50;
        break;
      }
    }
    else{
      *value = 0;
    }
  }
  
}

int main() {
  FILE* fp = fopen("data.bin", "rb");
  int num;
  fread(&num, sizeof(int), 1, fp);
  int *array = malloc(num * sizeof(int));
  fread(array, sizeof(int), num, fp);
  int input;
  printf("Enter a value to search: ");
  scanf("%d", &input);
  int value = 900;

  int pid = fork();

  for(int i = 0; i < 1; i++){
    if(pid == 0){
      search(array, 0, (num/2), input, pid, &value);
    }
    else{
      search(array, (num/2), num, input, pid, &value);
    }
  }
  if(value == 0){
    printf("not found\n");
  }

  fclose(fp); 
  free(array);
}