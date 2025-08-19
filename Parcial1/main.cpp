#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"
#include <map>

/**
 * Función auxiliar para mostrar comparación de rendimiento
 */
void mostrarComparacion(const std::string& operacion, 
                       double tiempoValor, long memoriaValor,
                       double tiempoApuntador, long memoriaApuntador);

/**
 * Muestra el menú principal de la aplicación.
 * 
 * POR QUÉ: Guiar al usuario a través de las funcionalidades disponibles.
 * CÓMO: Imprimiendo las opciones en consola.
 * PARA QUÉ: Interacción amigable con el usuario.
 */
void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Salir";
    std::cout << "\n1. Crear nuevo conjunto de datos";
    std::cout << "\n2. Mostrar resumen de todas las personas";
    std::cout << "\n3. Mostrar detalle completo por índice";
    std::cout << "\n4. Buscar persona por ID";
    std::cout << "\n5. Mostrar estadísticas de rendimiento";
    std::cout << "\n6. Exportar estadísticas a CSV";
    std::cout << "\n7. Consultar persona más longeva del país";
    std::cout << "\n8. Consultar persona más longeva de cada ciudad";
    std::cout << "\n9. Consultar persona con mayor patrimonio del país";
    std::cout << "\n10. Consultar persona con mayor patrimonio de cada ciudad";
    std::cout << "\n11. Consultar persona con mayor patrimonio por calendario";
    std::cout << "\n12. Listar y contar personas por calendario tributario (A, B, C)";
    std::cout << "\n13. Top 3 ciudades con mayor patrimonio promedio";
    std::cout << "\n14. Consultar persona con más deudas del país";
    std::cout << "\n15. Consultar persona con el nombre más largo";
    std::cout << "\n\nSeleccione una opción: ";
}

/**
 * Función auxiliar para mostrar comparación de rendimiento
 */
void mostrarComparacion(const std::string& operacion, 
                       double tiempoValor, long memoriaValor,
                       double tiempoApuntador, long memoriaApuntador) {
    std::cout << "\n=== COMPARACION DE RENDIMIENTO: " << operacion << " ===\n";
    std::cout << "Metodo          | Tiempo (ms)    | Memoria (KB)   | Eficiencia\n";
    std::cout << "----------------|----------------|----------------|------------\n";
    
    // Mostrar resultados de PASO POR VALOR
    std::cout << "Por Valor       | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) 
              << tiempoValor << " ms | " << std::setw(12) << memoriaValor << " KB | ";
    
    if (tiempoValor > tiempoApuntador * 1.5) {
        std::cout << "LENTO";
    } else {
        std::cout << "Normal";
    }
    std::cout << "\n";
    
    // Mostrar resultados de APUNTADORES
    std::cout << "Por Apuntador   | " << std::right << std::setw(12) << std::fixed << std::setprecision(2) 
              << tiempoApuntador << " ms | " << std::setw(12) << memoriaApuntador << " KB | ";
    
    if (tiempoApuntador < tiempoValor * 0.7) {
        std::cout << "RAPIDO";
    } else {
        std::cout << "Normal";
    }
    std::cout << "\n";
    
    std::cout << "----------------|----------------|----------------|------------\n";
    
    // Calcular y mostrar diferencias
    double diferenciaT = 0.0;
    double diferenciaM = 0.0;
    
    // Evitar división por cero en tiempo
    if (tiempoApuntador > 0) {
        diferenciaT = ((tiempoValor - tiempoApuntador) / tiempoApuntador) * 100;
    }
    
    // Evitar división por cero en memoria
    if (memoriaApuntador > 0) {
        diferenciaM = ((memoriaValor - memoriaApuntador) / static_cast<double>(memoriaApuntador)) * 100;
    }
    
    std::cout << "ANALISIS:\n";
    std::cout << "- Diferencia Tiempo: " << std::fixed << std::setprecision(1) 
              << std::abs(diferenciaT) << "% ";
    
    if (diferenciaT > 0) {
        std::cout << "(Apuntadores " << std::abs(diferenciaT) << "% mas rapido)\n";
    } else if (diferenciaT < 0) {
        std::cout << "(Valor " << std::abs(diferenciaT) << "% mas rapido)\n";
    } else {
        std::cout << "(Mismo rendimiento)\n";
    }
    
    std::cout << "- Diferencia Memoria: " << std::fixed << std::setprecision(1) 
              << std::abs(diferenciaM) << "% ";
    
    if (diferenciaM > 0) {
        std::cout << "(Apuntadores usa " << std::abs(diferenciaM) << "% menos memoria)\n";
    } else if (diferenciaM < 0) {
        std::cout << "(Valor usa " << std::abs(diferenciaM) << "% menos memoria)\n";
    } else {
        std::cout << "(Mismo uso de memoria)\n";
    }
    
    std::cout << "========================================\n";
}

