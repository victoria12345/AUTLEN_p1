#ifndef PACK_H
#define PACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transicion.h"

#define MAX_ESTADOS 50
#define MAX_TRANSICIONES 100

typedef struct _Pack
{
  int num_estados ;
  char nombre[50];
  int estados[MAX_ESTADOS];
  Transicion transiciones[MAX_TRANSICIONES];
  int t_index;
  int tipo;
} Pack;

Pack* pack(int* estados, int num_estados, char* nombre, int tipo);
int comparar(Pack* p1, Pack *p2);
int* get_estados(Pack* p);
char* get_nombre(Pack *p);
int pertenece(Pack **packs, Pack* p, int n_packs);
void annadir_transicion(Pack* p, char* destino, char*simbolo);
void destroy(Pack *p);
void imprime(Pack *p);

#endif
