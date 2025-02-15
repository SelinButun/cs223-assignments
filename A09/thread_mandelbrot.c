/*----------------------------------------------
 * Author: Selin Butun 
 * Date: April 5, 2023
 * Description: Program that outputs a PPM image 
 * of the mandelbrot set with randomized colors
 * using threads.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/shm.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <pthread.h>

void quadrant(double beginRow, double endRow, double beginCols, double endCols,int maxIterations, 
int size, float ymax, float ymin, float xmax, float xmin, struct ppm_pixel *palette, 
struct ppm_pixel **image){
  float xtmp;
  float xfrac;
  float yfrac;
  float x0;
  float y0;
  float x = 0;
  float y = 0;
  int iter = 0;
  for(int j = beginRow; j < endRow; j++){
    for(int k = beginCols; k < endCols; k++){
      xfrac = (float)k / size;
      yfrac = (float)j / size;
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);
      x = 0;
      y = 0;
      iter = 0;

      while ((iter < maxIterations) && (x*x + y*y < 2*2)){
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }
      if (iter < maxIterations){ // escaped
        image[j][k].red = palette[iter].red;
        image[j][k].green = palette[iter].green;
        image[j][k].blue = palette[iter].blue;
      }
      else{
        image[j][k].red = 0;
        image[j][k].green = 0;
        image[j][k].blue = 0;
      }
    }
  }
}

struct ThreadStruct{
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  double beginRow;
  double endRow;
  double beginCols;
  double endCols;
  struct ppm_pixel *palette;
  struct ppm_pixel **image;
};

void *ThreadFunction(void *id){
  struct ThreadStruct *threadObj = (struct ThreadStruct*) id;
  quadrant(threadObj -> beginRow, threadObj -> endRow, threadObj -> beginCols, threadObj -> endCols, 
  threadObj -> maxIterations, threadObj -> size, threadObj -> ymax, threadObj -> ymin, threadObj -> xmax,
  threadObj -> xmin, threadObj -> palette, threadObj -> image);
  return NULL;
}

int main(int argc, char* argv[]) {
  int size = 480;
  
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  srand(time(0));
  struct ppm_pixel *palette = malloc(maxIterations * sizeof(struct ppm_pixel));
  for(int i  = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  struct ppm_pixel **image = malloc(size * sizeof(struct ppm_pixel*));
  for(int p = 0; p < size; p++){
    image[p] = malloc(size * sizeof(struct ppm_pixel));
  }

  double beginRow = 0;
  double endRow = 0;
  double beginCols = 0;
  double endCols = 0;
  float xtmp;
  struct timeval tstart, tend;
  double timer;
  pthread_t thread1, thread2, thread3, thread4;
  long id1 = 1, id2 = 2, id3 = 3, id4 = 4;

  struct ThreadStruct *datas = malloc(4 * sizeof(struct ThreadStruct));

  gettimeofday(&tstart, NULL);
  for(int c = 0; c < numProcesses; c++){
      if(c == 0){
        datas[c].beginRow = 0;
        datas[c].endRow = 240;
        datas[c].beginCols = 0;
        datas[c].endCols = 240;
        datas[c].size = 480;
        datas[c].xmin = -2.0;
        datas[c].xmax = 0.47;
        datas[c].ymin = -1.12;
        datas[c].ymax = 1.12;
        datas[c].maxIterations = 1000;
        datas[c].palette = palette;
        datas[c].image = image;
        pthread_create(&thread1, NULL, ThreadFunction, &(datas[c]));
        printf("Launched thread process: %lu\n", id1);
        printf("%lu) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  id1, datas[c].beginCols, datas[c].endCols, datas[c].beginRow, datas[c].endRow);
      }
      else if(c == 1){
        datas[c].beginRow = 0;
        datas[c].endRow = 240;
        datas[c].beginCols = 240;
        datas[c].endCols = 480;
        datas[c].size = 480;
        datas[c].xmin = -2.0;
        datas[c].xmax = 0.47;
        datas[c].ymin = -1.12;
        datas[c].ymax = 1.12;
        datas[c].maxIterations = 1000;
        datas[c].palette = palette;
        datas[c].image = image;
        pthread_create(&thread2, NULL, ThreadFunction, &(datas[c]));
        printf("Launched thread process: %lu\n", id2);
        printf("%lu) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  id2, datas[c].beginCols, datas[c].endCols, datas[c].beginRow, datas[c].endRow);
      }
      else if(c == 2){
        datas[c].beginRow = 240;
        datas[c].endRow = 480;
        datas[c].beginCols = 0;
        datas[c].endCols = 240;
        datas[c].size = 480;
        datas[c].xmin = -2.0;
        datas[c].xmax = 0.47;
        datas[c].ymin = -1.12;
        datas[c].ymax = 1.12;
        datas[c].maxIterations = 1000;
        datas[c].palette = palette;
        datas[c].image = image;
        pthread_create(&thread3, NULL, ThreadFunction, &(datas[c]));
        printf("Launched thread process: %lu\n", id3);
        printf("%lu) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  id3, datas[c].beginCols, datas[c].endCols, datas[c].beginRow, datas[c].endRow);
      }
      else if(c == 3){
        datas[c].beginRow = 240;
        datas[c].endRow = 480;
        datas[c].beginCols = 240;
        datas[c].endCols = 480;
        datas[c].size = 480;
        datas[c].xmin = -2.0;
        datas[c].xmax = 0.47;
        datas[c].ymin = -1.12;
        datas[c].ymax = 1.12;
        datas[c].maxIterations = 1000;
        datas[c].palette = palette;
        datas[c].image = image;
        pthread_create(&thread4, NULL, ThreadFunction, &(datas[c]));
        printf("Launched thread process: %lu\n", id4);
        printf("%lu) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  id4, datas[c].beginCols, datas[c].endCols, datas[c].beginRow, datas[c].endRow);
      }
  }

  pthread_join(thread1, NULL);
  printf("Thread process complete: %lu\n", id1);
  pthread_join(thread2, NULL);
  printf("Thread process complete: %lu\n", id2);
  pthread_join(thread3, NULL);
  printf("Thread process complete: %lu\n", id3);
  pthread_join(thread4, NULL);
  printf("Thread process complete: %lu\n", id4);

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  char *filename = malloc(sizeof(char) * 64);
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm_2d(filename, image, size, size);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  printf("Writing file: %s\n", filename);
  free(filename);
  free(palette);
  for (int m = 0; m < size; m++){
    free(image[m]);
  }
  free(image);
  free(datas);
}
