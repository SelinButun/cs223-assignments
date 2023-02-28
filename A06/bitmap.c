/*---------------------------------------------
 * Author: Selin Butun
 * Date: March 1, 2023
 * Description: Program that reads in a file containing
 * a hex code, and prints out a 8x8 1 bpp sprite.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  long bitmask;
  //print 2d array
  bitmask = 0x1ul << 63;
  for(int i = 8; i > 0; i--){
    for(int j = 8; j > 0; j--){
        if((img & bitmask) == bitmask){
            printf("@ ");
        }
        else{
            printf("  ");
        }
	img = img << 1;
    }
    printf("\n");
  }
  printf("\n---\n");
  return 0;

}
