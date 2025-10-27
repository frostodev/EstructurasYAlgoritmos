#ifndef GRAPH_H
#define GRAPH_H

//#include "LinkedList.h"
#include "HashMap.h"

#include <iostream>
#include <set>
#include <list>     // pronto reemplazar por DS::LinkedList
#include <stdexcept>
#include <iomanip>

namespace DS {

    /**
     * @class Graph
     * @brief Implementación de una clase de Grafo genérica.
     * @tparam T El tipo de dato para el valor/identificador de los nodos.
     * Debe ser comparable (soportar el operador <) para usarse en el
     * mapa de adyacencia (DS::HashMap) y el conjunto de nodos (std::set).
     *
     * Esta clase puede representar grafos dirigidos o no dirigidos,
     * ponderados o no ponderados, usando una Lista de Adyacencia
     * implementada con DS::HashMap.
     */
    template <typename T>
    class Graph {
    private:
        /**
         * @struct Edge
         * @brief Estructura interna que representa una arista dirigida y ponderada.
         */
        struct Edge {
            T to;           // El nodo destino de la arista
            double weight;  // El peso de la arista
        };

    public:
        // -----------------------------------------------------------------
        // Constructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor del Grafo.
         * @param is_directed true si el grafo es dirigido, false (defecto) si es no dirigido.
         * @param is_weighted true si el grafo es ponderado, false (defecto) si no es ponderado.
         * @note Complejidad: O(1)
         */
        explicit Graph(bool is_directed = false, bool is_weighted = false)
            : is_directed_(is_directed),
              is_weighted_(is_weighted),
              edge_count_(0) {}

        // -----------------------------------------------------------------
        // Métodos de Capacidad y Observadores
        // -----------------------------------------------------------------

        /**
         * @brief Verifica si el grafo está vacío (sin nodos).
         * @return true si no hay nodos, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool is_empty() const {
            return nodes_.empty();
        }

        /**
         * @brief Obtiene el número de nodos (vértices) en el grafo.
         * @return int El conteo de nodos.
         * @note Complejidad: O(1)
         */
        int get_node_count() const {
            return nodes_.size();
        }

        /**
         * @brief Obtiene el número de aristas en el grafo.
         * @return int El conteo de aristas.
         * @note Complejidad: O(1)
         */
        int get_edge_count() const {
            return edge_count_;
        }

        /**
         * @brief Verifica si un nodo existe en el grafo.
         * @param node El valor del nodo a buscar.
         * @return true si el nodo existe, false en caso contrario.
         * @note Complejidad: O(log N) donde N es el número de nodos.
         */
        bool has_node(const T& node) const {
            return nodes_.count(node);
        }

