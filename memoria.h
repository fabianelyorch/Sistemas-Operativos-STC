#ifndef MEMORIA_H
#define MEMORIA_H
#include "proceso.h"

typedef struct Bloque {    //basicamente representa el proceso que esta usando un hueco de la memoria
    Proceso proc;          //los datos del proceso en un pedazo
    int inicio;            //la direccin en KB en que empieza un pedazo
    int tam_bloque;        //Buddy: tamano (potencia de 2) que ocupa el bloque
    char lado;             //Buddy: '#' si es el inicio de la mitad derecha
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
Bloque *buscar_bloque(Memoria *m, int id);
int liberar_proceso(Memoria *m, int id);
void condensar_huecos(Memoria *m);

#endif