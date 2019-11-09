#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include <stdio.h>

#include "afnd.h"
#include "pack.h"

AFND* AFNDTransforma(AFND* p_afnd);
Pack* nuevo_pack(AFND* afnd, Pack* est, int simb, int num_estados, int* final, int* pertenece_f);

#endif
