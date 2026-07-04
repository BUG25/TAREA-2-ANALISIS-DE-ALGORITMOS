#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const double INF = numeric_limits<double>::infinity();

class Grafo {
public:
    int n;
    vector<vector<double>> matriz;

    Grafo(int vertices) {
        n = vertices;
        matriz.assign(n, vector<double>(n, INF));

        for (int i = 0; i < n; i++)
            matriz[i][i] = 0;
    }

    void agregarArista(int u, int v, double peso) {
        matriz[u][v] = peso;
    }
};

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

void imprimirMatriz(const vector<vector<double>> &dist,
                    const string &nombre) {

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

            if (abs(a - b) > 1e-9) {
                cout << "FALLO en (" << i << "," << j
                     << "): obtenido=" << a
                     << " esperado=" << b << endl;
                return false;
            }
        }
    }

    cout << "OK - " << nombre << " correcto.\n";
    return true;
}

void testCasoSimple() {

    cout << "\n[TEST 1] Grafo simple\n";

    Grafo g(4);

    g.agregarArista(0,1,3);
    g.agregarArista(0,2,8);
    g.agregarArista(1,2,2);
    g.agregarArista(1,3,5);
    g.agregarArista(2,3,1);
    g.agregarArista(3,0,4);

    auto resultado = floydWarshall(g);

    imprimirMatriz(resultado.first,"Floyd-Warshall");

    cout << "Ciclo negativo: "
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

    cout << "\n[TEST 2] Pesos negativos (sin ciclo)\n";

    Grafo g(3);

    g.agregarArista(0,1,4);
    g.agregarArista(0,2,2);
    g.agregarArista(1,2,-1);
    g.agregarArista(2,1,1);

    auto resultado = floydWarshall(g);

    imprimirMatriz(resultado.first,"Floyd-Warshall");

    cout << "Ciclo negativo: "
         << (resultado.second ? "True" : "False") << endl;

    vector<vector<double>> esperado = {
        {0,3,2},
        {INF,0,-1},
        {INF,1,0}
    };

    verificarCorrectitud(resultado.first, esperado, "Test 2");
}

void testCicloNegativo() {

    cout << "\n[TEST 3] Ciclo negativo\n";

    Grafo g(3);

    g.agregarArista(0,1,1);
    g.agregarArista(1,2,-3);
    g.agregarArista(2,1,1);

    auto resultado = floydWarshall(g);

    cout << "Ciclo negativo: "
         << (resultado.second ? "True" : "False")
         << " (esperado: True)\n";
}

void testDesconectado() {

    cout << "\n[TEST 4] Grafo desconectado\n";

    Grafo g(4);

    g.agregarArista(0,1,5);
    g.agregarArista(2,3,3);

    auto resultado = floydWarshall(g);

    imprimirMatriz(resultado.first,"Floyd-Warshall");

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
    testDesconectado();

    cout << "\nTodos los tests finalizaron correctamente.\n";

    return 0;
}