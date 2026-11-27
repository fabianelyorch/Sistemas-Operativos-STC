#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memoria.h"
#include "RoundRobin.h"
#include "generador.h"
#include "simulacion.h"
#include "buddy.h"
#define PAUSA_MS 5000


int colocar(Memoria *m, Proceso p, int alg) {
    switch (alg) {
        case 1: return peor_ajuste(m, p);
        case 2: return buddy_colocar(m, p);
    }
    return 0;
}

int main(int argc, char *argv[]){
    int tam_memoria = 4096;
    int cuanto_sistema, tam_max, cuanto_max;
    int alg, mb;

    printf("Ingresa el tamano de la memoria (1024 4096 8192) en KB: \n");
    scanf("%d", &mb);
    printf("Ingresa el algoritmo de colocacion     1:Peor ajuste      2: Buddy system \n");
    scanf("%d", &alg);
    printf("Ingresa el cuanto del sistema: \n");
    scanf("%d", &cuanto_sistema);
    printf("Ingresa el tamano maximo de memoria del proceso en KB: \n");
    scanf("%d", &tam_max);
    printf("Ingresa el tamano maximo  del cuanto de cada proceso: \n");
    scanf("%d", &cuanto_max);

    tam_memoria = mb;
    if (alg == 2 && (mb < 32 || (mb & (mb - 1)) != 0)) {
        printf("Buddy system necesita una memoria potencia de 2 (1024, 4096, 8192)\n");
        return 1;
    }

    int id = 1;
    Memoria *m;
    ColaRR cola;
    Proceso listo;
    Bloque *b;
    ControlSim ctrl;

    srand(argc > 1 ? (unsigned int)atoi(argv[1]) : (unsigned int)time(NULL));
    inicializar_control(&ctrl, PAUSA_MS);

    m = crear_memoria(tam_memoria);
    iniciar_cola(&cola);
    listo = generar_proceso(id++, tam_max, cuanto_max);

    printf("Salida inicial\n");
    imprimir_memoria(m);
    printf("Escribe 's' en cualquier momento para detener la simulacion\n");

    while (debe_continuar(&ctrl)) {
        esperar_ciclo(&ctrl);
        if (!debe_continuar(&ctrl)) {
            break;
        }
        avanzar_ciclo(&ctrl);
        printf("\n--- CICLO %d ---\n", ctrl.ciclo_actual);

        if (colocar(m, listo, alg)) {
            printf("Entra Proceso [%d,%d,%d]\n", listo.id, listo.tam, listo.cuanto);
            encolar(&cola, buscar_bloque(m, listo.id));
            imprimir_memoria(m);
            listo = generar_proceso(id++, tam_max, cuanto_max);
        } else {
            printf("Proceso [%d,%d,%d] espera hueco\n", listo.id, listo.tam, listo.cuanto);
        }
        b = desencolar(&cola);
        if (b == NULL) {
            printf("No hay procesos que atender\n");
        } else {
            printf("Atendiendo proceso [%d,%d,%d]\n", b->proc.id, b->proc.tam, b->proc.cuanto);
            b->proc.cuanto -= cuanto_sistema;

            if (b->proc.cuanto > 0) {
                encolar(&cola, b);
                imprimir_cola(&cola);
                imprimir_memoria(m);
            } else {
                b->proc.cuanto = 0;
                imprimir_cola(&cola);
                printf("Descargando proceso [%d,%d,%d]\n", b->proc.id, b->proc.tam, b->proc.cuanto);
                if (alg == 2) {
                    buddy_liberar(m, b->proc.id);
                    imprimir_memoria(m);
                    printf("Fusionando buddies\n");
                    buddy_fusionar(m);
                    imprimir_memoria(m);
                } else {
                    liberar_proceso(m, b->proc.id);
                    imprimir_memoria(m);
                    printf("Condensando huecos\n");
                    condensar_huecos(m);
                    imprimir_memoria(m);
                }
            }
        }
    }

    printf("\nSimulacion detenida por el usuario\n");
    liberar_memoria(m);
    return 0;
}