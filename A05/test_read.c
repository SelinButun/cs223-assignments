/*----------------------------------------------
 * Author: Selin Butun 
 * Date: Feb 22, 2023
 * Description: Test file/main method for read_ppm.c
 * This also check write_ppm works correctly and is
 * used to read from test.ppm to ensure it matches
 * feep-raw.ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d("test.ppm", &w, &h);
  for(int i = 0; i < h; i++){
	  for(int j = 0; j < w; j++){
		printf("(%d, %d, %d)   ", pixels[i][j].red, pixels[i][j].green, pixels[i][j].blue);
	  }
  	  printf("\n");	  
  }

  for (int m = 0; m < h; m++){
	  free(pixels[m]);
  }
  free(pixels);
  return 0;
}

