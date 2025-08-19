#include "monitor.h"
#include <string.h>

static int mejor_por_edad(const Persona *a, const Persona *b) {
    int ea = persona_edad(a->fechaNacimiento);
    int eb = persona_edad(b->fechaNacimiento);
    return ea > eb; // mayor edad => mejor
}
static int mejor_por_neto(const Persona *a, const Persona *b) {
    return persona_patrimonio_neto(a) > persona_patrimonio_neto(b);
}

const Persona* persona_mas_longeva(const Personas *v) {
    if (!v || v->len==0) return NULL;
    const Persona *mej=&v->data[0];
    for (size_t i=1;i<v->len;i++)
        if (mejor_por_edad(&v->data[i], mej)) mej = &v->data[i];
    return mej;
}

const Persona* persona_mas_longeva_en_ciudad(const Personas *v, const char *ciudad) {
    const Persona *mej = NULL;
    for (size_t i=0;i<v->len;i++) {
        const Persona *p = &v->data[i];
        if (strcmp(p->ciudad, ciudad)==0) {
            if (!mej || mejor_por_edad(p, mej)) mej = p;
        }
    }
    return mej;
}

const Persona* mayor_patrimonio(const Personas *v) {
    if (!v || v->len==0) return NULL;
    const Persona *mej=&v->data[0];
    for (size_t i=1;i<v->len;i++)
        if (mejor_por_neto(&v->data[i], mej)) mej = &v->data[i];
    return mej;
}

const Persona* mayor_patrimonio_en_ciudad(const Personas *v, const char *ciudad) {
    const Persona *mej = NULL;
    for (size_t i=0;i<v->len;i++) {
        const Persona *p = &v->data[i];
        if (strcmp(p->ciudad, ciudad)==0) {
            if (!mej || mejor_por_neto(p, mej)) mej = p;
        }
    }
    return mej;
}

const Persona* mayor_patrimonio_por_cal(const Personas *v, char cal) {
    const Persona *mej = NULL;
    for (size_t i=0;i<v->len;i++) {
        const Persona *p = &v->data[i];
        if (p->calendario == cal) {
            if (!mej || mejor_por_neto(p, mej)) mej = p;
        }
    }
    return mej;
}

void contar_por_calendario(const Personas *v, size_t *a, size_t *b, size_t *c) {
    *a = *b = *c = 0;
    for (size_t i=0;i<v->len;i++) {
        switch (v->data[i].calendario) {
            case 'A': (*a)++; break;
            case 'B': (*b)++; break;
            case 'C': (*c)++; break;
        }
    }
}

int validar_calendarios(const Personas *v) {
    for (size_t i=0;i<v->len;i++) {
        char ok = persona_calendario_por_id(v->data[i].id);
        if (ok != v->data[i].calendario) return 0;
    }
    return 1;
}
