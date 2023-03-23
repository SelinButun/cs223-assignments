/*----------------------------------------------
 * Author: Selin Butun 
 * Date: March 22, 2023
 * Description: Program that outputs a PPM image 
 * of the mandelbrot set with randomized colors.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

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
  
  gettimeofday(&tstart, NULL);
  for(int j = 0; j < size; j++){
    for(int k = 0; k < size; k++){
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
}
