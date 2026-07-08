#include <iostream>
#include <chrono>

#include "leer_datasets.hpp"
#include "FloydWarshall.hpp"

int main()
{
    try
    {
        // Cambia por el archivo que quieras
        Grafo g = leerGrafo("power-685-bus.mtx");

        auto inicio =
            std::chrono::high_resolution_clock::now();

        auto resultado = floydWarshall(g);

        auto fin =
            std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> tiempo =
            fin - inicio;

        std::cout
            << "Tiempo de ejecucion: "
            << tiempo.count()
            << " segundos\n";

        if(resultado.second)
        {
            std::cout
                << "Advertencia: se detecto un ciclo negativo\n";
        }

        guardarCSV(
            "distanciasMTX.csv",
            resultado.first
        );

        std::cout
            << "Archivo distanciasMTX.csv generado correctamente\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}