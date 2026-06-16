#pragma once
#include <chrono>

class Timer {
private:
    std::chrono::high_resolution_clock::time_point inicio_;

public:
    Timer() {
        reiniciar();
    }

    void reiniciar() {
        inicio_ = std::chrono::high_resolution_clock::now();
    }

    double tiempoMs() const {
        auto fin = std::chrono::high_resolution_clock::now();

        return std::chrono::duration<double, std::milli>(
            fin - inicio_
        ).count();
    }
};