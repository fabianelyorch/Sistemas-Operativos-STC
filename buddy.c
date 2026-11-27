#include <stdlib.h>
#include "buddy.h"
#include "config.h"

int potencia2(int t) {
    int p = SPLIT_MIN;
    while (p < t) p *= 2;
    return p;
}

void partir(Bloque *b, int tam_total) {
    Bloque *der = malloc(sizeof(Bloque));
    int mitad = b->tam_bloque / 2;

    if (der == NULL) {
        return;
    }
    der->proc.id = 0;
    der->proc.tam = mitad;
    der->proc.cuanto = 0;
    der->proc.llegada = 0;
    der->tam_bloque = mitad;
    der->inicio = b->inicio + mitad;
    der->lado = ' ';
    der->sig = b->sig;

    if (b->tam_bloque == tam_total) {
        der->lado = 'D';
    }

    b->tam_bloque = mitad;
    b->proc.tam = mitad;
    b->sig = der;
}

int buddy_colocar(Memoria *m, Proceso p) {
    int need;
    Bloque *mejor = NULL, *b;

    if (m == NULL || p.id == 0 || p.tam <= 0) {
        return 0;
    }
    need = potencia2(p.tam);

    for (b = m->cabeza; b; b = b->sig)
        if (b->proc.id == 0 && b->tam_bloque >= need)
            if (!mejor || b->tam_bloque < mejor->tam_bloque)
                mejor = b;
    if (!mejor) return 0;

    while (mejor->tam_bloque / 2 >= need) {
        int antes = mejor->tam_bloque;
        partir(mejor, m->tam_total);
        if (mejor->tam_bloque == antes) {
            return 0;
        }
    }

    mejor->proc.id = p.id;
    mejor->proc.tam = p.tam;
    mejor->proc.cuanto = p.cuanto;
    mejor->proc.llegada = p.llegada;
    return 1;
}

int buddy_liberar(Memoria *m, int id) {
    Bloque *b = buscar_bloque(m, id);

    if (b == NULL) {
        return 0;
    }
    b->proc.id = 0;
    b->proc.tam = b->tam_bloque;
    b->proc.cuanto = 0;
    b->proc.llegada = 0;
    return 1;
}

void buddy_fusionar(Memoria *m) {
    Bloque *a, *der;
    int hubo;

    if (m == NULL) {
        return;
    }
    do {
        hubo = 0;
        for (a = m->cabeza; a != NULL && a->sig != NULL; a = a->sig) {
            der = a->sig;
            if (a->proc.id == 0 && der->proc.id == 0 &&
                a->tam_bloque == der->tam_bloque &&
                a->inicio % (2 * a->tam_bloque) == 0) {
                a->tam_bloque *= 2;
                a->proc.tam = a->tam_bloque;
                if (a->tam_bloque == m->tam_total) {
                    a->lado = ' ';
                }
                a->sig = der->sig;
                free(der);
                hubo = 1;
                break;
            }
        }
    } while (hubo);
}