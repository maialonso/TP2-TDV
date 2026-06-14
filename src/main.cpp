#include <string>
#include <iostream>
#include "instancia.h"
#include "solucion.h"
#include "heuristica1.h"
#include "heuristica2.h"
#include "heuristica3.h"
#include "bl.h"
#include "metaHeuristica.h"

int main(int argc, char* argv[]) {

    if(argc < 3) {
        std::cerr << "Uso: ./gap_simulator <archivo_input> <archivo_output>" << std::endl;
        return 1;
    }

    std::string archivoInput = argv[1];
    std::string archivoOutput = argv[2];

    Instancia instancia(archivoInput);

    std::cout << "Instancia cargada: "
              << instancia.cantidadVendedores() << " vendedores, "
              << instancia.cantidadDepositos() << " depositos"
              << std::endl;

    Solucion inicial = heuristica2(instancia);

    std::cout << "Costo heuristica inicial: "
              << inicial.costo(instancia)
              << std::endl;

    Solucion mejorada = VNDSwapRelocate(instancia, inicial);

    std::cout << "Costo VND: "
              << mejorada.costo(instancia)
              << std::endl;

    Solucion resultado = ILS(instancia, inicial, 100, 0.05);

    std::cout << "Costo ILS: "
              << resultado.costo(instancia)
              << std::endl;

    resultado.guardar(archivoOutput, instancia);

    std::cout << "Solucion guardada en "
              << archivoOutput
              << std::endl;

    return 0;
}