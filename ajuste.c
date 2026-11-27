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
    actual = m->cabeza; //empieza el recorrido desde el inicio de la memoria

    while (actual != NULL) {
        if (actual->proc.id == 0 && actual->proc.tam >= p.tam) { //recorre bloques libres de procesos 
            if (peor == NULL || actual->proc.tam > peor->proc.tam) { //asigna el proceso al bloque de mayor tamaño
                peor = actual;
            }
        }
        actual = actual->sig;
    }

    if (peor == NULL) { //si no se encuentra un bloque libre con suficiente capacidad
        return 0;
    }

    sobrante = peor->proc.tam - p.tam; //calcula el espacio que sobro
    if (sobrante > 0) {
        nuevo = malloc(sizeof(Bloque)); //divide el bloque si queda memoria libre
        if (nuevo == NULL) {
            return 0;
        }
        nuevo->proc.id = 0;
        nuevo->proc.tam = sobrante;
        nuevo->proc.cuanto = 0;
        nuevo->proc.llegada = 0;                          //incializa un nuevo bloque libre con el espacio sobrante
        nuevo->tam_bloque = sobrante;
        nuevo->lado = ' ';
        nuevo->inicio = peor->inicio + p.tam; //la direccion del nuevo bloque va justo desepues del bloque que ya se le asigno el proceso
        nuevo->sig = peor->sig;
        peor->sig = nuevo;
    }

    peor->proc.id = p.id;
    peor->proc.tam = p.tam;           //asigna los datos del proceso al bloque seleccionado
    peor->proc.cuanto = p.cuanto;
    peor->proc.llegada = p.llegada;
    peor->tam_bloque = p.tam;   /* sin Buddy el bloque mide justo lo del proceso */

    return 1;
}