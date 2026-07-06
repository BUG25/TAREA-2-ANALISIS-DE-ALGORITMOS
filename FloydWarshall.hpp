#ifndef FLOYDWARSHALL_HPP
#define FLOYDWARSHALL_HPP   
#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include "Grafo.hpp"

using namespace std;

pair<vector<vector<double>>, bool> floydWarshall(const Grafo &grafo) {

    int n = grafo.n;

    // Copia de la matriz de adyacencia
    vector<vector<double>> dist = grafo.matriz;

    // Algoritmo Floyd-Warshall
    for (int k = 0; k < n; k++) {

        for (int i = 0; i < n; i++) {

            if (dist[i][k] == INF)
                continue;

            for (int j = 0; j < n; j++) {

                if (dist[k][j] == INF)
                    continue;

                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    // Detección de ciclo negativo
    bool cicloNegativo = false;

    for (int i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            cicloNegativo = true;
            break;
        }
    }

    return {dist, cicloNegativo};
}
#endif