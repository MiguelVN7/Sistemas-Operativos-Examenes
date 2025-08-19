#include "generador.h"
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <random>    // std::mt19937, std::uniform_real_distribution
#include <vector>
#include <algorithm> // std::find_if, std::sort
#include <map>
#include <iostream>  // std::cout
#include <iomanip>   // std::setprecision

// Bases de datos para generación realista

// Nombres femeninos comunes en Colombia
const std::vector<std::string> nombresFemeninos = {
    "María", "Luisa", "Carmen", "Ana", "Sofía", "Isabel", "Laura", "Andrea", "Paula", "Valentina",
    "Camila", "Daniela", "Carolina", "Fernanda", "Gabriela", "Patricia", "Claudia", "Diana", "Lucía", "Ximena"
};

// Nombres masculinos comunes en Colombia
const std::vector<std::string> nombresMasculinos = {
    "Juan", "Carlos", "José", "James", "Andrés", "Miguel", "Luis", "Pedro", "Alejandro", "Ricardo",
    "Felipe", "David", "Jorge", "Santiago", "Daniel", "Fernando", "Diego", "Rafael", "Martín", "Óscar",
    "Edison", "Nestor", "Gertridis"
};

// Apellidos comunes en Colombia
const std::vector<std::string> apellidos = {
    "Gómez", "Rodríguez", "Martínez", "López", "García", "Pérez", "González", "Sánchez", "Ramírez", "Torres",
    "Díaz", "Vargas", "Castro", "Ruiz", "Álvarez", "Romero", "Suárez", "Rojas", "Moreno", "Muñoz", "Valencia",
};

// Principales ciudades colombianas
const std::vector<std::string> ciudadesColombia = {
    "Bogotá", "Medellín", "Cali", "Barranquilla", "Cartagena", "Bucaramanga", "Pereira", "Santa Marta", "Cúcuta", "Ibagué",
    "Manizales", "Pasto", "Neiva", "Villavicencio", "Armenia", "Sincelejo", "Valledupar", "Montería", "Popayán", "Tunja"
};

/**
 * Implementación de generarFechaNacimiento.
 * 
 * POR QUÉ: Simular fechas de nacimiento realistas.
 * CÓMO: Día (1-28), mes (1-12), año (1960-2009).
 * PARA QUÉ: Atributo fechaNacimiento de Persona.
 */
std::string generarFechaNacimiento() {
    int dia = 1 + rand() % 28;       // Día: 1 a 28 (evita problemas con meses)
    int mes = 1 + rand() % 12;        // Mes: 1 a 12
    int anio = 1960 + rand() % 50;    // Año: 1960 a 2009
    return std::to_string(dia) + "/" + std::to_string(mes) + "/" + std::to_string(anio);
}

/**
 * Implementación de generarID.
 * 
 * POR QUÉ: Generar identificadores únicos y secuenciales.
 * CÓMO: Contador estático que inicia en 1000000000 y se incrementa.
 * PARA QUÉ: Simular números de cédula.
 */
std::string generarID() {
    static long contador = 1000000000; // Inicia en 1,000,000,000
    return std::to_string(contador++); // Convierte a string e incrementa
}

/**
 * Implementación de randomDouble.
 * 
 * POR QUÉ: Generar números decimales aleatorios en un rango.
 * CÓMO: Mersenne Twister (mejor que rand()) y distribución uniforme.
 * PARA QUÉ: Valores de ingresos, patrimonio, etc.
 */
