#pragma once
#include <vector>
#include <climits>
#include "instancia.h"
#include "solucion.h"

class Solver {

private:

    const Instancia& instancia_;
public:

    Solver(const Instancia& instancia): instancia_(instancia) {}

    std::vector<int> capacidadRestante(const Solucion& solucion) const {
        std::vector<int> capacidad = instancia_.capacidades();

        for(int vendedor = 0; vendedor < instancia_.cantidadVendedores(); vendedor++) {

            int deposito = solucion.depositoDe(vendedor);

            if(deposito != -1) {
                capacidad[deposito] -= instancia_.demanda(deposito, vendedor);
            }
        }
        return capacidad;
    }


    int mejorDepositoFactible(int vendedor, const std::vector<int>& capacidad) const {
        int mejorDeposito = -1;
        double mejorCosto = INT_MAX;

        for(int deposito = 0; deposito < instancia_.cantidadDepositos(); deposito++) {
            int demanda = instancia_.demanda(deposito, vendedor);
            
            if(capacidad[deposito] >= demanda) {
                double costo = instancia_.costo(deposito, vendedor);

                if(costo < mejorCosto) {
                    mejorCosto = costo;
                    mejorDeposito = deposito;
                }
            }
        }
        return mejorDeposito;
    }

    int mejorDepositoFactibleDistinto(int vendedor, int depositoActual, const std::vector<int>& capacidadRestante) const {

        int mejorDeposito = -1;
        double mejorCosto = INT_MAX;

        for(int deposito = 0; deposito < capacidadRestante.size(); deposito++) {
            int demanda = instancia_.demanda(deposito, vendedor);
            
            if(deposito != depositoActual) {
                if(capacidadRestante[deposito] >= demanda) {
                    double costo = instancia_.costo(deposito, vendedor);

                    if(costo < mejorCosto) {
                        mejorCosto = costo;
                        mejorDeposito = deposito;
                    }
                }
            }
        }
        return mejorDeposito;
    }
};
