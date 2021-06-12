#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../imagelib/image.h"

#define BLACK 0
#define WHITE 127
#define GRAY 64

#define LUCKY 13

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


int hash_key(int* pixels, int width, int height){
  int num=0;
  if(width == 2){
    for(int i=0; i<4; i++){
      if(pixels[i]){
        num+=1<<i;
      }
    }
    return num;
  }
  else {
    int* pixels_A = malloc(((width-1)*(height-1))*sizeof(int));
    int* pixels_B = malloc(((width-1)*(height-1))*sizeof(int));
    int* pixels_C = malloc(((width-1)*(height-1))*sizeof(int));
    int* pixels_D = malloc(((width-1)*(height-1))*sizeof(int));
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
    int A = hash_key(pixels, width-1, height-1);
    int B = hash_key(pixels, width-1, height-1);
    int C = hash_key(pixels, width-1, height-1);
    int D = hash_key(pixels, width-1, height-1);
    free(pixels_A);
    free(pixels_B);
    free(pixels_C);
    free(pixels_D);
    return A*LUCKY+B*LUCKY*LUCKY+C*LUCKY*LUCKY*LUCKY+D*LUCKY*LUCKY*LUCKY*LUCKY;
    // return 0;
  }
  
}

int hash_func(int pos, int size, Subimagen** hash_table, int table_size) {
  int key = pos*size; // OBIAMENTE EDITAR ESTA WEA CON LA HASH FUNC CORRECTA
  return key % table_size;
}

bool mismo_patron(int pos1, int size1, int pos2, int size2){
  return true;
}

void insert(int pos, int size, Subimagen** hash_table, int table_size) {
  int hashIndex = hash_func(pos, size, hash_table, table_size);
  // Si no hay nada en esa posición, se crea un nodo y se apunta a el
  if(!hash_table[hashIndex]){
    Subimagen* new = subimagen_init(pos, size);
    hash_table[hashIndex] = new;
  } // Si hay un nodo, se revisa si es el mismo patrón o si cayó ahí por la función. Dependiendo de eso cómo se agrega.
  else {
    Subimagen* current = hash_table[hashIndex];
    while(current){
      if(mismo_patron(pos, size, current->pos, current->size)){
        append_to_list(hash_table[hashIndex], pos, size);
        return;
      }
      current=current->skip;
    }
    append_to_skip(current, pos, size);
  } 
}

int main(int argc, char** argv)
{  
  if (argc != 2)
  {
    printf("Modo de uso: %s INPUT \n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    // testing
  //   Image* e = malloc(sizeof(Image));
  //   *e = (Image) {
  //   .width = 4,
  //   .height = 4,
  //   .pixel_count = 16,
  //   .pixels=calloc(16, sizeof(int))
  // };
  //   e->pixels[0]=0;
  //   e->pixels[1]=1;
  //   e->pixels[2]=2;
  //   e->pixels[3]=3;
  //   e->pixels[4]=4;
  //   e->pixels[5]=5;
  //   e->pixels[6]=6;
  //   e->pixels[7]=7;
  //   e->pixels[8]=8;
  //   e->pixels[9]=9;
  //   e->pixels[10]=10;
  //   e->pixels[11]=11;
  //   e->pixels[12]=12;
  //   e->pixels[13]=13;
  //   e->pixels[14]=14;
  //   e->pixels[15]=15;
  //   hash_key(e->pixels, e->width, e->height);
  //   img_png_destroy(e);

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
