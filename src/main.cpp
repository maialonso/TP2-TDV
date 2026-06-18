#include "solver.h"
#include "timer.h"

#include <iostream>
#include <vector>
#include <string>

int main() {

    std::vector<std::string> instancias = {
        "instances/gap/gap_e/e801600"
    };

    std::vector<double> porcentajesPerturbacion = {
        0.02,
        0.05,
        0.10,
        0.15,
    };

    std::vector<int> iteraciones = {
        200
    };

    std::cout
        << "instancia,perturbacion,iteraciones,costo_inicial,costo_final,mejora_porcentual"
        << std::endl;

    for(const std::string& nombreInstancia : instancias) {

        Instancia instancia(nombreInstancia);
        Solver solver(instancia);

        // solución inicial sobre la que corre ILS
        Solucion inicial = solver.heuristicaSecuencial();
        double costoInicial = inicial.costo(instancia);

        for(double perturbacion : porcentajesPerturbacion) {

            for(int it : iteraciones) {

                Solucion resultado =
                    solver.ILS(
                        inicial,
                        it,
                        perturbacion
                    );

                double costoFinal = resultado.costo(instancia);

                double mejoraPorcentual =
                    100.0 * (costoInicial - costoFinal)
                    / costoInicial;

                std::cout
                    << nombreInstancia << ","
                    << perturbacion << ","
                    << it << ","
                    << costoInicial << ","
                    << costoFinal << ","
                    << mejoraPorcentual
                    << std::endl;
            }
        }
    }

    return 0;
}