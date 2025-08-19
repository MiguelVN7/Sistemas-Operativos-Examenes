// main.c — Menú completo (15 opciones) en C con structs

#include "generador.h"
#include "monitor.h"
#include "persona.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE 256
#define MAX_STATS 128

// ======================= Estado global simple =======================

static Personas G = { .data = NULL, .len = 0 };

typedef struct {
    size_t N;
    double gen_seconds;
    size_t rss_kb_after_gen;
    time_t when;
} StatEntry;

static StatEntry STATS[MAX_STATS];
static size_t STATS_LEN = 0;

// ======================= Helpers de impresión =======================

static void imprimir_persona_linea(const char *titulo, const Persona *p){
    if (!p) { printf("%s: (sin datos)\n", titulo); return; }
    printf("%s: [%s] %s %s | %s | edad %d | neto %.2f | cal %c\n",
        titulo, p->id, p->nombre, p->apellido, p->ciudad,
        persona_edad(p->fechaNacimiento), persona_patrimonio_neto(p), p->calendario);
}

static void pausar() {
    printf("\nPresione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ======================= Entrada segura =======================

static int leer_linea(char *buf, size_t cap) {
    if (!fgets(buf, (int)cap, stdin)) return 0;
    // quitar \n si viene
    buf[strcspn(buf, "\n")] = 0;
    return 1;
}

static int leer_entero(const char *prompt, long *out) {
    char line[MAX_LINE];
    printf("%s", prompt);
    if (!leer_linea(line, sizeof line)) return 0;
    char *end = NULL;
    long v = strtol(line, &end, 10);
    if (line == end) return 0;
    *out = v;
    return 1;
}

static int leer_char(const char *prompt, char *out) {
    char line[MAX_LINE];
    printf("%s", prompt);
    if (!leer_linea(line, sizeof line)) return 0;
    if (line[0] == '\0') return 0;
    *out = line[0];
    return 1;
}

// ======================= Funciones auxiliares (busquedas/estadísticas) =======================

static const Persona* buscar_por_id(const Personas *v, const char *id) {
    if (!v || !v->data) return NULL;
    for (size_t i = 0; i < v->len; ++i) {
        if (strcmp(v->data[i].id, id) == 0) return &v->data[i];
    }
    return NULL;
}

static void mostrar_resumen_todas(const Personas *v, size_t limite) {
    if (!v || v->len == 0) { printf("No hay datos.\n"); return; }
    size_t to = v->len;
    if (limite > 0 && limite < v->len) to = limite;
    for (size_t i = 0; i < to; ++i) persona_resumen(&v->data[i]);
    if (to < v->len) printf("... (%zu de %zu mostradas)\n", to, v->len);
}

static void mostrar_detalle_por_indice(const Personas *v, size_t idx) {
    if (!v || idx >= v->len) { printf("Índice fuera de rango.\n"); return; }
    persona_imprimir(&v->data[idx]);
}

static void registrar_estadistica(size_t N, double secs, size_t rss_kb) {
    if (STATS_LEN >= MAX_STATS) return;
    STATS[STATS_LEN].N = N;
    STATS[STATS_LEN].gen_seconds = secs;
    STATS[STATS_LEN].rss_kb_after_gen = rss_kb;
    STATS[STATS_LEN].when = time(NULL);
    STATS_LEN++;
}

static void imprimir_estadisticas() {
    if (STATS_LEN == 0) {
        printf("No hay estadísticas registradas aún.\n");
        return;
    }
    printf("\n===== Estadísticas de generaciones =====\n");
    for (size_t i = 0; i < STATS_LEN; ++i) {
        struct tm *tmv = localtime(&STATS[i].when);
        char buf[64]; buf[0] = 0;
        if (tmv) strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", tmv);
        printf("%2zu) N=%zu | t=%.3f s | RSS~%zu KB | %s\n",
               i+1, STATS[i].N, STATS[i].gen_seconds, STATS[i].rss_kb_after_gen, buf);
    }
}

static int exportar_estadisticas_csv(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fprintf(f, "index,N,seconds,rss_kb,timestamp\n");
    for (size_t i = 0; i < STATS_LEN; ++i) {
        fprintf(f, "%zu,%zu,%.6f,%zu,%ld\n",
                i+1, STATS[i].N, STATS[i].gen_seconds, STATS[i].rss_kb_after_gen,
                (long)STATS[i].when);
    }
    fclose(f);
    return 1;
}

static const Persona* mas_endeudado(const Personas *v) {
    if (!v || v->len == 0) return NULL;
    const Persona *best = &v->data[0];
    for (size_t i = 1; i < v->len; ++i) {
        if (v->data[i].deudas > best->deudas) best = &v->data[i];
    }
    return best;
}

static const Persona* nombre_mas_largo(const Personas *v) {
    if (!v || v->len == 0) return NULL;
    const Persona *best = &v->data[0];
    size_t best_len = strlen(best->nombre) + 1 + strlen(best->apellido);
    for (size_t i = 1; i < v->len; ++i) {
        size_t len = strlen(v->data[i].nombre) + 1 + strlen(v->data[i].apellido);
        if (len > best_len) { best = &v->data[i]; best_len = len; }
    }
    return best;
}

// Top-3 ciudades por patrimonio neto promedio
typedef struct {
    char ciudad[MAX_CIUDAD];
    double sum_neto;
    size_t count;
} AggCiudad;

static int cmp_top3(const void *a, const void *b) {
    const AggCiudad *A = (const AggCiudad*)a;
    const AggCiudad *B = (const AggCiudad*)b;
    double avgA = (A->count ? (A->sum_neto / A->count) : 0.0);
    double avgB = (B->count ? (B->sum_neto / B->count) : 0.0);
    if (avgA < avgB) return 1;
    if (avgA > avgB) return -1;
    return strcmp(A->ciudad, B->ciudad);
}

static void top3_ciudades_patrimonio_prom(const Personas *v) {
    if (!v || v->len == 0) { printf("No hay datos.\n"); return; }

    // construir lista de ciudades únicas con sumas
    AggCiudad *arr = calloc(v->len, sizeof(AggCiudad));
    size_t used = 0;

    for (size_t i = 0; i < v->len; ++i) {
        const char *c = v->data[i].ciudad;
        size_t j;
        for (j = 0; j < used; ++j) {
            if (strcmp(arr[j].ciudad, c) == 0) break;
        }
        if (j == used) { // nueva ciudad
            snprintf(arr[used].ciudad, sizeof arr[used].ciudad, "%s", c);
            arr[used].sum_neto = 0.0;
            arr[used].count = 0;
            used++;
        }
        double neto = persona_patrimonio_neto(&v->data[i]);
        arr[j].sum_neto += neto;
        arr[j].count += 1;
    }

    qsort(arr, used, sizeof(AggCiudad), cmp_top3);

    printf("\nTop 3 ciudades por patrimonio neto promedio:\n");
    size_t top = used < 3 ? used : 3;
    for (size_t i = 0; i < top; ++i) {
        double avg = arr[i].count ? (arr[i].sum_neto / arr[i].count) : 0.0;
        printf("%zu) %-15s  n=%zu  promedio_neto=%.2f\n", i+1, arr[i].ciudad, arr[i].count, avg);
    }
    if (used == 0) printf("(sin ciudades)\n");

    free(arr);
}

// ======================= Operaciones del Menú =======================

static void opcion_1_crear_dataset() {
    long N;
    if (!leer_entero("Ingrese N (cantidad de personas): ", &N) || N <= 0) {
        printf("Valor inválido.\n");
        return;
    }

    // liberar dataset previo
    liberar_personas(&G);

    double t0 = now_seconds();
    G = generar_personas((size_t)N);
    double t1 = now_seconds();
    size_t rss = memoria_rss_kb();

    printf("Generadas %zu personas en %.3f s | RSS ~ %zu KB\n", G.len, t1 - t0, rss);
    registrar_estadistica((size_t)N, t1 - t0, rss);
}

static void opcion_2_mostrar_resumen() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    long L = 0;
    if (!leer_entero("¿Cuántas mostrar? (0 = todas): ", &L)) L = 20;
    mostrar_resumen_todas(&G, (size_t)L);
}

static void opcion_3_detalle_por_indice() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    long idx;
    if (!leer_entero("Índice (0..N-1): ", &idx) || idx < 0 || (size_t)idx >= G.len) {
        printf("Índice inválido.\n"); return;
    }
    mostrar_detalle_por_indice(&G, (size_t)idx);
}

