//cola de Round Robin
#include <stdio.h>
#include "RoundRobin.h"

void iniciar_cola(ColaRR *c) {
    c->n = 0;
}

void encolar(ColaRR *c, Bloque *b) {
    if (c->n < MAX_COLA) {
        c->fila[c->n] = b;
        c->n++;
    }
}

Bloque *desencolar(ColaRR *c) {
    Bloque *primero;
    int i;

    if (c->n == 0) {
        return NULL;
    }

    primero = c->fila[0];
    for (i = 1; i < c->n; i++) {      /* recorre la fila una posicion */
        c->fila[i - 1] = c->fila[i];
    }
    c->n--;
    return primero;
}

void imprimir_cola(ColaRR *c) {
    int i;

    printf("Lista RR");
    for (i = 0; i < c->n; i++) {
        printf(" [%d,%d,%d]", c->fila[i]->proc.id, c->fila[i]->proc.tam, c->fila[i]->proc.cuanto);
    }
    printf("\n");
}