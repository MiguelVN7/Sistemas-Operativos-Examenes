#ifndef GENERADOR_H
#define GENERADOR_H

#include "persona.h"
#include <vector>
#include <map>

// Funciones para generación de datos aleatorios

/**
 * Genera una fecha de nacimiento aleatoria entre 1960 y 2010.
 * 
 * POR QUÉ: Simular fechas realistas para personas.
 * CÓMO: Combinando números aleatorios para día, mes y año.
 * PARA QUÉ: Inicializar el atributo fechaNacimiento de Persona.
 */
std::string generarFechaNacimiento();

/**
 * Genera un ID único secuencial.
 * 
 * POR QUÉ: Necesidad de identificadores únicos para cada persona.
 * CÓMO: Usando un contador estático que incrementa en cada llamada.
 * PARA QUÉ: Garantizar unicidad en los IDs.
 */
std::string generarID();

/**
 * Genera un número decimal aleatorio en un rango [min, max].
 * 
 * POR QUÉ: Necesidad de valores realistas para ingresos, patrimonio, etc.
 * CÓMO: Usando un generador Mersenne Twister y una distribución uniforme.
 * PARA QUÉ: Producir valores financieros aleatorios pero dentro de rangos lógicos.
 */
double randomDouble(double min, double max);

/**
 * Crea una persona con datos aleatorios.
 * 
 * POR QUÉ: Automatizar la creación de registros de personas.
 * CÓMO: Combinando las funciones generadoras y bases de datos de nombres, apellidos, etc.
 * PARA QUÉ: Poblar el sistema con datos de prueba.
 */
Persona generarPersona();

/**
 * Genera una colección (vector) de n personas.
 * 
 * POR QUÉ: Crear conjuntos de datos de diferentes tamaños.
 * CÓMO: Llamando a generarPersona() n veces.
 * PARA QUÉ: Pruebas de rendimiento y funcionalidad con volúmenes variables.
 */
std::vector<Persona> generarColeccion(int n);

/**
 * Busca una persona por ID en un vector de personas.
 * 
 * POR QUÉ: Recuperar una persona específica de una colección.
 * CÓMO: Usando un algoritmo de búsqueda lineal (o binaria si se ordena).
 * PARA QUÉ: Implementar funcionalidad de búsqueda en la aplicación.
 * 
 * @param personas Vector de personas donde buscar.
 * @param id ID a buscar.
 * @return Puntero a la persona encontrada o nullptr si no se encuentra.
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id);

/**
 * Busca la persona más mayor (longeva) en un vector de personas.
 * 
 * POR QUÉ: Encontrar la persona con la fecha de nacimiento más antigua.
 * CÓMO: Comparando las fechas de nacimiento de todas las personas.
 * PARA QUÉ: Análisis demográfico y estadísticas de edad.
 * 
 * @param personas Vector de personas donde buscar.
 * @return Puntero a la persona más mayor o nullptr si el vector está vacío.
 */
const Persona* buscarLongeva(const std::vector<Persona>& personas);

const Persona* buscarLongevaCiudad (const std::vector<Persona>& personas);

std::map<std::string, const Persona*> buscarLongevaPorCiudad(const std::vector<Persona>& personas);

const Persona* buscarPatrimonio (const std::vector<Persona>& personas);

std::map<std::string, const Persona*> buscarPatrimonioPorCiudad(const std::vector<Persona>& personas);

std::map<char, const Persona*> buscarPatrimonioPorCalendario(const std::vector<Persona>& personas); 

void listarPersonasCalendario (const std::vector<Persona>& personas);


// PREGUNTAS OPCIONALES

void top3CiudadesPatrimonio(const std::vector<Persona>& personas);

const Persona* buscarDeudas (const std::vector<Persona>& personas);

const Persona* buscarNombreMasLargo (const std::vector<Persona>& personas);

#endif // GENERADOR_H