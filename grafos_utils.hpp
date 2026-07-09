#ifndef GENERADOR_INSTANCIAS_HPP
#define GENERADOR_INSTANCIAS_HPP

#include <random>
#include "Grafo.hpp" 

using namespace std;

class GeneradorInstancias {

private:

    mt19937 rng;

public:

    GeneradorInstancias(unsigned int semilla = random_device{}())
        : rng(semilla) {}

    // 1. Grafo completo

    Grafo completo(int n, int minPeso = 1, int maxPeso = 100)
    {
        Grafo g(n);

        uniform_int_distribution<int> peso(minPeso, maxPeso);

        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(i!=j)
                    g.agregarArista(i,j,peso(rng));

        return g;
    }
    // 2. Camino (muy disperso)
    Grafo camino(int n, int minPeso = 1, int maxPeso = 100)
    {
        Grafo g(n);

        uniform_int_distribution<int> peso(minPeso,maxPeso);

        for(int i=0;i<n-1;i++)
            g.agregarArista(i,i+1,peso(rng));

        return g;
    }
   

    // 3. Aleatorio con densidad
    Grafo aleatorio(int n, double densidad, int minPeso = 1, int maxPeso = 100)
    {
        Grafo g(n);

        uniform_real_distribution<double> prob(0.0,1.0);
        uniform_int_distribution<int> peso(minPeso,maxPeso);

        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(i==j)
                    continue;

                if(prob(rng)<=densidad)
                    g.agregarArista(i,j,peso(rng));
            }
        }

        return g;
    }

    // 5. Aleatorio con pesos negativos (SIN ciclos negativos)

    Grafo negativos(int n,double densidad)
    {
        Grafo g(n);

        uniform_real_distribution<double> prob(0.0,1.0);
        uniform_int_distribution<int> positivo(1,20);
        uniform_int_distribution<int> negativo(-5,-1);

        // Se construye como DAG (i < j), por lo que nunca habrá ciclos.
        for(int i=0;i<n;i++)
        {
            for(int j=i+1;j<n;j++)
            {
                if(prob(rng)<=densidad)
                {
                    if(prob(rng)<0.20)
                        g.agregarArista(i,j,negativo(rng));
                    else
                        g.agregarArista(i,j,positivo(rng));
                }
            }
        }

        return g;
    }

};

#endif