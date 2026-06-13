#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "instancia.h"
#include "solucion.h"
#include "heuristica1.h"
#include "heuristica2.h"
#include "bl.h"
#include "swap.h"
#include "relocate.h"
#include "metaHeuristica.h"

//main metaheuristica
int main(int argc, char* argv[]) {

    if(argc < 3) {
        std::cerr << "Uso: ./tp2 <archivo_input> <archivo_output>" << std::endl;
        return 1;
    }

    std::string archivoInput = argv[1];
    std::string archivoOutput = argv[2];

    // leo la instancia
    Instancia instancia(archivoInput);

    std::cout << "Instancia cargada: " 
              << instancia.cantidadVendedores() << " vendedores, "
              << instancia.cantidadDepositos() << " depositos" << std::endl;

    // solución inicial con heurística
    Solucion inicial = heuristica2(instancia);
    std::cout << "Costo heuristica: " << inicial.costo(instancia) << std::endl;

    // simulated annealing
    Solucion resultado = simulatedAnnealing(instancia, inicial);
    std::cout << "Costo SA: " << resultado.costo(instancia) << std::endl;

    // guardo resultado
    resultado.guardar(archivoOutput, instancia);
    std::cout << "Solución guardada en " << archivoOutput << std::endl;

    return 0;
}





/*
int main() {

    Instancia instancia("instances/real/real_instance");

    Solucion s = heuristica1(instancia);

    std::cout
        << s.costo(instancia)
        << std::endl;


    Solucion mejor =
        busquedaLocal(
            instancia,
            s
        );

    std::cout
        << mejor.costo(instancia)
        << std::endl;
*/
//     std::cout << "Asignaciones:\n";

//     for(int vendedor = 0;
//         vendedor < instancia.cantidadVendedores();
//         vendedor++) {

//         std::cout << "Vendedor "
//                   << vendedor
//                   << " -> Deposito "
//                   << s.depositoDe(vendedor)
//                   << std::endl;
//     }

//     std::cout << "\nSin asignar:\n";

//     for(int vendedor = 0;
//         vendedor < instancia.cantidadVendedores();
//         vendedor++) {

//         if(!s.estaAsignado(vendedor)) {

//             std::cout << "Vendedor "
//                       << vendedor
//                       << " sin asignar"
//                       << std::endl;
//         }
//     }
//     for(int vendedor = 0;
//     vendedor < instancia.cantidadVendedores();
//     vendedor++) {

//     if(!s.estaAsignado(vendedor)) {

//         std::cout << "\nVendedor "
//                   << vendedor
//                   << " no pudo asignarse:\n";

//         for(int deposito = 0;
//             deposito < instancia.cantidadDepositos();
//             deposito++) {

//             std::cout
//                 << "  Deposito "
//                 << deposito
//                 << " necesita "
//                 << instancia.demanda(deposito, vendedor)
//                 << " y quedan "
//                 << instancia.capacidad(deposito)
//                 << std::endl;
//         }
//     }
// }
//     std::cout << "\nCosto total: "
//               << s.costo(instancia)
//               << std::endl;

    //return 0;
//}