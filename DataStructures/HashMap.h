#ifndef HASHMAP_H
#define HASHMAP_H

#include "Vector.h"
#include "LinkedList.h"

#include <stdexcept>
#include <utility>

namespace DS {

    /**
     * @class HashMap
     * @brief Implementación de una Tabla Hash (HashMap) genérica.
     *
     * @tparam K El tipo de la Clave (Key).
     * @tparam V El tipo del Valor (Value).
     * @tparam Hasher La función (o struct) para calcular el hash de K.
     * Por defecto usa std::hash<K>.
     * @tparam KeyEqual La función (o struct) para comparar la igualdad de dos K.
     * Por defecto usa std::equal_to<K> (que usa operator==).
     *
     * Esta clase utiliza encadenamiento separado para manejar colisiones.
     * - El almacenamiento principal es un DS::Vector de "cubos" (buckets).
     * - Cada cubo es un DS::LinkedList que almacena los pares (K, V) que
     * han colisionado en ese mismo índice.
     * - La tabla se redimensiona automáticamente (rehashing) cuando
     * el "factor de carga" supera un umbral.
     */
    template <
        typename K,
        typename V,
        typename Hasher = std::hash<K>,
        typename KeyEqual = std::equal_to<K>
    >
    class HashMap {
    public:
        // Tipo para el par (Clave, Valor) almacenado en las listas
        using PairType = std::pair<const K, V>;
        // Tipo para el "cubo" (bucket), que es una lista de pares
        using BucketType = LinkedList<PairType>;

        // -----------------------------------------------------------------
        // Constructores
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * @param initial_buckets Número inicial de cubos.
         * @param max_load Factor de carga máximo antes de redimensionar.
         * @note Complejidad: O(N) donde N es 'initial_buckets'
         * (debido al constructor de DS::Vector).
         */
        explicit HashMap(size_t initial_buckets = DEFAULT_INITIAL_BUCKETS,
                         double max_load = DEFAULT_MAX_LOAD_FACTOR)
            : buckets_(initial_buckets > 0 ? initial_buckets : DEFAULT_INITIAL_BUCKETS), // Usa el constructor de DS::Vector(size)
              size_(0),
              hasher_(),
              key_equal_(),
              max_load_factor_(max_load) {
            // buckets_ (Vector) se llena con LinkedLists vacías por defecto
        }

        // Gracias a que DS::Vector y DS::LinkedList ya implementan
        // la Regla de los 5, los constructores y asignaciones
        // por defecto de HashMap (copia, movimiento) funcionan
        // correctamente y realizan deep copies.
        ~HashMap() = default;
        HashMap(const HashMap& other) = default;
        HashMap& operator=(const HashMap& other) = default;
        HashMap(HashMap&& other) noexcept = default;
        HashMap& operator=(HashMap&& other) noexcept = default;


        // -----------------------------------------------------------------
        // Capacidad y Observadores
        // -----------------------------------------------------------------

        /**
         * @brief Retorna el número de elementos (pares K,V) en el mapa.
         * @return size_t El número de elementos.
         * @note Complejidad: O(1)
         */
        size_t size() const {
            return size_;
        }

        /**
         * @brief Verifica si el mapa está vacío.
         * @return true si size_ == 0.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return size_ == 0;
        }

        /**
         * @brief Retorna el número actual de cubos (buckets).
         * @return size_t El tamaño del array de cubos (buckets_.size()).
         * @note Complejidad: O(1)
         */
        size_t bucket_count() const {
            return buckets_.size(); // Llama a DS::Vector::size()
        }

        /**
         * @brief Calcula el factor de carga actual.
         * @return double (elementos / cubos)
         * @note Complejidad: O(1)
         */
        double load_factor() const {
            if (buckets_.empty()) return 0.0;
            return static_cast<double>(size_) / static_cast<double>(buckets_.size());
        }

        // -----------------------------------------------------------------
        // Modificadores
        // -----------------------------------------------------------------

        /**
         * @brief Inserta un par (K, V) o actualiza el valor si K ya existe.
         *
         * @param key La clave.
         * @param value El valor.
         * @note Complejidad:
         * - Promedio: O(1) amortizado (gracias al rehashing).
         * - Peor caso: O(k) donde k es la longitud de la cadena de colisión.
         */
        void insert(const K& key, const V& value) {
            size_t index = get_bucket_index(key);
            BucketType& bucket = buckets_[index];

            // Iterar sobre la lista de colisiones
            for (PairType& pair : bucket) {
                if (key_equal_(pair.first, key)) {
                    // Clave encontrada: actualizar el valor y salir
                    pair.second = value;
                    return;
                }
            }

            // Clave no encontrada, proceder a insertar
            if ((static_cast<double>(size_ + 1) / buckets_.size()) > max_load_factor_) {
                rehash(buckets_.size() * 2);
                index = get_bucket_index(key); // Recalcular índice
            }

            buckets_[index].push_back({key, value}); // push_back es O(1)
            size_++;
        }

