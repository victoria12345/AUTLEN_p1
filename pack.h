#ifndef PACK_H
#define PACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ESTADOS 50

typedef struct _Pack
{
  int num_estados ;
  char nombre[50];
  int estados[MAX_ESTADOS];
} Pack;

Pack* pack(int* estados, int num_estados, char* nombre);
int comparar(Pack* p1, Pack *p2);
int* get_estados(Pack* p);
char* get_nombre(Pack *p);
int pertenece(Pack **packs, Pack* p, int n_packs);
void destroy(Pack *p);
void imprime(Pack *p);

#endif
