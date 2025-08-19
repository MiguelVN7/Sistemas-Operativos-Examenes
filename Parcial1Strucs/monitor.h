#ifndef MONITOR_H
#define MONITOR_H

#include "persona.h"
#include "generador.h"

/* Consultas pedidas */
const Persona* persona_mas_longeva(const Personas *v);
const Persona* persona_mas_longeva_en_ciudad(const Personas *v, const char *ciudad);

const Persona* mayor_patrimonio(const Personas *v);
const Persona* mayor_patrimonio_en_ciudad(const Personas *v, const char *ciudad);
const Persona* mayor_patrimonio_por_cal(const Personas *v, char cal);

// conteos por calendario
void contar_por_calendario(const Personas *v, size_t *a, size_t *b, size_t *c);
int validar_calendarios(const Personas *v);

#endif
