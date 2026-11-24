#include <stdio.h>
#include "simulacion.h"

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <sys/select.h>
#endif

#define PASO_MS 100   /* cada cuantos ms revisa si el usuario pidio detener */

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

static int usuario_pidio_paro(void) {
#ifdef _WIN32
    int c;

    while (_kbhit()) {
        c = _getch();
        if (c == 's' || c == 'S') {
            return 1;
        }
    }
    return 0;
#else
    fd_set fds;
    struct timeval espera;
    char buf[64];
    ssize_t n, i;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    espera.tv_sec = 0;
    espera.tv_usec = 0;
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &espera) <= 0) {
        return 0;                    
    }
    n = read(STDIN_FILENO, buf, sizeof(buf));
    if (n <= 0) {
        return 1;                     
    }
    for (i = 0; i < n; i++) {
        if (buf[i] == 's' || buf[i] == 'S') {
            return 1;
        }
    }
    return 0;
#endif
}

void esperar_ciclo(ControlSim *ctrl) {
    int esperado = 0;

    if (ctrl == NULL) {
        return;
    }

    while (esperado < ctrl->velocidad_ms) {
        if (usuario_pidio_paro()) {
            solicitar_paro(ctrl);
            return;
        }
#ifdef _WIN32
        Sleep(PASO_MS);
#else
        usleep(PASO_MS * 1000);
#endif
        esperado += PASO_MS;
    }

    if (usuario_pidio_paro()) {
        solicitar_paro(ctrl);
    }
}
int debe_continuar(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return 0;
    }
    if (ctrl->detener != 0) {
        return 0;
    }
    return 1;
}

void solicitar_paro(ControlSim *ctrl) {
    if (ctrl == NULL) {
        return;
    }
    ctrl->detener = 1;
}

void avanzar_ciclo(ControlSim *ctrl) {
    if (ctrl == NULL || ctrl->detener != 0) {
        return;
    }
    ctrl->ciclo_actual = ctrl->ciclo_actual + 1;
}