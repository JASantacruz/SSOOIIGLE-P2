/****************************************************************
 *                      PRACTICA 2 - SSOO II
 *                          SSOOIIGLE
 *                      
 * 
 *                      Autor: Jose Antonio Santacruz Gallego
 *                          
****************************************************************/

#include <iostream>
#include <thread>
#include <algorithm>
#include <fstream>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sstream>
#include <cstring>
#include <cctype>
#include <string>

#include "encuentro.cpp"
#include "colores.h"
#include "globalVariables.h"
#include "limpiador.cpp"
#include "calcularLineas.cpp"
#include "buscador.cpp"
#include "showResult.cpp"

/****************************************************************
 *                      FUNCION PRINCIPAL
 *                      Autor: José Antonio Santacruz Gallego
****************************************************************/
int main(int argc, char *argv[])
{
    int num_lineas = 0;

    if (argc != 4)
    {
        std::cout << "Error en la linea de ordenes.\tSintaxis: buscador <nombre_fichero> <palabra> <num_hilos>." << std::endl;
        return EXIT_FAILURE;
    }
    std::string filename = argv[1];
    std::string word = argv[2];

    for (int i = 0; i < strlen(argv[3]); i++)
        if (!std::isdigit(argv[3][i]))
        {
            std::cout << "Error en la linea de ordenes. Debes introducir un valor numerico valido mayor que 0." << std::endl;
            return EXIT_FAILURE;
        }

    int num_hilos = atoi(argv[3]);
    num_lineas = calcularLineas(filename);
    lineasPorHilo(num_lineas, num_hilos, word, filename);
    std::for_each(g_vHilos.begin(), g_vHilos.end(), std::mem_fn(&std::thread::join));
    mostrarResultado();
    return EXIT_SUCCESS;
}