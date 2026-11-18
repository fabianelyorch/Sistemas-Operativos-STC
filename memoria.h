#ifndef MEMORIA_H
#define MEMORIA_H
#include "proceso.h"

typedef struct Bloque {
    Proceso proc;          /* si proc.id == 0, el bloque es un hueco */
    int inicio;            /* direccion de inicio en KB */
    struct Bloque *sig;
} Bloque;

typedef struct {
    Bloque *cabeza;
    int tam_total;
} Memoria;

#endif