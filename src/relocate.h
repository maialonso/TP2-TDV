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

bool swapVendedores(Solucion& solucion, const Instancia& instancia, int v1,int v2) {

    int d1 = solucion.depositoDe(v1);  //busco los depositos de los vendedores 
    int d2 = solucion.depositoDe(v2);

    if(d1 == -1 || d2 == -1) // si alguno de los dos no est asignado no puedo hacer swap
        return false;

    if(d1 == d2)            //si estan en el mismo depósito, no hago swap 
        return false;

    std::vector<int> capacidadRestante = instancia.capacidades(); //para no modificar la instancia, lo copio

    for(int vendedor = 0; vendedor < instancia.cantidadVendedores(); vendedor++) {

        int deposito = solucion.depositoDe(vendedor);

        if(deposito != -1) {// resta la demanda actual
            capacidadRestante[deposito] -= instancia.demanda(deposito, vendedor);
        }
    }
    
    capacidadRestante[d1] += instancia.demanda(d1, v1);
    capacidadRestante[d2] +=instancia.demanda(d2, v2);

    int nuevaDemandaV1 = instancia.demanda(d2, v1);
    int nuevaDemandaV2 = instancia.demanda(d1, v2);

    if(capacidadRestante[d2] < nuevaDemandaV1) return false;

    if(capacidadRestante[d1] < nuevaDemandaV2) return false;

    solucion.asignar(v1, d2);
    solucion.asignar(v2, d1);

    return true;
}

