#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <tuple>
#include <limits>

const double INF = std::numeric_limits<double>::infinity();

class Grafo {
public:

    int n;

    // Matriz de adyacencia
    std::vector<std::vector<double>> matriz;

    // Lista de aristas (Bellman-Ford)
    std::vector<std::tuple<int,int,double>> aristas;

    Grafo(int vertices)
    {
        n = vertices;

        matriz.assign(n, std::vector<double>(n, INF));

        for(int i = 0; i < n; i++)
            matriz[i][i] = 0;
    }

    void agregarArista(int u, int v, double peso)
    {
        matriz[u][v] = peso;
        aristas.push_back(std::make_tuple(u, v, peso));
    }
};

#endif