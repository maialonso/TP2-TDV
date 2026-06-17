#include "solver.h"
#include <algorithm>
#include <numeric>
#include <vector>
#include <climits>
#include <random>


Solver::Solver(const Instancia& instancia)
    : instancia_(instancia) {}



// FUNCIONES AUXILIARES


// calcula la capacidad restante de cada depósito
std::vector<int> Solver::capacidadRestante(const Solucion& solucion) const {

    std::vector<int> capacidad = instancia_.capacidades();  // comienza con las capacidades originales de la instancia
    
    for(int vendedor = 0; vendedor < instancia_.cantidadVendedores(); vendedor++) { // recorre todos los vendedores
        int deposito = solucion.depositoDe(vendedor);
        if(deposito != -1) {    // si el vendedor está asignado descuenta su demanda de la capacidad disponible
            capacidad[deposito] -= instancia_.demanda(deposito, vendedor);
        }
    }

    return capacidad;   // devuelve la capacidad restante de cada depósito
}


// busca el depósito factible de menor costo para un vendedor
int Solver::mejorDepositoFactible(int vendedor, const std::vector<int>& capacidad) const {

    int mejorDeposito = -1; // mejor depósito encontrado hasta el momento
    double mejorCosto = INT_MAX;    // menor costo encontrado hasta el momento

    for(int deposito = 0; deposito < instancia_.cantidadDepositos(); deposito++) {

        int demanda = instancia_.demanda(deposito, vendedor);

        if(capacidad[deposito] >= demanda) {    // verifica si el depósito tiene capacidad suficiente
            double costo = instancia_.costo(deposito, vendedor);
            if(costo < mejorCosto) {    // si el costo es menor al mejor encontrado actualiza el depósito candidato
                mejorCosto = costo;
                mejorDeposito = deposito;
            }
        }
    }

    return mejorDeposito;
}


// busca el depósito factible de menor costo distinto al depósito actual
int Solver::mejorDepositoFactibleDistinto(int vendedor, int depositoActual, const std::vector<int>& capacidadRestante) const {

    int mejorDeposito = -1; // mejor depósito encontrado hasta el momento
    double mejorCosto = INT_MAX;    // menor costo encontrado hasta el momento

    for(int deposito = 0; deposito < capacidadRestante.size(); deposito++) {

        int demanda = instancia_.demanda(deposito, vendedor);

        if(deposito != depositoActual && capacidadRestante[deposito] >= demanda) {  // verifica que el depósito sea distinto al actual y que tenga capacidad suficiente
            double costo = instancia_.costo(deposito, vendedor);
            if(costo < mejorCosto) {    // si el costo es menor al mejor encontrado actualiza el depósito candidato
                mejorCosto = costo;
                mejorDeposito = deposito;
            }
        }
    }

    return mejorDeposito;
}



//  HEURISTICAS 


// asigna los vendedores en el orden original de la instancia utilizando el depósito factible de menor costo
Solucion Solver::heuristicaSecuencial() const {

    int n = instancia_.cantidadVendedores();
    Solucion solucion(n);   // solución inicialmente vacía
    std::vector<int> capacidades = instancia_.capacidades(); // copia de capacidades para ir actualizándolas

    for(int vendedor = 0; vendedor < n; vendedor++) { // recorre los vendedores en orden

        int mejorDeposito = mejorDepositoFactible(vendedor, capacidades);   // busca el depósito factible de menor costo
        if(mejorDeposito != -1) {  // si encontró alguno, asigna
            solucion.asignar(vendedor, mejorDeposito);
            capacidades[mejorDeposito] -= instancia_.demanda(mejorDeposito, vendedor);   // actualiza la capacidad restante del depósito elegido
        }
    }

    return solucion;  // devuelve la solución construida
}


