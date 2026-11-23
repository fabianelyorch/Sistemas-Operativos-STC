#ifndef SIMULACION_H
#define SIMULACION_H

typedef struct {
    int velocidad;
    int ciclo_actual;
    int ciclo_limite;
    int detener;
} ControlSim;

void inicializar_control(ControlSim *ctrl, int vel_inicial, int limite_ciclos);
int configurar_velocidad(ControlSim *ctrl, int nueva_vel);
void configurar_limite(ControlSim *ctrl, int nuevo_limite);
void solicitar_paro(ControlSim *ctrl);
int puede_avanzar(ControlSim *ctrl);
int avanzar_ciclo(ControlSim *ctrl);

#endif
