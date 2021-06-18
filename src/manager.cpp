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
#include "encuentro.h"
#include "colores.h"

/****************************************************************
 *                      VARIABLES GLOBALES
****************************************************************/
std::vector<std::thread> g_vHilos;
std::vector<std::string> g_vLinea;
std::priority_queue<encontrado, std::vector<encontrado>, CompareTo> g_vEncontradas;
std::mutex g_Sem;

/****************************************************************
 *                      FUNCIONES
****************************************************************/
int calcularLineas(std::string filename);
void lineasPorHilo(int num_lineas, int num_threads, std::string word, std::string file);
void buscarPalabra(int id_hilo, int li, int ls, std::string word, std::string filename, std::vector<std::string> vWords, int linea);
void separarFichero(int idHilo, int li, int ls, std::string word, std::string filename);
void mostrarResultado();
std::vector<std::string> dividirLinea(std::string linea);
std::string limpiarLinea(std::string filename);

/****************************************************************
 *                      FUNCION QUE CALCULA 
 *                      LAS LINEAS DEL FICHERO
****************************************************************/
int calcularLineas(std::string filename)
{
    int line_counter = 0;
    std::ifstream file(filename, std::ifstream::in);
    std::string line;

    if (!file.is_open())
        exit(EXIT_FAILURE);

    while (std::getline(file, line, '\n'))
        line_counter++;

    file.close();
    return line_counter;
}

/****************************************************************
 *                      FUNCION QUE BUSCA
 *                      LA PALABRA EN EL FICHERO
****************************************************************/
void buscarPalabra(int id_hilo, int li, int ls, std::string word, std::string filename, std::vector<std::string> vWords, int linea)
{
    std::string prevWord;
    std::string nextWord;

    for (int i = 0; i < vWords.size(); i++)
    {
        if (vWords.at(i).compare(word) == 0)
        {

            if (vWords.size() == 1) //Unica palabra de la linea
            {
                prevWord = "";
                nextWord = "";
            }
            else
            {
                if (i == 0) //Primera palabra de la linea
                {
                    prevWord = "";
                }
                else
                {
                    prevWord = vWords.at(i - 1);
                }

                if (i == (vWords.size() - 1)) //Ultima palabra de la linea
                {
                    nextWord = "";
                }
                else
                {
                    nextWord = vWords.at(i + 1);
                }
            }
            std::lock_guard<std::mutex> lck(g_Sem);
            encontrado en(id_hilo, prevWord, word, nextWord, linea, li, ls);
            g_vEncontradas.push(en);
        }
    }
}

/****************************************************************
 *                      FUNCION QUE CALCULA LOS LIMITES
 *                      DE LOS HILOS Y LOS CREA
****************************************************************/
void lineasPorHilo(int num_lineas, int num_hilos, std::string word, std::string filename)
{

    int li = 0;
    int ls = 0;
    int id = 0;

    int lineasCadaHilo = num_lineas / num_hilos;

    for (int i = 0; i < num_hilos; i++)
    {
        li = (lineasCadaHilo * i) + 1;
        ls = lineasCadaHilo * (i + 1);
        id++;

        if (i == (num_hilos - 1))
        {
            ls = num_lineas + 1;
        }

        g_vHilos.push_back(std::thread(separarFichero, id, li, ls, word, filename));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

/****************************************************************
 *                      FUNCION QUE SEPARA EL
 *                      FICHERO EN LINEAS PARA IR 
 *                      TRATANDOLAS UNA A UNA
****************************************************************/
void separarFichero(int idHilo, int li, int ls, std::string word, std::string filename)
{
    std::vector<std::string> vWords;
    std::string thisLine;
    std::ifstream file(filename, std::ifstream::in);
    int linea = 1;

    while (linea <= ls)
    {
        std::getline(file, thisLine, '\n');
        if (linea >= li)
        {
            thisLine = limpiarLinea(thisLine);
            vWords = dividirLinea(thisLine);
            buscarPalabra(idHilo, li, ls, word, filename, vWords, linea);
        }
        linea++;
    }
}

/****************************************************************
 *                      FUNCION QUE CONVIERTE LA
 *                      LINEA EN UN VECTOR DE PALABRAS
****************************************************************/
std::vector<std::string> dividirLinea(std::string linea)
{
    std::vector<std::string> vWords;
    std::string token;
    std::istringstream iss(linea, std::istringstream::in);

    while (std::getline(iss, token, ' '))
    {
        vWords.push_back(token);
    }

    return vWords;
}

/****************************************************************
 *                      FUNCION QUE ELIMINA
 *                      LOS SIGNOS DE PUNTUACION
 *                      DE LA LINEA QUE ESTEMOS TRATANDO
****************************************************************/
std::string limpiarLinea(std::string linea)
{

    std::for_each(linea.begin(), linea.end(), [](char &c)
                  { c = ::tolower(c); });

    for (int i = 0; i < linea.size(); i++)
        linea.erase(std::remove_if(linea.begin(), linea.end(), ispunct), linea.end());

    return linea;
}

/****************************************************************
 *                     FUNCION QUE MUESTRA
 *                     LOS RESULTADOS DE LA BUSQUEDA
****************************************************************/
void mostrarResultado()
{
    int counter = 1;

    while (!g_vEncontradas.empty())
    {

        std::cout << CYAN << "[Hilo " << g_vEncontradas.top().id << " inicio: " << g_vEncontradas.top().inicio_fragmento
                  << " - final: " << g_vEncontradas.top().fin_fragmento << "]" << RESET << ":: linea " << g_vEncontradas.top().numero_linea
                  << " :: ... " << g_vEncontradas.top().palabra_anterior << " " << GREEN << g_vEncontradas.top().palabra_encontrada
                  << RESET << " " << g_vEncontradas.top().palabra_posterior << " ... " << std::endl;
        g_vEncontradas.pop();
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    std::cout << "\n\nVeces que aparece la palabra " << GREEN << g_vEncontradas.top().palabra_encontrada << RESET << ": " << YELLOW << counter << RESET << "\n"
              << std::endl;
}

/****************************************************************
 *                      FUNCION PRINCIPAL
****************************************************************/
int main(int argc, char *argv[])
{
    int num_lineas = 0;

    if (argc != 4)
    {
        std::cout << "Error en la linea de ordenes.\tSintaxis: buscador <nombre_fichero> <palabra> <num_hilos>" << std::endl;
        return EXIT_FAILURE;
    }
    std::string filename = argv[1];
    std::string word = argv[2];

    for (int i = 0; i < strlen(argv[3]); i++)
        if (!std::isdigit(argv[3][i]) || argv[3] <= 0)
        {
            std::cout << "Error en la linea de ordenes. Debes introducir un valor numerico valido mayor que 0" << std::endl;
            return EXIT_FAILURE;
        }

    int num_hilos = atoi(argv[3]);

    num_lineas = calcularLineas(filename);
    lineasPorHilo(num_lineas, num_hilos, word, filename);
    std::for_each(g_vHilos.begin(), g_vHilos.end(), std::mem_fn(&std::thread::join));
    mostrarResultado();
    return EXIT_SUCCESS;
}