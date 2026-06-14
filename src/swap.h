#pragma once

#include "instancia.h"
#include "solucion.h"
#include "solver.h"

// intenta intercambiar los depósitos de dos vendedores
bool swapVendedores(Solucion& solucion, const Instancia& instancia, int v1, int v2) {

    int d1 = solucion.depositoDe(v1); // depósito actual de v1
    int d2 = solucion.depositoDe(v2); // depósito actual de v2

    if(d1 == -1 || d2 == -1) return false;  // si alguno no está asignado, no se puede hacer swap

    if(d1 == d2) return false;  // si están en el mismo depósito, el swap no cambia nada

    Solver solver(instancia);

    std::vector<int> capacidadRestante = solver.capacidadRestante(solucion);    // calcula la capacidad restante de cada depósito

    // libera temporalmente la demanda actual de v1 y v2
    capacidadRestante[d1] += instancia.demanda(d1, v1);
    capacidadRestante[d2] += instancia.demanda(d2, v2);

    // calculo cuánto demandarían si se intercambian
    int nuevaDemandaV1 = instancia.demanda(d2, v1);
    int nuevaDemandaV2 = instancia.demanda(d1, v2);

    if(capacidadRestante[d2] < nuevaDemandaV1) return false;    // verifico si v1 entra en el depósito de v2
    if(capacidadRestante[d1] < nuevaDemandaV2) return false;    // verifico si v2 entra en el depósito de v1

    // si ambos entran, hago el swap
    solucion.asignar(v1, d2);
    solucion.asignar(v2, d1);

    return true; // swap exitoso
}