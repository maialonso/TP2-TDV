#pragma once
#include "instancia.h"
#include "solucion.h"
#include <vector>
#include <random>

class Solver {
private:
    const Instancia& instancia_;

public:
    Solver(const Instancia& instancia);

    // auxiliares
    std::vector<int> capacidadRestante(const Solucion&) const;
    int mejorDepositoFactible(int vendedor,
                              const std::vector<int>& capacidad) const;

    int mejorDepositoFactibleDistinto(
        int vendedor,
        int depositoActual,
        const std::vector<int>& capacidad) const;

    // heurísticas
    Solucion heuristicaSecuencial() const;
    Solucion heuristicaDemandaMax() const;
    Solucion heuristicaDemandaProm() const;

    // operadores/ movimientos
    bool swapVendedores(Solucion&, int v1, int v2) const;
    bool relocateViejo(Solucion&, int v1) const;
    bool relocate(Solucion&, int v1) const;
    bool relocateAleatorio(
        Solucion&,
        int v1,
        std::mt19937& rng) const;

    // búsqueda local
    Solucion busquedaLocalSwap(Solucion) const;
    Solucion busquedaLocalRelocate(Solucion) const;

    // metaheurísticas
    Solucion VNDSwapRelocate(Solucion) const;
    Solucion perturbarConRelocate(Solucion solucion, int cantidadRelocates, std::mt19937& rng) const;
    Solucion ILS(
        const Solucion& inicial,
        int iteraciones,
        double porcentajePerturbacion) const;

};