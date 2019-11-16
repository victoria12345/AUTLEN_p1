#include <stdio.h>
#include "transforma.h"
#include "afnd.h"
#include "pack.h"


/**
 * Funcion que transforma un automata no determinista en uno determinista
 * p_afnd: automata no determinista que queremos convertir
*/
AFND* AFNDTransforma(AFND* p_afnd){
    int i, j, index_p = 0;
    int inicial = AFNDIndiceEstadoInicial(p_afnd);
    int num_estados = AFNDNumEstados(p_afnd);
    int num_simbolos = AFNDNumSimbolos(p_afnd);
    int *array_estados;
    char ini[TAM_MAX_NOMBRE];
    int ini_fin = INICIAL;
    Pack* array_packs[100]; 
    Pack* array_packs_cerrados[100];
    int index_cerrados = 0; 
    Pack* inicial_p;
    AFND * afd;

    /*Array de estados del pack inicial*/
    array_estados = (int*)malloc(sizeof(array_estados[0]) * num_estados);
    if(array_estados == NULL){
        return NULL;
    }

    /*Annadimos el estado inicial al nuevo autómata*/
    /*Para ello tenemos que crear su pack*/
    strcpy(ini ,AFNDNombreEstadoEn(p_afnd, inicial));
    if(AFNDTipoEstadoEn(p_afnd, inicial) == INICIAL_Y_FINAL){
        ini_fin = INICIAL_Y_FINAL;
    }else{
        ini_fin = INICIAL;
    }
    /*Miramos si hay transiciones lambda*/
    for(i = 0; i < num_estados; i++){
        if(i == inicial){
            array_estados[i] = 1;
        }
        else if(AFNDCierreLTransicionIJ(p_afnd, inicial, i) == 1){
            array_estados[i] = 1;
            if(AFNDTipoEstadoEn(p_afnd, i) == FINAL){
                ini_fin = INICIAL_Y_FINAL;
            }    
            strcat(ini,  AFNDNombreEstadoEn(p_afnd, i));
        }
        else{
            array_estados[i] = 0;
        }
    }
    /*Creamos el pack del estado inicial y lo annadimos al array de packs 
     que seran los nuevos estados del automata determinista*/
    inicial_p = pack(array_estados, num_estados, ini, ini_fin);
    array_packs[index_p] = inicial_p;
    index_p ++;


    /*Exploramos los estados (= packs del array_packs)
    y vemos hacia donde pueden transitar*/
    for(i = 0; i < index_p; i++ ){
        Pack *p_tmp;

        p_tmp = pack(array_packs[i]->estados, num_estados, "temporal", NORMAL);

        /*array_packs_cerrados: lista de packs ya explorados*/
        if(pertenece(array_packs_cerrados, p_tmp, index_cerrados) != 0){
            /*Si no se ha explorado todavia lo annadimos y posteriormente exploramos*/
            array_packs_cerrados[index_cerrados] = p_tmp;
            index_cerrados++;

        } else {
            /*como ya se ha explorado saltamos al siguiente pack*/
            continue;

        }

        /*Exploramos por simbolos*/
        for(j = 0; j < num_simbolos; j++){
            Pack *p;
            int pertenece_f = -1;

            /*pack destino desde p_tmp con el simbolo de inidice j*/
            /*Utilizamos p_tmp que es una copia de array_packs[i] antes de haberlo explorado
                con algun simbolo */
            p = nuevo_pack(p_afnd, p_tmp, j, num_estados, &pertenece_f);

            /*si p esta formado por algun estado, es decir, habia transicion a algun
                estado desde p_tmp cno ese simbolo*/
            if(pertenece_f == 1){
                char* char_simb = AFNDSimboloEn(p_afnd, j);

                /*Si no se ha creado en el aut. determinista lo annadimos y annadimos
                    transicion*/
                if(pertenece(array_packs, p, index_p) != 0){
                    array_packs[index_p] = p;
                    index_p ++;
                    annadir_transicion(array_packs[i], p->nombre, char_simb);
                }else{
                    annadir_transicion(array_packs[i], p->nombre, char_simb);
                    destroy(p);
                } 
            }else{
                destroy(p);
            }
        }
    }  

    /*Finalmente creamos el nuevo automata*/
    /*index_p = numero de packs actuales*/
    afd = AFNDNuevo("afd", index_p, num_simbolos);

    /*Insertamos los simbolos*/
    for(i = 0; i < num_simbolos; i++){
        char* tmp = AFNDSimboloEn(p_afnd, i);
        AFNDInsertaSimbolo( afd, tmp);
        tmp = NULL;
    }

    /*Insertamos los estados*/
    for(i = 0; i < index_p; i++){
        AFNDInsertaEstado(afd, array_packs[i]->nombre, array_packs[i]->tipo);
    }

    /*Insertamos transiciones*/
    for(i=0; i < index_p; i++){
        for(j=0; j < array_packs[i]->t_index; j++){
            AFNDInsertaTransicion(afd, array_packs[i]->nombre, array_packs[i]->transiciones[j].simbolo, array_packs[i]->transiciones[j].destino);
        }
    }

    /*Liberamos memoria*/
    for(i = 0; i < index_p; i++){
        destroy(array_packs[i]);
    }

    for(i = 0; i < index_cerrados; i++){
        if(array_packs_cerrados[i] != NULL){
            destroy(array_packs_cerrados[i]);
        }
    }
    free(array_estados);
    return afd;
}


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
Pack* nuevo_pack(AFND* afnd, Pack* est, int simb, int num_estados, int* pertenece_f){
    int i, j;
    Pack *nuevo;
    int *estados;
    int tipo = NORMAL;
    char nombre[TAM_MAX_NOMBRE] = "";
    int* estados_pack = get_estados(est);

    estados = (int*)malloc(sizeof(estados[0]) * num_estados);
    if(estados == NULL){
        return NULL;
    }

    for(i = 0; i < num_estados; i++){
        estados[i] = 0;
    }  

    /*Inicialmente vemos las transiciones que hay a los distintos estados
      con el simbolo introducido*/
    for(i = 0; i < num_estados; i++){
        /*Si el estado_pack[i] forma parte del pack*/
        if(estados_pack[i] == 1){
            for(j = 0; j<num_estados; j++){
                /*Miro si hay una transicion en el automata ND inicial*/
                
                if ( 1 == AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, simb, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;
                    /*Vemos si tiene estado distinto al normal*/
                    if(AFNDTipoEstadoEn(afnd, j) == FINAL || AFNDTipoEstadoEn(afnd, j) == INICIAL_Y_FINAL){
                        tipo=FINAL;
                    }
                }
            }
        }           
        
    }

    /*Miramos si hay alguna transicion lambda*/
    for(i = 0; i < num_estados; i++){
        for(j = 0; j< num_estados; j++){
            /*estados[i]: miramos desde los estados transitados a traves de un simbolo
                          hasta donde podemos llegar con lambda*/
            if(estados[i] == 1){
                if(1 == AFNDCierreLTransicionIJ(afnd, i, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;
                    /*Vemos si tiene estado distinto al normal*/
                    if(AFNDTipoEstadoEn(afnd, j) == FINAL || AFNDTipoEstadoEn(afnd, j) == INICIAL_Y_FINAL){
                        tipo = FINAL;
                    }
                }
            }
        } 
    }

    /*Ahora tenemos un pack con 1 en los estados que lo conforman
            Construimos su nombre*/
    for(i = 0; i < num_estados; i++){
        if(estados[i] == 1){
            strcat(nombre, AFNDNombreEstadoEn(afnd, i));
        }
    }

    /*Creamos el nuevo pack y es el que devolvemos*/
    nuevo = pack(estados, num_estados, nombre, tipo);
    free(estados);
    return nuevo;
}

