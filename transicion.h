#ifndef TRANSICION_H
#define TRANSICION_H

#include <stdio.h>

#define MAX_NOMBRE 10
#define MAX_SIMBOLO 5

typedef struct _Transicion
{
  char destino[100]; /*Nombre del destino de la transicion*/
  char simbolo[100]; /*simbolo que provoca la transicion*/
} Transicion;


#endif
