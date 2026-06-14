#include "instancia.h"
#include "solucion.h"
#include <algorithm>
#include <numeric>

Solucion heuristica3(const Instancia& instancia) {

    int n = instancia.cantidadVendedores();
    int m = instancia.cantidadDepositos();

    Solucion solucion(n);

    std::vector<int> capacidades = instancia.capacidades();

    std::vector<int> vendedores(n);
    std::iota(vendedores.begin(), vendedores.end(), 0);

    std::sort(vendedores.begin(), vendedores.end(),
        [&](int v1, int v2) {

            double promedio1 = 0.0;
            double promedio2 = 0.0;

            for(int deposito = 0; deposito < m; deposito++) {
                promedio1 += instancia.demanda(deposito, v1);
                promedio2 += instancia.demanda(deposito, v2);
            }

            promedio1 /= m;
            promedio2 /= m;

            return promedio1 > promedio2;
        }
    );

    for(int vendedor : vendedores) {

        int mejorDeposito = -1;
        double mejorCosto = INT_MAX;

        for(int deposito = 0; deposito < m; deposito++) {

            int demanda = instancia.demanda(deposito, vendedor);

            if(capacidades[deposito] >= demanda) {

                double costo = instancia.costo(deposito, vendedor);

                if(costo < mejorCosto) {
                    mejorCosto = costo;
                    mejorDeposito = deposito;
                }
            }
        }

        if(mejorDeposito != -1) {
            solucion.asignar(vendedor, mejorDeposito);
            capacidades[mejorDeposito] -= instancia.demanda(mejorDeposito, vendedor);
        }
    }
    
    return solucion;
}