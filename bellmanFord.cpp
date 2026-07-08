#include <iostream>
#include <vector>
#include <tuple>
#include <iomanip>
#include <limits>
#include <cmath>
#include "Grafo.hpp"
#include "bellmanFord.hpp"

using namespace std;

void imprimirMatriz(const vector<vector<double>> &dist, const string &nombre) {

    int n = dist.size();

    cout << "\n=== " << nombre << " ===\n";

    cout << "      ";
    for (int j = 0; j < n; j++)
        cout << setw(7) << j;
    cout << endl;

    for (int i = 0; i < n; i++) {

        cout << setw(3) << i << ": ";

        for (int j = 0; j < n; j++) {

            if (dist[i][j] == INF)
                cout << setw(7) << "INF";
            else
                cout << setw(7) << dist[i][j];
        }

        cout << endl;
    }
}

bool verificarCorrectitud(const vector<vector<double>> &resultado,
                          const vector<vector<double>> &esperado,
                          const string &nombre) {

    int n = resultado.size();

    for (int i = 0; i < n; i++) {

        for (int j = 0; j < n; j++) {

            double a = resultado[i][j];
            double b = esperado[i][j];

            if (a == INF && b == INF)
                continue;

            if (a == INF || b == INF) {
                cout << "FALLO en (" << i << "," << j << ")\n";
                return false;
            }

            if (fabs(a - b) > 1e-9) {
                cout << "FALLO en (" << i << "," << j
                     << "): base=" << a
                     << " esperado=" << b << endl;
                return false;
            }
        }
    }

    cout << "OK - " << nombre << " coincide con resultado esperado.\n";
    return true;
}

void testCasoSimple() {

    cout << "\n[TEST 1] Grafo simple con pesos positivos\n";

    Grafo g(4);

    g.agregarArista(0,1,3);
    g.agregarArista(0,2,8);
    g.agregarArista(1,2,2);
    g.agregarArista(1,3,5);
    g.agregarArista(2,3,1);
    g.agregarArista(3,0,4);

    auto resultado = algoritmoBase(g);

    imprimirMatriz(resultado.first, "Algoritmo Base");

    cout << "Ciclo negativo detectado: "
         << (resultado.second ? "True" : "False") << endl;

    vector<vector<double>> esperado = {
        {0,3,5,6},
        {7,0,2,3},
        {5,8,0,1},
        {4,7,9,0}
    };

    verificarCorrectitud(resultado.first, esperado, "Test 1");
}

void testPesosNegativos() {

    cout << "\n[TEST 2] Grafo con pesos negativos (sin ciclo negativo)\n";

    Grafo g(3);

    g.agregarArista(0,1,4);
    g.agregarArista(0,2,2);
    g.agregarArista(1,2,-1);
    g.agregarArista(2,1,1);

    auto resultado = algoritmoBase(g);

    imprimirMatriz(resultado.first, "Algoritmo Base");

    cout << "Ciclo negativo detectado: "
         << (resultado.second ? "True" : "False") << endl;

    vector<vector<double>> esperado = {
        {0,3,2},
        {INF,0,-1},
        {INF,1,0}
    };

    verificarCorrectitud(resultado.first, esperado, "Test 2");
}

void testCicloNegativo() {

    cout << "\n[TEST 3] Grafo con ciclo negativo\n";

    Grafo g(3);

    g.agregarArista(0,1,1);
    g.agregarArista(1,2,-3);
    g.agregarArista(2,1,1);

    auto resultado = algoritmoBase(g);

    cout << "Ciclo negativo detectado: "
         << (resultado.second ? "True" : "False")
         << " (esperado: True)\n";

    if (resultado.second)
        cout << "OK - ciclo negativo detectado correctamente.\n";
    else
        cout << "ERROR: no detectó el ciclo negativo.\n";
}

void testGrafoDesconectado() {

    cout << "\n[TEST 4] Grafo desconectado\n";

    Grafo g(4);

    g.agregarArista(0,1,5);
    g.agregarArista(2,3,3);

    auto resultado = algoritmoBase(g);

    imprimirMatriz(resultado.first, "Algoritmo Base");

    vector<vector<double>> esperado = {
        {0,5,INF,INF},
        {INF,0,INF,INF},
        {INF,INF,0,3},
        {INF,INF,INF,0}
    };

    verificarCorrectitud(resultado.first, esperado, "Test 4");
}

int main() {

    testCasoSimple();
    testPesosNegativos();
    testCicloNegativo();
    testGrafoDesconectado();

    cout << "\nTodos los tests finalizados.\n";

    return 0;
}