// ordena los vendedores según su demanda máxima y los asigna al depósito factible de menor costo
Solucion Solver::heuristicaDemandaMax() const {

    int n = instancia_.cantidadVendedores(); 
    int m = instancia_.cantidadDepositos();  
    
    Solucion solucion(n); // solución inicialmente vacía
    std::vector<int> capacidades = instancia_.capacidades(); // copia de capacidades para ir actualizándolas
    std::vector<int> vendedores(n); // vector donde se guardan los vendedores

    std::iota(vendedores.begin(), vendedores.end(), 0); // crea vendedores = [0, 1, 2, ..., n-1]
    
    std::sort(vendedores.begin(), vendedores.end(), [&](int v1, int v2) {   // ordena los vendedores de mayor demanda máxima a menor demanda máxima

            int demandaMax1 = 0; 
            int demandaMax2 = 0; 

            for(int deposito = 0; deposito < m; deposito++) {   // recorre todos los depósitos para calcular la demanda máxima de cada vendedor
                demandaMax1 = std::max(demandaMax1, instancia_.demanda(deposito, v1));
                demandaMax2 = std::max(demandaMax2, instancia_.demanda(deposito, v2));
            }

            return demandaMax1 > demandaMax2;   // coloca primero al vendedor con mayor demanda máxima
        }
    );

    for(int vendedor : vendedores) {    // recorre los vendedores en el orden definido

        int mejorDeposito = mejorDepositoFactible(vendedor, capacidades);   // busca el depósito factible de menor costo para este vendedor

        if(mejorDeposito != -1) {   // si existe un depósito factible, asigna el vendedor
            solucion.asignar(vendedor, mejorDeposito);  
            capacidades[mejorDeposito] -= instancia_.demanda(mejorDeposito, vendedor);  // actualiza la capacidad restante del depósito elegido
        }
    }

    return solucion; // devuelve la solución construida
}


// ordena los vendedores según su demanda promedio y los asigna al depósito factible de menor costo
Solucion Solver::heuristicaDemandaProm() const {

    int n = instancia_.cantidadVendedores();
    int m = instancia_.cantidadDepositos();

    Solucion solucion(n);   // solución inicialmente vacía
    std::vector<int> capacidades = instancia_.capacidades(); // copia de capacidades para ir actualizándolas
    std::vector<int> vendedores(n); // vector con todos los vendedores

    std::iota(vendedores.begin(), vendedores.end(), 0); // crea vendedores = [0, 1, 2, ..., n-1]

    std::sort(vendedores.begin(), vendedores.end(), [&](int v1, int v2) {   // ordena los vendedores de mayor demanda promedio a menor demanda promedio

            double promedio1 = 0.0; 
            double promedio2 = 0.0; 

            for(int deposito = 0; deposito < m; deposito++) {   // recorre todos los depósitos para sumar las demandas
                promedio1 += instancia_.demanda(deposito, v1);
                promedio2 += instancia_.demanda(deposito, v2);
            }

            // divide por la cantidad de depósitos para obtener el promedio
            promedio1 /= m;
            promedio2 /= m;

            return promedio1 > promedio2;   // coloca primero al vendedor con mayor demanda promedio
        }
    );

    for(int vendedor : vendedores) { // recorre los vendedores en el orden definido

        int mejorDeposito = mejorDepositoFactible(vendedor, capacidades); // busca el depósito factible de menor costo

        if(mejorDeposito != -1) { // si encontró depósito factible, asigna
            solucion.asignar(vendedor, mejorDeposito);
            capacidades[mejorDeposito] -= instancia_.demanda(mejorDeposito, vendedor); // actualiza capacidad restante
        }
    }

    return solucion; // devuelve la solución construida
}



//  OPERADORES / MOVIMIENTOS


