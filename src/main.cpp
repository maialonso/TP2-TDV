#include "solver.h"
#include <iostream>
#include <string>
#include <fstream>

int main() {

    while(true) {

        std::string nombre;
        Instancia* instancia = nullptr;

        while(instancia == nullptr) {

            std::cout
                << "\nElija instancia por correr (0 para salir): ";

            std::cin >> nombre;

            if(nombre == "0") {
                return 0;
            }

            try {

                char familia = nombre[0];

                std::string archivo =
                    "instances/gap/gap_"
                    + std::string(1, familia)
                    + "/"
                    + nombre;

                instancia = new Instancia(archivo);

            } catch(const std::exception&) {

                std::cout
                    << "Error: la instancia '"
                    << nombre
                    << "' no existe.\n";
            }
        }

        Solver solver(*instancia);

        int opcion = -1;

        while(true) {

            std::cout << "\nSeleccione algoritmo:\n";
            std::cout << "1 - Heuristica Secuencial\n";
            std::cout << "2 - Heuristica Demanda Maxima\n";
            std::cout << "3 - Heuristica Demanda Promedio\n";
            std::cout << "4 - Busqueda Local Swap\n";
            std::cout << "5 - Busqueda Local Relocate\n";
            std::cout << "6 - swapRelocateSecuencial\n";
            std::cout << "7 - ILS\n";
            std::cout << "0 - Volver a elegir instancia\n";
            std::cout << "\nOpcion: ";

            std::cin >> opcion;

            if(opcion >= 0 && opcion <= 7) {
                break;
            }

            std::cout
                << "Opcion invalida. Intente nuevamente.\n";
        }

        if(opcion == 0) {
            delete instancia;
            continue;
        }

        Solucion solucion(instancia->cantidadVendedores());

        switch(opcion) {

            case 1:
                solucion = solver.heuristicaSecuencial();
                break;

            case 2:
                solucion = solver.heuristicaDemandaMax();
                break;

            case 3:
                solucion = solver.heuristicaDemandaProm();
                break;

            case 4:
                solucion =
                    solver.busquedaLocalSwap(
                        solver.heuristicaSecuencial()
                    );
                break;

            case 5:
                solucion =
                    solver.busquedaLocalRelocate(
                        solver.heuristicaSecuencial()
                    );
                break;

            case 6:
                solucion =
                    solver.swapRelocateSecuencial(
                        solver.heuristicaSecuencial()
                    );
                break;

            case 7: {

                int opcionILS;

                while(true) {

                    std::cout
                        << "\nConfiguracion ILS\n";
                    std::cout
                        << "1 - Parametros recomendados "
                           "(50 iteraciones, 15% perturbacion)\n";
                    std::cout
                        << "2 - Parametros personalizados\n";
                    std::cout
                        << "Opcion: ";

                    std::cin >> opcionILS;

                    if(opcionILS == 1 ||
                       opcionILS == 2) {
                        break;
                    }

                    std::cout
                        << "Opcion invalida.\n";
                }

                int iteraciones;
                double perturbacion;

                if(opcionILS == 1) {

                    iteraciones = 50;
                    perturbacion = 0.15;

                } else {

                    std::cout
                        << "Iteraciones: ";
                    std::cin
                        >> iteraciones;

                    std::cout
                        << "Perturbacion (ej 0.15): ";
                    std::cin
                        >> perturbacion;
                }

                solucion =
                    solver.ILS(
                        solver.heuristicaSecuencial(),
                        iteraciones,
                        perturbacion
                    );

                break;
            }
            
        }

        std::cout
            << "\nCosto: "
            << solucion.costo(*instancia)
            << "\n";
        char guardar;

        std::cout << "\nDesea guardar la solucion en un archivo? (s/n): ";

        std::cin >> guardar;

        if(guardar == 's' || guardar == 'S') {

            std::string nombreArchivo;

            std::cout
                << "Nombre del archivo: ";

            std::cin
                >> nombreArchivo;

            std::ofstream out(nombreArchivo);

            for(int vendedor = 0;
                vendedor < instancia->cantidadVendedores();
                vendedor++) {

                out
                    << vendedor
                    << " "
                    << solucion.depositoDe(vendedor)
                    << "\n";
            }

            std::cout
                << "Solucion guardada en "
                << nombreArchivo
                << "\n";
        }
        delete instancia;
    }

    return 0;
}