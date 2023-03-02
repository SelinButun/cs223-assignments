/*----------------------------------------------
 * Author: Selin Butun 
 * Date: March 1, 2023
 * Description: Encodes an image with user input 
 * by writing to the least significant bit of 
 * every color in a ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel** pixels = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int redBit;
  int blueBit;
  int greenBit;
  int size = 0;
  if(h == 0){
    printf("image can't have 0 height, no encoding\n");
    exit(1);
  }
  else{
    size = w * h * 3;
  }
  printf("Max number of chars for encoding in the image: %d\n", size/8);
  int inputSize = size/8;
  char *message = malloc(sizeof(char) * inputSize);
  printf("Enter phrase to encode: ");
  scanf("%s", message);

  int charLength = strlen(message);
  int index = 0;
  //binary conversion of inputted message
  int *bitHold = malloc(sizeof(int) * size);
    
    for(int i = 0; i < charLength; i++){
        int temp = (int)message[i];
        for(int j = 0; j < 8; j++){
            bitHold[index] = temp % 2; 
            temp = temp/2;
        }
        index++;
    }
  
  int count = 0;
  for(int j = 0; j < h; j++){
     for(int k = 0; k < w; k++){
	    redBit = pixels[j][k].red % 2;
        if((redBit == 1 && bitHold[count] == 1) || (redBit == 0 && bitHold[count] == 0)){ //stay same
            count++;
        }
        else if((redBit == 0 && bitHold[count] == 1) || (redBit == 1 && bitHold[count] == 0)){ //toggle
            pixels[j][k].red ^= 0x1;
            count++;
        }
        greenBit = pixels[j][k].green % 2;
        if((greenBit == 1 && bitHold[count] == 1) || (greenBit == 0 && bitHold[count] == 0)){ //stay same
            count++;
        }
        else if((greenBit == 0 && bitHold[count] == 1) || (greenBit == 1 && bitHold[count] == 0)){ //toggle
            pixels[j][k].green ^= 0x1;
            count++;
        }
        blueBit = pixels[j][k].blue % 2;
        if((blueBit == 1 && bitHold[count] == 1) || (blueBit == 0 && bitHold[count] == 0)){ //stay same
            count++;
        }
        else if((blueBit == 0 && bitHold[count] == 1) || (blueBit == 1 && bitHold[count] == 0)){ //toggle
            pixels[j][k].blue ^= 0x1;
            count++;
        }
     }
  }

  char delim[] = ".ppm";
  char token = strtok(argv[1], delim);
  char add[] = "-encoded.ppm";
  char filename = strcat(token, add);
  write_ppm_2d(filename, pixels, w, h);

  for (int m = 0; m < h; m++){
    free(pixels[m]);
  }
  free(pixels);
  free(bitHold);
  return 0;
}

