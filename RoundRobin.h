#ifndef RR_H
#define RR_H
#include "memoria.h"

#define MAX_COLA 200

/* Fila de Round Robin: fila[0] es el siguiente en atenderse */
typedef struct {
    Bloque *fila[MAX_COLA];
    int n;
} ColaRR;

void iniciar_cola(ColaRR *c);
void encolar(ColaRR *c, Bloque *b);     /* lo mete al final */
Bloque *desencolar(ColaRR *c);          /* saca al primero, NULL si esta vacia */
void imprimir_cola(ColaRR *c);          /* Lista RR [1,300,2] [6,256,10] ... */

#endif