double randomDouble(double min, double max) {
    static std::mt19937 generator(time(nullptr)); // Semilla basada en tiempo
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

/**
 * Implementación de generarPersona.
 * 
 * POR QUÉ: Crear una persona con datos aleatorios.
 * CÓMO: Seleccionando aleatoriamente de las bases de datos y generando números.
 * PARA QUÉ: Generar datos de prueba.
 */
Persona generarPersona() {
    // Decide si es hombre o mujer
    bool esHombre = rand() % 2;
    
    // Selecciona nombre según género
    std::string nombre = esHombre ? 
        nombresMasculinos[rand() % nombresMasculinos.size()] :
        nombresFemeninos[rand() % nombresFemeninos.size()];
    
    // Construye apellido compuesto (dos apellidos aleatorios)
    std::string apellido = apellidos[rand() % apellidos.size()];
    apellido += " ";
    apellido += apellidos[rand() % apellidos.size()];
    
    // Genera los demás atributos
    std::string id = generarID();
    std::string ciudad = ciudadesColombia[rand() % ciudadesColombia.size()];
    std::string fecha = generarFechaNacimiento();
    
    // Genera datos financieros realistas
    double ingresos = randomDouble(10000000, 500000000);   // 10M a 500M COP
    double patrimonio = randomDouble(0, 2000000000);       // 0 a 2,000M COP
    double deudas = randomDouble(0, patrimonio * 0.7);     // Deudas hasta el 70% del patrimonio
    bool declarante = (ingresos > 50000000) && (rand() % 100 > 30); // Probabilidad 70% si ingresos > 50M
    
    return Persona(nombre, apellido, id, ciudad, fecha, ingresos, patrimonio, deudas, declarante);
}

/**
 * Implementación de generarColeccion.
 * 
 * POR QUÉ: Generar un conjunto de n personas.
 * CÓMO: Reservando espacio y agregando n personas generadas.
 * PARA QUÉ: Crear datasets para pruebas.
 */
std::vector<Persona> generarColeccion(int n) {
    std::vector<Persona> personas;
    personas.reserve(n); // Reserva espacio para n personas (eficiencia)
    
    for (int i = 0; i < n; ++i) {
        personas.push_back(generarPersona());
    }
    
    return personas;
}

/**
 * Implementación de buscarPorID.
 * 
 * POR QUÉ: Encontrar una persona por su ID en una colección.
 * CÓMO: Usando un algoritmo de búsqueda secuencial (lineal).
 * PARA QUÉ: Para operaciones de búsqueda en la aplicación.
 */
const Persona* buscarPorID(const std::vector<Persona>& personas, const std::string& id) {
    // Usa find_if con una lambda para buscar por ID
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.getId() == id; });
    
    if (it != personas.end()) {
        return &(*it); // Devuelve puntero a la persona encontrada
    } else {
        return nullptr; // No encontrado
    }
}

const Persona* buscarLongeva (const std::vector<Persona>& personas)
{
    if (personas.empty()) {
        return nullptr; // Si no hay personas, retornar nullptr
    }
    
    const Persona* personaMayor = &personas[0]; // Empezar con la primera persona
    int diaMayor, mesMayor, anioMayor;
    personaMayor->obtenerFechaNacimiento(diaMayor, mesMayor, anioMayor);
    
    // Recorrer todas las personas para encontrar la más mayor
    for (size_t i = 1; i < personas.size(); ++i) {
        int dia, mes, anio;
        personas[i].obtenerFechaNacimiento(dia, mes, anio);
        
        // Comparar fechas: la persona más mayor tiene fecha de nacimiento más antigua
        if (anio < anioMayor || 
            (anio == anioMayor && mes < mesMayor) ||
            (anio == anioMayor && mes == mesMayor && dia < diaMayor)) {
            
            personaMayor = &personas[i];
            diaMayor = dia;
            mesMayor = mes;
            anioMayor = anio;
        }
    }
    
    return personaMayor;
}


std::map<std::string, const Persona*> buscarLongevaPorCiudad(const std::vector<Persona>& personas) 
{
    std::map<std::string, const Persona*> longevasPorCiudad;
    
    if (personas.empty()) {
        return longevasPorCiudad; 
    }
    
    // Recorrer todas las personas
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        
        // Si es la primera persona de esta ciudad, o si es más longeva que la actual
        if (longevasPorCiudad.find(ciudad) == longevasPorCiudad.end()) {
            // Primera persona de esta ciudad
            longevasPorCiudad[ciudad] = &persona;
        } else {
            // Comparar con la persona más longeva actual de esta ciudad
            const Persona* personaActual = longevasPorCiudad[ciudad];
            
            // Obtener fechas de ambas personas
            int diaActual, mesActual, anioActual;
            int diaNueva, mesNueva, anioNueva;
            
            personaActual->obtenerFechaNacimiento(diaActual, mesActual, anioActual);
            persona.obtenerFechaNacimiento(diaNueva, mesNueva, anioNueva);
            
            // Si la nueva persona es más longeva (fecha más antigua)
            if (anioNueva < anioActual || 
                (anioNueva == anioActual && mesNueva < mesActual) ||
                (anioNueva == anioActual && mesNueva == mesActual && diaNueva < diaActual)) {
                
                longevasPorCiudad[ciudad] = &persona;
            }
        }
    }
    
    return longevasPorCiudad;
}

