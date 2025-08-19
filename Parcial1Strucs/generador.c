#include "generador.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

static const char *NOMBRES[] = {"Juan","Maria","Carlos","Ana","Luis","Sofia","Jorge","Laura","Pedro","Valentina"};
static const char *APELLIDOS[] = {"Gomez","Rodriguez","Martinez","Perez","Restrepo","Garcia","Londono","Ramirez"};
static const char *CIUDADES[] = {"Medellin","Bogota","Cali","Barranquilla","Bucaramanga","Manizales","Pereira","Cartagena"};

static int randi(int a, int b) { return a + rand() % (b-a+1); }
static double randf(double a, double b){ return a + (b-a) * (rand()/(double)RAND_MAX); }

static void random_fecha(char *out) {
    int y = randi(1930, 2007);
    int m = randi(1, 12);
    int d = randi(1, 28);
    sprintf(out, "%04d-%02d-%02d", y,m,d);
}

static void random_id(char *out) {
    // 8–10 dígitos
    int n = randi(8,10);
    for (int i=0;i<n;i++) out[i] = '0'+randi(0,9);
    out[n] = 0;
}

Personas generar_personas(size_t n) {
    srand((unsigned)time(NULL));
    Personas v = { .data = calloc(n, sizeof(Persona)), .len = n };
    for (size_t i=0;i<n;i++) {
        Persona *p = &v.data[i];
        snprintf(p->nombre,   sizeof p->nombre,   "%s", NOMBRES[randi(0,(int)(sizeof NOMBRES/sizeof *NOMBRES)-1)]);
        snprintf(p->apellido, sizeof p->apellido, "%s", APELLIDOS[randi(0,(int)(sizeof APELLIDOS/sizeof *APELLIDOS)-1)]);
        random_id(p->id);
        snprintf(p->ciudad, sizeof p->ciudad, "%s", CIUDADES[randi(0,(int)(sizeof CIUDADES/sizeof *CIUDADES)-1)]);
        random_fecha(p->fechaNacimiento);
        p->ingresos   = randf(0, 50000000.0);
        p->patrimonio = randf(0, 5000000000.0);
        p->deudas     = randf(0, 2000000000.0);
        persona_normalizar(p);
    }
    return v;
}

void liberar_personas(Personas *v) {
    free(v->data);
    v->data = NULL;
    v->len = 0;
}