        /**
         * @brief Verifica si existe una arista entre dos nodos.
         * En grafos no dirigidos, comprueba ambas direcciones.
         * @param from El nodo de origen.
         * @param to El nodo de destino.
         * @return true si la arista existe, false en caso contrario.
         * @note Complejidad: O(log N + M) donde M es el grado máximo de 'from' o 'to'.
         */
        bool has_edge(const T& from, const T& to) const {
            if (!has_node(from)) {
                return false;
            }

            // Busca en la lista de adyacencia de 'from'
            // O(log N) para .at() + O(M_from) para el loop
            for (const auto& edge : adjacency_list_.at(from)) {
                if (edge.to == to) {
                    return true;
                }
            }

            // Si es no dirigido, debemos comprobar también en la otra dirección
            // O(log N) para .at() + O(M_to) para el loop
            if (!is_directed_ && has_node(to)) {
                 for (const auto& edge : adjacency_list_.at(to)) {
                    if (edge.to == from) {
                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * @brief Obtiene una copia del conjunto de todos los nodos.
         * @return std::set<T> Un conjunto con todos los nodos.
         * @note Complejidad: O(N)
         */
        std::set<T> get_all_nodes() const {
            return nodes_;
        }

        /**
         * @brief Obtiene una copia de la lista de vecinos de un nodo.
         * @param node El nodo del cual obtener los vecinos.
         * @return std::list<Edge> Una lista de aristas (vecinos).
         * @throws std::runtime_error si el nodo no existe.
         * @note Complejidad: O(log N + M) donde M es el grado del nodo.
         */
        std::list<Edge> get_neighbors(const T& node) const {
            if (!has_node(node)) {
                throw std::runtime_error("Graph::get_neighbors(): El nodo no existe en el grafo.");
            }
            // .at() es O(log N). Copiar la lista es O(M).
            return adjacency_list_.at(node);
        }

        // -----------------------------------------------------------------
        // Métodos de Modificación
        // -----------------------------------------------------------------

        /**
         * @brief Añade un nuevo nodo (vértice) al grafo.
         * Si el nodo ya existe, no hace nada.
         * @param value El valor del nodo a añadir.
         * @note Complejidad: O(log N)
         */
        void add_node(const T& value) {
            // nodes_.insert() es O(log N)
            nodes_.insert(value);
            if (adjacency_list_.count(value) == 0) {
                adjacency_list_[value] = std::list<Edge>();
            }
        }

        /**
         * @brief Añade una arista entre dos nodos.
         * Si los nodos no existen, se crean automáticamente.
         * @param from El nodo de origen.
         * @param to El nodo de destino.
         * @param weight El peso de la arista (ignorado si el grafo es no ponderado).
         * @return true si la arista se añadió, false si ya existía.
         * @note Complejidad: O(log N + M) donde M es el grado máximo de 'from' o 'to'.
         */
        bool add_edge(const T& from, const T& to, double weight = 1.0) {
            // add_node() es O(log N)
            add_node(from);
            add_node(to);

            // has_edge() es O(log N + M)
            if (has_edge(from, to)) {
                 return false;
            }

            double final_weight = is_weighted_ ? weight : 1.0;

            // adjacency_list_[] es O(log N). push_back() es O(1).
            adjacency_list_[from].push_back(Edge{to, final_weight});

            // Si no es dirigido, añade la arista recíproca (to -> from)
            if (!is_directed_) {
                // O(log N) + O(1)
                adjacency_list_[to].push_back(Edge{from, final_weight});
            }

            edge_count_++; // O(1)
            return true;
        }

        /**
         * @brief Imprime una representación del grafo en la consola.
         * Muestra la lista de adyacencia.
         * @note Complejidad: O(N + E) donde N es nodos y E es aristas.
         */
        void print_graph() const {
            std::cout << "--- Grafo ---" << std::endl;
            std::cout << "Nodos: " << get_node_count() << ", Aristas: " << get_edge_count() << std::endl;
            std::cout << (is_directed_ ? "Dirigido" : "No Dirigido") << ", "
                      << (is_weighted_ ? "Ponderado" : "No Ponderado") << std::endl;

            for (const T& node : nodes_) { // O(N)
                std::cout << std::setw(8) << std::left << node << " -> ";
                if (adjacency_list_.count(node) && !adjacency_list_.at(node).empty()) {
                    // Este loop interno se ejecuta E veces en total (2E si no es dirigido)
                    for (const auto& edge : adjacency_list_.at(node)) {
                        std::cout << "[" << edge.to;
                        if (is_weighted_) {
                            std::cout << " (w:" << edge.weight << ")";
                        }
                        std::cout << "] ";
                    }
                } else {
                    std::cout << "(ninguno)";
                }
                std::cout << std::endl;
            }
            std::cout << "-------------" << std::endl;
        }

    private:
        // La lista de adyacencia: Mapea un nodo (T) a una lista de sus aristas (vecinos)
        DS::HashMap<T, std::list<Edge>> adjacency_list_;

        // Un conjunto de todos los nodos en el grafo (incluyendo nodos aislados)
        std::set<T> nodes_;

        bool is_directed_;
        bool is_weighted_;
        int edge_count_;
    };

}

#endif //GRAPH_H