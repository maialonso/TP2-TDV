#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <climits>
#include "Instancia.h"
#include "Solucion.h"

Solucion heuristica1(const Instancia& instancia) {

    int n = instancia.cantidadVendedores();
    int m = instancia.cantidadDepositos();

    Solucion solucion(n);

    std::vector<int> capacidades =
        instancia.capacidades();

    for(int vendedor = 0; vendedor < n; vendedor++) {

        int mejorDeposito = -1;
        double mejorCosto = INT_MAX;

        for(int deposito = 0; deposito < m; deposito++) {

            int demanda =
                instancia.demanda(deposito, vendedor);

            if(capacidades[deposito] >= demanda) {

                double costo =
                    instancia.costo(deposito, vendedor);

                if(costo < mejorCosto) {

                    mejorCosto = costo;
                    mejorDeposito = deposito;
                }
            }
        }

        if(mejorDeposito != -1) {

            solucion.asignar(vendedor,
                             mejorDeposito);

            capacidades[mejorDeposito] -=
                instancia.demanda(mejorDeposito,
                                  vendedor);
        }
    }

    return solucion;
}



int main() {

    Instancia instancia("instances/gap/gap_b/b05200");

    std::cout << instancia.cantidadDepositos() << std::endl;
    std::cout << instancia.cantidadVendedores() << std::endl;

    Solucion s = heuristica1(instancia);

    double suma = 0;
    int sinAsignar = 0;

    for(int v = 0; v < instancia.cantidadVendedores(); v++) {

        int d = s.depositoDe(v);

        if(d == -1) {
            std::cout << "Vendedor sin asignar: "
                      << v << std::endl;
            sinAsignar++;
            continue;
        }

        std::cout << "vendedor "
                  << v
                  << " -> deposito "
                  << d
                  << std::endl;

        suma += instancia.costo(d, v);
    }

    std::cout << "Costo total: "
              << suma
              << std::endl;

    std::cout << "Sin asignar: "
              << sinAsignar
              << std::endl;

    return 0;
}
