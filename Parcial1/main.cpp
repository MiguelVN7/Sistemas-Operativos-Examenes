#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"
#include <map>

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
                
                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
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

                const Persona* mayor = buscarLongeva(*personas);
                std::cout << "\nLa persona más longeva de todo el país es: " << "\n";
                mayor->mostrar();

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

                auto longevasPorCiudad = buscarLongevaPorCiudad(*personas);

                std::cout << "\n=== PERSONA MÁS LONGEVA POR CIUDAD ===\n";
                std::cout << "Total de ciudades: " << longevasPorCiudad.size() << "\n\n";

                for (const auto& [ciudad, persona] : longevasPorCiudad)
                {
                    int dia, mes, anio;
                    persona->obtenerFechaNacimiento(dia, mes, anio);
                    std::cout << "\nCiudad: " << ciudad << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

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

                const Persona* masRico = buscarPatrimonio(*personas);
                std::cout << "\nLa persona con mayor patrimonio del país es:\n";
                masRico->mostrar();

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

                auto patrimonioPorCiudad = buscarPatrimonioPorCiudad(*personas);

                std::cout << "\n=== PERSONA MÁS RICA POR CIUDAD ===\n";
                std::cout << "Total de ciudades: " << patrimonioPorCiudad.size() << "\n\n";

                for (const auto& [ciudad, persona] : patrimonioPorCiudad)
                {
                    persona->getPatrimonio();
                    std::cout << "\nCiudad: " << ciudad << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

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

                auto patrimonioPorCalendario = buscarPatrimonioPorCalendario(*personas);
                std::cout << "\n=== PERSONA MÁS RICA POR CALENDARIO ===\n";
                std::cout << "Total de calendarios: " << patrimonioPorCalendario.size() << "\n\n";

                for (const auto& [calendario, persona] : patrimonioPorCalendario)
                {
                    persona->getPatrimonio();
                    std::cout << "\nCiudad: " << calendario << ": \n";
                    persona->mostrar();
                    std::cout << "\n";
                }

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

                listarPersonasCalendario(*personas);

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

                monitor.iniciar_tiempo();
                top3CiudadesPatrimonio(*personas);

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

                const Persona* masEndeudado = buscarDeudas(*personas);
                std::cout << "\nLa persona con más deudas del país es:\n";
                masEndeudado->mostrar();

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

                const Persona* nombreMasLargo = buscarNombreMasLargo(*personas);
                std::cout << "\nLa persona con el nombre más largo es:\n";
                nombreMasLargo->mostrar();
                int lonNombre = nombreMasLargo->getNombre().size();
                int lonApellido = nombreMasLargo->getApellido().size();
                int tamano = lonNombre + lonApellido - 3;
                std::cout << "\nEl nombre tiene: " << tamano << " caracteres.\n";

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