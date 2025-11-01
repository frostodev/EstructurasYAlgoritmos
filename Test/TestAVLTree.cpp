
#include "TestAVLTree.h"
#include "../DataStructures/Trees/AVLTree.h"

#include <iostream>
#include <string>

namespace Test {

    /**
     * @brief Helper para reportar éxito o error
     */
    void compare_bool_avl(bool actual, bool expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            std::cout << "    Obtenido: " << std::boolalpha << actual << " | Esperado: " << std::boolalpha << expected << std::endl;
        }
    }

    /**
     * @brief Template helper para verificar min/max/size/search
     */
    template<typename T>
    void verify_tree_state(const DS::AVLTree<T>& tree, int expected_size, T expected_min, T expected_max, T search_key, bool should_find) {
        compare_bool_avl(tree.get_size() == expected_size, true, "get_size() == " + std::to_string(expected_size));
        if (expected_size > 0) {
            compare_bool_avl(tree.get_min() == expected_min, true, "get_min() == " + std::to_string(expected_min));
            compare_bool_avl(tree.get_max() == expected_max, true, "get_max() == " + std::to_string(expected_max));
        }
        compare_bool_avl(tree.search_node(search_key), should_find, "search_node(" + std::to_string(search_key) + ")");
    }

    /**
     * @brief Realiza tests de inserción en un AVL
     */
    void test_avl_insertions() {
        std::cout << "\n--- Prueba 1: Inserciones y Rotaciones ---" << std::endl;

        // Caso 1: Rotación Simple Derecha (Left-Left)
        std::cout << "\nSub-prueba: Caso Left-Left (LL)" << std::endl;
        DS::AVLTree<int> tree_ll;
        tree_ll.insert_node(30);
        tree_ll.insert_node(20);
        tree_ll.insert_node(10); // Desbalancea en 30 (BF=2) y en 20 (BF=1) -> Rotación LL
        std::cout << "Insertado: 30, 20, 10" << std::endl;
        std::cout << "Esperado (Level-Order): 20 10 30" << std::endl;
        tree_ll.print_level_order();
        verify_tree_state(tree_ll, 3, 10, 30, 20, true);

        // Caso 2: Rotación Simple Izquierda (Right-Right)
        std::cout << "\nSub-prueba: Caso Right-Right (RR)" << std::endl;
        DS::AVLTree<int> tree_rr;
        tree_rr.insert_node(10);
        tree_rr.insert_node(20);
        tree_rr.insert_node(30); // Desbalancea en 10 (BF=-2) y en 20 (BF=-1) -> Rotación RR
        std::cout << "Insertado: 10, 20, 30" << std::endl;
        std::cout << "Esperado (Level-Order): 20 10 30" << std::endl;
        tree_rr.print_level_order();
        verify_tree_state(tree_rr, 3, 10, 30, 20, true);

        // Caso 3: Rotación Doble Izquierda-Derecha (Left-Right)
        std::cout << "\nSub-prueba: Caso Left-Right (LR)" << std::endl;
        DS::AVLTree<int> tree_lr;
        tree_lr.insert_node(30);
        tree_lr.insert_node(10);
        tree_lr.insert_node(20); // Desbalancea en 30 (BF=2) y en 10 (BF=-1) -> Rotación LR
        std::cout << "Insertado: 30, 10, 20" << std::endl;
        std::cout << "Esperado (Level-Order): 20 10 30" << std::endl;
        tree_lr.print_level_order();
        verify_tree_state(tree_lr, 3, 10, 30, 20, true);

        // Caso 4: Rotación Doble Derecha-Izquierda (Right-Left)
        std::cout << "\nSub-prueba: Caso Right-Left (RL)" << std::endl;
        DS::AVLTree<int> tree_rl;
        tree_rl.insert_node(10);
        tree_rl.insert_node(30);
        tree_rl.insert_node(20); // Desbalancea en 10 (BF=-2) y en 30 (BF=1) -> Rotación RL
        std::cout << "Insertado: 10, 30, 20" << std::endl;
        std::cout << "Esperado (Level-Order): 20 10 30" << std::endl;
        tree_rl.print_level_order();
        verify_tree_state(tree_rl, 3, 10, 30, 20, true);
    }

    /**
     * @brief Realiza tests de eliminación en un AVL
     */
    void test_avl_removals() {
        std::cout << "\n--- Prueba 2: Eliminaciones y Re-balanceo ---" << std::endl;

        // Crear un árbol más complejo para probar eliminaciones
        DS::AVLTree<int> tree;
        int nodes_to_insert[] = {40, 20, 60, 10, 30, 50, 70, 5, 15, 25, 35, 45, 55, 65, 80};
        for (int val : nodes_to_insert) {
            tree.insert_node(val);
        }
        std::cout << "Árbol inicial (15 nodos):" << std::endl;
        tree.print_level_order(); // Debería estar bien balanceado
        verify_tree_state(tree, 15, 5, 80, 45, true);

        // --- Prueba de eliminación que causa rotación (RR) ---
        std::cout << "\nSub-prueba: Eliminación (Caso RR)" << std::endl;
        tree.remove_node(5);  // Eliminar hoja
        tree.remove_node(15); // Eliminar hoja
        std::cout << "Eliminado: 5, 15. Árbol antes de rotar:" << std::endl;
        tree.print_level_order();
        tree.remove_node(10); // Causa desbalance en 20 (BF=-2) -> Rotación RR
        std::cout << "Eliminado: 10. Árbol después de rotar (Raíz 30):" << std::endl;
        tree.print_level_order();
        verify_tree_state(tree, 12, 20, 80, 10, false);

        // --- Prueba de eliminación que causa rotación (LR) ---
        std::cout << "\nSub-prueba: Eliminación (Caso LR)" << std::endl;
        tree.remove_node(80); // Eliminar 80
        tree.remove_node(65); // Eliminar 65
        std::cout << "Eliminado: 80, 65. Árbol antes de rotar:" << std::endl;
        tree.print_level_order();
        tree.remove_node(70); // Causa desbalance en 60 (BF=2) -> Rotación LR (en subárbol 50)
        std::cout << "Eliminado: 70. Árbol después de rotar (Raíz 40):" << std::endl;
        tree.print_level_order();
        verify_tree_state(tree, 9, 20, 55, 70, false);

        // --- Prueba de eliminación de raíz ---
        std::cout << "\nSub-prueba: Eliminación de Raíz" << std::endl;
        tree.remove_node(40); // Eliminar la raíz
        std::cout << "Eliminado: 40 (Raíz). Nueva raíz debe ser 45 (sucesor)." << std::endl;
        tree.print_level_order();
        verify_tree_state(tree, 8, 20, 55, 40, false);
    }

    /**
     * @brief Realiza tests un poco más complejos en un AVL
     */
    void test_avl_comprehensive() {
        std::cout << "\n--- Prueba 3: Prueba Completa (Búsqueda, Min/Max, Clear) ---" << std::endl;
        DS::AVLTree<int> tree;

        std::cout << "Probando árbol vacío..." << std::endl;
        compare_bool_avl(tree.is_empty(), true, "is_empty() en árbol nuevo");
        compare_bool_avl(tree.get_size() == 0, true, "get_size() en árbol nuevo");
        compare_bool_avl(tree.search_node(100), false, "search_node(100) en árbol vacío");
        try {
            tree.get_min();
            std::cout << "  [ERROR] get_min() no lanzó excepción en árbol vacío" << std::endl;
        } catch(const std::out_of_range& e) {
            std::cout << "  [ÉXITO] get_min() lanzó excepción: " << e.what() << std::endl;
        }

        std::cout << "\nInsertando 10, 20, 30, 40, 50, 60, 70 (múltiples rotaciones)..." << std::endl;
        for (int i = 10; i <= 70; i += 10) {
            tree.insert_node(i);
        }
        std::cout << "Árbol final (Level-Order):" << std::endl;
        tree.print_level_order();
        std::cout << "Árbol final (In-Order):" << std::endl;
        tree.print_in_order();

        verify_tree_state(tree, 7, 10, 70, 40, true);
        compare_bool_avl(tree.search_node(45), false, "search_node(45) (no existe)");

        std::cout << "\nEliminando 10, 70, 40..." << std::endl;
        tree.remove_node(10); // Dispara rotación
        tree.remove_node(70); // Dispara rotación
        tree.remove_node(40); // Elimina raíz

        std::cout << "Árbol final (Level-Order):" << std::endl;
        tree.print_level_order();
        verify_tree_state(tree, 4, 20, 60, 10, false);
        verify_tree_state(tree, 4, 20, 60, 70, false);
        verify_tree_state(tree, 4, 20, 60, 40, false);
        compare_bool_avl(tree.search_node(50), true, "search_node(50) (aún existe)");

        std::cout << "\nProbando clear()..." << std::endl;
        tree.clear();
        compare_bool_avl(tree.is_empty(), true, "is_empty() después de clear()");
        compare_bool_avl(tree.get_size() == 0, true, "get_size() después de clear()");
        compare_bool_avl(tree.search_node(50), false, "search_node(50) después de clear()");
    }

    /**
     * @brief Realiza todos los tests de AVL
     */
    void test_avl_tree() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Árbol AVL ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_avl_insertions();
        test_avl_removals();
        test_avl_comprehensive();
    }

} // namespace Test