// intenta intercambiar los depósitos asignados a dos vendedores
bool Solver::swapVendedores(Solucion& solucion, int v1, int v2) const {
    int d1 = solucion.depositoDe(v1); // depósito actual de v1
    int d2 = solucion.depositoDe(v2); // depósito actual de v2

    if(d1 == -1 || d2 == -1) return false; // si alguno no está asignado, no se puede hacer swap
    if(d1 == d2) return false; // si están en el mismo depósito, el swap no cambia nada

    std::vector<int> capacidadRestante_ = capacidadRestante(solucion); // calcula la capacidad restante de cada depósito

    // libera temporalmente el espacio ocupado por ambos vendedores
    capacidadRestante_[d1] += instancia_.demanda(d1, v1);
    capacidadRestante_[d2] += instancia_.demanda(d2, v2);

     // calcula la demanda que tendría cada vendedor luego de realizar el intercambio
    int nuevaDemandaV1 = instancia_.demanda(d2, v1);
    int nuevaDemandaV2 = instancia_.demanda(d1, v2);

    if(capacidadRestante_[d2] < nuevaDemandaV1) return false; // verifica si v1 entra en el depósito de v2
    if(capacidadRestante_[d1] < nuevaDemandaV2) return false;// verifica si v2 entra en el depósito de v1

    // si ambas asignaciones son factibles realiza el intercambio
    solucion.asignar(v1, d2);
    solucion.asignar(v2, d1);

    return true; // intercambio exitoso
}


// intenta mover un vendedor asignado a otro depósito factible de menor costo (versión original)
bool Solver::relocateViejo(Solucion& solucion,int v1) const {

    int d0 = solucion.depositoDe(v1); // depósito actual del vendedor
    std::vector<int> capacidadRestante_ = capacidadRestante(solucion); // calcula la capacidad restante de cada depósito

    int mejorDeposito = mejorDepositoFactibleDistinto(v1, d0, capacidadRestante_); // busca el depósito factible de menor costo distinto al depósito actual
    if(mejorDeposito == -1) return false;  // si no existe un depósito factible, no hace el movimiento

    solucion.asignar(v1, mejorDeposito); // reasigna el vendedor al nuevo depósito
    return true;    // movimiento exitoso
}


// intenta mover un vendedor a otro depósito factible considerando también el depósito -1 (no asignados)(versión final)
bool Solver::relocate(Solucion& solucion, int v1) const {

    int d0 = solucion.depositoDe(v1); // depósito actual del vendedor (-1 si no está asignado)
    std::vector<int> capacidadRestante_ = capacidadRestante(solucion);  // calcula la capacidad restante de cada depósito real

    int mejorDeposito = -1; // mejor destino encontrado
    double mejorCosto = 3.0 * instancia_.cmax(); // costo de dejarlo sin asignar

    for(int deposito = 0; deposito < instancia_.cantidadDepositos(); deposito++) {   // recorre todos los depósitos reales

        if(deposito == d0) continue;   // no considera quedarse en el mismo depósito
        int demanda = instancia_.demanda(deposito, v1);

        if(capacidadRestante_[deposito] >= demanda) {   // verifica si entra en el depósito candidato

            double costo = instancia_.costo(deposito, v1);

            if(costo < mejorCosto) {    // guarda el depósito factible de menor costo
                mejorCosto = costo;
                mejorDeposito = deposito;
            }
        }
    }

    if(mejorDeposito == d0) return false;   // si el mejor destino es el mismo estado actual no realiza ningún movimiento
    solucion.asignar(v1, mejorDeposito);    // reasigna el vendedor al mejor destino encontrado (depósito real o -1)
    return true; // movimiento realizado
}


// intenta mover un vendedor a un destino factible elegido al azar considerando también el depósito -1
bool Solver::relocateAleatorio( Solucion& solucion, int v1, std::mt19937& rng) const {

    int d0 = solucion.depositoDe(v1); // depósito actual del vendedor (-1 si no está asignado)

    std::vector<int> capacidadRestante_ = capacidadRestante(solucion);  // calcula la capacidad restante de cada depósito real
    std::vector<int> factibles;  // guarda todos los destinos posibles

    // si el vendedor está asignado  permite moverlo al depósito de no asignados
    if(d0 != -1) factibles.push_back(-1);

    // recorre todos los depósitos reales
    for(int deposito = 0; deposito < instancia_.cantidadDepositos(); deposito++) {

        // no considera quedarse en el mismo depósito
        if(deposito == d0) continue;
        int demanda = instancia_.demanda(deposito, v1);
        if(capacidadRestante_[deposito] >= demanda) factibles.push_back(deposito);  // si el vendedor entra en el depósito lo agrega como destino posible
    }

    if(factibles.empty()) return false;   // si no hay ningún destino posible no realiza ningún movimiento

    std::uniform_int_distribution<int> dist( 0, factibles.size() - 1);  // elige un destino factible al azar
    int depositoElegido = factibles[dist(rng)];
    solucion.asignar(v1, depositoElegido);  // reasigna el vendedor al destino elegido puede ser un depósito real o -1
    return true; // movimiento realizado
}



