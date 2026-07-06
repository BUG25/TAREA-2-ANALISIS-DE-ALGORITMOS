#ifndef BELLMANFORD_HPP
#define BELLMANFORD_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <iomanip>
#include <limits>
#include <cmath>
#include "Grafo.hpp"

using namespace std;

pair<vector<double>, bool> bellmanFord(const Grafo &grafo, int origen) {

    vector<double> dist(grafo.n, INF);
    dist[origen] = 0;

    for (int i = 0; i < grafo.n - 1; i++) {

        bool actualizado = false;

        for (auto arista : grafo.aristas) {

            int u, v;
            double peso;

            tie(u, v, peso) = arista;

            if (dist[u] != INF && dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                actualizado = true;
            }
        }

        if (!actualizado)
            break;
    }

    bool cicloNegativo = false;

    for (auto arista : grafo.aristas) {

        int u, v;
        double peso;

        tie(u, v, peso) = arista;

        if (dist[u] != INF && dist[u] + peso < dist[v]) {
            cicloNegativo = true;
            break;
        }
    }

    return {dist, cicloNegativo};
}

pair<vector<vector<double>>, bool> algoritmoBase(const Grafo &grafo) {

    vector<vector<double>> distancias;
    bool cicloNegativo = false;

    for (int i = 0; i < grafo.n; i++) {

        auto resultado = bellmanFord(grafo, i);

        distancias.push_back(resultado.first);

        if (resultado.second)
            cicloNegativo = true;
    }

    return {distancias, cicloNegativo};
}
#endif