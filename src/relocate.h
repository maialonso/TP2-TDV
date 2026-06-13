#pragma once
#include "instancia.h"
#include "solucion.h"

bool relocate(Solucion& solucion, const Instancia& instancia, int v1){
    
    int d0= solucion.depositoDe(v1); //donde ya estaba asignado
    
    //busco el proximo deposito donde entra

    std::vector<int> capacidadRestante = instancia.capacidades(); //para no modificar la instancia, lo copio

    for(int vendedor = 0; vendedor < instancia.cantidadVendedores(); vendedor++) {

        int deposito = solucion.depositoDe(vendedor);

        if(deposito != -1) {// resta la demanda actual
            capacidadRestante[deposito] -= instancia.demanda(deposito, vendedor);
        }
    }
    //para el final de este ciclo, capacidadRestante tiene lo que le queda de capacidad a cada deposito

    //Ahora queda buscar la asignacion factible con menor costo
    int mejorDeposito = -1;
    double mejorCosto = INT_MAX;

    for(int deposito = 0; deposito < capacidadRestante.size(); deposito++) { //recorre por deposito

        int demanda = instancia.demanda(deposito, v1); 

        if(d0 != deposito){ //deposito diferente 
            if(capacidadRestante[deposito] >= demanda) {

                double costo = instancia.costo(deposito, v1);

                if(costo < mejorCosto) {

                mejorCosto = costo;
                mejorDeposito = deposito;
                }
            }
        }
    }
    //para el final de este ciclo, mejorDeposito tiene el deposito al que se le va a asignar(puede ser -1)

    if(mejorDeposito == -1) return false;

    //reasigno sino
    solucion.asignar(v1, mejorDeposito);
    return true;

}

