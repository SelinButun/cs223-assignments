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

  // todo: your work here
  // generate pallet

  int shmid;
  int shmid2;
  int shmid3;

  shmid3 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * size, 0644 | IPC_CREAT);
  struct ppm_pixel *palette = shmat(shmid3, NULL, 0);

  int *shmids = malloc(size * sizeof(int));
  shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel*) * size, 0644 | IPC_CREAT);
  struct ppm_pixel **image = shmat(shmid, NULL, 0);
  for(int p = 0; p < size; p++){
    shmids[p] = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * size, 0644 | IPC_CREAT);
    image[p] = shmat(shmid2, NULL, 0); 
  }

  if (shmid == -1 || shmid3 == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  if (buffer == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  } 
  
    srand(time(0));
    for(int i  = 0; i < maxIterations; i++){
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }


  // start timer here, timer ends after writing image compute image
  float xfrac;
  float yfrac;
  float x0;
  float y0;
  float x = 0;
  float y = 0;
  int iter = 0;
  float xtmp;
  struct timeval tstart, tend;
  double timer;
  double beginRow = 0;
  double endRow = 0;
  double beginCol = 0;
  double endCol = 0;
  
  gettimeofday(&tstart, NULL);
  
  for(int c = 0; c < numProcesses; c++){
  int pid = fork();
  if(c == 0){
    beginRow = 0;
    endRow = 240;
    beginCol = 0;
    endCol = 240;
  }
  else if(c == 1){
    beginRow = 0;
    endRow = 240;
    beginCol = 240;
    endCol = 480;
  }
  else if(c == 2){
    beginRow = 240;
    endRow = 480;
    beginCol = 0;
    endCol = 240;
  }
  else if(c == 3){
    beginRow = 240;
    endRow = 480;
    beginCol = 240;
    endCol = 480;
  }
  printf("%d) Sub-image block: cols (%d, %d) to rows (%d, %d), pid, beginCols, endCols, beginRow, beginCol)";
  
  for(int j = beginRow; j < endRow; j++){
    for(int k = beginCol; k < endCol; k++){
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
    printf("Launched child process: %d\n", pid);
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
  
  if (shmdt(palette) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }

  if (shmctl(shmid3, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }
  
  for(int g = 0; g < size; g++){
    if (shmdt(image) == -1) {
      perror("Error: cannot detatch from shared memory\n");
      exit(1);
    }
    if (shmctl(shmids[g], IPC_RMID, 0) == -1) {
      perror("Error: cannot remove shared memory\n");
      exit(1);
    }
  }
  if (shmdt(image) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  
  if (shmctl(shmids, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }
  
  
}
