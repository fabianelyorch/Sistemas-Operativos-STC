#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memoria.h"
#include "RoundRobin.h"
#include "generador.h"
#include "simulacion.h"

int main(int argc, char *argv[]) {
    int tam_memoria, cuanto_sistema, tam_max, cuanto_max, id;
    char resp;
    Memoria *m;
    ColaRR cola;
    Proceso listo;
    Bloque *b;
    ControlSim ctrl;

    /* Valores iniciales de la simulacion */
    tam_memoria = 4096;
    cuanto_sistema = 2;
    tam_max = 600;
    cuanto_max = 10;
    id = 1;

    srand(argc > 1 ? (unsigned int)atoi(argv[1]) : (unsigned int)time(NULL));
    inicializar_control(&ctrl, 500);

    /* Crea memoria, cola y primer proceso */
    m = crear_memoria(tam_memoria);
    iniciar_cola(&cola);
    listo = generar_proceso(id++, tam_max, cuanto_max);

    printf("Salida inicial\n");
    imprimir_memoria(m);

    /* Avanza ciclo por ciclo hasta que pidan parar */
    while (debe_continuar(&ctrl)) {
        avanzar_ciclo(&ctrl);
        esperar_ciclo(&ctrl);

        printf("\n--- CICLO %d ---\n", ctrl.ciclo_actual);

        /* Intenta meter el proceso listo en memoria */
        if (peor_ajuste(m, listo)) {
            listo.llegada = ctrl.ciclo_actual;
            printf("Entra Proceso [%d,%d,%d]\n", listo.id, listo.tam, listo.cuanto);
            encolar(&cola, buscar_bloque(m, listo.id));
            imprimir_memoria(m);
            listo = generar_proceso(id++, tam_max, cuanto_max);
        } else {
            printf("Proceso [%d,%d,%d] espera hueco\n", listo.id, listo.tam, listo.cuanto);
        }

        /* Atiende un cuanto con Round Robin */
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
                liberar_proceso(m, b->proc.id);
                imprimir_memoria(m);
                printf("Condensando huecos\n");
                condensar_huecos(m);
                imprimir_memoria(m);
            }
        }

        /* Pregunta solo si desea detener */
        printf("Detener simulacion? (s/n): ");
        scanf(" %c", &resp);
        if (resp == 's' || resp == 'S') {
            solicitar_paro(&ctrl);
        }
    }

    /* Libera la memoria al terminar */
    liberar_memoria(m);
    return 0;
}
