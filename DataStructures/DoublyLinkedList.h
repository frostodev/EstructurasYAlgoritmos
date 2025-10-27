
#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stdexcept>
#include <iostream> 

namespace DS {

    /**
     * @class DoublyLinkedList
     * @brief Implementación de una lista doblemente enlazada genérica.
     * @tparam T El tipo de dato que almacenará la lista.
     *
     * Esta clase gestiona una colección de elementos donde cada nodo
     * mantiene punteros al nodo siguiente y al nodo anterior, permitiendo
     * inserciones y eliminaciones eficientes (O(1)) tanto al inicio
     * como al final de la lista.
     */
    template <typename T>
    class DoublyLinkedList {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea una lista doblemente enlazada vacía.
         * @note Complejidad: O(1)
         */
        DoublyLinkedList() : head_(nullptr), tail_(nullptr), node_count_(0) {}

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada para los nodos.
         * @note Complejidad: O(n)
         */
        ~DoublyLinkedList() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * Crea una nueva lista como una copia profunda de 'other'.
         * @param other La lista a copiar.
         * @note Complejidad: O(n)
         */
        DoublyLinkedList(const DoublyLinkedList& other)
            : head_(nullptr), tail_(nullptr), node_count_(0) {
            // Recorre la otra lista y usa push_back para copiar cada elemento
            Node* current = other.head_;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * Sobrescribe esta lista con una copia profunda de 'other'.
         * @param other La lista a asignar.
         * @return DoublyLinkedList& Una referencia a esta lista (*this).
         * @note Complejidad: O(n + m) donde n=this->size y m=other.size
         */
        DoublyLinkedList& operator=(const DoublyLinkedList& other) {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Borrar el contenido actual
            clear();

            // Copiar los elementos de 'other'
            Node* current = other.head_;
            while (current != nullptr) {
                push_back(current->data);
                current = current->next;
            }
            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * "Roba" los recursos de 'other' y la deja en estado vacío.
         * @param other La lista (r-value) de la que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        DoublyLinkedList(DoublyLinkedList&& other) noexcept
            : head_(other.head_), tail_(other.tail_), node_count_(other.node_count_) {
            // 'other' ya no es dueña de los nodos
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.node_count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * Libera los recursos de esta lista y "roba" los de 'other'.
         * @param other La lista (r-value) a asignar.
         * @return DoublyLinkedList& Una referencia a esta lista (*this).
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Liberar los recursos propios
            clear();

            // Transferir la propiedad de 'other' a 'this'
            head_ = other.head_;
            tail_ = other.tail_;
            node_count_ = other.node_count_;

            // Dejar 'other' en un estado vacío y válido
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.node_count_ = 0;

            return *this;
        }

        // -----------------------------------------------------------------
        // Métodos de Capacidad
        // -----------------------------------------------------------------

        /**
         * @brief Retorna el número de elementos en la lista.
         * @return int El número de elementos.
         * @note Complejidad: O(1)
         */
        int get_size() const {
            return node_count_;
        }

        /**
         * @brief Verifica si la lista está vacía.
         * @return true si la lista está vacía, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool is_empty() const {
            return node_count_ == 0;
        }

        // -----------------------------------------------------------------
        // Métodos de Inserción
        // -----------------------------------------------------------------

        /**
         * @brief Añade un elemento al inicio de la lista.
         * @param value El valor a añadir.
         * @note Complejidad: O(1)
         */
        void push_front(const T& value) {
            Node* new_node = new Node(value);
            if (is_empty()) {
                // La lista está vacía, el nuevo nodo es cabeza y cola
                head_ = new_node;
                tail_ = new_node;
            } else {
                // La lista tiene nodos
                new_node->next = head_; // El nuevo nodo apunta a la antigua cabeza
                head_->prev = new_node; // La antigua cabeza apunta (prev) al nuevo nodo
                head_ = new_node;       // El nuevo nodo es la nueva cabeza
            }
            node_count_++;
        }

        /**
         * @brief Añade un elemento al final de la lista.
         * @param value El valor a añadir.
         * @note Complejidad: O(1)
         */
        void push_back(const T& value) {
            Node* new_node = new Node(value);
            if (is_empty()) {
                // La lista está vacía, el nuevo nodo es cabeza y cola
                head_ = new_node;
                tail_ = new_node;
            } else {
                // La lista tiene nodos
                tail_->next = new_node; // La antigua cola apunta (next) al nuevo nodo
                new_node->prev = tail_; // El nuevo nodo apunta (prev) a la antigua cola
                tail_ = new_node;       // El nuevo nodo es la nueva cola
            }
            node_count_++;
        }

        /**
         * @brief Inserta un elemento en un índice específico.
         * @param index El índice (0-based) donde insertar.
         * @param value El valor a insertar.
         * @throws std::out_of_range si el índice es < 0 o > get_size().
         * @note Complejidad: O(n) en el peor caso, O(1) si index es 0 o get_size().
         */
        void insert_at(int index, const T& value) {
            if (index < 0 || index > node_count_) {
                throw std::out_of_range("DoublyLinkedList::insert_at(): Índice fuera de rango");
            }

            if (index == 0) {
                push_front(value);
            } else if (index == node_count_) {
                push_back(value);
            } else {
                // Encontrar el nodo *actualmente* en 'index'
                Node* current_at_index = get_node_at(index);
                Node* prev_node = current_at_index->prev;
                Node* new_node = new Node(value);

                // Conectar nuevo_nodo con el anterior (prev_node)
                prev_node->next = new_node;
                new_node->prev = prev_node;

                // Conectar nuevo_nodo con el siguiente (current_at_index)
                new_node->next = current_at_index;
                current_at_index->prev = new_node;

                node_count_++;
            }
        }

        // -----------------------------------------------------------------
        // Métodos de Eliminación
        // -----------------------------------------------------------------

        /**
         * @brief Elimina el primer elemento de la lista.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(1)
         */
        void pop_front() {
            if (is_empty()) {
                throw std::out_of_range("DoublyLinkedList::pop_front(): La lista está vacía.");
            }

            Node* node_to_delete = head_;

            if (node_count_ == 1) {
                // Caso especial: solo hay un elemento
                head_ = nullptr;
                tail_ = nullptr;
            } else {
                // Hay 2 o más elementos
                head_ = head_->next; // La nueva cabeza es el segundo nodo
                head_->prev = nullptr; // La nueva cabeza no tiene nodo previo
            }

            delete node_to_delete;
            node_count_--;
        }

        /**
         * @brief Elimina el último elemento de la lista.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(1)
         */
        void pop_back() {
            if (is_empty()) {
                throw std::out_of_range("DoublyLinkedList::pop_back(): La lista está vacía.");
            }

            Node* node_to_delete = tail_;

            if (node_count_ == 1) {
                // Caso especial: solo hay un elemento
                head_ = nullptr;
                tail_ = nullptr;
            } else {
                // Hay 2 o más elementos
                tail_ = tail_->prev; // La nueva cola es el penúltimo nodo
                tail_->next = nullptr; // La nueva cola no tiene nodo siguiente
            }

            delete node_to_delete;
            node_count_--;
        }

        /**
         * @brief Elimina un elemento en un índice específico.
         * @param index El índice (0-based) del elemento a eliminar.
         * @throws std::out_of_range si el índice es inválido.
         * @note Complejidad: O(n) en el peor caso, O(1) si index es 0 o get_size()-1.
         */
        void remove_at(int index) {
            if (index < 0 || index >= node_count_) {
                throw std::out_of_range("DoublyLinkedList::remove_at(): Índice fuera de rango.");
            }

            if (index == 0) {
                pop_front();
            } else if (index == node_count_ - 1) {
                pop_back();
            } else {
                // Encontrar el nodo a eliminar
                Node* node_to_delete = get_node_at(index);
                Node* prev_node = node_to_delete->prev;
                Node* next_node = node_to_delete->next;

                // "Saltar" el nodo a eliminar
                prev_node->next = next_node;
                next_node->prev = prev_node;

                delete node_to_delete;
                node_count_--;
            }
        }

        /**
         * @brief Elimina todos los nodos de la lista.
         * La lista queda en estado vacío.
         * @note Complejidad: O(n)
         */
        void clear() {
            Node* current = head_;
            while (current != nullptr) {
                Node* next_node = current->next;
                delete current;
                current = next_node;
            }
            head_ = nullptr;
            tail_ = nullptr;
            node_count_ = 0;
        }

        // -----------------------------------------------------------------
        // Métodos de Acceso
        // -----------------------------------------------------------------

        /**
         * @brief Obtiene una referencia al dato en el primer nodo.
         * @return T& Referencia al dato.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(1)
         */
        T& get_front() {
            if (is_empty()) {
                throw std::out_of_range("DoublyLinkedList::get_front(): La lista está vacía.");
            }
            return head_->data;
        }

        /**
         * @brief Obtiene una referencia al dato en el último nodo.
         * @return T& Referencia al dato.
         * @throws std::out_of_range si la lista está vacía.
         * @note Complejidad: O(1)
         */
        T& get_back() {
            if (is_empty()) {
                throw std::out_of_range("DoublyLinkedList::get_back(): La lista está vacía.");
            }
            return tail_->data;
        }

        /**
         * @brief Obtiene una referencia al dato en un índice específico.
         * @param index El índice (0-based) del elemento.
         * @return T& Referencia al dato.
         * @throws std::out_of_range si el índice es inválido.
         * @note Complejidad: O(n) - optimizado para O(n/2)
         */
        T& get(int index) {
            // get_node_at ya() valida el índice y lanza excepción si es necesario
            return get_node_at(index)->data;
        }

        /**
         * @brief Obtiene una referencia constante al dato en un índice.
         * (Versión const-correct para listas constantes).
         * @param index El índice (0-based) del elemento.
         * @return const T& Referencia constante al dato.
         * @throws std::out_of_range si el índice es inválido.
         * @note Complejidad: O(n) - optimizado para O(n/2)
         */
        const T& get(int index) const {
            return get_node_at(index)->data;
        }

        // -----------------------------------------------------------------
        // Utilidades
        // -----------------------------------------------------------------

        /**
         * @brief Imprime el contenido de la lista en la consola (hacia adelante).
         * Útil para depuración.
         * @note Complejidad: O(n)
         */
        void print_list() const {
            Node* current = head_;
            std::cout << "nullptr <- ";
            while (current != nullptr) {
                std::cout << "[" << current->data << "] <-> ";
                current = current->next;
            }
            std::cout << "nullptr" << std::endl;
        }

        /**
         * @brief Imprime el contenido de la lista en la consola (hacia atrás).
         * Útil para depurar los punteros 'prev'.
         * @note Complejidad: O(n)
         */
        void print_list_reverse() const {
            Node* current = tail_;
            std::cout << "nullptr <- ";
            while (current != nullptr) {
                std::cout << "[" << current->data << "] <-> ";
                current = current->prev;
            }
            std::cout << "nullptr" << std::endl;
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en la lista doble.
         * Contiene el dato y punteros al nodo siguiente y anterior.
         */
        struct Node {
            T data;
            Node* next;
            Node* prev;

            /**
             * @brief Constructor del Nodo.
             * @param val El valor (dato) que almacenará el nodo.
             */
            Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
        };

        Node* head_;        // Puntero al primer nodo (cabeza) de la lista.
        Node* tail_;        // Puntero al último nodo (cola) de la lista.
        int node_count_;    // Número total de nodos en la lista.

        /**
         * @brief Función auxiliar privada para encontrar un nodo en un índice.
         * Optimiza el recorrido empezando desde 'head_' o 'tail_'
         * dependiendo de qué esté más cerca del índice solicitado.
         * @param index El índice del nodo a buscar (0-based).
         * @return Node* Un puntero al nodo en el índice solicitado.
         * @throws std::out_of_range si el índice es inválido.
         * @note Complejidad: O(n)
         */
        Node* get_node_at(int index) const {
            if (index < 0 || index >= node_count_) {
                throw std::out_of_range("DoublyLinkedList::get_node_at(): Índice fuera de rango.");
            }

            // Optimización: decidir si empezar por la cabeza o la cola
            if (index < node_count_ / 2) {
                // Empezar desde la cabeza (más cerca del inicio)
                Node* current = head_;
                for (int i = 0; i < index; ++i) {
                    current = current->next;
                }
                return current;
            } else {
                // Empezar desde la cola (más cerca del final)
                Node* current = tail_;
                // (node_count_ - 1) es el índice de la cola
                for (int i = (node_count_ - 1); i > index; --i) {
                    current = current->prev;
                }
                return current;
            }
        }

    };

}

#endif //DOUBLYLINKEDLIST_H