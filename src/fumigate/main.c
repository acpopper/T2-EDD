#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../imagelib/image.h"

#define BLACK 0
#define WHITE 127
#define GRAY 64

#define LUCKY 17

struct subimagen;
typedef struct subimagen Subimagen;

struct subimagen {
  int size;
  int pos;
  Subimagen* next;
  Subimagen* skip;
} ;

Subimagen* subimagen_init(int pos, int size){
  Subimagen* subimagen = malloc(sizeof(Subimagen));

  *subimagen = (Subimagen) {
    .pos = pos,
    .size = size,
    .next = NULL,
    .skip = NULL,
  };
  return subimagen;
}

void list_print(Subimagen* subimagen){
  printf("pos %i, size %i -> ", subimagen->pos, subimagen->size);
  if (!subimagen->next){
    printf("\n");
  }
  else{
    list_print(subimagen->next);
  }
}

void append_to_list(Subimagen* first, int pos, int size){
  Subimagen* second = subimagen_init(pos, size);
  second->next=first->next;
  first->next=second;
}

void append_to_skip(Subimagen* list, int pos, int size){
  Subimagen *last = list;
  while (last->skip) {
    last = last->skip;
  }
  Subimagen *new_list = subimagen_init(pos, size);
  last->skip = new_list;
}

void list_destroy(Subimagen *list){
  if (list->next){
    list_destroy(list->next);
  }
  free(list);
}

void skips_destroy(Subimagen *list){
  if (list->skip){
    skips_destroy(list->skip);
  }
  list_destroy(list);
}

void destroy_table(Subimagen** table, int len){
  for(int i=0; i<len; i++){
    if(table[i]){
      skips_destroy(table[i]);
    }  
  }
}


