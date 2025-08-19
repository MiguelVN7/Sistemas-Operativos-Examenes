#ifndef PERSONA_H
#define PERSONA_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_NOMBRE 64
#define MAX_APELLIDO 64
#define MAX_CIUDAD 64
#define MAX_ID 32
#define MAX_FECHA 16   //YYYY-MM-DD

typedef struct {
    char nombre[MAX_NOMBRE];
    char apellido[MAX_APELLIDO];
    char id[MAX_ID];                
    char ciudad[MAX_CIUDAD];       
    char fechaNacimiento[MAX_FECHA];
    double ingresos;
    double patrimonio;
    double deudas;
    bool declaraRenta;             
    char calendario;                
} Persona;

/* ---- API ---- */
char persona_calendario_por_id(const char *id);
int  persona_edad(const char *fechaYYYYMMDD);
double persona_patrimonio_neto(const Persona *p);
void persona_normalizar(Persona *p);       
void persona_imprimir(const Persona *p);
void persona_resumen(const Persona *p);

#endif
