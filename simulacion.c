#include <stdio.h>
#include "simulacion.h"

void inicializar_control(ControlSim *ctrl, int vel_inicial, int limite_ciclos) {
    if (ctrl == NULL) {
        return;
    }
    if (vel_inicial < 1) {
        vel_inicial = 1;
    }
    if (limite_ciclos < 0) {
        limite_ciclos = 0;
    }
    ctrl->velocidad = vel_inicial;
    ctrl->ciclo_actual = 0;
    ctrl->ciclo_limite = limite_ciclos;
    ctrl->detener = 0;
}

int configurar_velocidad(ControlSim *ctrl, int nueva_vel) {
    if (ctrl == NULL || nueva_vel < 1) {
        return 0;
    }
    ctrl->velocidad = nueva_vel;
    return 1;
}

void configurar_limite(ControlSim *ctrl, int nuevo_limite) {
    if (ctrl == NULL) {
        return;
    }
    if (nuevo_limite < 0) {
        nuevo_limite = 0;
    }
    ctrl->ciclo_limite = nuevo_limite;
}

void solicitar_paro(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    ctrl->detener = 1;
}

int puede_avanzar(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return 0;
    }
    if (ctrl->detener != 0) {
        return 0;
    }
    if (ctrl->ciclo_limite > 0 && ctrl->ciclo_actual >= ctrl->ciclo_limite) {
        return 0;
    }
    return 1;
}

int avanzar_ciclo(ControlSim *ctrl) {
    int siguiente, tope;

    if (!puede_avanzar(ctrl)) {
        return 0;
    }

    siguiente = ctrl->ciclo_actual + ctrl->velocidad;
    if (ctrl->ciclo_limite > 0 && siguiente > ctrl->ciclo_limite) {
        tope = ctrl->ciclo_limite - ctrl->ciclo_actual;
        ctrl->ciclo_actual = ctrl->ciclo_limite;
        return tope;
    }

    ctrl->ciclo_actual = siguiente;
    return ctrl->velocidad;
}
