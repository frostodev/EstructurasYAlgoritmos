
#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

namespace DS {

    /**
     * @class Queue
     * @brief Implementación de una Cola (FIFO - First-In, First-Out) genérica.
     * @tparam T El tipo de dato que almacenará la cola.
     *
     * Esta estructura de datos restringe la inserción (enqueue) a un extremo
     * (la "cola" o "final") y la eliminación (dequeue) al otro extremo
     * (la "cabeza" o "frente").
     */
    template <typename T>
    class Queue {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * Crea una cola vacía.
         * @note Complejidad: O(1)
         */
        Queue() : head_(nullptr), tail_(nullptr), element_count_(0) {}

        /**
         * @brief Destructor.
         * Libera toda la memoria asignada para los nodos.
         * @note Complejidad: O(n)
         */
        ~Queue() {
            clear();
        }

        /**
         * @brief Constructor de copia (Deep Copy).
         * Crea una nueva cola como una copia profunda de 'other'.
         * @param other La cola a copiar.
         * @note Complejidad: O(n)
         */
        Queue(const Queue& other) : head_(nullptr), tail_(nullptr), element_count_(0) {
            // Recorre la 'other' y encola cada elemento en 'this'
            Node* current_other = other.head_;
            while (current_other != nullptr) {
                push(current_other->data);
                current_other = current_other->next;
            }
        }

        /**
         * @brief Operador de asignación por copia (Copy-and-Swap idiom).
         * @param other La cola a asignar.
         * @return Queue& Una referencia a esta cola (*this).
         * @note Complejidad: O(n)
         */
        Queue& operator=(const Queue& other) {
            if (this == &other) {
                return *this; // Protección contra auto-asignación
            }

            // Crea una copia temporal y luego intercambia
            Queue temp(other);
            swap(temp);

            return *this;
        }

        /**
         * @brief Constructor por movimiento.
         * "Roba" los recursos de 'other' y la deja en estado vacío.
         * @param other La cola (r-value) de la que se moverán los recursos.
         * @note Complejidad: O(1)
         */
        Queue(Queue&& other) noexcept
            : head_(other.head_),
              tail_(other.tail_),
              element_count_(other.element_count_) {
            // 'other' ya no es dueña de los nodos
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.element_count_ = 0;
        }

        /**
         * @brief Operador de asignación por movimiento.
         * @param other La cola (r-value) a asignar.
         * @return Queue& Una referencia a esta cola (*this).
         * @note Complejidad: O(n) para limpiar, O(1) para mover.
         */
        Queue& operator=(Queue&& other) noexcept {
            if (this == &other) {
                return *this;
            }

            clear(); // Libera los recursos propios

            // Transfiere la propiedad
            head_ = other.head_;
            tail_ = other.tail_;
            element_count_ = other.element_count_;

            // Deja 'other' en estado vacío
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.element_count_ = 0;

            return *this;
        }

        // -----------------------------------------------------------------
        // Operaciones Principales
        // -----------------------------------------------------------------

        /**
         * @brief Inserta un elemento al final (tail) de la cola.
         * @param value El valor a encolar.
         * @note Complejidad: O(1)
         */
        void push(const T& value) {
            Node* new_node = new Node(value);

            if (empty()) {
                // Si la cola está vacía, el nuevo nodo es cabeza y cola
                head_ = new_node;
                tail_ = new_node;
            } else {
                // La antigua cola apunta al nuevo nodo
                tail_->next = new_node;
                // El nuevo nodo se convierte en la nueva cola
                tail_ = new_node;
            }
            element_count_++;
        }

        /**
         * @brief Elimina el elemento del frente (head) de la cola.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        void pop() {
            if (empty()) {
                throw std::out_of_range("Queue::pop(): Llamado en una cola vacía.");
            }

            Node* node_to_delete = head_;
            head_ = head_->next; // Mueve la cabeza al siguiente
            delete node_to_delete;
            element_count_--;

            // Caso especial: si la cola quedó vacía, tail_ también debe ser null
            if (empty()) {
                tail_ = nullptr;
            }
        }

        /**
         * @brief Obtiene una referencia al elemento en el frente (head) de la cola.
         * @return T& Referencia al dato en el frente.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        T& front() {
            if (empty()) {
                throw std::out_of_range("Queue::front(): Llamado en una cola vacía.");
            }
            return head_->data;
        }

        /**
         * @brief Obtiene una referencia constante al elemento en el frente.
         * (Versión const-correct).
         * @return const T& Referencia constante al dato en el frente.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        const T& front() const {
            if (empty()) {
                throw std::out_of_range("Queue::front() (const): Llamado en una cola vacía.");
            }
            return head_->data;
        }

        /**
         * @brief Obtiene una referencia al elemento en el final (tail) de la cola.
         * @return T& Referencia al dato en el final.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        T& back() {
            if (empty()) {
                throw std::out_of_range("Queue::back(): Llamado en una cola vacía.");
            }
            return tail_->data;
        }

        /**
         * @brief Obtiene una referencia constante al elemento en el final.
         * (Versión const-correct).
         * @return const T& Referencia constante al dato en el final.
         * @throws std::out_of_range si la cola está vacía.
         * @note Complejidad: O(1)
         */
        const T& back() const {
             if (empty()) {
                throw std::out_of_range("Queue::back() (const): Llamado en una cola vacía.");
            }
            return tail_->data;
        }

        // -----------------------------------------------------------------
        // Métodos de Capacidad y Utilidad
        // -----------------------------------------------------------------

        /**
         * @brief Verifica si la cola está vacía.
         * @return true si la cola está vacía, false en caso contrario.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return element_count_ == 0;
        }

        /**
         * @brief Retorna el número de elementos en la cola.
         * @return int El número de elementos.
         * @note Complejidad: O(1)
         */
        int size() const {
            return element_count_;
        }

        /**
         * @brief Elimina todos los elementos de la cola.
         * La cola queda en estado vacío.
         * @note Complejidad: O(n)
         */
        void clear() {
            while (!empty()) {
                pop(); // pop() ya maneja la memoria y los punteros
            }
        }

    private:
        /**
         * @struct Node
         * @brief Estructura interna que representa un nodo en la cola.
         */
        struct Node {
            T data;
            Node* next; // Puntero al siguiente elemento en la cola

            explicit Node(const T& val) : data(val), next(nullptr) {}
        };

        Node* head_;          // Puntero al frente/cabeza de la cola (por donde se saca).
        Node* tail_;          // Puntero al final/cola de la cola (por donde se mete).
        int element_count_;   // Número de elementos en la cola.

        /**
         * @brief Función auxiliar para intercambiar el estado de dos colas.
         * @param other La otra cola con la que intercambiar el contenido.
         * @note Complejidad: O(1)
         */
        void swap(Queue& other) noexcept {
            std::swap(head_, other.head_);
            std::swap(tail_, other.tail_);
            std::swap(element_count_, other.element_count_);
        }
    };

}

#endif //QUEUE_H
