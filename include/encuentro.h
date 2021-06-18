#include <iostream>
#include <thread>
#include <string>
#include <cstring>
#include <queue>
#include <fstream>
#include <cctype>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>
#include <mutex>

class encontrado
{
public:
    int id;
    int inicio_fragmento;
    std::string palabra_anterior;
    std::string palabra_encontrada;
    std::string palabra_posterior;
    int numero_linea;
    int fin_fragmento;
    encontrado(int id, std::string palabra_anterior,
               std::string palabra_encontrada,
               std::string palabra_posterior,
               int numero_linea,
               int inicio_fragmento,
               int fin_fragmento)
    {
        this->id = id;
        this->palabra_anterior = palabra_anterior;
        this->palabra_encontrada = palabra_encontrada;
        this->palabra_posterior = palabra_posterior;
        this->numero_linea = numero_linea;
        this->inicio_fragmento = inicio_fragmento;
        this->fin_fragmento = fin_fragmento;
    }
};

class CompareTo
{
public:
    bool operator()(encontrado b1, encontrado b2)
    {
        if (b1.id > b2.id)
            return true;

        else if (b1.id == b2.id && b1.numero_linea > b2.numero_linea)
            return true;

        else
        {
            return false;
        }
    }
};