static void opcion_4_buscar_por_id() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    char id[MAX_ID];
    printf("Ingrese ID a buscar: ");
    if (!leer_linea(id, sizeof id)) { printf("Entrada inválida.\n"); return; }
    const Persona *p = buscar_por_id(&G, id);
    imprimir_persona_linea("Resultado", p);
}

static void opcion_5_estadisticas() {
    imprimir_estadisticas();
}

static void opcion_6_exportar_csv() {
    if (STATS_LEN == 0) { printf("No hay estadísticas para exportar.\n"); return; }
    char fname[128];
    printf("Nombre de archivo CSV (default: estadisticas.csv): ");
    if (!leer_linea(fname, sizeof fname) || fname[0] == '\0') {
        snprintf(fname, sizeof fname, "estadisticas.csv");
    }
    if (exportar_estadisticas_csv(fname)) {
        printf("Estadísticas exportadas a %s\n", fname);
    } else {
        printf("No se pudo escribir %s\n", fname);
    }
}

static void opcion_7_longeva_pais() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    imprimir_persona_linea("👵 Más longeva (país)", persona_mas_longeva(&G));
}

static void opcion_8_longeva_ciudad() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    char ciudad[MAX_CIUDAD];
    printf("Ciudad: ");
    if (!leer_linea(ciudad, sizeof ciudad)) { printf("Entrada inválida.\n"); return; }
    imprimir_persona_linea("👵 Más longeva (ciudad)", persona_mas_longeva_en_ciudad(&G, ciudad));
}

