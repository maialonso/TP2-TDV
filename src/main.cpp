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

    return 0;
}