const Persona* buscarPatrimonio (const std::vector<Persona>& personas)
{
    if (personas.empty()) {
        return nullptr; // Si no hay personas, retornar nullptr
    }

    const Persona* masRico = &personas[0];
    double patrimonioMayor = masRico->getPatrimonio();
    
    for (size_t i = 1; i < personas.size(); ++i)
    {
        double patrimonioActual = personas[i].getPatrimonio();
        if (patrimonioActual > patrimonioMayor)
        {
            masRico = &personas[i];
            patrimonioMayor = patrimonioActual;
        }
    }
    return masRico;
}

std::map<std::string, const Persona*> buscarPatrimonioPorCiudad(const std::vector<Persona>& personas) 
{
    std::map<std::string, const Persona*> patrimonioPorCiudad;
    
    if (personas.empty()) {
        return patrimonioPorCiudad;
    }

    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        
        // Si es la primera persona de esta ciudad, o si es más rica que la actual
        if (patrimonioPorCiudad.find(ciudad) == patrimonioPorCiudad.end()) {
            // Primera persona de esta ciudad
            patrimonioPorCiudad[ciudad] = &persona;
        } else {
            // Comparar con la persona más rica actual de esta ciudad
            const Persona* personaActual = patrimonioPorCiudad[ciudad];
            
            double patrimonioActual = personaActual->getPatrimonio();
            double patrimonioNuevo = persona.getPatrimonio();

            // Si la nueva persona es más rica
            if (patrimonioNuevo > patrimonioActual) {
                patrimonioPorCiudad[ciudad] = &persona;
            }
        }
    }

    return patrimonioPorCiudad;
}

std::map<char, const Persona*> buscarPatrimonioPorCalendario(const std::vector<Persona>& personas) 
{
    std::map<char, const Persona*> patrimonioPorCalendario;
    
    if (personas.empty()) {
        return patrimonioPorCalendario;
    }

    for (const auto& persona : personas) {
        char calendario = persona.getCalendarioTributario();
        
        // Si es la primera persona de este calendario, o si es más rica que la actual
        if (patrimonioPorCalendario.find(calendario) == patrimonioPorCalendario.end()) {
            // Primera persona de este calendario
            patrimonioPorCalendario[calendario] = &persona;
        } else {
            // Comparar con la persona más rica actual de este calendario
            const Persona* personaActual = patrimonioPorCalendario[calendario];
            
            double patrimonioActual = personaActual->getPatrimonio();
            double patrimonioNuevo = persona.getPatrimonio();

            // Si la nueva persona es más rica
            if (patrimonioNuevo > patrimonioActual) {
                patrimonioPorCalendario[calendario] = &persona;
            }
        }
    }

    return patrimonioPorCalendario;
}

