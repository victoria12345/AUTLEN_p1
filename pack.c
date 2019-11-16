#include "pack.h"
#include "afnd.h"

/**
 * Constructor del pack
 * 
 * estados: array de estados que lo conforman 
 * num_estados: numero de estados del AFND
 * nombre: nombre del pack
 * tipo: tipo de estado del estado(=pack)
*/
Pack* pack(int* estados, int num_estados, char* nombre, int tipo){
    Pack* p;
    int i;
    p = (Pack*)malloc(sizeof(Pack));
    strcpy(p->nombre, nombre);
    p->num_estados = num_estados;
    p->t_index = 0;
    p->tipo = tipo;
    for(i = 0; i < p->num_estados ; i++){
        p->estados[i] = estados[i];
    }

    return p;
}

/**
 * Libera memoria para el pack p
*/
void destroy(Pack *p){
    free(p);
}

/**
 * Compara 2 packs
 * 
 * p1: primer pack a comparar
 * p2: segundo pack a comparar
*/
int comparar(Pack* p1, Pack *p2){
    int i;

    if(p1->num_estados != p2->num_estados){
        return -1;
    }

    for(i = 0; i < p1->num_estados; i++){
        if(p1->estados[i] != p2->estados[i]){
            return -1;
        }
    }
    return 0;
}

/**
 * Devuelve el array de estados del AFND que lo conforman
*/
int* get_estados(Pack* p){
    return p->estados;
}

/**
 * Devuelve el nombre del pack
*/
char* get_nombre(Pack *p){
    return p->nombre;
}

/**
 * Devuelve 0 si el pack pertenece al array de packs
 * 
 * packs. array de packs que estamos mirando
 * p: pack que estamos buscando
 * n_packs: numero de packs que hay en el array
*/
int pertenece(Pack **packs, Pack* p, int n_packs){
    int i = 0;

    if(packs == NULL){
        return -1;
    }

    for(i = 0; i< n_packs; i++){
        if(comparar(packs[i], p) == 0){
            return 0;
        }
    }
    return -1;
}

/**
 * Annade una transicion al pack
 * 
 * p: pack al que annadimos una transicion
 * destino: nombre del pack(=estado) destino
 * simbolo: nombre del simbolo para la transicion
*/
void annadir_transicion(Pack* p, char* destino, char*simbolo){
    strcpy(p->transiciones[p->t_index].destino, destino);
    strcpy(p->transiciones[p->t_index].simbolo, simbolo);
    p->t_index++;
}

/**
 * imprime la informacion del pack
*/
void imprime(Pack *p){
    int i;
    printf("\n");
    printf("-> Nombre del pack: %s\n", p->nombre);
    printf("-> Estados del pack: [");
    for(i = 0; i< p->num_estados-1; i++){
        printf("%d,", p->estados[i]);
    }
    printf("%d]\n", p->estados[i]);

    printf("Transiciones:\n");
    for(i = 0; i < p->t_index; i++){
        printf("-->  %s-%s con %s\n",p->nombre, p->transiciones[i].destino, p->transiciones[i].simbolo);
    }

}