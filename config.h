#ifndef CONFIG_H
#define CONFIG_H

#define SPLIT_MIN 32       /* KB */

typedef enum { PRIMER_AJUSTE, MEJOR_AJUSTE, PEOR_AJUSTE, BUDDY } Algoritmo;

typedef struct {
    int tam_memoria;           /* 1024, 4096 u 8192 */
    int cuanto_sistema;
    int tam_max_proceso;
    int cuanto_max_proceso;
    Algoritmo algoritmo;
    int velocidad_ms;          /* pausa entre ciclos */
} Config;

#endif