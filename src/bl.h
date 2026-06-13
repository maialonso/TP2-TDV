#pragma once

#include "swap.h"
#include "relocate.h"

Solucion busquedaLocalSwap(const Instancia& instancia, Solucion actual) {

    bool mejora = true;

    while(mejora) {
        
        mejora = false;
        for(int v1 = 0; v1 < instancia.cantidadVendedores(); v1++) {

            for(int v2 = v1 + 1; v2 < instancia.cantidadVendedores(); v2++) {

                Solucion vecina = actual;

                if(swapVendedores(vecina, instancia, v1, v2)) {
                    if(vecina.mejorQue(actual, instancia)) {
                        actual = vecina;
                        mejora = true;
                    }
                }
            }
        }
    }
    return actual;
}

Solucion busquedaLocalRelocate(const Instancia& instancia, Solucion actual) {

    bool mejora = true;

    while(mejora) {
        
        mejora = false;
        for(int v1 = 0; v1 < instancia.cantidadVendedores(); v1++) {

            Solucion vecina = actual;

            if(relocate(vecina, instancia, v1)) {
                if(vecina.mejorQue(actual, instancia)) {
                    actual = vecina;
                    mejora = true;
                }
            }
        }
    }
    return actual;
}