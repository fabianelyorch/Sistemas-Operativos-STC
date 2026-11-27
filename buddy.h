#ifndef BUDDY_H
#define BUDDY_H
#include "memoria.h"

int buddy_colocar(Memoria *m, Proceso p);
int buddy_liberar(Memoria *m, int id);
void buddy_fusionar(Memoria *m);

#endif