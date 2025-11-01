#include "TestHashSet.h"
#include "../DataStructures/HashSet.h"
#include <iostream>

namespace Test {

    /**
     * @brief Verifica si la condición se cumple.
     */
    bool check(const std::string& test_name, bool condition) {
        if (condition) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
            return true;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            return false;
        }
    }

    /**
     * @brief Prueba los métodos de HashSet
     */
    void test_hash_set() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de HashSet ---" << std::endl;
        std::cout << "=============================================" << std::endl;

        DS::HashSet<std::string> set;

        // Test 1: empty() y size() en conjunto nuevo
        check("Nuevo conjunto está vacío", set.empty());
        check("Nuevo conjunto tiene tamaño 0", set.size() == 0);

        // Test 2: insert()
        set.insert("manzana");
        set.insert("banana");
        set.insert("naranja");
        check("Conjunto no está vacío después de insertar", !set.empty());
        check("Tamaño es 3 después de 3 inserciones", set.size() == 3);

        // Test 3: Inserción de duplicados
        set.insert("banana"); // Intentar insertar duplicado
        check("Tamaño sigue siendo 3 después de insertar duplicado", set.size() == 3);

        // Test 4: contains()
        check("contains() encuentra 'manzana'", set.contains("manzana"));
        check("contains() encuentra 'banana'", set.contains("banana"));
        check("contains() encuentra 'naranja'", set.contains("naranja"));
        check("contains() no encuentra 'uva'", !set.contains("uva"));

        // Test 5: remove()
        bool removed = set.remove("manzana");
        check("remove() retorna true para elemento existente", removed);
        check("Tamaño es 2 después de eliminar 'manzana'", set.size() == 2);
        check("contains() ya no encuentra 'manzana'", !set.contains("manzana"));
        check("contains() aún encuentra 'banana'", set.contains("banana"));

        // Test 6: remove() de elemento no existente
        bool removed_fail = set.remove("uva");
        check("remove() retorna false para elemento no existente", !removed_fail);
        check("Tamaño sigue siendo 2", set.size() == 2);

        // Test 7: clear()
        set.clear();
        check("Conjunto está vacío después de clear()", set.empty());
        check("Tamaño es 0 después de clear()", set.size() == 0);
        check("contains() ya no encuentra 'banana' después de clear", !set.contains("banana"));

        // Test 8: Prueba de Rehashing (depende de la implementación interna de HashMap)
        // HashMap por defecto tiene 8 cubos y factor de carga 0.75
        // (8 * 0.75 = 6). La 7ma inserción (size_ + 1 > 6) debe forzar un rehash.
        std::cout << "\nPrueba de Rehashing..." << std::endl;
        DS::HashSet<int> int_set;
        int_set.insert(1);
        int_set.insert(2);
        int_set.insert(3);
        int_set.insert(4);
        int_set.insert(5);
        int_set.insert(6); // size = 6, load = 6/8 = 0.75. Aún no hay rehash.
        check("Tamaño del cubo antes del rehash (esperado 8)", int_set.bucket_count() == 8);

        int_set.insert(7); // size = 7, (7/8 > 0.75). Rehash a 16 cubos.
        check("Tamaño del cubo después del rehash (esperado 16)", int_set.bucket_count() == 16);
        check("Tamaño es 7 después de 7 inserciones", int_set.size() == 7);

        // Verificar que todos los elementos sigan allí
        check("contains(1) después de rehash", int_set.contains(1));
        check("contains(4) después de rehash", int_set.contains(4));
        check("contains(7) después de rehash", int_set.contains(7));
        check("!contains(99) después de rehash", !int_set.contains(99));

        // Insertar más para estar seguros
        int_set.insert(8);
        int_set.insert(9);
        int_set.insert(10);
        check("Tamaño es 10", int_set.size() == 10);
        check("contains(10) después de rehash", int_set.contains(10));
    }

}
