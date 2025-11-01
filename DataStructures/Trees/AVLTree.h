#ifndef AVLTREE_H
#define AVLTREE_H

#include "../Queue.h"
#include "../../Math/MathUtils.h"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace DS {

    /**
     * @class AVLTree
     * @brief Implementación de un Árbol AVL (Árbol Binario de Búsqueda Auto-Balanceable).
     * @tparam T El tipo de dato que almacenará el árbol. Debe soportar
     * operadores de comparación (<, >, ==).
     *
     * Esta estructura garantiza que las operaciones de inserción, eliminación
     * y búsqueda se mantengan en una complejidad de O(log n) en el peor
     * de los casos, manteniendo el árbol balanceado.
     */
    template <typename T>
    class AVLTree {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * @note Complejidad: O(1)
         */
        AVLTree() : root_(nullptr), node_count_(0) {}

        /**
         * @brief Destructor.
         * @note Complejidad: O(n)
         */
        ~AVLTree() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * @param other El árbol a copiar.
         * @note Complejidad: O(n)
         */
        AVLTree(const AVLTree& other)
            : root_(nullptr), node_count_(other.node_count_) {
            root_ = copy_recursive(other.root_);
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * @param other El árbol a asignar.
         * @return AVLTree& Una referencia a este árbol (*this).
         * @note Complejidad: O(n + m)
         */
        AVLTree& operator=(const AVLTree& other) {
            if (this == &other) {
                return *this;
            }
            AVLTree temp(other);
            swap(temp);
            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * @param other El árbol (r-value) del que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        AVLTree(AVLTree&& other) noexcept
            : root_(other.root_), node_count_(other.node_count_) {
            other.root_ = nullptr;
            other.node_count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * @param other El árbol (r-value) a asignar.
         * @return AVLTree& Una referencia a este árbol (*this).
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        AVLTree& operator=(AVLTree&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            clear();
            root_ = other.root_;
            node_count_ = other.node_count_;
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
         * Si el valor ya existe, no hace nada. Mantiene el balance del árbol.
         * @param value El valor a insertar.
         * @note Complejidad: O(log n) garantizado.
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
         * Si el valor no existe, no hace nada. Mantiene el balance del árbol.
         * @param value El valor a eliminar.
         * @note Complejidad: O(log n) garantizado.
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
         * @note Complejidad: O(log n) garantizado.
         */
        bool search_node(const T& value) const {
            return search_node_recursive(root_, value);
        }

        /**
         * @brief Obtiene el valor mínimo en el árbol.
         * @return T& Referencia al valor mínimo.
         * @throws std::out_of_range si el árbol está vacío.
         * @note Complejidad: O(log n) garantizado.
         */
        T& get_min() const {
            if (is_empty()) {
                throw std::out_of_range("AVLTree::get_min(): Árbol vacío.");
            }
            return find_min_node(root_)->value_;
        }

        /**
         * @brief Obtiene el valor máximo en el árbol.
         * @return T& Referencia al valor máximo.
         * @throws std::out_of_range si el árbol está vacío.
         * @note Complejidad: O(log n) garantizado.
         */
        T& get_max() const {
            if (is_empty()) {
                throw std::out_of_range("AVLTree::get_max(): Árbol vacío.");
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

        void print_in_order() const {
            std::cout << "In-Orden:   ";
            in_order_recursive(root_);
            std::cout << std::endl;
        }

        void print_pre_order() const {
            std::cout << "Pre-Orden:  ";
            pre_order_recursive(root_);
            std::cout << std::endl;
        }

        void print_post_order() const {
            std::cout << "Post-Orden: ";
            post_order_recursive(root_);
            std::cout << std::endl;
        }

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
                if (current->left_ != nullptr) q.push(current->left_);
                if (current->right_ != nullptr) q.push(current->right_);
            }
            std::cout << std::endl;
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en el árbol.
         * Incluye un campo 'height_' para el balanceo AVL.
         */
        struct Node {
            T value_;
            Node* left_;
            Node* right_;
            int height_; // Altura del subárbol con raíz en este nodo

            /**
             * @brief Constructor del Nodo.
             * Un nuevo nodo (hoja) siempre tiene altura 1.
             * @param val El valor (dato) que almacenará el nodo.
             */
            Node(const T& val) : value_(val), left_(nullptr), right_(nullptr), height_(1) {}
        };

        Node* root_;          // Puntero al nodo raíz del árbol
        int node_count_;      // Número total de nodos en el árbol

        /**
         * @brief Función auxiliar para intercambiar el estado de dos árboles.
         * @note Complejidad: O(1)
         */
        void swap(AVLTree& other) noexcept {
            std::swap(root_, other.root_);
            std::swap(node_count_, other.node_count_);
        }

        // -----------------------------------------------------------------
        // Helpers de Lógica AVL
        // -----------------------------------------------------------------

        /**
         * @brief Obtiene la altura de un nodo de forma segura.
         * Un nodo nulo (nullptr) tiene altura 0.
         * @param node El nodo a consultar.
         * @return int La altura del nodo.
         * @note Complejidad: O(1)
         */
        int height(Node* node) const {
            return (node == nullptr) ? 0 : node->height_;
        }

        /**
         * @brief Actualiza la altura de un nodo basándose en la altura de sus hijos.
         * @param node El nodo a actualizar.
         * @note Complejidad: O(1)
         */
        void update_height(Node* node) {
            if (node != nullptr) {
                node->height_ = 1 + Math::max(height(node->left_), height(node->right_));
            }
        }

        /**
         * @brief Calcula el Factor de Balance (Balance Factor) de un nodo.
         * BF = altura(izquierda) - altura(derecha)
         * @param node El nodo a consultar.
         * @return int El factor de balance.
         * > 1: Pesado a la izquierda (Left-heavy)
         * < -1: Pesado a la derecha (Right-heavy)
         * -1, 0, 1: Balanceado
         * @note Complejidad: O(1)
         */
        int get_balance_factor(Node* node) const {
            return (node == nullptr) ? 0 : height(node->left_) - height(node->right_);
        }

        /**
         * @brief Realiza una rotación simple a la derecha (Caso Left-Left).
         * @param y El nodo raíz del subárbol desbalanceado (BF > 1).
         * @return Node* El nuevo nodo raíz del subárbol (x).
         * @note Complejidad: O(1)
         */
        Node* right_rotate(Node* y) {
            Node* x = y->left_;
            Node* T2 = x->right_;

            // Realizar rotación
            x->right_ = y;
            y->left_ = T2;

            // Actualizar alturas
            // 'y' es ahora hijo, 'x' es ahora padre
            update_height(y);
            update_height(x);

            // Retornar la nueva raíz
            return x;
        }

        /**
         * @brief Realiza una rotación simple a la izquierda (Caso Right-Right).
         * @param x El nodo raíz del subárbol desbalanceado (BF < -1).
         * @return Node* El nuevo nodo raíz del subárbol (y).
         * @note Complejidad: O(1)
         */
        Node* left_rotate(Node* x) {
            Node* y = x->right_;
            Node* T2 = y->left_;

            // Realizar rotación
            y->left_ = x;
            x->right_ = T2;

            // Actualizar alturas
            // 'x' es ahora hijo, 'y' es ahora padre.
            update_height(x);
            update_height(y);

            // Retornar la nueva raíz
            return y;
        }

        // -----------------------------------------------------------------
        // Helpers Recursivos
        // -----------------------------------------------------------------

        /**
         * @brief Helper recursivo para insertar y re-balancear.
         * @param node El nodo actual.
         * @param value El valor a insertar.
         * @param inserted Flag para actualizar el conteo de nodos.
         * @return Node* La nueva raíz de este subárbol (puede cambiar por rotaciones).
         * @note Complejidad: O(log n)
         */
        Node* insert_node_recursive(Node* node, const T& value, bool& inserted) {
            // Inserción estándar de BST
            if (node == nullptr) {
                inserted = true;
                return new Node(value);
            }

            if (value < node->value_) {
                node->left_ = insert_node_recursive(node->left_, value, inserted);
            } else if (value > node->value_) {
                node->right_ = insert_node_recursive(node->right_, value, inserted);
            } else {
                // Valor duplicado
                inserted = false;
                return node;
            }

            // Actualizar altura (en el camino de vuelta)
            update_height(node);

            // Obtener factor de balance y re-balancear
            int balance = get_balance_factor(node);

            // Caso 1: Left-Left (LL)
            if (balance > 1 && value < node->left_->value_) {
                return right_rotate(node);
            }

            // Caso 2: Right-Right (RR)
            if (balance < -1 && value > node->right_->value_) {
                return left_rotate(node);
            }

            // Caso 3: Left-Right (LR)
            if (balance > 1 && value > node->left_->value_) {
                node->left_ = left_rotate(node->left_);
                return right_rotate(node);
            }

            // Caso 4: Right-Left (RL)
            if (balance < -1 && value < node->right_->value_) {
                node->right_ = right_rotate(node->right_);
                return left_rotate(node);
            }

            // Si estaba balanceado, retornar el nodo sin cambios
            return node;
        }

        /**
         * @brief Helper recursivo para eliminar y re-balancear.
         * @param node El nodo actual.
         * @param value El valor a eliminar.
         * @param removed Flag para actualizar el conteo de nodos.
         * @return Node* La nueva raíz de este subárbol (puede cambiar por rotaciones).
         * @note Complejidad: O(log n)
         */
        Node* remove_node_recursive(Node* node, const T& value, bool& removed) {
            // Eliminación estándar de BST
            if (node == nullptr) {
                removed = false;
                return nullptr;
            }

            if (value < node->value_) {
                node->left_ = remove_node_recursive(node->left_, value, removed);
            } else if (value > node->value_) {
                node->right_ = remove_node_recursive(node->right_, value, removed);
            } else {
                // Nodo encontrado
                removed = true;

                // Casos 1 o 2 (0 o 1 hijo)
                if (node->left_ == nullptr || node->right_ == nullptr) {
                    Node* temp = (node->left_ != nullptr) ? node->left_ : node->right_;

                    if (temp == nullptr) { // 0 hijos
                        temp = node;
                        node = nullptr;
                    } else { // 1 hijo
                        *node = *temp; // Copiar contenido del hijo
                    }
                    delete temp;
                } else {
                    // Caso 3 (2 hijos)
                    // Obtener sucesor in-order (el más pequeño del subárbol derecho)
                    Node* successor = find_min_node(node->right_);
                    node->value_ = successor->value_;
                    // Eliminar el sucesor (esto tendrá 0 o 1 hijo derecho)
                    bool ignored = false;
                    node->right_ = remove_node_recursive(node->right_, successor->value_, ignored);
                }
            }

            // Si el árbol quedó vacío (p.ej., eliminamos el último nodo)
            if (node == nullptr) {
                return nullptr;
            }

            // Actualizar altura (en el camino de vuelta)
            update_height(node);

            // btener factor de balance y re-balancear
            int balance = get_balance_factor(node);

            // Caso 1: Left-Left (LL)
            if (balance > 1 && get_balance_factor(node->left_) >= 0) {
                return right_rotate(node);
            }

            // Caso 2: Right-Right (RR)
            if (balance < -1 && get_balance_factor(node->right_) <= 0) {
                return left_rotate(node);
            }

            // Caso 3: Left-Right (LR)
            if (balance > 1 && get_balance_factor(node->left_) < 0) {
                node->left_ = left_rotate(node->left_);
                return right_rotate(node);
            }

            // Caso 4: Right-Left (RL)
            if (balance < -1 && get_balance_factor(node->right_) > 0) {
                node->right_ = right_rotate(node->right_);
                return left_rotate(node);
            }

            // Si estaba balanceado, retornar el nodo sin cambios
            return node;
        }

        // -----------------------------------------------------------------
        // Helpers Recursivos
        // -----------------------------------------------------------------

        /**
         * @brief Helper recursivo para buscar un valor.
         * @note Complejidad: O(log n)
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
         * @note Complejidad: O(log n)
         */
        Node* find_min_node(Node* node) const {
            while (node != nullptr && node->left_ != nullptr) {
                node = node->left_;
            }
            return node;
        }

        /**
         * @brief Helper para encontrar el nodo con el valor máximo.
         * @note Complejidad: O(log n)
         */
        Node* find_max_node(Node* node) const {
            while (node != nullptr && node->right_ != nullptr) {
                node = node->right_;
            }
            return node;
        }

        /**
         * @brief Helper recursivo para eliminar todos los nodos (post-orden).
         * @note Complejidad: O(n)
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
         * @brief Helper recursivo para copiar el árbol (pre-orden).
         * @note Complejidad: O(n)
         */
        Node* copy_recursive(Node* other_node) {
            if (other_node == nullptr) {
                return nullptr;
            }
            Node* new_node = new Node(other_node->value_);
            new_node->left_ = copy_recursive(other_node->left_);
            new_node->right_ = copy_recursive(other_node->right_);
            // Actualizar la altura después de crear los hijos
            update_height(new_node);
            return new_node;
        }

        // -----------------------------------------------------------------
        // Helpers de Recorrido (Traversals) privados
        // -----------------------------------------------------------------

        void in_order_recursive(Node* node) const {
            if (node == nullptr) return;
            in_order_recursive(node->left_);
            std::cout << node->value_ << " ";
            in_order_recursive(node->right_);
        }

        void pre_order_recursive(Node* node) const {
            if (node == nullptr) return;
            std::cout << node->value_ << " ";
            pre_order_recursive(node->left_);
            pre_order_recursive(node->right_);
        }

        void post_order_recursive(Node* node) const {
            if (node == nullptr) return;
            post_order_recursive(node->left_);
            post_order_recursive(node->right_);
            std::cout << node->value_ << " ";
        }
    };

}

#endif //AVLTREE_H