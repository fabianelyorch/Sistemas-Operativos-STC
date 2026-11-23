#include <stdio.h>
#include "memoria.h"
#include "proceso.h"

int main(void){
    int tam_total = 4096;
    Memoria *m = crear_memoria(tam_total);
    printf("Salida inicial\n");
    imprimir_memoria(m);                    // [0,4096,0] 
    Proceso p1 = {1, 300, 10, 0};           // id, tam, cuanto, llegada 
    Proceso p5 = {5, 500, 9, 0};

    printf("Entra proceso [%d,%d,%d]\n", p1.id, p1.tam, p1.cuanto);
    peor_ajuste(m, p1);
    imprimir_memoria(m);
    printf("Entra proceso [%d,%d,%d]\n", p5.id, p5.tam, p5.cuanto);
    peor_ajuste(m, p5);
    imprimir_memoria(m);
    liberar_memoria(m);
    
    return 0;
}