// BUSQUEDA LOCAL


Solucion Solver::busquedaLocalSwap(Solucion actual) const {
    bool mejora = true;
    while(mejora) {
        mejora = false;

        for(int v1 = 0; v1 < instancia_.cantidadVendedores(); v1++) {

            for(int v2 = v1 + 1; v2 < instancia_.cantidadVendedores(); v2++) {
                Solucion vecina = actual;
                if(swapVendedores(vecina, v1, v2)) {
                    if(vecina.mejorQue(actual, instancia_)) {
                        actual = vecina;
                        mejora = true;
                    }
                }
            }
        }
    }
    return actual;
}
Solucion Solver::busquedaLocalRelocate(Solucion actual) const {
    bool mejora = true;
    while(mejora) {

        mejora = false;
        for(int v1 = 0; v1 < instancia_.cantidadVendedores(); v1++) {

            Solucion vecina = actual;
            if(relocate(vecina, v1)) {
                if(vecina.mejorQue(actual, instancia_)) {
                    actual = vecina;
                    mejora = true;
                }
            }
        }
    }
    return actual;
}
    // METAHEURISTICAS

    
// aplica búsqueda local combinando swap y relocate
Solucion Solver::VNDSwapRelocate(Solucion actual) const {
    while(true) { // repite hasta que ningún operador mejore
        actual = busquedaLocalSwap(actual); // prueba todos los swaps posibles
        Solucion despuesSwap = actual;  // guarda cómo quedó después de swap
        Solucion despuesRelocate = busquedaLocalRelocate(actual); // prueba todos los relocates posibles

        if(despuesRelocate.mejorQue(despuesSwap, instancia_)) { // si relocate mejoró, actualiza y vuelve a swap
            actual = despuesRelocate;
            continue;  // volver a swap
        }

        break; // si relocate no mejoró, termina
    }

    return actual; // devuelve el min local encontrado
}

// perturba una solución con relocates aleatorios
Solucion Solver::perturbarConRelocate(Solucion solucion, int cantidadRelocates,std::mt19937& rng) const {
    std::uniform_int_distribution<int> distVendedor(0, instancia_.cantidadVendedores() - 1); // sortea vendedores entre 0 y n-1
    int hechos = 0; // relocates exitosos
    int intentos = 0; // intentos realizados
    int maxIntentos = cantidadRelocates * 10; // evita quedar en loop infinito si no hay movimientos factibles

    while(hechos < cantidadRelocates && intentos < maxIntentos) {
        int vendedor = distVendedor(rng); // elije un vendedor al azar
        hechos += relocateAleatorio(solucion, vendedor, rng);
        intentos++;
    }

    return solucion; // devuelve la solución perturbada
}

// Iterated Local Search
Solucion Solver::ILS(const Solucion& inicial, int iteraciones, double porcentajePerturbacion) const {
    std::mt19937 rng(42); // semilla fija para poder reproducir resultados
    int n = instancia_.cantidadVendedores();

    int cantidadRelocates = std::max(1, (int)(porcentajePerturbacion * n)); // cantidad de vendedores a relocalizar (usa max porque no tiene sentido relocate con 0)
    Solucion mejor = VNDSwapRelocate(inicial); // primero mejora la solución inicial con VND
    
    for(int it = 0; it < iteraciones; it++) { // repite el proceso varias veces
        Solucion perturbada = perturbarConRelocate(mejor, cantidadRelocates, rng);  // perturba siempre la mejor solución encontrada
        Solucion nueva = VNDSwapRelocate(perturbada);  // después de perturbar, vuelve a mejorar con VND
        if(nueva.mejorQue( mejor, instancia_)) { // si la nueva solución es mejor, la guarda
            mejor = nueva;
        }
    }
    return mejor; // devuelve la mejor solución encontrada
}