void listarPersonasCalendario(const std::vector<Persona>& personas)
{
    if (personas.empty()) {
        std::cout << "\nNo hay personas para mostrar.\n";
        return;
    }

    // Vectores para almacenar personas por calendario
    std::vector<const Persona*> calendarioA;
    std::vector<const Persona*> calendarioB;
    std::vector<const Persona*> calendarioC;

    // Clasificar personas por calendario (una sola pasada)
    for (const auto& persona : personas) {
        char calendario = persona.getCalendarioTributario();
        switch (calendario) {
            case 'A':
                if (persona.getDeclaranteRenta())
                {
                    calendarioA.push_back(&persona);
                }
                break;
            case 'B':
                if (persona.getDeclaranteRenta())
                {
                    calendarioB.push_back(&persona);
                }
                break;
            case 'C':
                if (persona.getDeclaranteRenta())
                {
                calendarioC.push_back(&persona);
                }
                break;
        }
    }


    // Mostrar personas del calendario A
    if (!calendarioA.empty()) {
        std::cout << "CALENDARIO A (00-39):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto* persona : calendarioA) {
            persona->mostrarResumen();
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Mostrar personas del calendario B
    if (!calendarioB.empty()) {
        std::cout << "CALENDARIO B (40-79):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto* persona : calendarioB) {
            persona->mostrarResumen();
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Mostrar personas del calendario C
    if (!calendarioC.empty()) {
        std::cout << "CALENDARIO C (80-99):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto* persona : calendarioC) {
            persona->mostrarResumen();
            std::cout << "\n";
        }
    }

    // Mostrar resumen
    std::cout << "\n=== RESUMEN POR CALENDARIO TRIBUTARIO QUE DECLARAN ===\n";
    std::cout << "Total personas calendario A: " << calendarioA.size() << "\n";
    std::cout << "Total personas calendario B: " << calendarioB.size() << "\n";
    std::cout << "Total personas calendario C: " << calendarioC.size() << "\n\n";
}


// PREGUNTAS OPCIONALES


// Tres ciudades con mayor patrimonio promedio
void top3CiudadesPatrimonio(const std::vector<Persona>& personas)
{
    if (personas.empty()) {
        std::cout << "\nNo hay personas para analizar.\n";
        return;
    }

    // Estructura para organizar información estadística por ciudad
    struct DatosCiudad {
        std::string nombre;           // Nombre de la ciudad
        double patrimonioTotal;       // Suma de todos los patrimonios en la ciudad
        int numeroPersonas;           // Contador de personas en la ciudad  
        double patrimonioPromedio;    // Valor calculado: patrimonioTotal / numeroPersonas
    };

    // Contenedor asociativo para agrupar automáticamente por ciudad
    // La clave es el nombre de la ciudad, el valor es la estructura de datos
    std::map<std::string, DatosCiudad> ciudades;

    // FASE 1: Recorrido único de los datos para acumular estadísticas por ciudad
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        double patrimonio = persona.getPatrimonio();
        
        // Verificar si es la primera persona de esta ciudad
        if (ciudades.find(ciudad) == ciudades.end()) {
            // Inicializar entrada nueva: primera persona de esta ciudad
            ciudades[ciudad] = {ciudad, patrimonio, 1, 0.0};
        } else {
            // Acumular datos: agregar patrimonio y incrementar contador
            ciudades[ciudad].patrimonioTotal += patrimonio;
            ciudades[ciudad].numeroPersonas++;
        }
    }

    // FASE 2: Calcular promedios y preparar para ordenamiento
    // Transferir del mapa a vector para permitir ordenamiento eficiente
    std::vector<DatosCiudad> listaCiudades;
    listaCiudades.reserve(ciudades.size()); // Reservar memoria para eficiencia
    
    for (auto& [nombreCiudad, datos] : ciudades) {
        // Calcular promedio: total acumulado / número de personas
        datos.patrimonioPromedio = datos.patrimonioTotal / datos.numeroPersonas;
        listaCiudades.push_back(datos);
    }

    // FASE 3: Ordenamiento descendente por patrimonio promedio
    // Utiliza algoritmo de ordenamiento optimizado (puede ser QuickSort o IntroSort)
    std::sort(listaCiudades.begin(), listaCiudades.end(),
        [](const DatosCiudad& a, const DatosCiudad& b) {
            // Función lambda para comparación: ordenar de mayor a menor promedio
            return a.patrimonioPromedio > b.patrimonioPromedio;
        });

    // FASE 4: Presentación de resultados con formato profesional
    std::cout << "\nTOP 3 CIUDADES CON MAYOR PATRIMONIO PROMEDIO\n";
    std::cout << "=" << std::string(65, '=') << "\n\n";

    // Determinar cuántas ciudades mostrar (máximo 3, o menos si hay pocas ciudades)
    int limite = std::min(3, static_cast<int>(listaCiudades.size()));
    
    // Iterar sobre las ciudades con mayor patrimonio promedio
    for (int i = 0; i < limite; i++) {
        const auto& ciudad = listaCiudades[i];
        
        // Mostrar información detallada de cada ciudad
        std::cout << " #" << (i + 1) << " - " << ciudad.nombre << "\n";
        std::cout << "    Patrimonio Promedio: $" << std::fixed << std::setprecision(2) 
                  << ciudad.patrimonioPromedio << " COP\n";
        std::cout << "    Personas en la ciudad: " << ciudad.numeroPersonas << "\n";
        std::cout << "    Patrimonio Total: $" << std::fixed << std::setprecision(2) 
                  << ciudad.patrimonioTotal << " COP\n";
        
        // Separador visual entre ciudades (excepto para la última)
        if (i < limite - 1) {
            std::cout << "   " << std::string(50, '-') << "\n";
        }
        std::cout << "\n";
    }
}


