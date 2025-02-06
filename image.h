#ifndef __IMAGE_H
#define __IMAGE_H
#endif //__IMAGE_H

#ifndef STD_HEADERS_H
#define STD_HEADERS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#endif//STD_HEADERS_H

typedef struct Img_Dimensions{
  size_t height, width;
} Dimensions;
typedef  struct Row_Pixel{
  uint8_t r, g, b;
} row_pixel;

typedef struct PPM_Image{
  char header[5];
  Dimensions dimensions;
  uint8_t opacity;
  row_pixel *image;
} ppm_image;

#define PIXEL_AT(img, i, j) ((img).image[(img).dimensions.width * (i) + (j)])


/* Return the ppm_image*/
ppm_image get_ppm_image(char *path);
/* Cheacks if the file is valid ppm plain image or not (true if image is P3 non compress)*/
bool is_valid_ascii_ppm(FILE *img);
/* Return the dimensions of the given image Image must be valid PPM image*/
Dimensions get_dimensions(FILE *img);
/* Return the pixel opacity and place the file pointer at the file piaxel first value, so before reading actual data use this function. */
uint8_t get_opacity(FILE *img);
/* Print given image on the given output file*/
void print_ppm_image(ppm_image img, FILE *output);
/*saves given image on in the file*/
void free_ppm_image(ppm_image img);

#ifndef __IMAGE_H_IMPL
#define __IMAGE_H_IMPL

ppm_image get_ppm_image(char *path){
  assert(path != NULL);
  ppm_image img;
  FILE *f; 
  if((f = fopen(path, "r"))  == NULL ){
    img.image = NULL;
    img.dimensions.height = img.dimensions.width = 0; }
  if(!is_valid_ascii_ppm(f)) 
    return img;
  // given image contain valid P3 signature;
  strcpy(img.header, "P3"); 
  Dimensions d = get_dimensions(f);
  if(d.height == 0 || d.width == 0 )
    return img;
  img.dimensions = d;
  if( (img.opacity = get_opacity(f) ) == 0)
    return img;
  img.image =(row_pixel*)malloc((d.height * d.width) * sizeof(row_pixel));
  for (size_t i = 0; i < d.height; i++){
    for (size_t j = 0; j < d.width; j++){
      fscanf(f, "%hhu", &(PIXEL_AT(img, i, j)).r);
      fscanf(f, "%hhu", &(PIXEL_AT(img, i, j)).g);
      fscanf(f, "%hhu", &(PIXEL_AT(img, i, j)).b);
    }
  }
  fclose(f);
  return img;
}
bool is_valid_ascii_ppm(FILE *img){
  assert(img != NULL);
  rewind(img);
  char *line;
  size_t n;
  if(getline(&line, &n, img) != 3) return false;
  if(strcmp(line, "P3\n") != 0) return false;
  free(line);
  return true;
}

Dimensions get_dimensions(FILE *img){
  assert(img != NULL);
  rewind(img);
  char *line = NULL;
  size_t n;
  Dimensions d = {.height = 0, .width = 0};
  // read second line
  if(getline(&line, &n, img) < 0){
    if(line != NULL) free(line);
    return d;
  }
  free(line);
  line = NULL;  n = 0;
  if(getline(&line, &n, img) < 0){
    if(line != NULL) free(line);
    return d;
  }
  d.width = atoi(line);
  d.height = atoi(strchr(line, ' '));
  free(line);
  return d;
}
uint8_t get_opacity(FILE *img){
  assert(img != NULL);
  get_dimensions(img);     // file pointer pointing to opacity attr.
  uint8_t opacity = 0;
  char *line = NULL;
  size_t n;
  if(getline(&line, &n, img) < 0){
    if(line != NULL) free(line);
    return opacity;
  }
  opacity = atoi(line);
  free(line);
  return opacity;
}

void print_ppm_image(ppm_image img, FILE *output){
  assert(img.image != NULL);
  for (size_t i = 0; i < img.dimensions.height; i++){
    for (size_t j = 0; j < img.dimensions.width; j++){
      row_pixel p = PIXEL_AT(img, i, j);
      fprintf(output, "%d %d %d \t ", p.r, p.g, p.b);
    }
    fprintf(output, "\n");
  }
}
void save_ppm_image(ppm_image img, char *path){
  assert(img.image != NULL);
  assert(path != NULL);
  FILE *f = fopen(path, "w");
  if(f == NULL)
    return;
  // print image header
  fprintf(f, "%s\n", img.header);
  fprintf(f, "%zu %zu\n", img.dimensions.width, img.dimensions.height);
  fprintf(f, "%hhu\n", img.opacity);

  for (size_t i = 0; i < img.dimensions.height; i++){
    for (size_t j = 0; j < img.dimensions.width; j++){
      fprintf(f, "%hhu %hhu %hhu ", PIXEL_AT(img, i, j).r, PIXEL_AT(img, i, j).g, PIXEL_AT(img, i, j).b);
    }
    fprintf(f, "\n");
  }
  fclose(f);
  return;
}
void free_ppm_image(ppm_image img){
  if(img.image == NULL) return;
  free(img.image);
}
#endif//__IMAGE_H_IMPL

