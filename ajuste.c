#include <stdlib.h>
#include "memoria.h"

/* Busca el bloque libre mas grande donde quepa el proceso */
int peor_ajuste(Memoria *m, Proceso p) {
    Bloque *actual;
    Bloque *peor;
    Bloque *nuevo;
    int sobrante;

    if (m == NULL || p.id == 0 || p.tam <= 0) {
        return 0;
    }

    peor = NULL;
    actual = m->cabeza;

    while (actual != NULL) {
        if (actual->proc.id == 0 && actual->proc.tam >= p.tam) {
            if (peor == NULL || actual->proc.tam > peor->proc.tam) {
                peor = actual;
            }
        }
        actual = actual->sig;
    }

    if (peor == NULL) {
        return 0;
    }

    sobrante = peor->proc.tam - p.tam;
    if (sobrante > 0) {
        nuevo = malloc(sizeof(Bloque));
        if (nuevo == NULL) {
            return 0;
        }
        nuevo->proc.id = 0;
        nuevo->proc.tam = sobrante;
        nuevo->proc.cuanto = 0;
        nuevo->proc.llegada = 0;
        nuevo->inicio = peor->inicio + p.tam;
        nuevo->sig = peor->sig;
        peor->sig = nuevo;
    }

    peor->proc.id = p.id;
    peor->proc.tam = p.tam;
    peor->proc.cuanto = p.cuanto;
    peor->proc.llegada = p.llegada;

    return 1;
}