// Persona con más deudas
const Persona* buscarDeudas (const std::vector<Persona>& personas)
{
    if (personas.empty()) 
    {
        return nullptr; // Si no hay personas, retornar nullptr
    }

    const Persona* personaEndeudada = &personas[0];
    double deuda = personaEndeudada->getDeudas();

    for (size_t i = 1; i < personas.size(); i++)
    {
        double deudaActual = personas[i].getDeudas();
        if (deudaActual > deuda)
        {
            personaEndeudada = &personas[i];
            deuda = deudaActual;
        }
    }

    return personaEndeudada;
}


// Persona con el nombre y apellido más largo de todos
const Persona* buscarNombreMasLargo (const std::vector<Persona>& personas)
{
    if (personas.empty()) 
    {
        return nullptr; // Si no hay personas, retornar nullptr
    }

    const Persona* personaNombreLargo = &personas[0];
    int lonNombre = personaNombreLargo->getNombre().size();
    int lonApellido = personaNombreLargo->getApellido().size();
    int tamano = lonNombre + lonApellido;

    for (size_t i = 1; i < personas.size(); i++)
    {
        int lonNombreActual = personas[i].getNombre().size();
        int lonApellidoActual = personas[i].getApellido().size();
        int tamanoActual = lonNombreActual + lonApellidoActual;

        if (tamanoActual > tamano)
        {
            personaNombreLargo = &personas[i];
            tamano = tamanoActual;
        }
    }

    return personaNombreLargo;
}

// ============= FUNCIONES CON PASO POR VALOR =============

/**
 * Implementación de buscarPorIDValor.
 * 
 * POR QUÉ: Encontrar una persona por su ID usando paso por valor.
 * CÓMO: Usando un algoritmo de búsqueda secuencial (lineal).
 * PARA QUÉ: Para operaciones de búsqueda en la aplicación con paso por valor.
 */
Persona buscarPorIDValor(std::vector<Persona> personas, std::string id) {
    // Usa find_if con una lambda para buscar por ID
    auto it = std::find_if(personas.begin(), personas.end(),
        [&id](const Persona& p) { return p.getId() == id; });
    
    if (it != personas.end()) {
        return *it; // Devuelve una copia de la persona encontrada
    } else {
        return Persona(); // Retorna persona vacía si no se encuentra
    }
}

/**
 * Implementación de buscarLongevaValor.
 * 
 * POR QUÉ: Encontrar la persona más longeva usando paso por valor.
 * CÓMO: Comparando fechas de nacimiento de todas las personas.
 * PARA QUÉ: Para análisis demográfico con paso por valor.
 */
Persona buscarLongevaValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        return Persona(); // Si no hay personas, retornar persona vacía
    }
    
    Persona personaMayor = personas[0]; // Empezar con la primera persona
    int diaMayor, mesMayor, anioMayor;
    personaMayor.obtenerFechaNacimiento(diaMayor, mesMayor, anioMayor);
    
    // Recorrer todas las personas para encontrar la más mayor
    for (size_t i = 1; i < personas.size(); ++i) {
        int dia, mes, anio;
        personas[i].obtenerFechaNacimiento(dia, mes, anio);
        
        // Comparar fechas: la persona más mayor tiene fecha de nacimiento más antigua
        if (anio < anioMayor || 
            (anio == anioMayor && mes < mesMayor) ||
            (anio == anioMayor && mes == mesMayor && dia < diaMayor)) {
            
            personaMayor = personas[i];
            diaMayor = dia;
            mesMayor = mes;
            anioMayor = anio;
        }
    }
    
    return personaMayor;
}

/**
 * Implementación de buscarLongevaPorCiudadValor.
 * 
 * POR QUÉ: Encontrar la persona más longeva de cada ciudad usando paso por valor.
 * CÓMO: Agrupando por ciudad y comparando fechas de nacimiento.
 * PARA QUÉ: Para análisis demográfico por ciudad con paso por valor.
 */