        /**
         * @brief Elimina un elemento por su clave.
         *
         * @param key La clave del elemento a eliminar.
         * @return true si el elemento fue encontrado y eliminado, false si no.
         * @note Complejidad:
         * - Promedio: O(1).
         * - Peor caso: O(k) donde k es el tamaño de la cadena de colisión
         */
        bool remove(const K& key) {
            size_t index = get_bucket_index(key);
            BucketType& bucket = buckets_[index];

            // Usar un iterador 'prev' para poder llamar a erase_after(prev)
            auto it = bucket.begin();
            auto prev_it = bucket.end(); // Inicialmente no hay 'prev'

            while (it != bucket.end()) {
                if (key_equal_((*it).first, key)) {
                    // Clave encontrada
                    if (prev_it == bucket.end()) {
                        // Es el primer nodo (head)
                        bucket.pop_front(); // O(1)
                    } else {
                        // Es un nodo en medio o al final
                        bucket.erase_after(prev_it); // O(1)
                    }
                    size_--;
                    return true;
                }
                prev_it = it;
                ++it;
            }
            return false; // No se encontró la clave
        }

        /**
         * @brief Elimina todos los elementos del mapa.
         * Mantiene la capacidad de cubos actual.
         * @note Complejidad: O(N + M) donde N es el número de cubos
         * y M es el número total de elementos (size_).
         */
        void clear() {
            // Llama a clear() en cada LinkedList
            for (size_t i = 0; i < buckets_.size(); ++i) {
                buckets_[i].clear();
            }
            size_ = 0;
        }

        // -----------------------------------------------------------------
        // Búsqueda y Acceso
        // -----------------------------------------------------------------

        /**
         * @brief Busca si una clave existe en el mapa.
         *
         * @param key La clave a buscar.
         * @return true si la clave existe, false en caso contrario.
         * @note Complejidad:
         * - Promedio: O(1).
         * - Peor caso: O(k) donde k es el tamaño de la cadena de colisión.
         */
        bool contains(const K& key) const {
            size_t index = get_bucket_index(key);
            const BucketType& bucket = buckets_[index];

            // Iterar sobre la lista de colisiones
            for (const PairType& pair : bucket) {
                if (key_equal_(pair.first, key)) {
                    return true; // Clave encontrada
                }
            }
            return false;
        }

        /**
         * @brief Comprueba si una clave existe.
         *
         * @return 1 si la clave existe, 0 si no.
         * @note Complejidad:
         * - Promedio: O(1).
         * - Peor caso: O(k) donde k es el tamaño de la cadena de colisión.
         */
        size_t count(const K& key) const {
            // Reutiliza la lógica de contains()
            return contains(key) ? 1 : 0;
        }

        /**
         * @brief Obtiene una referencia al valor asociado con una clave.
         *
         * @param key La clave a buscar.
         * @return V& Una referencia modificable al valor.
         * @throws std::out_of_range si la clave no se encuentra.
         * @note Complejidad:
         * - Promedio: O(1).
         * - Peor caso: O(k) donde k es el tamaño de la cadena de colisión.
         */
        V& at(const K& key) {
            size_t index = get_bucket_index(key);
            BucketType& bucket = buckets_[index];

            // Iterar sobre la lista de colisiones
            for (PairType& pair : bucket) {
                if (key_equal_(pair.first, key)) {
                    return pair.second; // Clave encontrada
                }
            }
            throw std::out_of_range("HashMap::at(): Clave no encontrada.");
        }

        /**
         * @brief Versión const de at().
         * @throws std::out_of_range si la clave no se encuentra.
         * @note Complejidad:
         * - Promedio: O(1).
         * - Peor caso: O(k) donde k es el tamaño de la cadena de colisión.
         */
        const V& at(const K& key) const {
            size_t index = get_bucket_index(key);
            const BucketType& bucket = buckets_[index];

            for (const PairType& pair : bucket) {
                if (key_equal_(pair.first, key)) {
                    return pair.second;
                }
            }
            throw std::out_of_range("HashMap::at() (const): Clave no encontrada.");
        }

