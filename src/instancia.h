#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <stdexcept>

class Instancia {

private:

    int m_;
    int n_;

    std::vector<std::vector<int>> costos_;
    std::vector<std::vector<int>> demandas_;
    std::vector<int> capacidades_;

    int cmax_;

public:

    explicit Instancia(const std::string& path) {

        std::ifstream archivo(path);

        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo");
        }

        archivo >> m_ >> n_;

        costos_.assign(m_, std::vector<int>(n_));
        demandas_.assign(m_, std::vector<int>(n_));
        capacidades_.assign(m_, 0);

        cmax_ = 0;

        // matriz de costos
        for (int i = 0; i < m_; i++) {
            for (int j = 0; j < n_; j++) {

                archivo >> costos_[i][j];

                cmax_ = std::max(cmax_, costos_[i][j]);
            }
        }

        // matriz de demandas
        for (int i = 0; i < m_; i++) {
            for (int j = 0; j < n_; j++) {
                archivo >> demandas_[i][j];
            }
        }

        // capacidades
        for (int i = 0; i < m_; i++) {
            archivo >> capacidades_[i];
        }
    }

    int cantidadDepositos() const {
        return m_;
    }

    int cantidadVendedores() const {
        return n_;
    }

    int costo(int deposito, int vendedor) const {
        if (deposito < 0 || deposito >= m_) {
            throw std::out_of_range("Indice de deposito invalido");
        }

        if (vendedor < 0 || vendedor >= n_) {
            throw std::out_of_range("Indice de vendedor invalido");
        }

        return costos_[deposito][vendedor];
    }

    int demanda(int deposito, int vendedor) const {

    if (deposito < 0 || deposito >= m_) {
        throw std::out_of_range("Indice de deposito invalido");
    }

    if (vendedor < 0 || vendedor >= n_) {
        throw std::out_of_range("Indice de vendedor invalido");
    }

    return demandas_[deposito][vendedor];
}

    int capacidad(int deposito) const {

        if (deposito < 0 || deposito >= m_) {
            throw std::out_of_range("Indice de deposito invalido");
        }

        return capacidades_[deposito];
    }

    int cmax() const {
        return cmax_;
    }

    const std::vector<int>& capacidades() const {
        return capacidades_;
    }

    const std::vector<std::vector<int>>& costos() const {
        return costos_;
    }

    const std::vector<std::vector<int>>& demandas() const {
        return demandas_;
    }
    };