std::map<std::string, Persona> buscarLongevaPorCiudadValor(std::vector<Persona> personas) {
    std::map<std::string, Persona> longevasPorCiudad;
    
    if (personas.empty()) {
        return longevasPorCiudad; 
    }
    
    // Recorrer todas las personas
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        
        // Si es la primera persona de esta ciudad, o si es más longeva que la actual
        if (longevasPorCiudad.find(ciudad) == longevasPorCiudad.end()) {
            // Primera persona de esta ciudad
            longevasPorCiudad[ciudad] = persona;
        } else {
            // Comparar con la persona más longeva actual de esta ciudad
            Persona personaActual = longevasPorCiudad[ciudad];
            
            // Obtener fechas de ambas personas
            int diaActual, mesActual, anioActual;
            int diaNueva, mesNueva, anioNueva;
            
            personaActual.obtenerFechaNacimiento(diaActual, mesActual, anioActual);
            persona.obtenerFechaNacimiento(diaNueva, mesNueva, anioNueva);
            
            // Si la nueva persona es más longeva (fecha más antigua)
            if (anioNueva < anioActual || 
                (anioNueva == anioActual && mesNueva < mesActual) ||
                (anioNueva == anioActual && mesNueva == mesActual && diaNueva < diaActual)) {
                
                longevasPorCiudad[ciudad] = persona;
            }
        }
    }
    
    return longevasPorCiudad;
}

/**
 * Implementación de buscarPatrimonioValor.
 * 
 * POR QUÉ: Encontrar la persona con mayor patrimonio usando paso por valor.
 * CÓMO: Comparando patrimonio de todas las personas.
 * PARA QUÉ: Para análisis financiero con paso por valor.
 */
Persona buscarPatrimonioValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        return Persona(); // Si no hay personas, retornar persona vacía
    }

    Persona masRico = personas[0];
    double patrimonioMayor = masRico.getPatrimonio();
    
    for (size_t i = 1; i < personas.size(); ++i) {
        double patrimonioActual = personas[i].getPatrimonio();
        if (patrimonioActual > patrimonioMayor) {
            masRico = personas[i];
            patrimonioMayor = patrimonioActual;
        }
    }
    return masRico;
}

/**
 * Implementación de buscarPatrimonioPorCiudadValor.
 * 
 * POR QUÉ: Encontrar la persona con mayor patrimonio de cada ciudad usando paso por valor.
 * CÓMO: Agrupando por ciudad y comparando patrimonio.
 * PARA QUÉ: Para análisis financiero por ciudad con paso por valor.
 */
std::map<std::string, Persona> buscarPatrimonioPorCiudadValor(std::vector<Persona> personas) {
    std::map<std::string, Persona> patrimonioPorCiudad;
    
    if (personas.empty()) {
        return patrimonioPorCiudad;
    }

    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        
        // Si es la primera persona de esta ciudad, o si es más rica que la actual
        if (patrimonioPorCiudad.find(ciudad) == patrimonioPorCiudad.end()) {
            // Primera persona de esta ciudad
            patrimonioPorCiudad[ciudad] = persona;
        } else {
            // Comparar con la persona más rica actual de esta ciudad
            Persona personaActual = patrimonioPorCiudad[ciudad];
            
            double patrimonioActual = personaActual.getPatrimonio();
            double patrimonioNuevo = persona.getPatrimonio();

            // Si la nueva persona es más rica
            if (patrimonioNuevo > patrimonioActual) {
                patrimonioPorCiudad[ciudad] = persona;
            }
        }
    }

    return patrimonioPorCiudad;
}

/**
 * Implementación de buscarPatrimonioPorCalendarioValor.
 * 
 * POR QUÉ: Encontrar la persona con mayor patrimonio de cada calendario usando paso por valor.
 * CÓMO: Agrupando por calendario tributario y comparando patrimonio.
 * PARA QUÉ: Para análisis financiero por calendario con paso por valor.
 */
std::map<char, Persona> buscarPatrimonioPorCalendarioValor(std::vector<Persona> personas) {
    std::map<char, Persona> patrimonioPorCalendario;
    
    if (personas.empty()) {
        return patrimonioPorCalendario;
    }

    for (const auto& persona : personas) {
        char calendario = persona.getCalendarioTributario();
        
        // Si es la primera persona de este calendario, o si es más rica que la actual
        if (patrimonioPorCalendario.find(calendario) == patrimonioPorCalendario.end()) {
            // Primera persona de este calendario
            patrimonioPorCalendario[calendario] = persona;
        } else {
            // Comparar con la persona más rica actual de este calendario
            Persona personaActual = patrimonioPorCalendario[calendario];
            
            double patrimonioActual = personaActual.getPatrimonio();
            double patrimonioNuevo = persona.getPatrimonio();

            // Si la nueva persona es más rica
            if (patrimonioNuevo > patrimonioActual) {
                patrimonioPorCalendario[calendario] = persona;
            }
        }
    }

    return patrimonioPorCalendario;
}