        /**
         * @brief Retorna un vector con todas las claves del mapa.
         * @return DS::Vector<K> Un vector con todas las claves.
         * @note Complejidad: O(N + M) donde N es el número de cubos
         * y M es el número total de elementos.
         */
        Vector<K> get_all_keys() const {
            Vector<K> keys_vector;
            keys_vector.reserve(size_);

            // Recorrer cada cubo (bucket)
            for (size_t i = 0; i < buckets_.size(); ++i) {
                // Usar el iterador de LinkedList (O(k) por cubo)
                for (const PairType& pair : buckets_[i]) {
                    keys_vector.push_back(pair.first);
                }
            }
            return keys_vector;
        }

        /**
         * @brief Operador de acceso [].
         *
         * Si la clave 'key' existe, retorna una referencia a su valor.
         * Si 'key' no existe, la inserta automáticamente con un valor
         * por defecto (V{}) y retorna una referencia a ese nuevo valor.
         *
         * @param key La clave.
         * @return V& Una referencia al valor (existente o nuevo).
         * @note Complejidad:
         * - Promedio: O(1) amortizado (igual que insert).
         * - Peor caso: O(n) (igual que insert).
         */
        V& operator[](const K& key) {
            // Intentar encontrar la clave
            size_t index = get_bucket_index(key);
            BucketType& bucket = buckets_[index];

            // Buscar la clave
            for (PairType& pair : bucket) {
                if (key_equal_(pair.first, key)) {
                    return pair.second; // Clave encontrada
                }
            }

            // Clave no encontrada, insertar con valor por defecto
            if ((static_cast<double>(size_ + 1) / buckets_.size()) > max_load_factor_) {
                rehash(buckets_.size() * 2);
                index = get_bucket_index(key); // Recalcular índice
            }

            // Insertar el nuevo par (key, V{})
            // V{} es el valor por defecto (ej: 0 para int, "" para string)
            buckets_[index].push_back({key, V{}}); // O(1)
            size_++;

            // Retornar la referencia al valor recién insertado
            return buckets_[index].back().second;
        }

    private:
        Vector<BucketType> buckets_;     // El array dinámico de cubos
        size_t size_;                    // Número total de elementos (pares K, V)
        Hasher hasher_;                  // Instancia del 'functor' de hash
        KeyEqual key_equal_;             // Instancia del 'functor' de comparación
        double max_load_factor_;         // Límite para disparar el rehash

        static constexpr size_t DEFAULT_INITIAL_BUCKETS = 8;
        static constexpr double DEFAULT_MAX_LOAD_FACTOR = 0.75;

        /**
         * @brief Calcula el índice del cubo para una clave dada.
         * @param key La clave.
         * @return size_t El índice ( 0 <= index < buckets_.size() ).
         * @note Complejidad: O(1) (asumiendo que Hasher es O(1)).
         */
        size_t get_bucket_index(const K& key) const {
            // Llamar al 'functor' de hash y aplica módulo
            // para que quepa en el tamaño de nuestro vector
            return hasher_(key) % buckets_.size();
        }

        /**
         * @brief Redimensiona la tabla hash (rehashing).
         *
         * Crea un nuevo array de cubos más grande y re-inserta
         * todos los elementos existentes en él.
         *
         * @param new_bucket_count El nuevo número de cubos.
         * @note Complejidad: O(N + M) donde N es el nuevo número de cubos
         * y M es el número de elementos (size_).
         */
        void rehash(size_t new_bucket_count) {
            // Mover el vector de cubos antiguo a una variable temporal
            // (std::move transfiere la propiedad sin copiar)
            Vector<BucketType> old_buckets = std::move(buckets_);

            // Crear un nuevo vector de cubos vacío con el nuevo tamaño
            buckets_ = Vector<BucketType>(new_bucket_count);

            // Resetear el tamaño (los re-insertaremos)
            size_ = 0;

            // Iterar sobre el antiguo vector de cubos
            for (size_t i = 0; i < old_buckets.size(); ++i) {
                // Iterar sobre cada LinkedList en el cubo antiguo
                for (const PairType& pair : old_buckets[i]) {
                    // Re-insertar el elemento en la nueva tabla
                    insert(pair.first, pair.second);
                }
                // La old_buckets[i] (LinkedList) se destruye aquí
            }
            // El old_buckets (Vector) se destruye aquí
        }
    };

} // namespace DS

#endif //HASHMAP_H