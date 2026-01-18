#include "TestGraph.h"

#include "../DataStructures/Graph.h"
#include "../Algorithms/Graphs.h"
#include "../DataStructures/HashMap.h"
#include "../Math/MathUtils.h"

#include <iostream>
#include <string>
#include <iomanip>


namespace Test {

    /**
     * @brief Helper simple para reportar éxito o error
     */
    void compare_bool_graph(bool actual, bool expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            std::cout << "    Obtenido: " << std::boolalpha << actual << " | Esperado: " << std::boolalpha << expected << std::endl;
        }
    }

    /**
     * @brief Helper para comparar resultados de Dijkstra (usando doubles)
     */
    void compare_dijkstra_result(DS::HashMap<std::string, double>& distances, const std::string& node, double expected) {
        std::string test_name = "Dijkstra a '" + node + "'";

        // El nodo puede no estar en el mapa si es inalcanzable (distancia infinita)
        double actual = distances.at(node);
        const double INF = std::numeric_limits<double>::infinity();

        // Comparación con epsilon para doubles
        bool pass = false;
        if (expected == INF) {
            pass = (actual == INF);
        } else {
            pass = (Math::abs(actual - expected) < 0.001);
        }

        if (pass) {
            std::cout << "  [ÉXITO] " << test_name << " (Esperado: " << expected << ", Obtenido: " << actual << ")" << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << " (Esperado: " << expected << ", Obtenido: " << actual << ")" << std::endl;
        }
    }


    /**
     * @brief Realiza tests básicos al grafo
     */
    void test_graph_basic() {
        std::cout << "\n--- Prueba 1: Métodos Básicos (add_node, has_node, count) ---" << std::endl;
        DS::Graph<std::string> g;

        compare_bool_graph(g.is_empty(), true, "is_empty() en grafo nuevo");
        compare_bool_graph(g.get_node_count(), 0, "get_node_count() en grafo nuevo");

        g.add_node("A");
        g.add_node("B");
        g.add_node("C");

        compare_bool_graph(g.is_empty(), false, "is_empty() después de añadir nodos");
        compare_bool_graph(g.get_node_count(), 3, "get_node_count() == 3");
        compare_bool_graph(g.has_node("A"), true, "has_node('A')");
        compare_bool_graph(g.has_node("C"), true, "has_node('C')");
        compare_bool_graph(g.has_node("D"), false, "has_node('D') (no existe)");

        g.add_node("A"); // Añadir duplicado
        compare_bool_graph(g.get_node_count(), 3, "get_node_count() no cambia con duplicado");
    }

    /**
     * @brief Realiza tests de aristas en un grafo no ponderado
     */
    void test_graph_edges_unweighted() {
        std::cout << "\n--- Prueba 2: Aristas (Grafo No Ponderado) ---" << std::endl;

        // --- Grafo No Dirigido ---
        std::cout << "Sub-prueba: No Dirigido (bool is_directed = false)" << std::endl;
        DS::Graph<int> g_undir(false, false);
        g_undir.add_edge(1, 2);
        g_undir.add_edge(1, 3);

        compare_bool_graph(g_undir.get_edge_count(), 2, "get_edge_count() == 2");
        compare_bool_graph(g_undir.has_edge(1, 2), true, "has_edge(1, 2)");
        compare_bool_graph(g_undir.has_edge(2, 1), true, "has_edge(2, 1) (simetría)");
        compare_bool_graph(g_undir.has_edge(1, 3), true, "has_edge(1, 3)");
        compare_bool_graph(g_undir.has_edge(3, 1), true, "has_edge(3, 1) (simetría)");
        compare_bool_graph(g_undir.has_edge(2, 3), false, "has_edge(2, 3) (no existe)");

        bool r = g_undir.add_edge(1, 2); // Añadir duplicado
        compare_bool_graph(r, false, "add_edge() retorna false para duplicado");
        compare_bool_graph(g_undir.get_edge_count(), 2, "get_edge_count() no cambia con duplicado");

        // --- Grafo Dirigido ---
        std::cout << "Sub-prueba: Dirigido (bool is_directed = true)" << std::endl;
        DS::Graph<int> g_dir(true, false);
        g_dir.add_edge(1, 2);
        g_dir.add_edge(1, 3);

        compare_bool_graph(g_dir.get_edge_count(), 2, "get_edge_count() == 2");
        compare_bool_graph(g_dir.has_edge(1, 2), true, "has_edge(1, 2)");
        compare_bool_graph(g_dir.has_edge(2, 1), false, "has_edge(2, 1) (sin simetría)");
        compare_bool_graph(g_dir.has_edge(1, 3), true, "has_edge(1, 3)");
        compare_bool_graph(g_dir.has_edge(3, 1), false, "has_edge(3, 1) (sin simetría)");
    }

    /**
     * @brief Realiza tests de aristas en un grafo ponderado
     */
    void test_graph_edges_weighted() {
        std::cout << "\n--- Prueba 3: Aristas y Vecinos (Grafo Ponderado) ---" << std::endl;
        DS::Graph<std::string> g(true, true); // Dirigido, Ponderado

        g.add_edge("A", "B", 5.5);
        g.add_edge("A", "C", 3.2);

        std::cout << "Grafo de prueba:" << std::endl;
        g.print_graph(); // Imprimir para verificación visual

        std::cout << "Probando get_neighbors('A')... (debería tener 2 vecinos)" << std::endl;
        try {
            // Nota: El tipo es std::list<Edge>, donde Edge es el struct privado
            // pero podemos usar 'auto' para obtener la lista
            auto neighbors = g.get_neighbors("A");
            int count = 0;
            double total_weight = 0;

            // Iterar sobre la std::list devuelta
            for (const auto& edge : neighbors) {
                count++;
                total_weight += edge.weight;
                std::cout << "  Vecino " << count << ": " << edge.to << " (Peso: " << edge.weight << ")" << std::endl;
            }
            compare_bool_graph(count == 2, true, "Conteo de vecinos de 'A'");
            compare_bool_graph(Math::abs(total_weight - 8.7) < 0.001, true, "Suma de pesos de 'A'");

        } catch (const std::runtime_error& e) {
            std::cout << "  [ERROR] get_neighbors('A') lanzó excepción: " << e.what() << std::endl;
        }

        std::cout << "Probando get_neighbors('Z') (no existe)..." << std::endl;
        try {
            g.get_neighbors("Z");
            std::cout << "  [ERROR] get_neighbors('Z') no lanzó excepción." << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "  [ÉXITO] Capturó excepción esperada: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Realiza tests de algoritmos en un grafo
     */
    void test_graph_algorithms() {
        std::cout << "\n--- Prueba 4: Algoritmos (BFS, DFS, Dijkstra) ---" << std::endl;

        // Grafo ponderado y dirigido para las pruebas
        DS::Graph<std::string> g(true, true);
        g.add_edge("A", "B", 2.0);
        g.add_edge("A", "C", 5.0);
        g.add_edge("B", "C", 1.0); // Camino A->B->C (costo 3) es mejor que A->C (costo 5)
        g.add_edge("B", "D", 6.0);
        g.add_edge("C", "D", 3.0);
        g.add_edge("C", "E", 10.0);
        g.add_edge("E", "F", 2.0);
        g.add_node("G"); // Nodo aislado

        std::cout << "Grafo para algoritmos:" << std::endl;
        g.print_graph();

        // --- Prueba BFS ---
        std::cout << "\nPrueba BFS (verificar salida visualmente):" << std::endl;
        try {
            Algorithms::bfs(g, std::string("A"));
            // Salida esperada (orden de niveles, puede variar por HashMap): A B C D E F
        } catch (const std::exception& e) {
            std::cout << "[ERROR] BFS lanzó excepción: " << e.what() << std::endl;
        }

        // --- Prueba DFS ---
        std::cout << "\nPrueba DFS (verificar salida visualmente):" << std::endl;
        try {
            Algorithms::dfs(g, std::string("A"));
            // Salida esperada (ejemplo, depende de HashMap): A B C D E F
        } catch (const std::exception& e) {
            std::cout << "[ERROR] DFS lanzó excepción: " << e.what() << std::endl;
        }

        // --- Prueba Dijkstra ---
        std::cout << "\nPrueba Dijkstra (desde 'A'):" << std::endl;
        try {
            DS::HashMap<std::string, double> distances = Algorithms::dijkstra(g, std::string("A"));

            const double INF = std::numeric_limits<double>::infinity();

            // Distancias esperadas desde "A":
            // A: 0
            // B: 2 (A->B)
            // C: 3 (A->B->C)
            // D: 6 (A->B->C->D)
            // E: 13 (A->B->C->E)
            // F: 15 (A->B->C->E->F)
            // G: inf (aislado)

            compare_dijkstra_result(distances, "A", 0.0);
            compare_dijkstra_result(distances, "B", 2.0);
            compare_dijkstra_result(distances, "C", 3.0);
            compare_dijkstra_result(distances, "D", 6.0);
            compare_dijkstra_result(distances, "E", 13.0);
            compare_dijkstra_result(distances, "F", 15.0);
            compare_dijkstra_result(distances, "G", INF);

        } catch (const std::exception& e) {
            std::cout << "[ERROR] Dijkstra lanzó excepción: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Ejecuta las pruebas de grafo
     */
    void test_graph() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Grafo y Algoritmos ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_graph_basic();
        test_graph_edges_unweighted();
        test_graph_edges_weighted();
        test_graph_algorithms();
    }

}