/**
 * Implementación de listarPersonasCalendarioValor.
 * 
 * POR QUÉ: Listar personas por calendario tributario usando paso por valor.
 * CÓMO: Agrupando personas por calendario y mostrando solo declarantes.
 * PARA QUÉ: Para análisis tributario con paso por valor.
 */
void listarPersonasCalendarioValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay personas para mostrar.\n";
        return;
    }

    // Vectores para almacenar personas por calendario
    std::vector<Persona> calendarioA;
    std::vector<Persona> calendarioB;
    std::vector<Persona> calendarioC;

    // Clasificar personas por calendario (una sola pasada)
    for (const auto& persona : personas) {
        char calendario = persona.getCalendarioTributario();
        switch (calendario) {
            case 'A':
                if (persona.getDeclaranteRenta()) {
                    calendarioA.push_back(persona);
                }
                break;
            case 'B':
                if (persona.getDeclaranteRenta()) {
                    calendarioB.push_back(persona);
                }
                break;
            case 'C':
                if (persona.getDeclaranteRenta()) {
                    calendarioC.push_back(persona);
                }
                break;
        }
    }

    // Mostrar personas del calendario A
    if (!calendarioA.empty()) {
        std::cout << "CALENDARIO A (00-39):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto& persona : calendarioA) {
            persona.mostrarResumen();
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Mostrar personas del calendario B
    if (!calendarioB.empty()) {
        std::cout << "CALENDARIO B (40-79):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto& persona : calendarioB) {
            persona.mostrarResumen();
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    // Mostrar personas del calendario C
    if (!calendarioC.empty()) {
        std::cout << "CALENDARIO C (80-99):\n";
        std::cout << "=" << std::string(50, '=') << "\n";
        for (const auto& persona : calendarioC) {
            persona.mostrarResumen();
            std::cout << "\n";
        }
    }

    // Mostrar resumen
    std::cout << "\n=== RESUMEN POR CALENDARIO TRIBUTARIO QUE DECLARAN ===\n";
    std::cout << "Total personas calendario A: " << calendarioA.size() << "\n";
    std::cout << "Total personas calendario B: " << calendarioB.size() << "\n";
    std::cout << "Total personas calendario C: " << calendarioC.size() << "\n\n";
}

/**
 * Implementación de top3CiudadesPatrimonioValor.
 * 
 * POR QUÉ: Encontrar las 3 ciudades con mayor patrimonio promedio usando paso por valor.
 * CÓMO: Agrupando por ciudad, calculando promedios y ordenando.
 * PARA QUÉ: Para análisis financiero por ciudad con paso por valor.
 */
void top3CiudadesPatrimonioValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay personas para analizar.\n";
        return;
    }

    // Estructura para organizar información estadística por ciudad
    struct DatosCiudad {
        std::string nombre;           // Nombre de la ciudad
        double patrimonioTotal;       // Suma de todos los patrimonios en la ciudad
        int numeroPersonas;           // Contador de personas en la ciudad  
        double patrimonioPromedio;    // Valor calculado: patrimonioTotal / numeroPersonas
    };

    // Contenedor asociativo para agrupar automáticamente por ciudad
    // La clave es el nombre de la ciudad, el valor es la estructura de datos
    std::map<std::string, DatosCiudad> ciudades;

    // FASE 1: Recorrido único de los datos para acumular estadísticas por ciudad
    for (const auto& persona : personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        double patrimonio = persona.getPatrimonio();
        
        // Verificar si es la primera persona de esta ciudad
        if (ciudades.find(ciudad) == ciudades.end()) {
            // Inicializar entrada nueva: primera persona de esta ciudad
            ciudades[ciudad] = {ciudad, patrimonio, 1, 0.0};
        } else {
            // Acumular datos: agregar patrimonio y incrementar contador
            ciudades[ciudad].patrimonioTotal += patrimonio;
            ciudades[ciudad].numeroPersonas++;
        }
    }

    // FASE 2: Calcular promedios y preparar para ordenamiento
    // Transferir del mapa a vector para permitir ordenamiento eficiente
    std::vector<DatosCiudad> listaCiudades;
    listaCiudades.reserve(ciudades.size()); // Reservar memoria para eficiencia
    
    for (auto& [nombreCiudad, datos] : ciudades) {
        // Calcular promedio: total acumulado / número de personas
        datos.patrimonioPromedio = datos.patrimonioTotal / datos.numeroPersonas;
        listaCiudades.push_back(datos);
    }

    // FASE 3: Ordenamiento descendente por patrimonio promedio
    // Utiliza algoritmo de ordenamiento optimizado (puede ser QuickSort o IntroSort)
    std::sort(listaCiudades.begin(), listaCiudades.end(),
        [](const DatosCiudad& a, const DatosCiudad& b) {
            // Función lambda para comparación: ordenar de mayor a menor promedio
            return a.patrimonioPromedio > b.patrimonioPromedio;
        });

    // FASE 4: Presentación de resultados con formato profesional
    std::cout << "\nTOP 3 CIUDADES CON MAYOR PATRIMONIO PROMEDIO\n";
    std::cout << "=" << std::string(65, '=') << "\n\n";

    // Determinar cuántas ciudades mostrar (máximo 3, o menos si hay pocas ciudades)
    int limite = std::min(3, static_cast<int>(listaCiudades.size()));
    
    // Iterar sobre las ciudades con mayor patrimonio promedio
    for (int i = 0; i < limite; i++) {
        const auto& ciudad = listaCiudades[i];
        
        // Mostrar información detallada de cada ciudad
        std::cout << " #" << (i + 1) << " - " << ciudad.nombre << "\n";
        std::cout << "    Patrimonio Promedio: $" << std::fixed << std::setprecision(2) 
                  << ciudad.patrimonioPromedio << " COP\n";
        std::cout << "    Personas en la ciudad: " << ciudad.numeroPersonas << "\n";
        std::cout << "    Patrimonio Total: $" << std::fixed << std::setprecision(2) 
                  << ciudad.patrimonioTotal << " COP\n";
        
        // Separador visual entre ciudades (excepto para la última)
        if (i < limite - 1) {
            std::cout << "   " << std::string(50, '-') << "\n";
        }
        std::cout << "\n";
    }
}

