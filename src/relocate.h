#pragma once
#include "instancia.h"
#include "solucion.h"
#include "solver.h"
#include <random>
#include <vector>

// intenta mover un vendedor al depósito factible de menor costo
bool relocate(Solucion& solucion, const Instancia& instancia, int v1) {
    
    int d0 = solucion.depositoDe(v1); // depósito actual del vendedor
    Solver solver(instancia);
    std::vector<int> capacidadRestante = solver.capacidadRestante(solucion);    // calcula la capacidad restante de cada depósito

    int mejorDeposito = solver.mejorDepositoFactibleDistinto(v1, d0,capacidadRestante);    // busca el mejor depósito factible distinto al actual

    if(mejorDeposito == -1) return false;   // si no existe un depósito factible, no hace el movimiento

    solucion.asignar(v1, mejorDeposito);    // reasigna el vendedor al nuevo depósito

    return true;
}

// intenta mover un vendedor a un depósito factible elegido al azar
bool relocateAleatorio(Solucion& solucion, const Instancia& instancia, int v1, std::mt19937& rng) {

    int d0 = solucion.depositoDe(v1); // depósito actual del vendedor
    Solver solver(instancia);

    std::vector<int> capacidadRestante = solver.capacidadRestante(solucion);    // calcula la capacidad restante de cada depósito
    std::vector<int> factibles; // guarda los depósitos factibles distintos al actual

    for(int deposito = 0; deposito < (int)capacidadRestante.size(); deposito++) {

        if(deposito != d0) {
            int demanda = instancia.demanda(deposito, v1);
            if(capacidadRestante[deposito] >= demanda) {
                factibles.push_back(deposito);
            }
        }
    }

    if(factibles.empty()) return false; // si no hay depósitos factibles, no hace el movimiento

    std::uniform_int_distribution<int> dist(0, factibles.size() - 1);   // elige un depósito factible al azar

    int depositoElegido = factibles[dist(rng)];

    solucion.asignar(v1, depositoElegido);  // reasigna el vendedor al depósito elegido

    return true;
}