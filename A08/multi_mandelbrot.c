/*----------------------------------------------
 * Author: Selin Butun 
 * Date: March 29, 2023
 * Description: Program that outputs a PPM image 
 * of the mandelbrot set with randomized colors.
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

<<<<<<< HEAD
void quadrant(double beginRow, double endRow, double beginCols, double endCols,int maxIterations, 
int size, float ymax, float ymin, float xmax, float xmin, struct ppm_pixel *palette, 
struct ppm_pixel **image){
=======
void quadrant(double beginRow, double endRow, double beginCols, double endCols,int maxIterations, int size, 
	      float ymax, float ymin, float xmax, float xmin, struct ppm_pixel *palette, struct ppm_pixel **image){
>>>>>>> 1dde0512d1a6c3f62e8d9388005f35aa87c0fa71
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

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  } 
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  int shmid;
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size *size, 0644 | IPC_CREAT); 
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  struct ppm_pixel *raw = shmat(shmid, NULL, 0);
  if (raw == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 
  struct ppm_pixel **image = malloc(size * sizeof(struct ppm_pixel*));
  for(int p = 0; p < size; p++){
    image[p] = &(raw[p * size]);
  }
    
  srand(time(0));
  struct ppm_pixel *palette = malloc(maxIterations * sizeof(struct ppm_pixel));
  for(int i  = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }


  double beginRow = 0;
  double endRow = 0;
  double beginCols = 0;
  double endCols = 0;
  float xtmp;
  struct timeval tstart, tend;
  double timer;
  
  gettimeofday(&tstart, NULL);
  for(int c = 0; c < numProcesses; c++){
    int pid = fork();
    if(pid == 0){
      if(c == 0){
        beginRow = 0;
        endRow = 240;
        beginCols = 0;
        endCols = 240;
        quadrant(beginRow, endRow, beginCols, endCols, maxIterations, size, ymax, ymin, xmax, xmin, palette, image);
        printf("Launched child process: %d\n", pid);
        printf("%d) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  pid, beginCols, endCols, beginRow, endRow);
        free(image);
	free(palette);
	exit(0);
      }
      else if(c == 1){
        beginRow = 0;
        endRow = 240;
        beginCols = 240;
        endCols = 480;
        quadrant(beginRow, endRow, beginCols, endCols, maxIterations, size, ymax, ymin, xmax, xmin, palette, image);
        printf("Launched child process: %d\n", pid);
        printf("%d) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  pid, beginCols, endCols, beginRow, endRow);
        free(palette);
	free(image);
	exit(0);
      }
      else if(c == 2){
        beginRow = 240;
        endRow = 480;
        beginCols = 0;
        endCols = 240;
        quadrant(beginRow, endRow, beginCols, endCols, maxIterations, size, ymax, ymin, xmax, xmin, palette, image);
        printf("Launched child process: %d\n", pid);
        printf("%d) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  pid, beginCols, endCols, beginRow, endRow);
        free(palette);
	free(image);
	exit(0);
      }
      else if(c == 3){
        beginRow = 240;
        endRow = 480;
        beginCols = 240;
        endCols = 480;
        quadrant(beginRow, endRow, beginCols, endCols, maxIterations, size, ymax, ymin, xmax, xmin, palette, image);
        printf("Launched child process: %d\n", pid);
        printf("%d) Sub-image block: cols (%f, %f) to rows (%f, %f)\n",
                  pid, beginCols, endCols, beginRow, endRow);
        free(palette);
	free(image);
	exit(0);
      }
    }
  }
  
  for (int r = 0; r < numProcesses; r++) {
    int status;
    int pid = wait(&status);
    printf("Child process complete: %d\n", pid);
  }

  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  char *filename = malloc(sizeof(char) * 64);
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm_2d(filename, image, size, size);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  printf("Writing file: %s\n", filename);
  free(filename);
  free(palette);
  free(image);

  if (shmdt(raw) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }
  exit(0);
 }
