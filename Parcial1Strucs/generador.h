#ifndef GENERADOR_H
#define GENERADOR_H

#include "persona.h"
#include <stddef.h>

typedef struct {
    Persona *data;
    size_t   len;
} Personas;

Personas generar_personas(size_t n);  
void     liberar_personas(Personas *v);

#endif
