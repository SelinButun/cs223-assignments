/*----------------------------------------------
 * Author: Selin Butun
 * Date: March 1, 2023
 * Description: Decode a ppm file by getting the least
 * significant bit of each color and converting them
 * to ASCII characters at the end of reading file
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  
  //char filename[1024] = argv[1];
  //scanf(" %s", &filename);
  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int redBit;
  int blueBit;
  int greenBit;
  int size = 0;
  if(h == 0){
    size = w * 3;
  }
  else{
    size = w * h * 3;
  }
  //int size = w * h * 3;
  printf("Max number of chars in the image: %d\n", size/8);
  int index = 0;
  int *bitHold = malloc(sizeof(int) * size);

  for(int i = 0; i < h; i++){
     for(int j = 0; j < w; j++){
	redBit = pixels[i][j].red % 2;
        bitHold[index] = redBit;
        index++;
        greenBit = pixels[i][j].green % 2;
        bitHold[index] = greenBit;
        index++;
        blueBit = pixels[i][j].blue % 2;
        bitHold[index] = blueBit;
        index++;
     }
  }
  
  int *convert = malloc(sizeof(int) * size);
  int count = 0;
  for(int m = 0; m < size; m += 8){
    convert[count] = (2*2*2*2*2*2*2 * bitHold[m]) + (2*2*2*2*2*2 * bitHold[m+1]) + 
    (2*2*2*2*2 * bitHold[m+2]) + (2*2*2*2 * bitHold[m+3]) + (2*2*2 * bitHold[m+4]) + 
    (2*2 * bitHold[m+5]) + (2 * bitHold[m+6]) + (1 * bitHold[m+7]);
    if(convert[count] == 0){
        break;
    }
    else{
        printf("%c", (char)(convert[count]));
        count++;
    }
  }
  
  for (int m = 0; m < h; m++){
	free(pixels[m]);
  }
  free(pixels);
  free(bitHold);
  free(convert);
  return 0;
}
