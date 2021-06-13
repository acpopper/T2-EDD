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
  Subimagen *new_list = subimagen_init(pos, size);
  list->skip = new_list;
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

u_int32_t hash_func(int* pixels, int width, int table_size) { 
  u_int32_t key = hash_key(pixels, width);
  return key % table_size;
}

bool mismo_patron(int* pixels, int width, Subimagen* current, Image* pared){
  if(width != current->size){
    return false;
  }
  int pos = current->pos;
  int w = current->size;
  int w_pared = pared->width;
  int* current_pixels = calloc(w*w, sizeof(int));
  int c=0;
  for(int i=0; i<w; i++){
      for(int j=0; j<w; j++){
          current_pixels[c]=pared->pixels[pos+j+w_pared*i];
          // printf("%i ", current_pixels[c]);
          c+=1;
          
      }
      // printf("\n");
  }
  for(int i=0; i<w*w; i++){
    if(pixels[i]!=current_pixels[i]){
      free(current_pixels);
      return false;
    }
  }
  return true;
}

Subimagen** insert(int indice, int pos, int width, int* pixels, Subimagen** hash_table, Image* pared) {
  // Si no hay nada en esa posición, se crea un nodo y se apunta a el
  if(!hash_table[indice]){
    Subimagen* new = subimagen_init(pos, width);
    hash_table[indice] = new;
    return hash_table;
  } // Si hay un nodo, se revisa si es el mismo patrón o si cayó ahí por la función. Dependiendo de eso cómo se agrega.

  else {
    Subimagen* current = hash_table[indice];
    while(current->skip){
      if(mismo_patron(pixels, width, current, pared)){
        append_to_list(current, pos, width);
        return hash_table;
      }
      current=current->skip;
    }
    if(mismo_patron(pixels, width, current, pared)){
      append_to_list(current, pos, width);
      return hash_table;
    }
    append_to_skip(current, pos, width);
    return hash_table;
  }
} 

Subimagen** hash_pared(Image* pared, int table_size){
  Subimagen** hash_table = calloc(table_size, sizeof(Subimagen*));
  int total=pared->pixel_count;
  int w = pared->width;
  // j=posicion i=size
  for(int i=2; i<w; i++){
    for(int j=0; j<(w*w); j++){
      int sub[i*i];
      if(w-j%w<i || floor(j/w)+i>w){
          // printf("invalid pos %i w %i\n", j, i);
          continue;
      }
      else {
        int c=0;
          for(int f=0; f<i; f++){
            for(int g=0; g<i; g++){
              sub[c]=pared->pixels[j+f*w+g];
              // printf("%i ", sub[c]);
              c+=1;
            }
          }
          // printf("\n");
        u_int32_t indice = hash_func(sub, i, table_size);
        hash_table = insert(indice, j, i, sub, hash_table, pared);
      }
    }
  }
  return hash_table;
}

Subimagen** create_table(Image* pared){ // retorna Subimagen**
  int table_size=0;
  // total de combinaciones posibles con las casillas
  for(int i=2; i < (pared->width); i++) {
        table_size += i*i;
    }
  // se toma un 20% del total posible
  table_size=floor(table_size/5);
  // printf("TABLE SIZE %i\n", table_size);
  // Se crea la tabla que se retornará
  Subimagen** hash_table = hash_pared(pared, table_size);
  return hash_table;
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
  Subimagen** hash_table =  create_table(image);

  int table_size=0;
  // total de combinaciones posibles con las casillas
  for(int i=2; i < (image->width); i++) {
        table_size += i*i;
    }
  // se toma un 20% del total maximo posible
  table_size=floor(table_size/5);


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
    for(int i=0; i<image->pixel_count; i++){
      out_image->pixels[i] = image->pixels[i];
    }



    /* pintamos los pixeles de su color final*/
    // Se encuentra el patrón (lista ligada donde cada nodo guarda la posicion y width, así se ubican
    // fácilmente los pixeles en la imagen original, o en este caso en la copia)
    u_int32_t indice_insecto = hash_func(query_image->pixels, query_image->width, table_size);
    Subimagen* patron = hash_table[indice_insecto];

    while(patron){
      int pos = patron->pos;
      int w = patron->size;
      int w_pared = image->width;
      for(int i=0; i<w; i++){
          for(int j=0; j<w; j++){
              out_image->pixels[pos+j+w_pared*i]=GRAY;
          }
      }
      patron=patron->next;
    }
    
    
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
