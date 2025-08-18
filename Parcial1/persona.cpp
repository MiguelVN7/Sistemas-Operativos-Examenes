#include "persona.h"
#include <iomanip> // Para std::setprecision
#include <sstream>

/**
 * Implementación del constructor de Persona.
 * 
 * POR QUÉ: Inicializar los miembros de la clase.
 * CÓMO: Usando la lista de inicialización y moviendo los strings para evitar copias.
 * PARA QUÉ: Eficiencia y correcta construcción del objeto.
 */
Persona::Persona(std::string nom, std::string ape, std::string id, 
                 std::string ciudad, std::string fecha, double ingresos, 
                 double patri, double deud, bool declara)
    : nombre(std::move(nom)), 
      apellido(std::move(ape)), 
      id(std::move(id)), 
      ciudadNacimiento(std::move(ciudad)),
      fechaNacimiento(std::move(fecha)), 
      ingresosAnuales(ingresos), 
      patrimonio(patri),
      deudas(deud), 
      declaranteRenta(declara),
      calendarioTributario(calcularCalendarioTributario()) {}

/**
 * Implementación de mostrar.
 * 
 * POR QUÉ: Mostrar todos los datos de la persona de forma estructurada.
 * CÓMO: Usando flujos de salida y formateadores.
 * PARA QUÉ: Facilitar la lectura de los datos completos de una persona.
 */
void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";
    std::cout << std::fixed << std::setprecision(2); // Formato de números
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
    std::cout << "   - Calendario Tributario: " << calendarioTributario << "\n"; // Según los 2 últimos digitos de la cédulo: A de 00 a 39; B de 40 a 79; y C de 80 a 99

}

/**
 * Implementación de mostrarResumen.
 * 
 * POR QUÉ: Mostrar información esencial de la persona en una sola línea.
 * CÓMO: Imprime ID, nombre completo, ciudad e ingresos.
 * PARA QUÉ: Listados rápidos y eficientes.
 */
void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento 
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales;
}

char Persona::calcularCalendarioTributario() const {
    char calendario = 'A'; 
   
    if (id.length() >= 2) {
        std::string ultDosDigs = id.substr(id.length() - 2, 2);
        int numeroCal = std::stoi(ultDosDigs);
        
        if (numeroCal < 40) {
            calendario = 'A';
        }
        else if (numeroCal >= 40 && numeroCal < 80) {
            calendario = 'B';
        }
        else if (numeroCal >= 80 && numeroCal <= 99) {
            calendario = 'C';
        }
        
    }
    
    return calendario;
}



void Persona::obtenerFechaNacimiento(int& dia, int& mes, int& anio) const
{
    // Crear un stringstream con la fecha completa (formato: DD/MM/AAAA)
    std::stringstream ss(fechaNacimiento);
    std::string token;

    // Extraer el día (primer token antes del primer '/')
    std::getline(ss, token, '/');
    dia = std::stoi(token);

    // Extraer el mes (segundo token entre las dos '/')
    std::getline(ss, token, '/');
    mes = std::stoi(token);

    // Extraer el año (tercer token después del segundo '/')
    std::getline(ss, token, '/');
    anio = std::stoi(token);
}