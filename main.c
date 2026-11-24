#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memoria.h"
#include "RoundRobin.h"
#include "generador.h"
#include "simulacion.h"

int main(int argc, char *argv[]) {
    int tam_memoria = 4096;
    int cuanto_sistema = 2;
    int tam_max = 600;
    int cuanto_max = 10;
    int limite_usuario, opcion, nueva_vel, pasos_dados, i;
    ControlSim ctrl;

    int id = 1;
    Memoria *m;
    ColaRR cola;
    Proceso listo;          
    Bloque *b;

    srand(argc > 1 ? (unsigned int)atoi(argv[1]) : (unsigned int)time(NULL));
    
    printf("Ciclo limite para parar (0 para detener manualmente): ");
    scanf("%d", &limite_usuario);

    printf("Velocidad inicial (ciclos por paso, minimo 1): ");
    scanf("%d", &nueva_vel);

    inicializar_control(&ctrl, nueva_vel, limite_usuario);

    m = crear_memoria(tam_memoria);
    iniciar_cola(&cola);
    listo = generar_proceso(id++, tam_max, cuanto_max);

    printf("Salida inicial\n");
    imprimir_memoria(m);

    while (puede_avanzar(&ctrl)) {
        pasos_dados = avanzar_ciclo(&ctrl);
        if (pasos_dados <= 0) {
            break;
        }

        for (i = 0; i < pasos_dados; i++) {
            printf("\n--- CICLO %d (Vel: %d) ---\n", ctrl.ciclo_actual - pasos_dados + i + 1, ctrl.velocidad);

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
                continue;
            }

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

        if (!puede_avanzar(&ctrl)) {
            break;
        }

        printf("\n1. Siguiente paso (%d ciclos)\n", ctrl.velocidad);
        printf("2. Cambiar velocidad\n");
        printf("3. Cambiar ciclo limite\n");
        printf("4. Detener simulacion ahora\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 2) {
            printf("Nueva velocidad (minimo 1): ");
            scanf("%d", &nueva_vel);
            if (configurar_velocidad(&ctrl, nueva_vel)) {
                printf("Velocidad actual: %d\n", ctrl.velocidad);
            } else {
                printf("Velocidad no valida\n");
            }
        } else if (opcion == 3) {
            printf("Nuevo ciclo limite (0 sin limite): ");
            scanf("%d", &limite_usuario);
            configurar_limite(&ctrl, limite_usuario);
            printf("Limite actualizado a %d\n", ctrl.ciclo_limite);
        } else if (opcion == 4) {
            solicitar_paro(&ctrl);
            printf("Simulacion detenida por el usuario\n");
        }
    }

    liberar_memoria(m);
    return 0;
}
