#include "./image.h"
#include <stdio.h>
int main(){
  ppm_image img = get_ppm_image("./testObj.ppm");
  printf("Everything working fine\n");
  save_ppm_image(img, "output.ppm");
  free_ppm_image(img);
  return 0;
}
