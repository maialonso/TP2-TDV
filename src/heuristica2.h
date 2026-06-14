#pragma once
#include "instancia.h"
#include "solucion.h"
#include "solver.h"
#include <algorithm>
#include <numeric>
#include <vector>

// ordena los vendedores por demanda y los asigna al depósito factible de menor costo
Solucion heuristica2(const Instancia& instancia) {

    int n = instancia.cantidadVendedores();
    Solucion solucion(n);
    Solver solver(instancia);

    std::vector<int> capacidades = instancia.capacidades(); // copia de capacidades para ir actualizándolas

    std::vector<int> vendedores(n); // vector con todos los vendedores

    std::iota(vendedores.begin(), vendedores.end(), 0); // crea vendedores = [0, 1, 2, ..., n-1]

    std::sort(vendedores.begin(), vendedores.end(), [&](int v1, int v2) {   // ordena vendedores de mayor demanda a menor demanda
            int demanda1 = instancia.demanda(0, v1);
            int demanda2 = instancia.demanda(0, v2);
            return demanda1 > demanda2;
        }
    );

    for(int vendedor : vendedores) {    // recorre los vendedores en el orden definido

        int mejorDeposito = solver.mejorDepositoFactible(vendedor, capacidades);    // busca el depósito factible de menor costo

        if(mejorDeposito != -1) {   // si encontró depósito factible, asigna
            solucion.asignar(vendedor, mejorDeposito);
            capacidades[mejorDeposito] -= instancia.demanda(mejorDeposito, vendedor);   // actualiza capacidad restante
        }
    }

    return solucion; // devuelve la solución construida
}