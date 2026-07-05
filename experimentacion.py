import random
import time
import math
import csv

INF = float('inf')

# ==========================================
# 1. IMPLEMENTACIÓN DE LOS ALGORITMOS (NATIVA)
# ==========================================

def bellman_ford(n, aristas, origen):
    dist = [INF] * n
    dist[origen] = 0
    
    for _ in range(n - 1):
        actualizado = False
        for u, v, peso in aristas:
            if dist[u] != INF and dist[u] + peso < dist[v]:
                dist[v] = dist[u] + peso
                actualizado = False
        if actualizado:
            break
            
    # Detección de ciclo negativo
    for u, v, peso in aristas:
        if dist[u] != INF and dist[u] + peso < dist[v]:
            return dist, True
            
    return dist, False

def algoritmo_base(n, aristas):
    matriz_dist = []
    ciclo_negativo = False
    for i in range(n):
        dist, c_neg = bellman_ford(n, aristas, i)
        matriz_dist.append(dist)
        if c_neg:
            ciclo_negativo = True
    return matriz_dist, ciclo_negativo

def floyd_warshall(n, matriz_ady):
    # Copia in-place para no destruir la matriz original
    dist = [fila[:] for fila in matriz_ady]
    
    for k in range(n):
        for i in range(n):
            if dist[i][k] == INF:
                continue
            for j in range(n):
                if dist[k][j] == INF:
                    continue
                if dist[i][k] + dist[k][j] < dist[i][j]:
                    dist[i][j] = dist[i][k] + dist[k][j]
                    
    ciclo_negativo = any(dist[i][i] < 0 for i in range(n))
    return dist, ciclo_negativo

# ==========================================
# 2. GENERADOR DE INSTANCIAS ALEATORIAS
# ==========================================

def generar_grafo_aleatorio(n, densidad, peso_min=1, peso_max=20):
    """
    densidad: float entre 0 y 1 (0.1 = disperso, 0.8 = denso)
    """
    matriz_ady = [[0 if i == j else INF for j in range(n)] for i in range(n)]
    aristas = []
    
    for i in range(n):
        for j in range(n):
            if i != j:
                if random.random() < densidad:
                    peso = random.randint(peso_min, peso_max)
                    matriz_ady[i][j] = peso
                    aristas.append((i, j, peso))
                    
    return n, aristas, matriz_ady

# ==========================================
# 3. MÉTRICAS ESTADÍSTICAS (32 EJECUCIONES)
# ==========================================

def calcular_estadisticas(tiempos):
    m = len(tiempos)
    promedio = sum(tiempos) / m
    varianza = sum((x - promedio) ** 2 for x in tiempos) / (m - 1) if m > 1 else 0
    desv_est = math.sqrt(varianza)
    return promedio, desv_est

# ==========================================
# 4. CIRCUITO DE EXPERIMENTOS
# ==========================================

def ejecutar_experimentos():
    # Tamaños de prueba (ajusta según la potencia de tu PC para que no tarde horas)
    tamanos_n = [10, 20, 40, 60, 80, 100]
    densidad = 0.3  # Estructura fija para este test
    pasadas = 32    # Requerimiento estricto de la tarea
    
    resultados = []
    
    print(f"Iniciando experimentos ({pasadas} ejecuciones por tamaño)...")
    print(f"{'N':<6}{'Base Avg (s)':<15}{'Base Std (s)':<15}{'Floyd Avg (s)':<15}{'Floyd Std (s)':<15}")
    print("-" * 70)
    
    for n in tamanos_n:
        tiempos_base = []
        tiempos_floyd = []
        
        for _ in range(pasadas):
            n, aristas, matriz_ady = generar_grafo_aleatorio(n, densidad)
            
            # Control de tiempo Algoritmo Base
            t0 = time.perf_counter()
            _, _ = algoritmo_base(n, aristas)
            t1 = time.perf_counter()
            tiempos_base.append(t1 - t0)
            
            # Control de tiempo Floyd-Warshall
            t2 = time.perf_counter()
            _, _ = floyd_warshall(n, matriz_ady)
            t3 = time.perf_counter()
            tiempos_floyd.append(t3 - t2)
            
        avg_base, std_base = calcular_estadisticas(tiempos_base)
        avg_floyd, std_floyd = calcular_estadisticas(tiempos_floyd)
        
        print(f"{n:<6}{avg_base:<15.6f}{std_base:<15.6f}{avg_floyd:<15.6f}{std_floyd:<15.6f}")
        
        resultados.append({
            'n': n,
            'base_avg': avg_base,
            'base_std': std_base,
            'floyd_avg': avg_floyd,
            'floyd_std': std_floyd
        })
        
    # Guardar datos en CSV para graficar después
    with open('resultados_experimentos.csv', 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=['n', 'base_avg', 'base_std', 'floyd_avg', 'floyd_std'])
        writer.writeheader()
        writer.writerows(resultados)
    print("\nResultados guardados exitosamente en 'resultados_experimentos.csv'")

if __name__ == "__main__":
    ejecutar_experimentos()