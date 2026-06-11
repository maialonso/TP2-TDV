#include "instancia.h"
#include "solucion.h"

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
