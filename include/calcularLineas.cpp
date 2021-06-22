/****************************************************************
 *                      FUNCION QUE CALCULA 
 *                      LAS LINEAS DEL FICHERO
 *                      Autor: José Antonio Santacruz Gallego
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