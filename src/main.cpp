#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "instancia.h"
#include "solucion.h"
#include "heuristica1.h"

int main() {

    Instancia instancia("instances/gap/gap_e/e20200");

    Solucion s = heuristica1(instancia);

    std::cout << s.costo(instancia) << std::endl;

    s.guardar("salida.txt", instancia);
}