u_int32_t hash_key(int* pixels, int width){
  int num=0;
  if(width == 2){
    for(int i=0; i<4; i++){
      if(pixels[i]){
        // printf("it %i\n", i);
        num+=1<<i;
      }
    }
    // printf("2x2: %i\n", num);
    return num;
  }
  else {
    int* pixels_A = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_B = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_C = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_D = malloc(((width-1)*(width-1))*sizeof(int));
    int hugh_jackman=0;
    for(int i=0; i<width-1; i++){
      for(int j=0; j<width-1; j++){
        pixels_A[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=0; i<width-1; i++){
      for(int j=1; j<width; j++){
        pixels_B[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=1; i<width; i++){
      for(int j=0; j<width-1; j++){
        pixels_C[hugh_jackman]=pixels[(width)*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=1; i<width; i++){
      for(int j=1; j<width; j++){
        pixels_D[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    // for(int i=0; i<((width-1)*(width-1));i++){
    //   printf("%i ", pixels_A[i]);
    // }
    // printf("\n");
    // for(int i=0; i<((width-1)*(width-1));i++){
    //   printf("%i ", pixels_B[i]);
    // }
    // printf("\n");
    // for(int i=0; i<((width-1)*(width-1));i++){
    //   printf("%i ", pixels_C[i]);
    // }
    // printf("\n");
    // for(int i=0; i<((width-1)*(width-1));i++){
    //   printf("%i ", pixels_D[i]);
    // }
    // printf("\n");
    int A = hash_key(pixels_A, width-1);
    int B = hash_key(pixels_B, width-1);
    int C = hash_key(pixels_C, width-1);
    int D = hash_key(pixels_D, width-1);
    free(pixels_A);
    free(pixels_B);
    free(pixels_C);
    free(pixels_D);
    // printf("A %i, B %i, C %i, D %i\n", A,B,C,D);
    return (A*LUCKY+B*LUCKY*LUCKY+C*LUCKY*LUCKY*LUCKY+D*LUCKY*LUCKY*LUCKY*LUCKY)&0xFFFFFFFF;
    // return 0;
  }
  
}

u_int32_t hash_func(int* pixels, int width, int table_size) { // Solo para insectos
  u_int32_t key = hash_key(pixels, width);
  return key % table_size;
}

bool mismo_patron(){
  return true;
}

int insert(int indice, int pos, int width, int* pixels, Subimagen*** table, int* pared_pixels) { // retorna Subimagen**
  printf("Insertando indice %i, pos %i, width %i en tabla\n", indice, pos, width);
  printf("Pixeles: ");
  for(int i=0; i<(width*width);i++){
    printf("%i", pixels[i]);
  }
  printf("\n");
  return 0;
}

u_int32_t hash_pared(int* pixels, int width, int pos, Subimagen** table, int table_size, Image* pared){
  int num=0;
  if(width == 2){
    for(int i=0; i<4; i++){
      if(pixels[i]){
        num+=1<<i;
      }
    }
    u_int32_t indice = num%table_size;
    // *table = insert(indice, pos, width, pixels, &table, pared->pixels);
    insert(indice, pos, width, pixels, &table, pared->pixels);
    return num;
  }
  else {
    // spliteo la matriz en sus 4 esquinas
    int* pixels_A = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_B = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_C = malloc(((width-1)*(width-1))*sizeof(int));
    int* pixels_D = malloc(((width-1)*(width-1))*sizeof(int));
    int hugh_jackman=0;
    for(int i=0; i<width-1; i++){
      for(int j=0; j<width-1; j++){
        pixels_A[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=0; i<width-1; i++){
      for(int j=1; j<width; j++){
        pixels_B[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=1; i<width; i++){
      for(int j=0; j<width-1; j++){
        pixels_C[hugh_jackman]=pixels[(width)*i+j];
        hugh_jackman+=1;
      }
    }
    hugh_jackman=0;
    for(int i=1; i<width; i++){
      for(int j=1; j<width; j++){
        pixels_D[hugh_jackman]=pixels[width*i+j];
        hugh_jackman+=1;
      }
    }
    // recursion
    int A = hash_pared(pixels_A, width-1, pos, table, table_size, pared);
    int B = hash_pared(pixels_B, width-1, pos, table, table_size, pared);
    int C = hash_pared(pixels_C, width-1, pos, table, table_size, pared);
    int D = hash_pared(pixels_D, width-1, pos, table, table_size, pared);
    u_int32_t hash_key = (A*LUCKY+B*LUCKY*LUCKY+C*LUCKY*LUCKY*LUCKY+D*LUCKY*LUCKY*LUCKY*LUCKY)&0xFFFFFFFF;
    u_int32_t indice = hash_key%table_size;
    if(width<pared->width){
      // *table = insert(indice, pos, width, pixels, &table, pared->pixels);
      insert(indice, pos, width, pixels, &table, pared->pixels);
    }
    //libero memoria de los subarreglos
    free(pixels_A);
    free(pixels_B);
    free(pixels_C);
    free(pixels_D);
    
    return hash_key;
  }
  
}

void create_table(Image* pared){ // retorna Subimagen**
  int table_size=0;
  for(int i=2; i < (pared->width); i++) {
        table_size += i*i;
    }
  table_size=floor(table_size/10);
  Subimagen** hash_table = calloc(table_size, sizeof(Subimagen*));
  hash_pared(pared->pixels, pared->width, 0, hash_table, table_size, pared);


  free(hash_table);
}


int main(int argc, char** argv)
{  
  if (argc != 2)
  {
    printf("Modo de uso: %s INPUT \n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    // testing
    Image* e = malloc(sizeof(Image));
    *e = (Image) {
    .width = 4,
    .height = 4,
    .pixel_count = 16,
    .pixels=calloc(16, sizeof(int))
  };
    e->pixels[0]=0;
    e->pixels[1]=1;
    e->pixels[2]=0;
    e->pixels[3]=1;
    e->pixels[4]=1;
    e->pixels[5]=0;
    e->pixels[6]=0;
    e->pixels[7]=1;
    e->pixels[8]=0;
    e->pixels[9]=0;
    e->pixels[10]=0;
    e->pixels[11]=0;
    e->pixels[12]=0;
    e->pixels[13]=0;
    e->pixels[14]=0;
    e->pixels[15]=1;
  //   printf("%u\n",hash_key(e->pixels, e->width, e->height));
    create_table(e);
    img_png_destroy(e);

    return 1;
  }

  /* Abrimos el archivo de input */
  FILE* input_file = fopen(argv[1], "r");

  /* Leemos la ruta de la imagen principal */
  char filename[32];
  fscanf(input_file, "%s", filename);
  /* Abrimos la imagen principal */
  Image* image = img_png_read_from_file(filename);

  /* Aca puedes crear la tabla de hash */


  /* Leemos cada una de las consultas */
  int Q;
  char query_in[32];
  char query_out[32];
  fscanf(input_file, "%d", &Q);


  for (int q = 0; q < Q; q++)
  {
    /* Leemos las rutas de las imagenes */
    fscanf(input_file, "%s", query_in);
    fscanf(input_file, "%s", query_out);

    /* Abrimos la imagen de consulta de input */
    Image* query_image = img_png_read_from_file(query_in);


    /* Creamos una nueva imagen en blanco con las mismas dimensiones que la original */
    Image *out_image = calloc(1, sizeof(Image));
    out_image->height = image->height;
    out_image->width = image->width;
    out_image->pixel_count = image->pixel_count;
    out_image->pixels = calloc(image->pixel_count, sizeof(int));



    /* pintamos los pixeles de su color final (borrar las siguientes tres lineas)*/
      out_image->pixels[0] = BLACK;
      out_image->pixels[4] = WHITE; 
      out_image->pixels[5] = GRAY;



    /* Escribimos la imagen de output*/
    img_png_write_to_file(out_image, query_out);


    /* Liberamos la memoria de las imagenes*/
    img_png_destroy(query_image);
    img_png_destroy(out_image);
  
  }

  /* Liberamos la memoria de la imagen principal */
  img_png_destroy(image);
  /* Cerramos el archivo de input */
  fclose(input_file);


  return 0;
}
