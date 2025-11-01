
#include "TestMatrix.h"
#include "../Math/Matrix.h"
#include <iostream>
#include <stdexcept>

namespace Test {

    /**
     * @brief Compara dos valores y reporta el resultado.
     */
    void compare_cases(double actual_value, double expected_value) {
        std::cout << "Valor obtenido: " << actual_value << " | Valor esperado: " << expected_value
        << " (Error: " << expected_value - actual_value << ")" << std::endl;
    }

    /**
     * @brief Compara dos matrices usando operator== y reporta el resultado.
     */
    template<Math::Numeric T>
    void compare_matrices(const Math::Matrix<T>& actual, const Math::Matrix<T>& expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            std::cout << "    Obtenido:" << std::endl << actual;
            std::cout << "    Esperado:" << std::endl << expected;
        }
    }

    /**
     * @brief Compara dos valores bool y reporta el resultado.
     */
    void compare_bool(bool actual, bool expected, const std::string& test_name) {
        if (actual == expected) {
            std::cout << "  [ÉXITO] " << test_name << std::endl;
        } else {
            std::cout << "  [ERROR] " << test_name << std::endl;
            std::cout << "    Obtenido: " << std::boolalpha << actual << " | Esperado: " << std::boolalpha << expected << std::endl;
        }
    }

    /**
     * @brief Prueba el operador de acceso.
     */
    void test_operator_access() {
        std::cout << "\n--- Prueba Operador de Acceso () --- " << std::endl;

        Math::Matrix<int> m(2, 3);
        m(0, 0) = 1; m(0, 1) = 2; m(0, 2) = 3;
        m(1, 0) = 4; m(1, 1) = 5; m(1, 2) = 6;

        std::cout << "Matriz Base:" << std::endl << m << std::endl;

        // Prueba de lectura
        compare_bool(m(0, 0) == 1, true, "Lectura (0, 0)");
        compare_bool(m(1, 2) == 6, true, "Lectura (1, 2)");

        // Prueba de escritura
        m(0, 0) = 99;
        compare_bool(m(0, 0) == 99, true, "Escritura (0, 0)");

        // Prueba de acceso 'const'
        const Math::Matrix<int>& const_m = m;
        compare_bool(const_m(1, 2) == 6, true, "Lectura Const (1, 2)");

        // Pruebas de excepciones (fuera de rango)
        std::cout << "\nPruebas de Excepciones (out_of_range):" << std::endl;
        try {
            m(2, 0); // Fila fuera de rango
            std::cout << "  [ERROR] Acceso (2, 0) no lanzó excepción." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "  [ÉXITO] Acceso (2, 0) capturó: " << e.what() << std::endl;
        }

        try {
            m(0, 3); // Columna fuera de rango
            std::cout << "  [ERROR] Acceso (0, 3) no lanzó excepción." << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "  [ÉXITO] Acceso (0, 3) capturó: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Prueba el operador de adición.
     */
    void test_operator_addition() {
        std::cout << "\n--- Prueba Operador de Suma + --- " << std::endl;

        Math::Matrix<int> m1(2, 2);
        m1(0, 0) = 1; m1(0, 1) = 2;
        m1(1, 0) = 3; m1(1, 1) = 4;

        Math::Matrix<int> m2(2, 2);
        m2(0, 0) = 5; m2(0, 1) = 6;
        m2(1, 0) = 7; m2(1, 1) = 8;

        // Caso Típico
        Math::Matrix<int> expected1(2, 2);
        expected1(0, 0) = 6;  expected1(0, 1) = 8;
        expected1(1, 0) = 10; expected1(1, 1) = 12;
        compare_matrices(m1 + m2, expected1, "Suma Típica 2x2");

        // Caso Borde: Suma con matriz cero
        Math::Matrix<int> m_zero(2, 2);
        compare_matrices(m1 + m_zero, m1, "Suma con Matriz Cero");

        // Caso Borde: 1x1
        Math::Matrix<double> m1x1_a(1, 1); m1x1_a(0, 0) = 10.5;
        Math::Matrix<double> m1x1_b(1, 1); m1x1_b(0, 0) = -5.0;
        Math::Matrix<double> m1x1_exp(1, 1); m1x1_exp(0, 0) = 5.5;
        compare_matrices(m1x1_a + m1x1_b, m1x1_exp, "Suma 1x1 con double");

        // Pruebas de excepciones (dimensiones incompatibles)
        std::cout << "\nPruebas de Excepciones (invalid_argument):" << std::endl;
        Math::Matrix<int> m_diff_cols(2, 3);
        Math::Matrix<int> m_diff_rows(3, 2);

        try {
            auto result = m1 + m_diff_cols;
            std::cout << "  [ERROR] Suma (2x2 + 2x3) no lanzó excepción." << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "  [ÉXITO] Suma (2x2 + 2x3) capturó: " << e.what() << std::endl;
        }

        try {
            auto result = m1 + m_diff_rows;
            std::cout << "  [ERROR] Suma (2x2 + 3x2) no lanzó excepción." << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cout << "  [ÉXITO] Suma (2x2 + 3x2) capturó: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Prueba el operador de resta.
     */
    void test_operator_subtraction() {
        std::cout << "\n--- Prueba Operador de Resta - --- " << std::endl;

        Math::Matrix<int> m1(2, 2);
        m1(0, 0) = 10; m1(0, 1) = 20;
        m1(1, 0) = 30; m1(1, 1) = 40;

        Math::Matrix<int> m2(2, 2);
        m2(0, 0) = 1; m2(0, 1) = 2;
        m2(1, 0) = 3; m2(1, 1) = 4;

        // Caso Típico
        Math::Matrix<int> expected1(2, 2);
        expected1(0, 0) = 9;  expected1(0, 1) = 18;
        expected1(1, 0) = 27; expected1(1, 1) = 36;
        compare_matrices(m1 - m2, expected1, "Resta Típica 2x2");

        // Caso Borde: Restarse a sí misma
        Math::Matrix<int> m_zero(2, 2);
        compare_matrices(m1 - m1, m_zero, "Resta de sí misma (A - A = 0)");

        // Caso Borde: Restar matriz cero
        compare_matrices(m1 - m_zero, m1, "Resta de Matriz Cero (A - 0 = A)");

        // Pruebas de excepciones (dimensiones incompatibles)
        std::cout << "\nPruebas de Excepciones (invalid_argument):" << std::endl;
        Math::Matrix<int> m_diff_cols(2, 3);

        try {
            auto result = m1 - m_diff_cols;
            std::cout << "  [ERROR] Resta (2x2 - 2x3) no lanzó excepción." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  [ÉXITO] Resta (2x2 - 2x3) capturó: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Prueba el operador de multiplicación.
     */
    void test_operator_multiplication() {
        std::cout << "\n--- Prueba Operador de Multiplicación * --- " << std::endl;

        // Caso Típico (Cuadrada)
        Math::Matrix<int> m1(2, 2);
        m1(0, 0) = 1; m1(0, 1) = 2;
        m1(1, 0) = 3; m1(1, 1) = 4;

        Math::Matrix<int> m2(2, 2);
        m2(0, 0) = 2; m2(0, 1) = 0;
        m2(1, 0) = 1; m2(1, 1) = 2;

        Math::Matrix<int> exp1(2, 2);
        exp1(0, 0) = 4;  exp1(0, 1) = 4; // (1*2+2*1), (1*0+2*2)
        exp1(1, 0) = 10; exp1(1, 1) = 8; // (3*2+4*1), (3*0+4*2)
        compare_matrices(m1 * m2, exp1, "Multiplicación Típica 2x2");

        // Caso Típico (No Cuadrada) 2x3 * 3x2 -> 2x2
        Math::Matrix<int> m3(2, 3);
        m3(0, 0) = 1; m3(0, 1) = 2; m3(0, 2) = 3;
        m3(1, 0) = 4; m3(1, 1) = 5; m3(1, 2) = 6;

        Math::Matrix<int> m4(3, 2);
        m4(0, 0) = 7; m4(0, 1) = 8;
        m4(1, 0) = 9; m4(1, 1) = 10;
        m4(2, 0) = 11; m4(2, 1) = 12;

        Math::Matrix<int> exp2(2, 2);
        exp2(0, 0) = 58;  exp2(0, 1) = 64;  // (1*7+2*9+3*11), (1*8+2*10+3*12)
        exp2(1, 0) = 139; exp2(1, 1) = 154; // (4*7+5*9+6*11), (4*8+5*10+6*12)
        compare_matrices(m3 * m4, exp2, "Multiplicación No Cuadrada (2x3 * 3x2)");

        // Caso Borde: Multiplicación por Identidad
        Math::Matrix<int> m_id(2, 2);
        m_id(0, 0) = 1; m_id(1, 1) = 1;
        compare_matrices(m1 * m_id, m1, "Multiplicación por Identidad (A * I)");

        // Caso Borde: Multiplicación por Cero
        Math::Matrix<int> m_zero(2, 2);
        compare_matrices(m1 * m_zero, m_zero, "Multiplicación por Cero (A * 0)");

        // Caso Borde: 1x1
        Math::Matrix<int> m1x1_a(1, 1); m1x1_a(0, 0) = 5;
        Math::Matrix<int> m1x1_b(1, 1); m1x1_b(0, 0) = 6;
        Math::Matrix<int> m1x1_exp(1, 1); m1x1_exp(0, 0) = 30;
        compare_matrices(m1x1_a * m1x1_b, m1x1_exp, "Multiplicación 1x1");

        // Pruebas de excepciones (dimensiones incompatibles)
        std::cout << "\nPruebas de Excepciones (invalid_argument):" << std::endl;
        try {
            auto result = m1 * m4; // 2x2 * 3x2 (this->cols != other.rows)
            std::cout << "  [ERROR] Mult (2x2 * 3x2) no lanzó excepción." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  [ÉXITO] Mult (2x2 * 3x2) capturó: " << e.what() << std::endl;
        }
    }

    /**
     * @brief Prueba el operador de igualdad.
     */
    void test_operator_equality() {
        std::cout << "\n--- Prueba Operadores de Igualdad == y != --- " << std::endl;

        Math::Matrix<int> m1(2, 2);
        m1(0, 0) = 1; m1(0, 1) = 2;
        m1(1, 0) = 3; m1(1, 1) = 4;

        Math::Matrix<int> m2(2, 2);
        m2(0, 0) = 1; m2(0, 1) = 2;
        m2(1, 0) = 3; m2(1, 1) = 4;

        Math::Matrix<int> m3(2, 2);
        m3(0, 0) = 1; m3(0, 1) = 2;
        m3(1, 0) = 3; m3(1, 1) = 99; // Diferente

        Math::Matrix<int> m_diff_rows(3, 2);
        Math::Matrix<int> m_diff_cols(2, 3);

        // Pruebas operator==
        std::cout << "Pruebas operator==" << std::endl;
        compare_bool(m1 == m2, true, "Igualdad (Verdadero)");
        compare_bool(m1 == m3, false, "Igualdad (Falso por contenido)");
        compare_bool(m1 == m_diff_rows, false, "Igualdad (Falso por filas)");
        compare_bool(m1 == m_diff_cols, false, "Igualdad (Falso por columnas)");

        // Pruebas operator!=
        std::cout << "\nPruebas operator!=" << std::endl;
        compare_bool(m1 != m2, false, "Desigualdad (Falso)");
        compare_bool(m1 != m3, true, "Desigualdad (Verdadero por contenido)");
        compare_bool(m1 != m_diff_rows, true, "Desigualdad (Verdadero por filas)");
        compare_bool(m1 != m_diff_cols, true, "Desigualdad (Verdadero por columnas)");
    }

    /**
     * @brief Realiza pruebas de cálculo de determinante.
     */
    void test_determinant() {
        std::cout << "\n--- Prueba Cálculo de Determinante --- " << std::endl;
        std::cout << std::endl;

        // --- Casos Base (Existentes) ---

        Math::Matrix<double> m1x1(1, 1);
        m1x1(0, 0) = 5;
        std::cout << "Caso base: Matriz de 1x1" << std::endl;
        std::cout << "Matriz:" << std::endl;
        std::cout << m1x1 << std::endl;
        std::cout << "Determinante: " << std::endl;
        compare_cases(m1x1.determinant(), 5);
        std::cout << std::endl;

        Math::Matrix<double> m2x2(2, 2);
        m2x2(0, 0) = 1; m2x2(0, 1) = 3;
        m2x2(1, 0) = 2; m2x2(1, 1) = 4;
        std::cout << "Caso base: Matriz de 2x2" << std::endl;
        std::cout << "Matriz:" << std::endl;
        std::cout << m2x2 << std::endl;
        std::cout << "Determinante: " << std::endl;
        compare_cases(m2x2.determinant(), -2); // (1*4 - 3*2) = 4 - 6 = -2
        std::cout << std::endl;

        Math::Matrix<double> m3x3_singular(3, 3);
        m3x3_singular(0, 0) = 1; m3x3_singular(0, 1) = 2; m3x3_singular(0, 2) = 3;
        m3x3_singular(1, 0) = 4; m3x3_singular(1, 1) = 5; m3x3_singular(1, 2) = 6;
        m3x3_singular(2, 0) = 7; m3x3_singular(2, 1) = 8; m3x3_singular(2, 2) = 9;
        std::cout << "Caso: Matriz de 3x3 (Singular)" << std::endl;
        std::cout << "Matriz:" << std::endl;
        std::cout << m3x3_singular << std::endl;
        std::cout << "Determinante: " << std::endl;
        compare_cases(m3x3_singular.determinant(), 0);
        std::cout << std::endl;

        std::cout << "Caso Borde: Matriz Identidad 4x4" << std::endl;
        Math::Matrix<double> m4x4_id(4, 4);
        for (size_t i = 0; i < 4; ++i) m4x4_id(i, i) = 1;
        std::cout << "Matriz:" << std::endl;
        std::cout << m4x4_id << std::endl;
        std::cout << "Determinante: " << std::endl;
        compare_cases(m4x4_id.determinant(), 1);
        std::cout << std::endl;

        std::cout << "Caso Borde: Matriz Triangular Superior 3x3" << std::endl;
        Math::Matrix<double> m3x3_tri(3, 3);
        m3x3_tri(0, 0) = 1; m3x3_tri(0, 1) = 2; m3x3_tri(0, 2) = 3;
        // m3x3_tri(1, 0) = 0; (por defecto)
        m3x3_tri(1, 1) = 4; m3x3_tri(1, 2) = 5;
        // m3x3_tri(2, 0) = 0; (por defecto)
        // m3x3_tri(2, 1) = 0; (por defecto)
        m3x3_tri(2, 2) = 6;
        std::cout << "Matriz:" << std::endl;
        std::cout << m3x3_tri << std::endl;
        std::cout << "Determinante (1 * 4 * 6): " << std::endl;
        compare_cases(m3x3_tri.determinant(), 24);
        std::cout << std::endl;

        std::cout << "Caso Borde: Pivoteo Forzado (Elemento (0,0) es 0)" << std::endl;
        Math::Matrix<double> m3x3_pivot(3, 3);
        m3x3_pivot(0, 0) = 0; m3x3_pivot(0, 1) = 1; m3x3_pivot(0, 2) = 2;
        m3x3_pivot(1, 0) = 3; m3x3_pivot(1, 1) = 0; m3x3_pivot(1, 2) = 4;
        m3x3_pivot(2, 0) = 5; m3x3_pivot(2, 1) = 6; m3x3_pivot(2, 2) = 0;
        std::cout << "Matriz:" << std::endl;
        std::cout << m3x3_pivot << std::endl;
        std::cout << "Determinante (0 - 1(0-20) + 2(18-0) = 20 + 36): " << std::endl;
        compare_cases(m3x3_pivot.determinant(), 56);
        std::cout << std::endl;

        std::cout << "Caso Borde: Tipo de dato <int>" << std::endl;
        Math::Matrix<int> m2x2_int(2, 2);
        m2x2_int(0, 0) = 1; m2x2_int(0, 1) = 3;
        m2x2_int(1, 0) = 2; m2x2_int(1, 1) = 4;
        std::cout << "Matriz:" << std::endl;
        std::cout << m2x2_int << std::endl;
        std::cout << "Determinante: " << std::endl;
        // compare_cases espera double, la conversión implícita de int funciona
        compare_cases(m2x2_int.determinant(), -2.0);
        std::cout << std::endl;

        // --- Prueba de Manejo de Excepciones ---

        std::cout << "Caso Excepción: Matriz No Cuadrada (2x3)" << std::endl;
        try {
            Math::Matrix<double> m_no_cuadrada(2, 3);
            m_no_cuadrada(0, 0) = 1;
            m_no_cuadrada(1, 1) = 1;

            std::cout << "Matriz:" << std::endl;
            std::cout << m_no_cuadrada << std::endl;

            std::cout << "Intentando calcular determinante..." << std::endl;
            m_no_cuadrada.determinant(); // Esto debe lanzar una excepción

            // Si llegamos aquí, la prueba falló
            std::cout << "ERROR: Se esperaba std::runtime_error, pero no se lanzó." << std::endl;

        } catch (const std::runtime_error& e) {
            // Si llegamos aquí, la prueba fue exitosa
            std::cout << "ÉXITO: Excepción capturada correctamente." << std::endl;
            std::cout << "Mensaje: " << e.what() << std::endl;
        } catch (...) {
            // Captura cualquier otra excepción inesperada
            std::cout << "ERROR: Se capturó una excepción inesperada." << std::endl;
        }
        std::cout << std::endl;

        std::cout << "--- Fin de Pruebas de Determinante --- " << std::endl;
    }

    /**
     * @brief Ejecuta todas las pruebas de matrices
     */
    void test_matrix() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Matrices ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_operator_access();
        test_operator_addition();
        test_operator_subtraction();
        test_operator_multiplication();
        test_operator_equality();
        test_determinant();
    }

}