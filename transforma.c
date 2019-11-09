#include <stdio.h>
#include "transforma.h"
#include "afnd.h"
#include "pack.h"

AFND* AFNDTransforma(AFND* p_afnd){
    int i, j, index_p = 0;
    int inicial = AFNDIndiceEstadoInicial(p_afnd);
    int num_estados = AFNDNumEstados(p_afnd);
    int num_simbolos = AFNDNumSimbolos(p_afnd);
    int *array_estados;
    char* ini;
    Pack* array_packs[100]; 
    Pack* inicial_p;
    AFND * afd;

    array_estados = (int*)malloc(sizeof(array_estados[0]) * num_estados);
    if(array_estados == NULL){
        return NULL;
    }

    afd = AFNDNuevo("af11", num_estados, num_simbolos);

    /*Insertamos los simbolos*/
    for(i = 0; i < num_simbolos; i++){
        char* tmp = AFNDSimboloEn(p_afnd, i);
        AFNDInsertaSimbolo( afd, tmp);
        tmp = NULL;
    }

    /*Annadimos el estado inicial al nuevo autómata*/
    ini = AFNDNombreEstadoEn(p_afnd, inicial);
    AFNDInsertaEstado(afd, ini,INICIAL);

    /*Annadimos pack inicial a packs ya creados*/
    for(i = 0; i < num_estados; i++){
        if(i == inicial){
            array_estados[i] = 1;
        }else{
            array_estados[i] = 0;
        }
    }
    inicial_p = pack(array_estados, num_estados, AFNDNombreEstadoEn(p_afnd, inicial));
    array_packs[index_p] = inicial_p;
    index_p ++;

    /*Exploramos los estados*/
    for(i = 0; i < index_p; i++ ){

        /*Exploramos el simbolo*/
        for(j = 0; j < num_simbolos; j++){
            Pack *p;
            int final = -1;
            int pertenece_f = -1;
            p = nuevo_pack(p_afnd, array_packs[i], j, num_estados, &final, &pertenece_f);


            /*Si no se ha creado lo annadimos*/
            if(pertenece(array_packs, p, index_p) != 0){
                array_packs[index_p] = p;
                index_p ++;

                    if((index_p-1) != 0 && final != 1){
                        AFNDInsertaEstado(afd, array_packs[index_p -1]->nombre,NORMAL);
                       
                    }else{
                        AFNDInsertaEstado(afd, array_packs[index_p -1]->nombre,FINAL);
                    }
                if(pertenece_f == 1){
                char* char_simb = AFNDSimboloEn(p_afnd, j);
                AFNDInsertaTransicion(afd, array_packs[i]->nombre, char_simb, p->nombre);
                }
            }else{
                if(pertenece_f == 1){
                char* char_simb = AFNDSimboloEn(p_afnd, j);
                AFNDInsertaTransicion(afd, array_packs[i]->nombre, char_simb, p->nombre);
                }
                destroy(p);
            } 
        }

        /*Miramos con transiciones lambda*/
        

    }

    /*Liberamos memoria*/
    for(i = 0; i < index_p; i++){
        destroy(array_packs[i]);
    }
    free(array_estados);
    return afd;
}


/**
* est: pack con los estados correspondientes del automata inicial
* simb: simbolo que exploramos para ver el nuevo estado
*/
Pack* nuevo_pack(AFND* afnd, Pack* est, int simb, int num_estados, int* final, int* pertenece_f){
    int i, j;
    Pack *nuevo;
    int *estados;
    char nombre[100] = "";
    int* estados_pack = get_estados(est);

    estados = (int*)malloc(sizeof(estados[0]) * num_estados);
    if(estados == NULL){
        return NULL;
    }

    for(i = 0; i < num_estados; i++){
        estados[i] = 0;
    }

    for(i = 0; i < num_estados; i++){
        /*Si el estado[i] forma parte del pack*/
        if(estados_pack[i] == 1){

            for(j = 0; j<num_estados; j++){
                /*Miro si hay una transicion en el automata ND inicial*/
                if ( 1 == AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, simb, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;
                    if(AFNDTipoEstadoEn(afnd, j) == FINAL || AFNDTipoEstadoEn(afnd, j) == INICIAL_Y_FINAL){
                        *final = 1;
                    }
                }
                /*else if(1 == AFNDCierreLTransicionIJ(afnd, i, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;
                    if(AFNDTipoEstadoEn(afnd, j) == FINAL || AFNDTipoEstadoEn(afnd, j) == INICIAL_Y_FINAL){
                        *final = 1;
                    }
                }*/
            }

        }
    }

    /*Ahora tenemos un pack con 1 en los estados que lo conforman
            Construimos su nombre*/
    for(i = 0; i < num_estados; i++){
        char aux[15];
        if(estados[i] == 1){
            sprintf(aux, "q%d", i);
            strcat(nombre, aux);
        }
    }

    nuevo = pack(estados, num_estados, nombre);
    free(estados);
    /*free(estados);*/
    return nuevo;
}

