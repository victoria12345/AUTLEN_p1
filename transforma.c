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
    char ini[TAM_MAX_NOMBRE];
    int ini_fin = INICIAL;
    Pack* array_packs[100]; 
    Pack* array_packs_cerrados[100];
    int index_cerrados = 0; 
    Pack* inicial_p;
    AFND * afd;

    array_estados = (int*)malloc(sizeof(array_estados[0]) * num_estados);
    if(array_estados == NULL){
        return NULL;
    }

    /*Annadimos el estado inicial al nuevo autómata*/
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

    inicial_p = pack(array_estados, num_estados, ini, ini_fin);
    array_packs[index_p] = inicial_p;
    index_p ++;

    /*Exploramos los estados*/
    for(i = 0; i < index_p; i++ ){
        Pack *p_tmp;

        p_tmp = pack(array_packs[i]->estados, num_estados, "dummy", 0);

        if(pertenece(array_packs_cerrados, p_tmp, index_cerrados) != 0){

            array_packs_cerrados[index_cerrados] = p_tmp;
            index_cerrados++;

        } else {

            continue;

        }

        /*Exploramos el simbolo*/

        for(j = 0; j < num_simbolos; j++){
            Pack *p;
            int pertenece_f = -1;

            p = nuevo_pack(p_afnd, p_tmp /* array_packs[i]*/, j, num_estados, &pertenece_f);
            imprime(p);
            if(pertenece_f == 1){
                char* char_simb = AFNDSimboloEn(p_afnd, j);

                /*Si no se ha creado lo annadimos*/
                if(pertenece(array_packs, p, index_p) != 0){
                    /* printf("El pack q estoy mirando %s\n", p->nombre);
                    imprime(p); */
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
        imprime(array_packs[i]);
        destroy(array_packs[i]);
    }
    free(array_estados);
    return afd;
}

Pack* nuevo_pack_lambda(AFND* afnd, Pack* est, int num_estados){
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

    /*Miramos las transiciones lambda*/
    for(i = 0; i < num_estados; i++){
        for(j = 0; j< num_estados; j++){

            /*estados_pack[i]: del estdo inicial miramos hacia donde podemos llegar con lambda*/
            /*estados[i]: miramos desde los estados transitados a traves d eun simbolo
                          hasta donde podemos llegar con lambda*/
            if(estados_pack[i] == 1 /*|| estados[i] == 1*/){
                if(1 == AFNDCierreLTransicionIJ(afnd, i, j)){
                    estados[j] = 1;

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

    nuevo = pack(estados, num_estados, nombre, tipo);
    free(estados);
    return nuevo;
}

/**
* est: pack con los estados correspondientes del automata inicial
* simb: simbolo que exploramos para ver el nuevo estado
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

    
    for(i = 0; i < num_estados; i++){
        /*Si el estado[i] forma parte del pack*/
        if(estados_pack[i] == 1){
            for(j = 0; j<num_estados; j++){
                /*Miro si hay una transicion en el automata ND inicial*/
                
                if ( 1 == AFNDTransicionIndicesEstadoiSimboloEstadof(afnd, i, simb, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;
                    if(AFNDTipoEstadoEn(afnd, j) == FINAL || AFNDTipoEstadoEn(afnd, j) == INICIAL_Y_FINAL){
                        tipo=FINAL;
                    }
                }
            }
        }           
        
    }

    printf("Acabo de añadir simbolos\n");
    for(i = 0; i < num_estados; i++){
        printf("%d,\t", estados[i]);
    }
    printf("\n");

    /*Miramos las transiciones lambda*/
    for(i = 0; i < num_estados; i++){
        for(j = 0; j< num_estados; j++){

            /*estados_pack[i]: del estdo inicial miramos hacia donde podemos llegar con lambda*/
            /*estados[i]: miramos desde los estados transitados a traves d eun simbolo
                          hasta donde podemos llegar con lambda*/
            if(estados[i] == 1){
                if(1 == AFNDCierreLTransicionIJ(afnd, i, j)){
                    *pertenece_f = 1;
                    estados[j] = 1;

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

    nuevo = pack(estados, num_estados, nombre, tipo);
    free(estados);
    return nuevo;
}

