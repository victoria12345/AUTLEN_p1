#ifndef TRANSFORMA_H
#define TRANSFORMA_H

#include <stdio.h>
#include "afnd.h"
#include "pack.h"

#define TAM_MAX_NOMBRE 150

/**
 * Funcion que transforma un automata no determinista en uno determinista
 * p_afnd: automata no determinista que queremos convertir
*/
AFND* AFNDTransforma(AFND* p_afnd);

/**
 * Devuelve el pack = nuevo estado, al que podemos transitar desde
 * un estado (= pack est) y a traves de un simbolo
 * 
 * afnd: automata no determinista que estamos transformando 
 * est: pack con los estados correspondientes del automata inicial
 * simb: simbolo que exploramos para ver el nuevo estado
 * num_estados: numero de estados del automata no determinista
 * pertenece_f: flag para saber si va a tener algun estado el nuevo pack
*/
Pack* nuevo_pack(AFND* afnd, Pack* est, int simb, int num_estados, int* pertenece_f);

#endif
