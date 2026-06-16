#pragma once

#include "instancia.h"   
#include "solucion.h"    
#include "bl.h"         
#include "relocate.h"   
#include <random>        
#include <algorithm>     



// aplica búsqueda local combinando swap y relocate
Solucion VNDSwapRelocate(const Instancia& instancia, Solucion actual) {

    while(true) {   // repite hasta que ningún operador mejore

        actual = busquedaLocalSwap(instancia, actual);    // prueba todos los swaps posibles
        
        Solucion despuesSwap = actual;  // guarda cómo quedó después de swap
        
        Solucion despuesRelocate = busquedaLocalRelocate(instancia, actual); // prueba todos los relocates posibles
        
        if(despuesRelocate.mejorQue(despuesSwap, instancia)) {   // si relocate mejoró, actualiza y vuelve a swap
            actual = despuesRelocate;
            continue;   // volver a swap
        }

        break;  // si relocate no mejoró, termina
    }

    return actual; // devuelve el óptimo local encontrado
}



// perturba una solución con relocates aleatorios
Solucion perturbarConRelocate(const Instancia& instancia, Solucion solucion, int cantidadRelocates, std::mt19937& rng) {

    std::uniform_int_distribution<int> distVendedor(0, instancia.cantidadVendedores() - 1); // sortea vendedores entre 0 y n-1

    int hechos = 0;   // relocates exitosos
    int intentos = 0; // intentos realizados
    int maxIntentos = cantidadRelocates * 10;   // evita quedar en loop infinito si no hay movimientos factibles

    while(hechos < cantidadRelocates && intentos < maxIntentos) {
        int vendedor = distVendedor(rng);   // elije un vendedor al azar
        hechos += relocateAleatorio(solucion, instancia, vendedor, rng);
        intentos++;
    }

    return solucion; // devuelve la solución perturbada
}


// Iterated Local Search
Solucion ILS(const Instancia& instancia, const Solucion& inicial, int iteraciones, double porcentajePerturbacion) {

    std::mt19937 rng(42); // semilla fija para poder reproducir resultados
    int n = instancia.cantidadVendedores();

    int cantidadRelocates = std::max(1, (int)(porcentajePerturbacion * n)); // cantidad de vendedores a relocalizar (usa max porque no tiene sentido relocate con 0)

    Solucion mejor = VNDSwapRelocate(instancia, inicial); // primero mejora la solución inicial con VND

    for(int it = 0; it < iteraciones; it++) {   // repite el proceso varias veces

        Solucion perturbada = perturbarConRelocate(instancia, mejor, cantidadRelocates, rng);   // perturba siempre la mejor solución encontrada
        Solucion nueva = VNDSwapRelocate(instancia, perturbada);    // después de perturbar, vuelve a mejorar con VND
        if(nueva.mejorQue(mejor, instancia)) {  // si la nueva solución es mejor, la guarda
            mejor = nueva;
        }
    }

    return mejor; // devuelve la mejor solución encontrada
}