#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {
  FILE * fp;
  fp = fopen (filename, "wb");
  fprintf(fp, "P6 \n");
  fprintf(fp, "# Created by GIMP version 2.10.24 PNM plug-in \n");
  fprintf(fp, "%d %d \n", h, w);
  fprintf(fp, "255 \n");
  for(int j = 0; j < h; j++){
	  for(int k = 0; k < w; k++){
		fwrite(&pixels[j][k], sizeof(struct ppm_pixel), 1, fp);
  	  }
  }
  printf("\n");
  fclose(fp);
}
