#ifndef GUARDAR_RESULTADOS_HPP
#define GUARDAR_RESULTADOS_HPP

#include <fstream>

#include "Grafo.hpp"

inline void guardarCSV(
    const std::string& nombreArchivo,
    const std::vector<std::vector<double>>& dist)
{
    std::ofstream salida(nombreArchivo);

    salida << "Nodo1,Nodo2,DistanciaMinima\n";

    int n = dist.size();

    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            salida << i << ","
                   << j << ",";

            if(dist[i][j] == INF)
                salida << "INF";
            else
                salida << dist[i][j];

            salida << "\n";
        }
    }
}

#endif