#ifndef HASHSET_H
#define HASHSET_H

#include "HashMap.h"
#include <functional>

namespace DS {

    /**
     * @class HashSet
     * @brief Implementación de un Conjunto Hash (HashSet) genérico.
     *
     * @tparam K El tipo de la Clave (Key).
     * @tparam Hasher La función (o struct) para calcular el hash de K.
     * Por defecto usa std::hash<K>.
     * @tparam KeyEqual La función (o struct) para comparar la igualdad de dos K.
     * Por defecto usa std::equal_to<K> (que usa operator==).
     *
     * Esta clase se implementa como un adaptador sobre DS::HashMap.
     * Almacena un DS::HashMap<K, bool> internamente, donde el valor
     * booleano es simplemente un marcador de posición.
     */
    template <
        typename K,
        typename Hasher = std::hash<K>,
        typename KeyEqual = std::equal_to<K>
    >
    class HashSet {
    public:
        // -----------------------------------------------------------------
        // Constructores y Destructor (Rule of Zero)
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * @note Complejidad: O(N) donde N es el número inicial de cubos.
         */
        HashSet() : map_() {}

        // El HashMap interno ya maneja correctamente la Regla de los 5,
        // por lo que podemos usar los constructores y asignaciones
        // por defecto (Rule of Zero para HashSet).
        ~HashSet() = default;
        HashSet(const HashSet& other) = default;
        HashSet& operator=(const HashSet& other) = default;
        HashSet(HashSet&& other) noexcept = default;
        HashSet& operator=(HashSet&& other) noexcept = default;

        // -----------------------------------------------------------------
        // Operaciones Principales
        // -----------------------------------------------------------------

        /**
         * @brief Inserta un elemento en el conjunto.
         * Si el elemento ya existe, no hace nada.
         * @param key La clave a insertar.
         * @note Complejidad: O(1) amortizado (igual que HashMap::operator[]).
         */
        void insert(const K& key) {
            // Usamos operator[] de HashMap, que inserta un valor
            // por defecto (false en este caso) si no existe
            // Lo establecemos en 'true' por consistencia
            map_[key] = true;
        }

        /**
         * @brief Elimina un elemento del conjunto.
         * @param key La clave a eliminar.
         * @return true si el elemento existía y fue eliminado, false si no.
         * @note Complejidad: O(1) promedio (igual que HashMap::remove()).
         */
        bool remove(const K& key) {
            return map_.remove(key);
        }

        /**
         * @brief Verifica si un elemento existe en el conjunto.
         * @param key La clave a buscar.
         * @return true si la clave existe, false en caso contrario.
         * @note Complejidad: O(1) promedio (igual que HashMap::contains()).
         */
        bool contains(const K& key) const {
            return map_.contains(key);
        }

        // -----------------------------------------------------------------
        // Capacidad y Observadores
        // -----------------------------------------------------------------

        /**
         * @brief Retorna el número de elementos en el conjunto.
         * @return size_t El número de elementos.
         * @note Complejidad: O(1)
         */
        size_t size() const {
            return map_.size();
        }

        /**
         * @brief Verifica si el conjunto está vacío.
         * @return true si size() == 0.
         * @note Complejidad: O(1)
         */
        bool empty() const {
            return map_.empty();
        }

        /**
         * @brief Elimina todos los elementos del conjunto.
         * @note Complejidad: O(N + M) (igual que HashMap::clear).
         */
        void clear() {
            map_.clear();
        }

        /**
         * @brief Retorna el número actual de cubos (buckets).
         * @return size_t El tamaño del array de cubos.
         * @note Complejidad: O(1)
         */
        size_t bucket_count() const {
            return map_.bucket_count();
        }

        /**
         * @brief Calcula el factor de carga actual.
         * @return double (elementos / cubos)
         * @note Complejidad: O(1)
         */
        double load_factor() const {
            return map_.load_factor();
        }

        /**
         * @brief Retorna un vector con todos los elementos del conjunto.
         * @return DS::Vector<K> Un vector con todos los elementos.
         * @note Complejidad: O(N + M)
         */
        Vector<K> get_all_elements() const {
            return map_.get_all_keys();
        }

    private:
        // El HashMap interno que almacena los datos
        // Usamos <K, bool> donde bool es un valor ficticio
        HashMap<K, bool, Hasher, KeyEqual> map_;
    };

}

#endif //HASHSET_H
