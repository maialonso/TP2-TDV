#include "instancia.h"
#include "solucion.h"
#include "swap.h"
#include "relocate.h"
#include <cmath>
#include <random>

Solucion simulatedAnnealing(const Instancia& instancia, const Solucion& inicial) {

    Solucion actual = inicial;
    Solucion mejor = inicial;

    double T = 1000.0; //t y alpha son arbitrarios
    double alpha = 0.999;
    double T_final = 0.01;

    std::mt19937 rng(42); //generador de numeros random

    //deja que cualquier vendedor y cualquier numero entre 0.0 y 1.0 tengan la misma probabilidad
    std::uniform_int_distribution<int> distVendedor(0, instancia.cantidadVendedores() - 1); 
    std::uniform_real_distribution<double> distProb(0.0, 1.0);

    while(T > T_final) {

        Solucion vecina = actual;
        
        int v1 = distVendedor(rng);//algun vendedor

        
        if(distProb(rng) < 0.5) {// usa swap
            int v2 = distVendedor(rng);
            swapVendedores(vecina, instancia, v1, v2);

        } else {//usa relocate
            relocate(vecina, instancia, v1);
        }

        double delta = vecina.costo(instancia) - actual.costo(instancia);//diferencia de costos

        if(delta < 0) {//caso de mejora, siempre se acepta
            actual = vecina;
            if(actual.mejorQue(mejor, instancia)) {
                mejor = actual;
            }
        } else {
            double p = std::exp(-delta / T);
            if(distProb(rng) < p) {//si T es muy grande, va a aceptar casi siempre
                //como T va decreciendo, cafa vez se aceptan menos soluciones que empeoren=>se vuelve cada vez mas greedy
                actual = vecina;  // aceptamos aunque sea peor
            }
        }

        T = T * alpha; //decremento de la temperatura
    }

    return mejor;
}