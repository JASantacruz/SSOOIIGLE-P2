
/****************************************************************
 *                     FUNCION QUE MUESTRA
 *                     LOS RESULTADOS DE LA BUSQUEDA
 *                     Autor: José Antonio Santacruz Gallego
****************************************************************/
void mostrarResultado()
{
    int counter = 0;

    while (!g_vEncontradas.empty())
    {

        std::cout << CYAN << "[Hilo " << g_vEncontradas.top().id << " inicio: " << g_vEncontradas.top().inicio_fragmento
                  << " - final: " << g_vEncontradas.top().fin_fragmento << "]" << RESET << ":: linea " << g_vEncontradas.top().numero_linea
                  << " :: ... " << g_vEncontradas.top().palabra_anterior << " " << GREEN << g_vEncontradas.top().palabra_encontrada
                  << RESET << " " << g_vEncontradas.top().palabra_posterior << " ... " << std::endl;
        g_vEncontradas.pop();
        counter++;
    }

    std::cout << "\n\nVeces que aparece la palabra " << GREEN << g_vEncontradas.top().palabra_encontrada << RESET << ": " << YELLOW << counter << RESET << "\n"
              << std::endl;
}