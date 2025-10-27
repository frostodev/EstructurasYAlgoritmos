
#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "../Queue.h"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace DS {

    /**
     * @class BinarySearchTree
     * @brief Implementación de un Árbol Binario de Búsqueda (BST) genérico.
     * @tparam T El tipo de dato que almacenará el árbol. Debe soportar
     * operadores de comparación (<, >, ==).
     *
     * Mantiene los elementos en un ordenado, permitiendo inserciones,
     * eliminaciones y búsquedas eficientes (tiempo promedio O(log n)).
     */
    template <typename T>
    class BinarySearchTree {
    public:

        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea un árbol vacío.
         * @note Complejidad: O(1)
         */
        BinarySearchTree() : root_(nullptr), node_count_(0) {}

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada para los nodos.
         * @note Complejidad: O(n)
         */
        ~BinarySearchTree() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * @param other El árbol a copiar.
         * @note Complejidad: O(n)
         */
        BinarySearchTree(const BinarySearchTree& other)
            : root_(nullptr), node_count_(other.node_count_) {
            root_ = copy_recursive(other.root_);
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * @param other El árbol a asignar.
         * @return BinarySearchTree& Una referencia a este árbol (*this).
         * @note Complejidad: O(n + m)
         */
        BinarySearchTree& operator=(const BinarySearchTree& other) {
            if (this == &other) {
                return *this;
            }
            BinarySearchTree temp(other); // Llama al constructor de copia
            swap(temp);
            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * @param other El árbol (r-value) del que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        BinarySearchTree(BinarySearchTree&& other) noexcept
            : root_(other.root_), node_count_(other.node_count_) {
            other.root_ = nullptr;
            other.node_count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * @param other El árbol (r-value) a asignar.
         * @return BinarySearchTree& Una referencia a este árbol (*this).
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            clear(); // Libera recursos propios
            // Roba recursos de 'other'
            root_ = other.root_;
            node_count_ = other.node_count_;
            // Deja 'other' en estado vacío
            other.root_ = nullptr;
            other.node_count_ = 0;
            return *this;
        }

        // -----------------------------------------------------------------
        // Operaciones Principales
        // -----------------------------------------------------------------

        /**
         * @brief Verifica si el árbol está vacío.
         * @return true si el árbol está vacío, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool is_empty() const {
            return root_ == nullptr;
        }

        /**
         * @brief Retorna el número de nodos en el árbol.
         * @return int El número de nodos.
         * @note Complejidad: O(1)
         */
        int get_size() const {
            return node_count_;
        }

        /**
         * @brief Inserta un valor en el árbol.
         * Si el valor ya existe, la operación no hace nada.
         * @param value El valor a insertar.
         * @note Complejidad: O(log n) en promedio, O(n) en el peor caso (árbol degenerado).
         */
        void insert_node(const T& value) {
            bool was_inserted = false;
            root_ = insert_node_recursive(root_, value, was_inserted);
            if (was_inserted) {
                node_count_++;
            }
        }

        /**
         * @brief Elimina un valor del árbol.
         * Si el valor no existe, la operación no hace nada.
         * @param value El valor a eliminar.
         * @note Complejidad: O(log n) en promedio, O(n) en el peor caso.
         */
        void remove_node(const T& value) {
            bool was_removed = false;
            root_ = remove_node_recursive(root_, value, was_removed);
            if (was_removed) {
                node_count_--;
            }
        }

        /**
         * @brief Busca un valor en el árbol.
         * @param value El valor a buscar.
         * @return true si el valor se encuentra, false en caso contrario.
         * @note Complejidad: O(log n) en promedio, O(n) en el peor caso.
         */
        bool search_node(const T& value) const {
            return search_node_recursive(root_, value);
        }

        /**
         * @brief Obtiene el valor mínimo en el árbol.
         * @return T& Referencia al valor mínimo.
         * @throws std::out_of_range si el árbol está vacío.
         * @note Complejidad: O(log n) en promedio, O(n) en el peor caso.
         */
        T& get_min() const {
            if (is_empty()) {
                throw std::out_of_range("BinarySearchTree::get_min(): Llamado en un árbol vacío.");
            }
            return find_min_node(root_)->value_;
        }

        /**
         * @brief Obtiene el valor máximo en el árbol.
         * @return T& Referencia al valor máximo.
         * @throws std::out_of_range si el árbol está vacío.
         * @note Complejidad: O(log n) en promedio, O(n) en el peor caso.
         */
        T& get_max() const {
            if (is_empty()) {
                throw std::out_of_range("BinarySearchTree::get_max(): llamado en un árbol vacío.");
            }
            return find_max_node(root_)->value_;
        }

        /**
         * @brief Elimina todos los nodos del árbol.
         * @note Complejidad: O(n)
         */
        void clear() {
            clear_recursive(root_);
            root_ = nullptr;
            node_count_ = 0;
        }

        // -----------------------------------------------------------------
        // Recorridos (Traversals)
        // -----------------------------------------------------------------

        /**
         * @brief Imprime el árbol en recorrido In-Orden (Izquierda, Raíz, Derecha).
         * Produce una lista ordenada de los valores.
         * @note Complejidad: O(n)
         */
        void print_in_order() const {
            std::cout << "In-Orden:   ";
            in_order_recursive(root_);
            std::cout << std::endl;
        }

        /**
         * @brief Imprime el árbol en recorrido Pre-Orden (Raíz, Izquierda, Derecha).
         * Útil para copiar árboles.
         * @note Complejidad: O(n)
         */
        void print_pre_order() const {
            std::cout << "Pre-Orden:  ";
            pre_order_recursive(root_);
            std::cout << std::endl;
        }

        /**
         * @brief Imprime el árbol en recorrido Post-Orden (Izquierda, Derecha, Raíz).
         * Útil para eliminar árboles (usado en clear()).
         * @note Complejidad: O(n)
         */
        void print_post_order() const {
            std::cout << "Post-Orden: ";
            post_order_recursive(root_);
            std::cout << std::endl;
        }

        /**
         * @brief Imprime el árbol por nivel (Recorrido en Amplitud - BFS).
         * Útil para visualizar la estructura del árbol nivel por nivel.
         * @note Complejidad: O(n)
         */
        void print_level_order() const {
            std::cout << "Nivel-Orden: ";
            if (is_empty()) {
                std::cout << "(vacío)" << std::endl;
                return;
            }

            DS::Queue<Node*> q;
            q.push(root_);

            while (!q.empty()) {
                Node* current = q.front();
                q.pop();

                std::cout << current->value_ << " ";

                if (current->left_ != nullptr) {
                    q.push(current->left_);
                }
                if (current->right_ != nullptr) {
                    q.push(current->right_);
                }
            }
            std::cout << std::endl;
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en el árbol.
         */
        struct Node {
            T value_;
            Node* left_;
            Node* right_;

            /**
             * @brief Constructor del Nodo.
             * @param val El valor (dato) que almacenará el nodo.
             * @note Complejidad: O(1)
             */
            Node(const T& val) : value_(val), left_(nullptr), right_(nullptr) {}
        };

        Node* root_;          // Puntero al nodo raíz del árbol.
        int node_count_;      // Número total de nodos en el árbol.

        /**
         * @brief Función auxiliar para intercambiar el estado de dos árboles.
         * @param other El otro árbol con el que intercambiar el contenido.
         * Complejidad: O(1)
         */
        void swap(BinarySearchTree& other) noexcept {
            using std::swap;
            swap(root_, other.root_);
            swap(node_count_, other.node_count_);
        }

        // -----------------------------------------------------------------
        // Helpers Recursivos Privados
        // -----------------------------------------------------------------

        /**
         * @brief Helper recursivo para insertar un nuevo nodo.
         * @param node El nodo actual que se está inspeccionando (raíz del subárbol).
         * @param value El valor a insertar.
         * @param inserted Referencia bool para notificar si la inserción fue exitosa
         * (se usa para actualizar node_count_).
         * @return Node* El nodo (posiblemente nuevo) que debe enlazarse al padre.
         * @note Complejidad: O(h) donde h es la altura. O(log n) promedio, O(n) peor caso.
         */
        Node* insert_node_recursive(Node* node, const T& value, bool& inserted) {
            // Caso base: Árbol (o subárbol) vacío. Aquí es donde insertamos
            if (node == nullptr) {
                inserted = true;
                return new Node(value);
            }

            // Caso recursivo: Decidir si ir a la izquierda o derecha
            if (value < node->value_) {
                node->left_ = insert_node_recursive(node->left_, value, inserted);
            } else if (value > node->value_) {
                node->right_ = insert_node_recursive(node->right_, value, inserted);
            } else {
                // Caso de duplicado: No hacemos nada.
                inserted = false;
            }
            return node;
        }

        /**
         * @brief Helper recursivo para eliminar un nodo.
         * @param node El nodo actual que se está inspeccionando.
         * @param value El valor a eliminar.
         * @param removed Referencia bool para notificar si la eliminación fue exitosa.
         * @return Node* El nodo que debe reemplazar al 'node' actual en el árbol.
         * @note Complejidad: O(h) donde h es la altura. O(log n) promedio, O(n) peor caso
         */
        Node* remove_node_recursive(Node* node, const T& value, bool& removed) {
            if (node == nullptr) {
                // Caso base: No se encontró el valor
                removed = false;
                return nullptr;
            }

            // Buscar el nodo a eliminar
            if (value < node->value_) {
                node->left_ = remove_node_recursive(node->left_, value, removed);
            } else if (value > node->value_) {
                node->right_ = remove_node_recursive(node->right_, value, removed);
            } else {
                // Se encontró el nodo (node->value_ == value)
                removed = true;

                // Caso 1: Nodo sin hijos (hoja)
                if (node->left_ == nullptr && node->right_ == nullptr) {
                    delete node;
                    return nullptr; // El padre se enlazará a null
                }
                // Caso 2: Nodo con un solo hijo (izquierdo o derecho)
                else if (node->left_ == nullptr) {
                    Node* temp = node->right_;
                    delete node;
                    return temp; // El padre se enlaza al hijo derecho
                } else if (node->right_ == nullptr) {
                    Node* temp = node->left_;
                    delete node;
                    return temp; // El padre se enlaza al hijo izquierdo
                }
                // Caso 3: Nodo con dos hijos (el más complejo)
                else {
                    // Encontrar el sucesor in-order (el nodo más pequeño
                    // en el subárbol derecho)
                    Node* successor = find_min_node(node->right_);

                    // Copiar el valor del sucesor a este nodo
                    node->value_ = successor->value_;

                    // Eliminar el nodo sucesor (que ahora es un duplicado)
                    // del subárbol derecho. Esta llamada *siempre* será
                    // un Caso 1 o Caso 2 (fácil de eliminar)
                    bool ignored_removed = false; // Ya sabemos que se eliminará
                    node->right_ = remove_node_recursive(node->right_, successor->value_, ignored_removed);
                }
            }
            return node;
        }

        /**
         * @brief Helper recursivo para buscar un valor.
         * @param node El nodo actual que se está inspeccionando.
         * @param value El valor a buscar.
         * @return true si se encuentra, false en caso contrario.
         * @note Complejidad: O(h) donde h es la altura. O(log n) promedio, O(n) peor caso.
         */
        bool search_node_recursive(Node* node, const T& value) const {
            if (node == nullptr) {
                return false;
            }
            if (node->value_ == value) {
                return true;
            }
            if (value < node->value_) {
                return search_node_recursive(node->left_, value);
            } else {
                return search_node_recursive(node->right_, value);
            }
        }

        /**
         * @brief Helper para encontrar el nodo con el valor mínimo.
         * @param node El nodo raíz del subárbol donde buscar.
         * @return Node* Un puntero al nodo que contiene el valor mínimo.
         * @note Complejidad: O(h) donde h es la altura del subárbol. O(log h) promedio, O(h) peor caso
         */
        Node* find_min_node(Node* node) const {
            // El valor más pequeño siempre está lo más a la izquierda posible.
            while (node != nullptr && node->left_ != nullptr) {
                node = node->left_;
            }
            return node;
        }

        /**
         * @brief Helper para encontrar el nodo con el valor máximo.
         * @param node El nodo raíz del subárbol donde buscar.
         * @return Node* Un puntero al nodo que contiene el valor máximo.
         * @note Complejidad: O(h) donde h es la altura del subárbol. O(log h) promedio, O(h) peor caso
         */
        Node* find_max_node(Node* node) const {
            // El valor más grande siempre está lo más a la derecha posible
            while (node != nullptr && node->right_ != nullptr) {
                node = node->right_;
            }
            return node;
        }

        /**
         * @brief Helper recursivo para eliminar todos los nodos (recorrido post-orden).
         * @param node El nodo actual a eliminar.
         * @note Complejidad: O(k), donde k es el número de nodos en el subárbol
         */
        void clear_recursive(Node* node) {
            if (node == nullptr) {
                return;
            }
            clear_recursive(node->left_);
            clear_recursive(node->right_);
            delete node;
        }

        /**
         * @brief Helper recursivo para copiar el árbol (recorrido pre-orden).
         * @param other_node El nodo del otro árbol que se está copiando.
         * @return Node* Un puntero al nodo recién creado (copiado).
         * @note Complejidad: O(k), donde k es el número de nodos en el subárbol
         */
        Node* copy_recursive(Node* other_node) {
            if (other_node == nullptr) {
                return nullptr;
            }
            // Copiar el nodo actual (Pre-orden: Raíz)
            Node* new_node = new Node(other_node->value_);
            // Copiar recursivamente los subárboles
            new_node->left_ = copy_recursive(other_node->left_);
            new_node->right_ = copy_recursive(other_node->right_);
            return new_node;
        }

        // -----------------------------------------------------------------
        // Helpers de Recorrido (Traversals) privados
        // -----------------------------------------------------------------

        /**
         * @brief Helper recursivo In-Orden.
         * @param node El nodo de donde iniciar el recorrido
         * Complejidad: O(k), donde k es el número de nodos en el subárbol.
         */
        void in_order_recursive(Node* node) const {
            if (node == nullptr) return;
            in_order_recursive(node->left_);      // Izquierda
            std::cout << node->value_ << " ";     // Raíz
            in_order_recursive(node->right_);     // Derecha
        }

        /**
         * @brief Helper recursivo Pre-Orden.
         * @param node El nodo de donde iniciar el recorrido
         * Complejidad: O(k), donde k es el número de nodos en el subárbol.
         */
        void pre_order_recursive(Node* node) const {
            if (node == nullptr) return;
            std::cout << node->value_ << " ";     // Raíz
            pre_order_recursive(node->left_);     // Izquierda
            pre_order_recursive(node->right_);    // Derecha
        }

        /**
         * @brief Helper recursivo Post-Orden.
         * @param node El nodo de donde iniciar el recorrido
         * Complejidad: O(k), donde k es el número de nodos en el subárbol.
         */
        void post_order_recursive(Node* node) const {
            if (node == nullptr) return;
            post_order_recursive(node->left_);    // Izquierda
            post_order_recursive(node->right_);   // Derecha
            std::cout << node->value_ << " ";     // Raíz
        }

    };

}

#endif //BINARYSEARCHTREE_H