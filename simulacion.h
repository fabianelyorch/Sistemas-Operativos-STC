#ifndef SIMULACION_H
#define SIMULACION_H

typedef struct {
    int velocidad_ms;
    int ciclo_actual;
    int detener;
} ControlSim;

void inicializar_control(ControlSim *ctrl, int vel_ms);
void esperar_ciclo(ControlSim *ctrl);
int debe_continuar(ControlSim *ctrl);
void solicitar_paro(ControlSim *ctrl);
void avanzar_ciclo(ControlSim *ctrl);

#endif
