/*----------------------------------------------
 * Author: Selin Butun 
 * Date: Feb 22, 2023
 * Description: Main method to test write_ppm.c is
 * working correctly. First reads in from feep-raw.ppm,
 * then writes it to test.ppm 
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"


int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("feep-raw.ppm", &w, &h);
  for(int i = 0; i < h; i++){
          for(int j = 0; j < w; j++){
                printf("(%d, %d, %d)   ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
          }
          printf("\n");
  }
  write_ppm_2d("test.ppm", pixels, w, h); 
  
  for (int m = 0; m < h; m++){
          free(pixels[m]);
  }
  free(pixels);
  return 0;
}
