
INF = float('inf')


class Grafo:
    def __init__(self, n):
        self.n = n
        self.matriz = [[INF] * n for _ in range(n)]
        for i in range(n):
            self.matriz[i][i] = 0
        self.aristas = []

    def agregar_arista(self, u, v, peso):
        self.matriz[u][v] = peso
        self.aristas.append((u, v, peso))


def bellman_ford(grafo, s):
    n = grafo.n
    dist = [INF] * n
    dist[s] = 0
    for _ in range(n - 1):
        actualizado = False
        for u, v, peso in grafo.aristas:
            if dist[u] != INF and dist[u] + peso < dist[v]:
                dist[v] = dist[u] + peso
                actualizado = True
        if not actualizado:
            break
    tiene_ciclo_negativo = False
    for u, v, peso in grafo.aristas:
        if dist[u] != INF and dist[u] + peso < dist[v]:
            tiene_ciclo_negativo = True
            break
    return dist, tiene_ciclo_negativo


def algoritmo_base(grafo):
    n = grafo.n
    dist = []
    ciclo_negativo = False
    for s in range(n):
        fila, tiene_ciclo = bellman_ford(grafo, s)
        dist.append(fila)
        if tiene_ciclo:
            ciclo_negativo = True
    return dist, ciclo_negativo


def imprimir_matriz(dist, nombre="Distancias"):
    n = len(dist)
    print(f"\n=== {nombre} ===")
    print("     " + "  ".join(f"{j:>6}" for j in range(n)))
    for i in range(n):
        fila = "  ".join("   INF" if d == INF else f"{d:>6}" for d in dist[i])
        print(f"{i:>3}: {fila}")


def verificar_correctitud(dist_base, dist_esperada, nombre=""):
    n = len(dist_base)
    for i in range(n):
        for j in range(n):
            a = dist_base[i][j]
            b = dist_esperada[i][j]
            if a == INF and b == INF:
                continue
            if a == INF or b == INF:
                print(f"  FALLO en ({i},{j}): base={a}, esperado={b}")
                return False
            if abs(a - b) > 1e-9:
                print(f"  FALLO en ({i},{j}): base={a:.4f}, esperado={b:.4f}")
                return False
    print(f"  OK - {nombre} coincide con resultado esperado.")
    return True


def test_caso_simple():
    print("\n[TEST 1] Grafo simple con pesos positivos")
    g = Grafo(4)
    g.agregar_arista(0, 1, 3)
    g.agregar_arista(0, 2, 8)
    g.agregar_arista(1, 2, 2)
    g.agregar_arista(1, 3, 5)
    g.agregar_arista(2, 3, 1)
    g.agregar_arista(3, 0, 4)
    dist, ciclo = algoritmo_base(g)
    imprimir_matriz(dist, "Algoritmo Base")
    print(f"  Ciclo negativo detectado: {ciclo}")
    esperado = [
        [0,  3,  5,  6],
        [7,  0,  2,  3],
        [5,  8,  0,  1],
        [4,  7,  9,  0],
    ]
    verificar_correctitud(dist, esperado, "Test 1")


def test_pesos_negativos():
    print("\n[TEST 2] Grafo con pesos negativos (sin ciclo negativo)")
    g = Grafo(3)
    g.agregar_arista(0, 1,  4)
    g.agregar_arista(0, 2,  2)
    g.agregar_arista(1, 2, -1)
    g.agregar_arista(2, 1,  1)
    dist, ciclo = algoritmo_base(g)
    imprimir_matriz(dist, "Algoritmo Base")
    print(f"  Ciclo negativo detectado: {ciclo}")
    esperado = [
        [0,  3,  2],
        [INF,  0, -1],
        [INF,  1,  0],
    ]
    verificar_correctitud(dist, esperado, "Test 2")


def test_ciclo_negativo():
    print("\n[TEST 3] Grafo con ciclo negativo")
    g = Grafo(3)
    g.agregar_arista(0, 1,  1)
    g.agregar_arista(1, 2, -3)
    g.agregar_arista(2, 1,  1)
    _, ciclo = algoritmo_base(g)
    print(f"  Ciclo negativo detectado: {ciclo}  (esperado: True)")
    assert ciclo, "ERROR: no detectó el ciclo negativo"
    print("  OK - ciclo negativo detectado correctamente.")


def test_grafo_desconectado():
    print("\n[TEST 4] Grafo desconectado")
    g = Grafo(4)
    g.agregar_arista(0, 1, 5)
    g.agregar_arista(2, 3, 3)
    dist, ciclo = algoritmo_base(g)
    imprimir_matriz(dist, "Algoritmo Base")
    esperado = [
        [0,  5,  INF,  INF],
        [INF,  0,  INF,  INF],
        [INF,  INF,  0,  3],
        [INF,  INF,  INF,  0],
    ]
    verificar_correctitud(dist, esperado, "Test 4")


if __name__ == "__main__":
    test_caso_simple()
    test_pesos_negativos()
    test_ciclo_negativo()
    test_grafo_desconectado()
    print("\nTodos los tests finalizados.")