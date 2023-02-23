/*----------------------------------------------
 * Author: Selin Butun 
 * Date: Feb 22, 2023
 * Description: Read in Portable Pixel Map files
 * in binary format. Places information into a
 * 2D dynamic array of structs.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  return NULL;
}

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  FILE* fp = fopen(filename, "rb");
  char attribute[1024];
  fgets(attribute, 1024, fp);
  fgets(attribute, 1024, fp);
  fgets(attribute, 1024, fp);//3rd line contains row and column size
  sscanf(attribute,"%d %d", w, h);
  fgets(attribute, 1024, fp);
  struct ppm_pixel** arr; //declaring a 2D array of structs
  arr = malloc(sizeof(struct ppm_pixel*) * *h);
  for (int i = 0; i < *h; i++) {
        arr[i] = malloc(sizeof(struct ppm_pixel) * *w);
  }
  for(int j = 0; j < *h; j++){
	  for(int k = 0; k < *w; k++){
		fread(&arr[j][k], sizeof(struct ppm_pixel), 1, fp);
  	  }
  }

  fclose(fp);
  return arr;
}
