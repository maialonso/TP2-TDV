#include "instancia.h"
#include "solucion.h"
#include "swap.h"
#include "relocate.h"
#include <cmath>
#include <random>
#include <iostream>

Solucion simulatedAnnealing(const Instancia& instancia, const Solucion& inicial) {

    Solucion actual = inicial;
    Solucion mejor = inicial;

    double costoActual = actual.costo(instancia);
    double costoMejor = costoActual;

    double T = 10000.0;
    double alpha = 0.99999;
    double T_final = 0.01;

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> distVendedor(0, instancia.cantidadVendedores() - 1);
    std::uniform_real_distribution<double> distProb(0.0, 1.0);

    int mejoras = 0;
    int aceptadas = 0;
    int rechazadas = 0;

    std::cout << "Iniciando SA..." << std::endl;

    while(T > T_final) {

        int v1 = distVendedor(rng);
        int v2 = distVendedor(rng);

        //ve si dada la probabilidad va a elegir hacer swap o relocate
        bool esSwap = (distProb(rng) < 0.5) 
                   && (actual.depositoDe(v1) != -1) 
                   && (actual.depositoDe(v2) != -1);

        double delta = 0.0;
        bool movimientoValido = false;
        int depositoNuevo = -1;


        
        if(esSwap) {
            int d1 = actual.depositoDe(v1);
            int d2 = actual.depositoDe(v2);

            if(v1 != v2 && d1 != d2) {
                Solucion vecina = actual;
                //si es swap posible
                if(swapVendedores(vecina, instancia, v1, v2)) {
                    delta = instancia.costo(d2, v1) + instancia.costo(d1, v2)
                          - instancia.costo(d1, v1) - instancia.costo(d2, v2);
                    movimientoValido = true;//uso de variable adicional para no volver a ejecutar swapVendedores
                }
            }
        } else {
            Solucion vecina = actual;
            if(relocateAleatorio(vecina, instancia, v1, rng)) {
                int d1 = actual.depositoDe(v1);
                depositoNuevo = vecina.depositoDe(v1);
                if(d1 == -1) {
                    delta = instancia.costo(depositoNuevo, v1) - 3.0 * instancia.cmax();
                } else {
                    delta = instancia.costo(depositoNuevo, v1) - instancia.costo(d1, v1);
                }
                movimientoValido = true;
            }
        }

        
        if(movimientoValido) {
            if(delta < 0 || distProb(rng) < std::exp(-delta / T)) { //caso de si hay mejora=>acepta siempre o si sale un numero aleatorio
                if(delta < 0) mejoras++;                            // que es mas chico que ...
                else aceptadas++;

                if(esSwap) {
                    swapVendedores(actual, instancia, v1, v2); //hace el swap sobre vendedores
                } else {
                    actual.asignar(v1, depositoNuevo); //reasigna
                }

                costoActual += delta;

                if(costoActual < costoMejor) {
                    costoMejor = costoActual;
                    mejor = actual;
                    
                }
            } else {
                rechazadas++;
            }
        }

        T = T * alpha;
    }

    std::cout << "Mejoras: " << mejoras << std::endl;
    std::cout << "Aceptadas peores: " << aceptadas << std::endl;
    std::cout << "Rechazadas: " << rechazadas << std::endl;

    return mejor;
}