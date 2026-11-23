#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memoria.h"
#include "RoundRobin.h"
#include "generador.h"

int main(int argc, char *argv[]) {
    int tam_memoria = 4096;
    int cuanto_sistema = 0;
    int tam_max = 0;
    int cuanto_max = 0;
    printf("Ingresa el cuanto del sistema: \n");
    scanf("%d", &cuanto_sistema);
    printf("Ingresa el tamano maximo de memoria del proceso: \n");
    scanf("%d", &tam_max);
    printf("Ingresa el tamano maximo  del cuanto de cada proceso: \n");
    scanf("%d", &cuanto_max);
    int ciclos = 20;            

    int id = 1;
    int ciclo;
    Memoria *m;
    ColaRR cola;
    Proceso listo;          
    Bloque *b;

    srand(argc > 1 ? (unsigned int)atoi(argv[1]) : (unsigned int)time(NULL));

    m = crear_memoria(tam_memoria);
    iniciar_cola(&cola);
    listo = generar_proceso(id++, tam_max, cuanto_max);

    printf("Salida inicial\n");
    imprimir_memoria(m);

    for (ciclo = 1; ciclo <= ciclos; ciclo++) {
        printf("\n--- CICLO %d ---\n", ciclo);

        /* 1. Entra el proceso listo si hay hueco */
        if (peor_ajuste(m, listo)) {
            printf("Entra Proceso [%d,%d,%d]\n", listo.id, listo.tam, listo.cuanto);
            encolar(&cola, buscar_bloque(m, listo.id));
            imprimir_memoria(m);
            listo = generar_proceso(id++, tam_max, cuanto_max);
        } else {
            printf("Proceso [%d,%d,%d] espera hueco\n", listo.id, listo.tam, listo.cuanto);
        }

        /* 2. Round Robin: se atiende al primero de la fila un cuanto */
        b = desencolar(&cola);
        if (b == NULL) {
            printf("No hay procesos que atender\n");
            continue;
        }

        printf("Atendiendo proceso [%d,%d,%d]\n", b->proc.id, b->proc.tam, b->proc.cuanto);
        b->proc.cuanto -= cuanto_sistema;

        if (b->proc.cuanto > 0) {
            encolar(&cola, b);                  /* le queda cuanto: vuelve al final */
            imprimir_cola(&cola);
            imprimir_memoria(m);
        } else {
            b->proc.cuanto = 0;                 /* termino */
            imprimir_cola(&cola);
            printf("Descargando proceso [%d,%d,%d]\n", b->proc.id, b->proc.tam, b->proc.cuanto);
            liberar_proceso(m, b->proc.id);
            imprimir_memoria(m);
            printf("Condensando huecos\n");
            condensar_huecos(m);
            imprimir_memoria(m);
        }
    }

    liberar_memoria(m);
    return 0;
}