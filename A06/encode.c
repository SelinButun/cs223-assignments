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

int main(int argc, char **argv){
  if (argc != 2){
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  int w, h;
  struct ppm_pixel **pixels = read_ppm_2d(argv[1], &w, &h);
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int redBit;
  int blueBit;
  int greenBit;
  int size = 0;
  if (h == 0){
    printf("image can't have 0 height, no encoding\n");
    exit(1);
  }
  else{
    size = w * h * 3;
  }
  printf("Max number of chars for encoding in the image: %d\n", size / 8);
  int inputSize = size / 8;
  char *message = malloc(sizeof(char) * inputSize);
  printf("Enter phrase to encode: ");
  scanf("%s", message);
  int charLength = strlen(message);
  int binSize = charLength * 8;
  int index = 7;
  // binary conversion of inputted message
  int *bitHold = malloc(sizeof(int) * binSize);
  memset(bitHold, 0, sizeof(int) * size);
  for (int i = 1; i < charLength + 1; i++){
    int temp = (int)message[i - 1];
    for (int j = 0; j < 8; j++){
      bitHold[index] = temp % 2;
      temp = temp / 2;
      index--;
    }
    index = (8 * (i + 1)) - 1;
  }

  int counter = 0;
  for (int y = 0; y < h; y++){
    for (int k = 0; k < w; k++){
      if (counter >= binSize){
        break;
      }
      else{
        redBit = pixels[y][k].red % 2;
        if ((redBit == 1 && bitHold[counter] == 1) || (redBit == 0 && bitHold[counter] == 0)){ // stay same
          counter++;
        }
        else if ((redBit == 0 && bitHold[counter] == 1) || (redBit == 1 && bitHold[counter] == 0)){ // toggle
          pixels[y][k].red ^= 0x1;
          counter++;
        }
      }

      if (counter >= binSize){
        break;
      }
      else{
        greenBit = pixels[y][k].green % 2;
        if ((greenBit == 1 && bitHold[counter] == 1) || (greenBit == 0 && bitHold[counter] == 0)){ // stay same
          counter++;
        }
        else if ((greenBit == 0 && bitHold[counter] == 1) || (greenBit == 1 && bitHold[counter] == 0)){ // toggle
          pixels[y][k].green ^= 0x1;
          counter++;
        }
      }

      if (counter >= binSize){
        break;
      }
      else{
        blueBit = pixels[y][k].blue % 2;
        if ((blueBit == 1 && bitHold[counter] == 1) || (blueBit == 0 && bitHold[counter] == 0)){ // stay same
          counter++;
        }
        else if ((blueBit == 0 && bitHold[counter] == 1) || (blueBit == 1 && bitHold[counter] == 0)){ // toggle
          pixels[y][k].blue ^= 0x1;
          counter++;
        }
      }
    }
  }

  char delim[] = ".";
  char *token = strtok(argv[1], delim);
  char add[] = "-encoded.ppm";
  char *filename = strcat(token, add);
  write_ppm_2d(filename, pixels, w, h);

  for (int m = 0; m < h; m++)
  {
    free(pixels[m]);
  }
  free(pixels);
  free(bitHold);
  free(message);
  return 0;
}