/**
 * Implementación de buscarDeudasValor.
 * 
 * POR QUÉ: Encontrar la persona con más deudas usando paso por valor.
 * CÓMO: Comparando deudas de todas las personas.
 * PARA QUÉ: Para análisis financiero con paso por valor.
 */
Persona buscarDeudasValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        return Persona(); // Si no hay personas, retornar persona vacía
    }

    Persona personaEndeudada = personas[0];
    double deuda = personaEndeudada.getDeudas();

    for (size_t i = 1; i < personas.size(); i++) {
        double deudaActual = personas[i].getDeudas();
        if (deudaActual > deuda) {
            personaEndeudada = personas[i];
            deuda = deudaActual;
        }
    }

    return personaEndeudada;
}

/**
 * Implementación de buscarNombreMasLargoValor.
 * 
 * POR QUÉ: Encontrar la persona con el nombre más largo usando paso por valor.
 * CÓMO: Comparando longitud de nombres y apellidos de todas las personas.
 * PARA QUÉ: Para análisis de datos con paso por valor.
 */
Persona buscarNombreMasLargoValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        return Persona(); // Si no hay personas, retornar persona vacía
    }

    Persona personaNombreLargo = personas[0];
    int lonNombre = personaNombreLargo.getNombre().size();
    int lonApellido = personaNombreLargo.getApellido().size();
    int tamano = lonNombre + lonApellido;

    for (size_t i = 1; i < personas.size(); i++) {
        int lonNombreActual = personas[i].getNombre().size();
        int lonApellidoActual = personas[i].getApellido().size();
        int tamanoActual = lonNombreActual + lonApellidoActual;

        if (tamanoActual > tamano) {
            personaNombreLargo = personas[i];
            tamano = tamanoActual;
        }
    }

    return personaNombreLargo;
}