/**
 * Punto de entrada principal del programa.
 * 
 * POR QUÉ: Iniciar la aplicación y manejar el flujo principal.
 * CÓMO: Mediante un bucle que muestra el menú y procesa la opción seleccionada.
 * PARA QUÉ: Ejecutar las funcionalidades del sistema.
 */
int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria
    
    // Puntero inteligente para gestionar la colección de personas
    // POR QUÉ: Evitar fugas de memoria y garantizar liberación automática.
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    
    Monitor monitor; // Monitor para medir rendimiento
    
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        // Variables locales para uso en los casos
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para la operación actual
        monitor.iniciar_tiempo();
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: // Salir
                std::cout << "Saliendo...\n";
                break;

            case 1: { // Crear nuevo conjunto de datos
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                // Generar el nuevo conjunto de personas
                auto nuevasPersonas = generarColeccion(n);
                tam = nuevasPersonas.size();
                
                // Mover el conjunto al puntero inteligente (propiedad única)
                personas = std::make_unique<std::vector<Persona>>(std::move(nuevasPersonas));
                
                // Medir tiempo y memoria usada
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                // Registrar la operación
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 2: { // Mostrar resumen de todas las personas
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }
                
                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }
                
            case 3: { // Mostrar detalle por índice
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }
                
            case 4: { // Buscar por ID
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }
                
                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;
                
                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                const Persona* encontrada_ap = buscarPorID(*personas, idBusqueda);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                Persona encontrada_val = buscarPorIDValor(*personas, idBusqueda);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                // Mostrar resultados
                if(encontrada_ap) {
                    std::cout << "\n=== RESULTADO BÚSQUEDA POR ID ===" << std::endl;
                    encontrada_ap->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
                // Mostrar comparación de rendimiento
                mostrarComparacion("Buscar por ID", tiempo_val, memoria_val, tiempo_ap, memoria_ap);
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }
                
            case 5: // Mostrar estadísticas de rendimiento
                monitor.mostrar_resumen();
                break;
                
            case 6: // Exportar estadísticas a CSV
                monitor.exportar_csv();
                break;

            case 7: 
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                const Persona* mayor_ap = buscarLongeva(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                Persona mayor_val = buscarLongevaValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                // Mostrar resultados
                std::cout << "\n=== PERSONA MÁS LONGEVA DEL PAÍS ===" << std::endl;
                mayor_ap->mostrar();

                // Mostrar comparación de rendimiento
                mostrarComparacion("Persona más longeva", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_longeva1 = monitor.detener_tiempo();
                long memoria_longeva1 = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Longeva del país", tiempo_longeva1, memoria_longeva1);
                break;
            }

            case 8:
            {
                if (!personas || personas->empty())
                {
                    std::cout<<"\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                auto longevasPorCiudad_ap = buscarLongevaPorCiudad(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                auto longevasPorCiudad_val = buscarLongevaPorCiudadValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;

                std::cout << "\n=== PERSONA MÁS LONGEVA POR CIUDAD ===\n";
                std::cout << "Total de ciudades: " << longevasPorCiudad_ap.size() << "\n\n";

                for (const auto& [ciudad, persona] : longevasPorCiudad_ap)
                {
                    int dia, mes, anio;
                    persona->obtenerFechaNacimiento(dia, mes, anio);
                    std::cout << "\nCiudad: " << ciudad << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

                // Mostrar comparación de rendimiento
                mostrarComparacion("Longeva por ciudad", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_longeva = monitor.detener_tiempo();
                long memoria_longeva = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Longeva por ciudad", tiempo_longeva, memoria_longeva);
                break;
            }

            case 9:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                const Persona* masRico_ap = buscarPatrimonio(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                Persona masRico_val = buscarPatrimonioValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                std::cout << "\nLa persona con mayor patrimonio del país es:\n";
                masRico_ap->mostrar();

                // Mostrar comparación de rendimiento
                mostrarComparacion("Mayor patrimonio", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mayor patrimonio", tiempo_patrimonio, memoria_patrimonio);
                break;
            }

            case 10:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                auto patrimonioPorCiudad_ap = buscarPatrimonioPorCiudad(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                auto patrimonioPorCiudad_val = buscarPatrimonioPorCiudadValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;

                std::cout << "\n=== PERSONA MÁS RICA POR CIUDAD ===\n";
                std::cout << "Total de ciudades: " << patrimonioPorCiudad_ap.size() << "\n\n";

                for (const auto& [ciudad, persona] : patrimonioPorCiudad_ap)
                {
                    persona->getPatrimonio();
                    std::cout << "\nCiudad: " << ciudad << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

                // Mostrar comparación de rendimiento
                mostrarComparacion("Mayor patrimonio por ciudad", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mayor patrimonio por ciudad", tiempo_patrimonio, memoria_patrimonio);
                break;
            }

            case 11:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                auto patrimonioPorCalendario_ap = buscarPatrimonioPorCalendario(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                auto patrimonioPorCalendario_val = buscarPatrimonioPorCalendarioValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                std::cout << "\n=== PERSONA MÁS RICA POR CALENDARIO ===\n";
                std::cout << "Total de calendarios: " << patrimonioPorCalendario_ap.size() << "\n\n";

                for (const auto& [calendario, persona] : patrimonioPorCalendario_ap)
                {
                    persona->getPatrimonio();
                    std::cout << "\nCalendario: " << calendario << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

                // Mostrar comparación de rendimiento
                mostrarComparacion("Mayor patrimonio por calendario", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mayor patrimonio por calendario", tiempo_patrimonio, memoria_patrimonio);
                break;

            }

            case 12:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                listarPersonasCalendario(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                listarPersonasCalendarioValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;

                // Mostrar comparación de rendimiento
                mostrarComparacion("Lista personas por calendario", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Lista personas por calendario", tiempo_patrimonio, memoria_patrimonio);
                break;
            }

            case 13:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                top3CiudadesPatrimonio(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                top3CiudadesPatrimonioValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;

                // Mostrar comparación de rendimiento
                mostrarComparacion("Top 3 ciudades patrimonio", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Top 3 ciudades patrimonio", tiempo_patrimonio, memoria_patrimonio);
                break;
            }

            case 14:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                const Persona* masEndeudado_ap = buscarDeudas(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                Persona masEndeudado_val = buscarDeudasValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                std::cout << "\nLa persona con más deudas del país es:\n";
                masEndeudado_ap->mostrar();

                // Mostrar comparación de rendimiento
                mostrarComparacion("Mayor deuda", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mayor deuda", tiempo_patrimonio, memoria_patrimonio);
                break;

            }

            case 15:
            {
                if (!personas || personas->empty())
                {
                    std::cout << "\nNo hay datos disponibles. Use opción 1 primero.\n";
                    break;
                }

                // Ejecutar con apuntadores
                monitor.iniciar_tiempo();
                long memoria_inicio_ap = monitor.obtener_memoria();
                const Persona* nombreMasLargo_ap = buscarNombreMasLargo(*personas);
                double tiempo_ap = monitor.detener_tiempo();
                long memoria_ap = monitor.obtener_memoria() - memoria_inicio_ap;
                
                // Ejecutar con paso por valor
                monitor.iniciar_tiempo();
                long memoria_inicio_val = monitor.obtener_memoria();
                Persona nombreMasLargo_val = buscarNombreMasLargoValor(*personas);
                double tiempo_val = monitor.detener_tiempo();
                long memoria_val = monitor.obtener_memoria() - memoria_inicio_val;
                
                std::cout << "\nLa persona con el nombre más largo es:\n";
                nombreMasLargo_ap->mostrar();
                int lonNombre = nombreMasLargo_ap->getNombre().size();
                int lonApellido = nombreMasLargo_ap->getApellido().size();
                int tamano = lonNombre + lonApellido - 3;
                std::cout << "\nEl nombre tiene: " << tamano << " caracteres.\n";

                // Mostrar comparación de rendimiento
                mostrarComparacion("Nombre más largo", tiempo_val, memoria_val, tiempo_ap, memoria_ap);

                double tiempo_patrimonio = monitor.detener_tiempo();
                long memoria_patrimonio = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Nombre más largo", tiempo_patrimonio, memoria_patrimonio);
                break;
            }
                  
            default:
                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas de la operación (excepto para opciones 0,5,6)
        if (opcion != 0 && opcion != 5 && opcion != 6) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 0);
    
    return 0;
}