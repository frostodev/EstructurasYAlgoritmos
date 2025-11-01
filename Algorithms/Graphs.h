#ifndef GRAPHS_H
#define GRAPHS_H

#include "../DataStructures/Graph.h"
#include "../DataStructures/PriorityQueue.h"
#include "../DataStructures/Queue.h"
#include "../DataStructures/HashSet.h"

#include <limits>
#include <iostream>

namespace Algorithms {

    /*
     * NOTA: Los algoritmos de recorrido (BFS, DFS) usan DS::HashSet
     * para la lista 'visited' (logrando O(N+E) promedio). Esto requiere
     * que el tipo T sea "hasheable" (tenga una especialización de std::hash<T>
     * y un operador ==). Si T no es hasheable, se debe reemplazar
     * DS::HashSet por std::set en esos métodos (la complejidad
     * cambiará a O((N + E) log N)).
     */

    /**
     * @brief Helper recursivo para el Recorrido en Profundidad (DFS).
     * @param graph El grafo que se quiere recorrer
     * @param node El nodo actual que se está visitando.
     * @param visited Referencia a un conjunto de nodos ya visitados.
     * @note Complejidad: O(N + E) (promedio) para el recorrido total.
     */
    template <typename T>
    void dfs_recursive(const DS::Graph<T>& graph, const T& node, DS::HashSet<T>& visited) {
        visited.insert(node);
        std::cout << node << " ";

        try {
            for (const auto& edge : graph.get_neighbors(node)) {
                if (!visited.contains(edge.to)) {
                    dfs_recursive(graph, edge.to, visited);
                }
            }
        } catch (const std::runtime_error& e) {
            // El nodo no tiene vecinos, no hacer nada
        }
    }

    /**
     * @brief Realiza un Recorrido en Profundidad (DFS) e imprime los nodos.
     * @param graph El grafo que se quiere recorrer
     * @param start_node El nodo desde el cual comenzar el recorrido.
     * @throws std::runtime_error si el nodo inicial no existe.
     * Requiere que T sea hasheable.
     * @note Complejidad: O(N + E) (promedio, usando HashSet).
     */
    template <typename T>
    void dfs(const DS::Graph<T>& graph, const T& start_node) {
        if (!graph.has_node(start_node)) {
            throw std::runtime_error("dfs(): El nodo inicial de DFS no existe.");
        }

        DS::HashSet<T> visited;
        std::cout << "DFS desde " << start_node << ": ";
        dfs_recursive(graph, start_node, visited);
        std::cout << std::endl;
    }

    /**
     * @brief Realiza un Recorrido en Amplitud (BFS) e imprime los nodos.
     * @param graph El grafo que se quiere recorrer
     * @param start_node El nodo desde el cual comenzar el recorrido.
     * @throws std::runtime_error si el nodo inicial no existe.
     * @note Requiere que T sea hasheable.
     * @note Complejidad: O(N + E) (promedio, usando HashSet).
     */
    template <typename T>
    void bfs(const DS::Graph<T>& graph, const T& start_node) {
        if (!graph.has_node(start_node)) {
            throw std::runtime_error("bfs(): El nodo inicial de BFS no existe.");
        }

        DS::HashSet<T> visited;
        DS::Queue<T> q;

        q.push(start_node);
        visited.insert(start_node);
        std::cout << "BFS desde " << start_node << ": ";

        while (!q.empty()) {
            T current_node = q.front();
            q.pop();
            std::cout << current_node << " ";

            try {
                for (const auto& edge : graph.get_neighbors(current_node)) {
                    if (!visited.contains(edge.to)) {
                        visited.insert(edge.to);
                        q.push(edge.to);
                    }
                }
            } catch (const std::runtime_error& e) {
                // El nodo no tiene vecinos, no hacer nada
            }
        }
        std::cout << std::endl;
    }

    /**
     * @brief Implementación del Algoritmo de Dijkstra.
     * @param graph El grafo (ponderado).
     * @param start_node El nodo de inicio.
     * @return DS::HashMap<T, double> Un mapa que asocia cada nodo con su
     * distancia más corta desde el inicio.
     * @note Complejidad: O(E log N)
     */
    template <typename T>
    DS::HashMap<T, double> dijkstra(const DS::Graph<T>& graph, const T& start_node) {
        if (!graph.has_node(start_node)) {
            throw std::runtime_error("dijkstra(): El nodo inicial de Dijkstra no existe.");
        }

        // Mapa de distancias
        DS::HashMap<T, double> distances;

        // Usamos std::pair<double, T> -> (distancia, nodo)
        // PriorityQueue es un Min-Heap por defecto, lo cual es perfecto
        DS::PriorityQueue<std::pair<double, T>> pq;

        // Inicializar todas las distancias a infinito
        const double INF = std::numeric_limits<double>::infinity();

        for (const T& node : graph.get_all_nodes()) {
            distances[node] = INF;
        }

        // Distancia al inicio es 0
        distances[start_node] = 0.0;
        pq.push({0.0, start_node});

        // Bucle principal
        while (!pq.empty()) {
            double current_dist = pq.top().first;
            T current_node = pq.top().second;
            pq.pop();

            // Optimización: Si ya encontramos un camino mejor, ignoramos
            if (current_dist > distances[current_node]) {
                continue;
            }

            // Revisar vecinos
            try {
                for (const auto& edge : graph.get_neighbors(current_node)) {
                    double new_dist = current_dist + edge.weight;

                    // Si encontramos un camino más corto
                    if (new_dist < distances[edge.to]) {
                        distances[edge.to] = new_dist;
                        pq.push({new_dist, edge.to});
                    }
                }
            } catch (const std::runtime_error& e) {
                // Nodo sin vecinos
            }
        }

        return distances;
    }

}

#endif //GRAPHS_H