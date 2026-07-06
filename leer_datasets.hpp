#ifndef LEER_DATASETS_HPP
#define LEER_DATASETS_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <stdexcept>

#include "Grafo.hpp"

//-----------------------------------------------------
// Lectura de archivos .edges
//-----------------------------------------------------

inline Grafo leerEdges(const std::string& nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);

    if(!archivo.is_open())
        throw std::runtime_error("No se pudo abrir el archivo.");

    std::vector<std::tuple<int,int,double>> aristas;
    std::unordered_map<int,int> idMap;

    int siguienteId = 0;

    int u,v;
    double peso;

    while(archivo >> u >> v >> peso)
    {
        if(idMap.find(u)==idMap.end())
            idMap[u] = siguienteId++;

        if(idMap.find(v)==idMap.end())
            idMap[v] = siguienteId++;

        aristas.emplace_back(
            idMap[u],
            idMap[v],
            peso
        );
    }

    Grafo g(siguienteId);

    for(const auto& a : aristas)
    {
        std::tie(u,v,peso) = a;
        g.agregarArista(u,v,peso);
    }

    return g;
}

//-----------------------------------------------------
// Lectura de archivos Matrix Market (.mtx)
//-----------------------------------------------------

inline Grafo leerMTX(const std::string& nombreArchivo)
{
    std::ifstream archivo(nombreArchivo);

    if(!archivo.is_open())
        throw std::runtime_error("No se pudo abrir el archivo.");

    std::string linea;

    while(std::getline(archivo,linea))
    {
        if(linea.empty())
            continue;

        if(linea[0]=='%')
            continue;

        break;
    }

    std::stringstream ss(linea);

    int filas,columnas,noNulos;

    ss >> filas >> columnas >> noNulos;

    Grafo g(filas);

    int u,v;
    double peso;

    while(archivo >> u >> v >> peso)
    {
        // Ignorar diagonal
        if(u == v)
            continue;

        g.agregarArista(
            u - 1,
            v - 1,
            peso
        );
    }

    return g;
}

//-----------------------------------------------------
// Detecta automáticamente la extensión
//-----------------------------------------------------

inline Grafo leerGrafo(const std::string& nombreArchivo)
{
    if(nombreArchivo.size() >= 4 &&
       nombreArchivo.substr(nombreArchivo.size()-4) == ".mtx")
    {
        return leerMTX(nombreArchivo);
    }

    if(nombreArchivo.size() >= 6 &&
       nombreArchivo.substr(nombreArchivo.size()-6) == ".edges")
    {
        return leerEdges(nombreArchivo);
    }

    throw std::runtime_error("Formato no soportado.");
}

#endif