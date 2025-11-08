#include "TestTensor3D.h"
#include "../Math/Tensor3D.h"
#include <iostream>
#include <stdexcept>
#include <string>

namespace Test {

    /**
     * @brief Compara dos valores bool y reporta el resultado.
     */
    void compare_bool_tensor(bool actual, bool expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            std::cout << "    Obtenido: " << std::boolalpha << actual << " | Esperado: " << std::boolalpha << expected << std::endl;
        }
    }

    /**
     * @brief Prueba la funcionalidad básica del Tensor3D
     */
    void test_tensor3d_functionality() {
        std::cout << "\n--- Prueba 1: Funcionalidad Básica (Constructor, Acceso, Escritura) ---" << std::endl;

        // Dimensiones: 2 capas (Z), 3 filas (Y), 4 columnas (X)
        size_t Z = 2, Y = 3, X = 4;
        Math::Tensor3D<int> tensor(Z, Y, X);

        // Verificar constructor
        compare_bool_tensor(tensor.layers() == Z, true, "Constructor: layers() == 2");
        compare_bool_tensor(tensor.rows() == Y, true, "Constructor: rows() == 3");
        compare_bool_tensor(tensor.cols() == X, true, "Constructor: cols() == 4");
        compare_bool_tensor(tensor.size() == Z * Y * X, true, "Constructor: size() == 24");

        // Verificar inicialización a cero
        compare_bool_tensor(tensor(0, 0, 0) == 0, true, "Inicialización: (0, 0, 0) es 0");
        compare_bool_tensor(tensor(1, 2, 3) == 0, true, "Inicialización: (1, 2, 3) es 0");

        // Prueba de Escritura y Lectura
        std::cout << "\nProbando Escritura y Lectura..." << std::endl;
        tensor(0, 0, 0) = 100; // Primera esquina
        tensor(1, 2, 3) = 200; // Última esquina
        tensor(0, 1, 1) = 55;  // Un punto en medio

        compare_bool_tensor(tensor(0, 0, 0) == 100, true, "Lectura/Escritura (0, 0, 0)");
        compare_bool_tensor(tensor(1, 2, 3) == 200, true, "Lectura/Escritura (1, 2, 3)");
        compare_bool_tensor(tensor(0, 1, 1) == 55, true, "Lectura/Escritura (0, 1, 1)");

        // Verificar que otros elementos no cambiaron
        compare_bool_tensor(tensor(0, 0, 1) == 0, true, "Verificación de no-cambio (0, 0, 1)");

        // Prueba de acceso 'const'
        const Math::Tensor3D<int>& const_tensor = tensor;
        compare_bool_tensor(const_tensor(1, 2, 3) == 200, true, "Lectura Const (1, 2, 3)");
    }

    /**
     * @brief Prueba el manejo de errores del Tensor3D
     */
    void test_tensor3d_errors() {
        std::cout << "\n--- Prueba 2: Manejo de Errores (Constructor y Acceso) ---" << std::endl;

        // Pruebas de constructor
        std::cout << "Probando excepciones del constructor..." << std::endl;
        try {
            Math::Tensor3D<int> t_bad(1, 1, 0); // Columna 0
            std::cout << "  [ERROR] Constructor (1, 1, 0) no lanzó excepción." << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "  [ÉXITO] Constructor (1, 1, 0) capturó: " << e.what() << std::endl;
        }

        try {
            Math::Tensor3D<int> t_bad(1, 0, 1); // Fila 0
            std::cout << "  [ERROR] Constructor (1, 0, 1) no lanzó excepción." << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "  [ÉXITO] Constructor (1, 0, 1) capturó: " << e.what() << std::endl;
        }

        // Pruebas de acceso fuera de rango
        std::cout << "\nProbando excepciones de acceso (out_of_range)..." << std::endl;
        Math::Tensor3D<int> tensor(2, 3, 4);

        try {
            tensor(2, 0, 0); // Capa (z) fuera de rango
            std::cout << "  [ERROR] Acceso (2, 0, 0) no lanzó excepción." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "  [ÉXITO] Acceso (2, 0, 0) capturó: " << e.what() << std::endl;
        }

        try {
            tensor(0, 3, 0); // Fila (y) fuera de rango
            std::cout << "  [ERROR] Acceso (0, 3, 0) no lanzó excepción." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "  [ÉXITO] Acceso (0, 3, 0) capturó: " << e.what() << std::endl;
        }

        try {
            tensor(0, 0, 4); // Columna (x) fuera de rango
            std::cout << "  [ERROR] Acceso (0, 0, 4) no lanzó excepción." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "  [ÉXITO] Acceso (0, 0, 4) capturó: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Prueba la lógica de indexación (aplanamiento)
     */
    void test_tensor3d_indexing() {
        std::cout << "\n--- Prueba 3: Lógica de Indexación (Aplanamiento) ---" << std::endl;
        // Tensor de 2x2x2 = 8 elementos
        Math::Tensor3D<int> tensor(2, 2, 2);

        // Llenar con valores únicos basados en la fórmula de indexación
        // index = (z * rows_ * cols_) + (y * cols_) + x
        // index = (z * 2 * 2) + (y * 2) + x
        // index = 4*z + 2*y + x
        int counter = 0;
        for (size_t z = 0; z < tensor.layers(); ++z) {
            for (size_t y = 0; y < tensor.rows(); ++y) {
                for (size_t x = 0; x < tensor.cols(); ++x) {
                    tensor(z, y, x) = counter++;
                }
            }
        }

        std::cout << "Tensor (2x2x2) Lleno:" << std::endl;
        std::cout << tensor;

        // Verificar valores específicos
        // (z=0, y=0, x=0) -> 4*0 + 2*0 + 0 = 0
        compare_bool_tensor(tensor(0, 0, 0) == 0, true, "Index (0,0,0) -> 0");
        // (z=0, y=0, x=1) -> 4*0 + 2*0 + 1 = 1
        compare_bool_tensor(tensor(0, 0, 1) == 1, true, "Index (0,0,1) -> 1");
        // (z=0, y=1, x=0) -> 4*0 + 2*1 + 0 = 2
        compare_bool_tensor(tensor(0, 1, 0) == 2, true, "Index (0,1,0) -> 2");
        // (z=0, y=1, x=1) -> 4*0 + 2*1 + 1 = 3
        compare_bool_tensor(tensor(0, 1, 1) == 3, true, "Index (0,1,1) -> 3");
        // (z=1, y=0, x=0) -> 4*1 + 2*0 + 0 = 4
        compare_bool_tensor(tensor(1, 0, 0) == 4, true, "Index (1,0,0) -> 4");
        // (z=1, y=1, x=1) -> 4*1 + 2*1 + 1 = 7
        compare_bool_tensor(tensor(1, 1, 1) == 7, true, "Index (1,1,1) -> 7");
    }

    /**
     * @brief Ejecuta todas las pruebas de Tensor3D
     */
    void test_tensor3d() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Tensor3D ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_tensor3d_functionality();
        test_tensor3d_errors();
        test_tensor3d_indexing();
    }

}