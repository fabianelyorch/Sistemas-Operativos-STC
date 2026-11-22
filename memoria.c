//manejo de la memoria: la lista de bloques insertar y liberar procesos
#include <stdio.h>  //         <>
#include <stdlib.h>
#include "memoria.h"

Memoria *crear_memoria(int tam_total) {
    Bloque *b = malloc(sizeof(Bloque));
    if (b == NULL){
        return NULL;
    }
    b->proc.id = 0;
    b->proc.tam = tam_total;
    b->proc.cuanto = 0;
    b->proc.llegada = 0;
    b->inicio = 0;
    b->sig = NULL;

    Memoria *m = malloc(sizeof(Memoria));
    if (m == NULL){
        return NULL;
    }
    m->cabeza = b;
    m->tam_total = tam_total;
    return m;


}

void imprimir_memoria(Memoria *m) {
    Bloque *actual;
    int fin;

    if (m == NULL) {
        printf("Memoria no creada\n");
        return;
    }

    printf("\n--- Memoria (%d KB) ---\n", m->tam_total);
    actual = m->cabeza;

    while (actual != NULL) {
        fin = actual->inicio + actual->proc.tam - 1;
        printf("[%d - %d] (%d KB): ", actual->inicio, fin, actual->proc.tam);

        if (actual->proc.id == 0) {
            printf("Libre\n");
        } else {
            printf("Proceso %d\n", actual->proc.id);
        }

        actual = actual->sig;
    }
}

void liberar_memoria(Memoria *m) {
    Bloque *actual;
    Bloque *sig;

    if (m == NULL) {
        return;
    }

    actual = m->cabeza;
    while (actual != NULL) {
        sig = actual->sig;
        free(actual);
        actual = sig;
    }

    free(m);
}
