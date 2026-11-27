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
    b->tam_bloque = tam_total;
    b->lado = ' ';
    b->sig = NULL;

    Memoria *m = malloc(sizeof(Memoria));
    if (m == NULL){
        free(b);
        return NULL;
    }
    m->cabeza = b;
    m->tam_total = tam_total;
    return m;

}

void imprimir_memoria(Memoria *m) {
    Bloque *actual;

    if (m == NULL) {
        printf("Memoria no creada\n");
        return;
    }

    actual = m->cabeza;
    while (actual != NULL) {
        if (actual->lado == 'D') {
            printf("#");
        }
        printf("[%d,%d,%d]", actual->proc.id, actual->proc.tam, actual->proc.cuanto);
        if (actual->proc.id != 0 && actual->tam_bloque > actual->proc.tam) {
            printf("[0,%d,0]", actual->tam_bloque - actual->proc.tam);
        }
        actual = actual->sig;
    }
    printf("\n");
}

void imprimir_memoria_detalle(Memoria *m) {
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

Bloque *buscar_bloque(Memoria *m, int id) {
    Bloque *actual;

    if (m == NULL || id == 0) {
        return NULL;
    }

    actual = m->cabeza;
    while (actual != NULL) {
        if (actual->proc.id == id) {
            return actual;
        }
        actual = actual->sig;
    }
    return NULL;
}

int liberar_proceso(Memoria *m, int id) {
    Bloque *b = buscar_bloque(m, id);

    if (b == NULL) {
        return 0;
    }

    b->proc.id = 0;
    b->proc.cuanto = 0;
    b->proc.llegada = 0;
    return 1;
}

void condensar_huecos(Memoria *m) {
    Bloque *actual;
    Bloque *siguiente;

    if (m == NULL) {
        return;
    }

    actual = m->cabeza;
    while (actual != NULL && actual->sig != NULL) {
        siguiente = actual->sig;
        if (actual->proc.id == 0 && siguiente->proc.id == 0) {
            actual->proc.tam += siguiente->proc.tam;
            actual->sig = siguiente->sig;
            free(siguiente);
        } else {
            actual = actual->sig;
        }
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