//generacion aleatoria de procesos
#include <stdlib.h>
#include "generador.h"
#include "config.h"

Proceso generar_proceso(int id, int tam_max, int cuanto_max) {
    Proceso p;
    p.id = id;
    p.tam = SPLIT_MIN + rand() % (tam_max - SPLIT_MIN + 1);
    p.cuanto = 1 + rand() % cuanto_max;
    p.llegada = 0;
    return p;
}