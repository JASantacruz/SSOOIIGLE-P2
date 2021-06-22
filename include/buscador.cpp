/****************************************************************
 *                                  EN ESTA CLASE
 *                      SE GUARDARÁ TODA LA LOGICA DEL BUSCADOR
 *                      Autor: José Antonio Santacruz Gallego
****************************************************************/

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