static void opcion_9_mayor_patrimonio_pais() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    imprimir_persona_linea("💰 Mayor patrimonio (país)", mayor_patrimonio(&G));
}

static void opcion_10_mayor_patrimonio_ciudad() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    char ciudad[MAX_CIUDAD];
    printf("Ciudad: ");
    if (!leer_linea(ciudad, sizeof ciudad)) { printf("Entrada inválida.\n"); return; }
    imprimir_persona_linea("💰 Mayor patrimonio (ciudad)", mayor_patrimonio_en_ciudad(&G, ciudad));
}

static void opcion_11_mayor_patrimonio_cal() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    char cal;
    if (!leer_char("Calendario (A/B/C): ", &cal)) { printf("Entrada inválida.\n"); return; }
    imprimir_persona_linea("💰 Mayor patrimonio (calendario)", mayor_patrimonio_por_cal(&G, cal));
}

static void opcion_12_conteo_calendario() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    size_t a,b,c;
    contar_por_calendario(&G, &a, &b, &c);
    printf("📅 Conteo por calendario -> A:%zu  B:%zu  C:%zu\n", a,b,c);
}

static void opcion_13_top3_ciudades_prom() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    top3_ciudades_patrimonio_prom(&G);
}

static void opcion_14_mas_endeudado() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    imprimir_persona_linea("💸 Persona con más deudas", mas_endeudado(&G));
}

static void opcion_15_nombre_mas_largo() {
    if (!G.data) { printf("Primero genere el dataset (opción 1).\n"); return; }
    imprimir_persona_linea("🔤 Nombre completo más largo", nombre_mas_largo(&G));
}

// ======================= Menú =======================

static void mostrar_menu() {
    printf("\n================= MENÚ =================\n");
    printf(" 1. Crear nuevo conjunto de datos\n");
    printf(" 2. Mostrar resumen de personas\n");
    printf(" 3. Mostrar detalle por índice\n");
    printf(" 4. Buscar persona por ID\n");
    printf(" 5. Mostrar estadísticas de rendimiento\n");
    printf(" 6. Exportar estadísticas a CSV\n");
    printf(" 7. Persona más longeva (país)\n");
    printf(" 8. Persona más longeva por ciudad\n");
    printf(" 9. Mayor patrimonio (país)\n");
    printf("10. Mayor patrimonio por ciudad\n");
    printf("11. Mayor patrimonio por calendario (A/B/C)\n");
    printf("12. Conteo por calendario\n");
    printf("13. Top 3 ciudades por patrimonio neto promedio\n");
    printf("14. Persona con más deudas\n");
    printf("15. Persona con el nombre más largo\n");
    printf(" 0. Salir\n");
    printf("Seleccione una opción: ");
}

int main(int argc, char **argv) {
    // Arranque opcional con N inicial si viene por argv
    if (argc >= 2) {
        size_t N = (size_t)atoll(argv[1]);
        double t0 = now_seconds();
        G = generar_personas(N);
        double t1 = now_seconds();
        size_t rss = memoria_rss_kb();
        printf("Generadas %zu personas en %.3f s | RSS ~ %zu KB\n", G.len, t1 - t0, rss);
        registrar_estadistica(N, t1 - t0, rss);
    }

    for (;;) {
        mostrar_menu();

        char line[MAX_LINE];
        if (!leer_linea(line, sizeof line)) break;
        long opt = strtol(line, NULL, 10);

        switch (opt) {
            case 1: opcion_1_crear_dataset(); break;
            case 2: opcion_2_mostrar_resumen(); break;
            case 3: opcion_3_detalle_por_indice(); break;
            case 4: opcion_4_buscar_por_id(); break;
            case 5: opcion_5_estadisticas(); break;
            case 6: opcion_6_exportar_csv(); break;
            case 7: opcion_7_longeva_pais(); break;
            case 8: opcion_8_longeva_ciudad(); break;
            case 9: opcion_9_mayor_patrimonio_pais(); break;
            case 10: opcion_10_mayor_patrimonio_ciudad(); break;
            case 11: opcion_11_mayor_patrimonio_cal(); break;
            case 12: opcion_12_conteo_calendario(); break;
            case 13: opcion_13_top3_ciudades_prom(); break;
            case 14: opcion_14_mas_endeudado(); break;
            case 15: opcion_15_nombre_mas_largo(); break;
            case 0:
                liberar_personas(&G);
                printf("Saliendo...\n");
                return 0;
            default:
                printf("Opción no válida.\n");
        }

        pausar();
    }

    liberar_personas(&G);
    return 0;
}
