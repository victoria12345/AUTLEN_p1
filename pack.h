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
  int num_estados ; /*Numero de estados del automata ND*/
  char nombre[50];  /*Nombre del estado correspondiente a este pack*/
  int estados[MAX_ESTADOS]; /*array de estados q lo conforman respecto al ND*/
  Transicion transiciones[MAX_TRANSICIONES];  /*Transiciones a otros packs*/
  int t_index;  /*Inidice para controlar el array de transiones*/
  int tipo; /*tipo del estado*/
} Pack;

/**
 * Constructor del pack
 * 
 * estados: array de estados que lo conforman 
 * num_estados: numero de estados del AFND
 * nombre: nombre del pack
 * tipo: tipo de estado del estado(=pack)
*/
Pack* pack(int* estados, int num_estados, char* nombre, int tipo);

/**
 * Compara 2 packs
 * 
 * p1: primer pack a comparar
 * p2: segundo pack a comparar
*/
int comparar(Pack* p1, Pack *p2);

/**
 * Devuelve el array de estados del AFND que lo conforman
*/
int* get_estados(Pack* p);

/**
 * Devuelve el nombre del pack
*/
char* get_nombre(Pack *p);

/**
 * Devuelve 0 si el pack pertenece al array de packs
 * 
 * packs. array de packs que estamos mirando
 * p: pack que estamos buscando
 * n_packs: numero de packs que hay en el array
*/
int pertenece(Pack **packs, Pack* p, int n_packs);

/**
 * Annade una transicion al pack
 * 
 * p: pack al que annadimos una transicion
 * destino: nombre del pack(=estado) destino
 * simbolo: nombre del simbolo para la transicion
*/
void annadir_transicion(Pack* p, char* destino, char*simbolo);

/**
 * Libera memoria para el pack p
*/
void destroy(Pack *p);

/**
 * imprime la informacion del pack
*/
void imprime(Pack *p);

#endif
