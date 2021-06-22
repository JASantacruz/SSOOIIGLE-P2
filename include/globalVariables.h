/****************************************************************
 *                      VARIABLES GLOBALES
 *                      Autor: José Antonio Santacruz Gallego
****************************************************************/
std::vector<std::thread> g_vHilos;
std::vector<std::string> g_vLinea;
std::priority_queue<encontrado, std::vector<encontrado>, CompareTo> g_vEncontradas;
std::mutex g_Sem;