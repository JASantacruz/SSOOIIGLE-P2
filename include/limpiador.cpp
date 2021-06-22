/****************************************************************
 *                      FUNCION QUE ELIMINA
 *                      LOS SIGNOS DE PUNTUACION
 *                      DE LA LINEA QUE ESTEMOS TRATANDO
 * 
 *                      Autor: José Antonio Santacruz Gallego
****************************************************************/
std::string limpiarLinea(std::string linea)
{

    std::for_each(linea.begin(), linea.end(), [](char &c)
                  { c = ::tolower(c); });

    for (int i = 0; i < linea.size(); i++)
        linea.erase(std::remove_if(linea.begin(), linea.end(), ispunct), linea.end());

    return linea;
}