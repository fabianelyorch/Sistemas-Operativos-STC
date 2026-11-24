#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memoria.h"
#include "RoundRobin.h"
#include "generador.h"
#include "simulacion.h"

#define PAUSA_MS 10000   

int main(int argc, char *argv[]){
    int tam_memoria = 4096;
    int cuanto_sistema, tam_max, cuanto_max;
    printf("Ingresa el cuanto del sistema: \n");
    scanf("%d", &cuanto_sistema);
    printf("Ingresa el tamano maximo de memoria del proceso: \n");
    scanf("%d", &tam_max);
    printf("Ingresa el tamano maximo  del cuanto de cada proceso: \n");
    scanf("%d", &cuanto_max);          

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

        if (peor_ajuste(m, listo)) {
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
                liberar_proceso(m, b->proc.id);
                imprimir_memoria(m);
                printf("Condensando huecos\n");
                condensar_huecos(m);
                imprimir_memoria(m);
            }
        }
    }

    printf("\nSimulacion detenida por el usuario\n");
    liberar_memoria(m);
    return 0;
}