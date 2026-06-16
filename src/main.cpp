#include "solver.h"
#include "timer.h"

#include <iostream>

int main() {

    Instancia instancia("instances/gap/gap_a/a20100");

    Solver solver(instancia);

    Timer t;

    t.reiniciar();
    Solucion s1 = solver.heuristicaSecuencial();
    std::cout
        << "H1 -> costo: " << s1.costo(instancia)
        << " | tiempo: " << t.tiempoMs() << " ms"
        << std::endl;

    t.reiniciar();
    Solucion s2 = solver.heuristicaDemandaMax();
    std::cout
        << "H2 -> costo: " << s2.costo(instancia)
        << " | tiempo: " << t.tiempoMs() << " ms"
        << std::endl;

    t.reiniciar();
    Solucion s3 = solver.heuristicaDemandaProm();
    std::cout
        << "H3 -> costo: " << s3.costo(instancia)
        << " | tiempo: " << t.tiempoMs() << " ms"
        << std::endl;

    return 0;
}
    // t.reiniciar();
    // Solucion mejorSwap = busquedaLocalSwap(instancia, s1);
    // std::cout
    //     << "BL Swap -> costo: " << mejorSwap.costo(instancia)
    //     << " | tiempo: " << t.tiempoMs() << " ms"
    //     << std::endl;

    // t.reiniciar();
    // Solucion mejorRelocate = busquedaLocalRelocate(instancia, s1);
    // std::cout
    //     << "BL Relocate -> costo: " << mejorRelocate.costo(instancia)
    //     << " | tiempo: " << t.tiempoMs() << " ms"
    //     << std::endl;

    

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
