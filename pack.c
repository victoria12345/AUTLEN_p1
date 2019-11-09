#include "pack.h"

Pack* pack(int* estados, int num_estados, char* nombre){
    Pack* p;
    int i;
    p = (Pack*)malloc(sizeof(Pack));
    strcpy(p->nombre, nombre);
    p->num_estados = num_estados;
    
    for(i = 0; i < p->num_estados ; i++){
        p->estados[i] = estados[i];
    }

    return p;
}

void destroy(Pack *p){
    free(p);
}


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


int* get_estados(Pack* p){
    return p->estados;
}

char* get_nombre(Pack *p){
    return p->nombre;
}

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


void imprime(Pack *p){
    int i;
    printf("\n");
    printf("-> Nombre del pack: %s\n", p->nombre);
    printf("-> Estados del pack: [");
    for(i = 0; i< p->num_estados-1; i++){
        printf("%d,", p->estados[i]);
    }
    printf("%d]\n\n", p->estados[i]);

}