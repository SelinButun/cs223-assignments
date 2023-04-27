#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

void search(int begin, int end, int* array, int input, int id){
  for(int i = begin; i < end; i++){
    if(array[i] == input){
      printf("Thread %d found %d at index %d\n", id, input, i);
      //*value = 50;
      break;
    }
    else{
      if(i + 1 == end){
        printf("not found\n");
      }
    }
  }
}

struct ThreadStruct{
  int begin;
  int end;
  int* array;
  int input;
  //int* value;
  int id;
};

void *ThreadFunction(void *id){
  struct ThreadStruct *threadObj = (struct ThreadStruct*) id;
  search(threadObj->begin, threadObj->end, threadObj->array, threadObj->input,
  threadObj-> id);
  return NULL;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }

  FILE* fp = fopen("data.bin", "rb");
  int num;
  fread(&num, sizeof(int), 1, fp);
  int *array = malloc(num * sizeof(int));
  fread(array, sizeof(int), num, fp);
  int input;
  printf("Enter a value to search: ");
  scanf("%d", &input);
  //int value = 900;

  pthread_t thread1, thread2;
  long id = 1, id2 = 2;
  struct ThreadStruct *datas = malloc(2 * sizeof(struct ThreadStruct));
  for(int i = 0; i < 2; i++){
    if(i == 0){
      datas[i].array = array;
      datas[i].input = input;
      datas[i].begin = 0;
      datas[i].end = num/2;
      //datas[i].value = value;
      datas[i].id = id;
      pthread_create(&thread1, NULL, ThreadFunction, &(datas[i]));
    }
    if(i == 1){
      datas[i].array = array;
      datas[i].input = input;
      datas[i].begin = num/2;
      datas[i].end = num;
      //datas[i].value = value;
      datas[i].id = id2;
      pthread_create(&thread2, NULL, ThreadFunction, &(datas[i]));
    }
  }
  // if(value == 0){
  //   printf("not found\n");
  // }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  free(datas);
  free(array);
  fclose(fp);

  return 0;
}
