#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../imagelib/image.h"

#define BLACK 0
#define WHITE 127
#define GRAY 64

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

void add_to_list(Subimagen* first, int pos, int size){
  Subimagen* second = list_init(pos, size);
  second->next=first->next;
  first->next=second;
}

void append_skip(Subimagen* list, int pos, int size){
  Subimagen *last = list;
  while (last->skip) {
    last = last->skip;
  }
  Subimagen *new_list = list_init(pos, size);
  last->skip = new_list;
}

int list_destroy(Subimagen *list){
  if (list->next){
    list_destroy(list->next);
  }
  free(list);
}

int skips_destroy(Subimagen *list){
  if (list->skip){
    skips_destroy(list->skip);
  }
  list_destroy(list);
}

int destroy_table(Subimagen** table, int len){
  for(int i=0; i<len; i++){
    if(table[i]){
      skips_destroy(table[i]);
    }  
  }
}

int main(int argc, char** argv)
{  
  if (argc != 2)
  {
    printf("Modo de uso: %s INPUT \n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
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
