#include <stdio.h>
#include "simulacion.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/* Inicia valores del control */
void inicializar_control(ControlSim *ctrl, int vel_ms) {
    if (ctrl == NULL) {
        return;
    }
    if (vel_ms < 0) {
        vel_ms = 0;
    }
    ctrl->velocidad_ms = vel_ms;
    ctrl->ciclo_actual = 0;
    ctrl->detener = 0;
}

/* Pausa segun velocidad en ms */
void esperar_ciclo(ControlSim *ctrl) {
    useconds_t microsegundos;

    if (ctrl == NULL || ctrl->velocidad_ms <= 0) {
        return;
    }

#ifdef _WIN32
    Sleep((DWORD)ctrl->velocidad_ms);
#else
    microsegundos = (useconds_t)ctrl->velocidad_ms * 1000;
    usleep(microsegundos);
#endif
}

/* Devuelve 1 si sigue corriendo */
int debe_continuar(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return 0;
    }
    if (ctrl->detener != 0) {
        return 0;
    }
    return 1;
}

/* Marca paro de simulacion */
void solicitar_paro(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    ctrl->detener = 1;
}

/* Suma uno al ciclo actual */
void avanzar_ciclo(ControlSim *ctrl) {
    if (ctrl == NULL || ctrl->detener != 0) {
        return;
    }
    ctrl->ciclo_actual = ctrl->ciclo_actual + 1;
}
