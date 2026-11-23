#ifndef MEMORIA_H
#define MEMORIA_H
#include "proceso.h"

typedef struct Bloque {    //basicamente representa el proceso que esta usando un hueco de la memoria
    Proceso proc;          //los datos del proceso en un pedazo
    int inicio;            //la direccin en KB en que empieza un pedazo
    struct Bloque *sig;    // apuntador al siguiente espacio de la cadena
} Bloque;

typedef struct {
    Bloque *cabeza;
    int tam_total;
} Memoria;

Memoria *crear_memoria(int tam_total);
void imprimir_memoria(Memoria *m);          
void imprimir_memoria_detalle(Memoria *m);  
void liberar_memoria(Memoria *m);
int peor_ajuste(Memoria *m, Proceso p);

#endif