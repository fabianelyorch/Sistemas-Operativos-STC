#include <stdio.h>
#include "memoria.h"

int main(void) {
    int tam_total = 4096;
    Memoria *m = crear_memoria(tam_total);
    imprimir_memoria(m);
    liberar_memoria(m);
    return 0;
}