#pragma once

#include <vector>
#include <string>
#include <fstream>
#include "instancia.h"

class Solucion {

private:

    // asignacion_[vendedor] = deposito
    // -1 significa no asignado
    std::vector<int> asignacion_;

public:

    explicit Solucion(int cantidadVendedores)
        : asignacion_(cantidadVendedores, -1)
    {}

    void asignar(int vendedor, int deposito) {
        asignacion_[vendedor] = deposito;
    }

    int depositoDe(int vendedor) const {
        return asignacion_[vendedor];
    }

    bool estaAsignado(int vendedor) const {
        return asignacion_[vendedor] != -1;
    }

    bool tienePenalizacion() const {
        for (int deposito : asignacion_) {

            if (deposito == -1) {
                return true;
            }
        }

        return false;
    }

    double costo(const Instancia& instancia) const {

        double total = 0.0;

        for (int vendedor = 0;
            vendedor < asignacion_.size();
            vendedor++) {

            int deposito = asignacion_[vendedor];

            if (deposito == -1) {

                total += 3.0 * instancia.cmax();

            } else {

                total += instancia.costo(deposito,
                                        vendedor);
            }
        }

        return total;
    }

    bool mejorQue(const Solucion& otra,
                  const Instancia& instancia) const {

        return costo(instancia)
             < otra.costo(instancia);
    }

    void guardar(const std::string& path,
                 const Instancia& instancia) const {

        std::ofstream archivo(path);

        int m = instancia.cantidadDepositos();

        std::vector<std::vector<int>> depositos(m);

        for (int vendedor = 0;
             vendedor < asignacion_.size();
             vendedor++) {

            int deposito = asignacion_[vendedor];

            if (deposito != -1) {
                depositos[deposito].push_back(vendedor);
            }
        }

        for (int deposito = 0;
             deposito < m;
             deposito++) {

            for (int vendedor : depositos[deposito]) {
                archivo << vendedor << " ";
            }

            archivo << "\n";
        }
    }

    const std::vector<int>& asignaciones() const {
        return asignacion_;
    }
}