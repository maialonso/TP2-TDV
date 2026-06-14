#include <vector>
#include "instancia.h"
#include "solucion.h"
#include "solver.h"


// asigna cada vendedor al depósito factible de menor costo
Solucion heuristica1(const Instancia& instancia) {

    Solver solver(instancia);
    int n = instancia.cantidadVendedores();
    Solucion solucion(n);

    std::vector<int> capacidades = instancia.capacidades(); // copia de capacidades para ir actualizándolas

    for(int vendedor = 0; vendedor < n; vendedor++) {   // recorre los vendedores en orden

        int mejorDeposito = solver.mejorDepositoFactible(vendedor, capacidades);
        if(mejorDeposito != -1) {    // si encontró alguno, asigna
            solucion.asignar(vendedor, mejorDeposito);
            capacidades[mejorDeposito] -= instancia.demanda(mejorDeposito, vendedor);   // actualiza capacidad restante
        }
    }

    return solucion;    // devuelve la solución construida
}