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

    /* Formato del requerimiento: [id,tamano,cuanto] por bloque, todo en una linea.
       Los huecos tienen id 0, p. ej. [0,96,0] */
    actual = m->cabeza;
    while (actual != NULL) {
        printf("[%d,%d,%d]", actual->proc.id, actual->proc.tam, actual->proc.cuanto);
        actual = actual->sig;
    }
    printf("\n");
}

/* Version de depuracion: muestra direcciones de inicio y fin de cada bloque */
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

/* Devuelve el bloque que contiene al proceso con ese id (NULL si no existe).
   Sirve para guardar el bloque en la cola de Round Robin despues de insertarlo. */
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

/* Convierte el bloque del proceso en un hueco (id 0). No une huecos: eso lo hace
   condensar_huecos, para poder imprimir los dos pasos por separado como pide el PDF.
   Antes de llamarla el proceso ya debe estar fuera de la cola RR. */
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

/* Une cada par de huecos contiguos en uno solo. Solo libera nodos de huecos,
   nunca de procesos, asi los apuntadores de la cola RR siguen validos. */
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
            /* no avanzamos: el nuevo vecino tambien podria ser hueco */
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