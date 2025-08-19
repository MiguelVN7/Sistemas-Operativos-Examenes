#include "persona.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


/* Últimos dos dígitos del documento:
   00–39 -> A, 40–79 -> B, 80–99 -> C  */
char persona_calendario_por_id(const char *id) {
    size_t n = strlen(id);
    if (n == 0) return 'A';
    int d1 = (n>=2)? (id[n-2]-'0') : 0;
    int d2 = id[n-1]-'0';
    if (d1 < 0 || d1 > 9 || d2 < 0 || d2 > 9) return 'A';
    int last2 = d1*10 + d2;
    if (last2 <= 39) return 'A';
    if (last2 <= 79) return 'B';
    return 'C';
}

int persona_edad(const char *fecha) {
    // "YYYY-MM-DD"
    int y=0,m=0,d=0;
    if (sscanf(fecha, "%d-%d-%d", &y,&m,&d) != 3) return 0;
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    int edad = now->tm_year + 1900 - y;
    if ((m > (now->tm_mon+1)) || (m == (now->tm_mon+1) && d > now->tm_mday))
        edad--;
    return edad < 0 ? 0 : edad;
}

double persona_patrimonio_neto(const Persona *p) {
    return p->patrimonio - p->deudas;
}

void persona_normalizar(Persona *p) {
    p->calendario = persona_calendario_por_id(p->id);
    // En este ejemplo asumimos que toda persona con patrimonio_neto > 0 declara renta
    p->declaraRenta = persona_patrimonio_neto(p) > 0.0;
}

void persona_imprimir(const Persona *p) {
    printf("--------------------------------------------------\n");
    printf("[%s] %s %s\n", p->id, p->nombre, p->apellido);
    printf("Ciudad: %s | Nace: %s | Edad: %d\n", p->ciudad, p->fechaNacimiento, persona_edad(p->fechaNacimiento));
    printf("Ingresos: %.2f | Patrimonio: %.2f | Deudas: %.2f | Neto: %.2f\n",
           p->ingresos, p->patrimonio, p->deudas, persona_patrimonio_neto(p));
    printf("Calendario: %c | Declara: %s\n", p->calendario, p->declaraRenta ? "SI":"NO");
}

void persona_resumen(const Persona *p) {
    printf("[%s] %-12s %-12s %-12s Neto: %.2f Cal:%c\n",
           p->id, p->nombre, p->apellido, p->ciudad, persona_patrimonio_neto(p), p->calendario);
}
