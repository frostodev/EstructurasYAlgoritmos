
#include "TestSimplex.h"
#include "../Algorithms/Simplex.h"
#include "../Math/Matrix.h"

#include <iostream>
#include <iomanip>

namespace Test {

    /**
     * @brief Prueba un problema de maximización Símplex.
     *
     * Problema:
     * Maximizar P = 3x + 2y
     *
     * Sujeto a:
     * 2x + y <= 18
     * 2x + 3y <= 42
     * 3x + y <= 24
     * x, y >= 0
     *
     * Solución Óptima: x = 3, y = 12, P = 33
     */
    void test_simplex_problem_1() {
        std::cout << "\n--- Prueba 1: Problema Símplex de Maximización ---" << std::endl;

        size_t num_vars = 2; // x, y
        size_t num_constraints = 3;
        size_t num_slack = 3; // s1, s2, s3

        // x, y, s1, s2, s3, RHS
        size_t num_cols = num_vars + num_slack + 1; // 2 + 3 + 1 = 6
        size_t num_rows = num_constraints + 1; // 3 + 1 = 4

        Math::Matrix<double> tableau(num_rows, num_cols);

        // R1: 2x + y + s1 = 18
        tableau(0, 0) = 2; tableau(0, 1) = 1; tableau(0, 2) = 1; tableau(0, 5) = 18;

        // R2: 2x + 3y + s2 = 42
        tableau(1, 0) = 2; tableau(1, 1) = 3; tableau(1, 3) = 1; tableau(1, 5) = 42;

        // R3: 3x + y + s3 = 24
        tableau(2, 0) = 3; tableau(2, 1) = 1; tableau(2, 4) = 1; tableau(2, 5) = 24;

        // Obj: P = 3x + 2y  =>  -3x - 2y + P = 0
        tableau(3, 0) = -3; tableau(3, 1) = -2; tableau(3, 5) = 0;

        std::cout << "Tablero Inicial (Problema 1):" << std::endl;
        std::cout << tableau << std::endl;

        // Resolver
        Algorithms::SimplexResult result = Algorithms::solve_simplex(tableau, num_vars, Algorithms::ProblemType::MAXIMIZE);

        std::cout << "Tablero Final (Problema 1):" << std::endl;
        std::cout << tableau << std::endl;

        // Imprimir Resultados
        std::cout << "--- Resultados (Problema 1) ---" << std::endl;
        if (!result.is_bounded) {
            std::cout << "Solución no acotada." << std::endl;
        } else {
            std::cout << "Solución Óptima Encontrada:" << std::endl;
            std::cout << "  Valor Óptimo (P) = " << std::fixed << std::setprecision(2) << result.optimal_value << std::endl;
            std::cout << "  Esperado: 33.00" << std::endl;

            for (size_t i = 0; i < result.solution.size(); ++i) {
                std::cout << "  Variable x" << i + 1 << " = " << result.solution[i] << std::endl;
            }
            std::cout << "  Esperado: x1 = 3.00, x2 = 12.00" << std::endl;
        }
    }

    /**
     * @brief Prueba un segundo problema de maximización Símplex.
     *
     * Problema:
     * Maximizar Z = 5x1 + 4x2
     *
     * Sujeto a:
     * 6x1 + 4x2 <= 24
     * x1 + 2x2 <= 6
     * -x1 + x2 <= 1
     * x2 <= 2
     * x1, x2 >= 0
     *
     * Solución Óptima: x1 = 3, x2 = 1.5, Z = 21
     */
    void test_simplex_problem_2() {
        std::cout << "\n--- Prueba 2: Problema Símplex de Maximización ---" << std::endl;

        size_t num_vars = 2; // x1, x2
        size_t num_constraints = 4;
        size_t num_slack = 4; // s1, s2, s3, s4

        // x1, x2, s1, s2, s3, s4, RHS
        size_t num_cols = num_vars + num_slack + 1; // 2 + 4 + 1 = 7
        size_t num_rows = num_constraints + 1; // 4 + 1 = 5

        Math::Matrix<double> tableau(num_rows, num_cols);

        // R1: 6x1 + 4x2 + s1 = 24
        tableau(0, 0) = 6; tableau(0, 1) = 4; tableau(0, 2) = 1; tableau(0, 6) = 24;

        // R2: x1 + 2x2 + s2 = 6
        tableau(1, 0) = 1; tableau(1, 1) = 2; tableau(1, 3) = 1; tableau(1, 6) = 6;

        // R3: -x1 + x2 + s3 = 1
        tableau(2, 0) = -1; tableau(2, 1) = 1; tableau(2, 4) = 1; tableau(2, 6) = 1;

        // R4: x2 + s4 = 2
        tableau(3, 0) = 0; tableau(3, 1) = 1; tableau(3, 5) = 1; tableau(3, 6) = 2;

        // Obj: Z = 5x1 + 4x2  =>  -5x1 - 4x2 + Z = 0
        tableau(4, 0) = -5; tableau(4, 1) = -4; tableau(4, 6) = 0;

        std::cout << "Tablero Inicial (Problema 2):" << std::endl;
        std::cout << tableau << std::endl;

        // Resolver
        Algorithms::SimplexResult result = Algorithms::solve_simplex(tableau, num_vars, Algorithms::ProblemType::MAXIMIZE);

        std::cout << "Tablero Final (Problema 2):" << std::endl;
        std::cout << tableau << std::endl;

        // Imprimir Resultados
        std::cout << "--- Resultados (Problema 2) ---" << std::endl;
        if (!result.is_bounded) {
            std::cout << "Solución no acotada." << std::endl;
        } else {
            std::cout << "Solución Óptima Encontrada:" << std::endl;
            std::cout << "  Valor Óptimo (Z) = " << std::fixed << std::setprecision(2) << result.optimal_value << std::endl;
            std::cout << "  Esperado: 21.00" << std::endl;

            for (size_t i = 0; i < result.solution.size(); ++i) {
                std::cout << "  Variable x" << i + 1 << " = " << result.solution[i] << std::endl;
            }
            std::cout << "  Esperado: x1 = 3.00, x2 = 1.50" << std::endl;
        }
    }

    /**
     * @brief Prueba un tercer problema de maximización Símplex (4 variables).
     *
     * Problema:
     * Maximizar P = 40x1 + 30x2 + 20x3 + 10x4
     *
     * Sujeto a:
     * x1 + x2 + x3 + x4 <= 50
     * 2x1 + x2 + 3x3 + x4 <= 100
     * 2x1 + 3x2 + x3 + x4 <= 80
     * x1, x2, x3, x4 >= 0
     *
     * Solución Óptima: x1 = 40, x2 = 0, x3 = 0, x4 = 0, P = 1600
     */
    void test_simplex_problem_3() {
        std::cout << "\n--- Prueba 3: Problema Símplex de Maximización (4 variables) ---" << std::endl;

        size_t num_vars = 4; // x1, x2, x3, x4
        size_t num_constraints = 3;
        size_t num_slack = 3; // s1, s2, s3

        // x1, x2, x3, x4, s1, s2, s3, RHS
        size_t num_cols = num_vars + num_slack + 1; // 4 + 3 + 1 = 8
        size_t num_rows = num_constraints + 1; // 3 + 1 = 4

        Math::Matrix<double> tableau(num_rows, num_cols);

        // R1: x1 + x2 + x3 + x4 + s1 = 50
        tableau(0, 0) = 1; tableau(0, 1) = 1; tableau(0, 2) = 1; tableau(0, 3) = 1; tableau(0, 4) = 1; tableau(0, 7) = 50;

        // R2: 2x1 + x2 + 3x3 + x4 + s2 = 100
        tableau(1, 0) = 2; tableau(1, 1) = 1; tableau(1, 2) = 3; tableau(1, 3) = 1; tableau(1, 5) = 1; tableau(1, 7) = 100;

        // R3: 2x1 + 3x2 + x3 + x4 + s3 = 80
        tableau(2, 0) = 2; tableau(2, 1) = 3; tableau(2, 2) = 1; tableau(2, 3) = 1; tableau(2, 6) = 1; tableau(2, 7) = 80;

        // Obj: P = 40x1 + 30x2 + 20x3 + 10x4  =>  -40x1 - 30x2 - 20x3 - 10x4 + P = 0
        tableau(3, 0) = -40; tableau(3, 1) = -30; tableau(3, 2) = -20; tableau(3, 3) = -10; tableau(3, 7) = 0;

        std::cout << "Tablero Inicial (Problema 3):" << std::endl;
        std::cout << tableau << std::endl;

        // Resolver
        Algorithms::SimplexResult result = Algorithms::solve_simplex(tableau, num_vars, Algorithms::ProblemType::MAXIMIZE);

        std::cout << "Tablero Final (Problema 3):" << std::endl;
        std::cout << tableau << std::endl;

        // Imprimir Resultados
        std::cout << "--- Resultados (Problema 3) ---" << std::endl;
        if (!result.is_bounded) {
            std::cout << "Solución no acotada." << std::endl;
        } else {
            std::cout << "Solución Óptima Encontrada:" << std::endl;
            std::cout << "  Valor Óptimo (P) = " << std::fixed << std::setprecision(2) << result.optimal_value << std::endl;
            std::cout << "  Esperado: 1600.00" << std::endl;

            for (size_t i = 0; i < result.solution.size(); ++i) {
                std::cout << "  Variable x" << i + 1 << " = " << result.solution[i] << std::endl;
            }
            std::cout << "  Esperado: x1 = 40.00, x2 = 0.00, x3 = 0.00, x4 = 0.00" << std::endl;
        }
    }

    /**
     * @brief Prueba un problema de minimización Símplex directamente.
     *
     * Problema:
     * Minimizar C = 2x1 - 3x2 + 1x3
     *
     * Sujeto a:
     * x1 + x2 + 2x3 <= 6
     * 2x1 - x2 - x3 <= 4
     * x1, x2, x3 >= 0
     *
     * Solución Óptima: x1=0, x2=6, x3=0, C = -18
     */
    void test_simplex_problem_4() {
        std::cout << "\n--- Prueba 4: Problema Símplex de Minimización (Directo) ---" << std::endl;

        size_t num_vars = 3; // x1, x2, x3
        size_t num_constraints = 2;
        size_t num_slack = 2; // s1, s2

        // x1, x2, x3, s1, s2, RHS
        size_t num_cols = num_vars + num_slack + 1; // 3 + 2 + 1 = 6
        size_t num_rows = num_constraints + 1; // 2 + 1 = 3

        Math::Matrix<double> tableau(num_rows, num_cols);

        // R1: x1 + x2 + 2x3 + s1 = 6
        tableau(0, 0) = 1; tableau(0, 1) = 1; tableau(0, 2) = 2; tableau(0, 3) = 1; tableau(0, 5) = 6;

        // R2: 2x1 - x2 - x3 + s2 = 4
        tableau(1, 0) = 2; tableau(1, 1) = -1; tableau(1, 2) = -1; tableau(1, 4) = 1; tableau(1, 5) = 4;

        // Obj: C = 2x1 - 3x2 + x3  =>  C - 2x1 + 3x2 - x3 = 0
        // (Nota: para minimizar, buscamos el valor MÁS POSITIVO en la fila Obj)
        tableau(2, 0) = -2; tableau(2, 1) = 3; tableau(2, 2) = -1; tableau(2, 5) = 0;

        std::cout << "Tablero Inicial (Problema 4 - Minimización):" << std::endl;
        std::cout << tableau << std::endl;

        // Resolver
        Algorithms::SimplexResult result = Algorithms::solve_simplex(tableau, num_vars, Algorithms::ProblemType::MINIMIZE);

        std::cout << "Tablero Final (Problema 4 - Minimización):" << std::endl;
        std::cout << tableau << std::endl;

        // Imprimir Resultados
        std::cout << "--- Resultados (Problema 4 - Minimización) ---" << std::endl;
        if (!result.is_bounded) {
            std::cout << "Solución no acotada." << std::endl;
        } else {
            std::cout << "Solución Óptima Encontrada:" << std::endl;
            std::cout << "  Valor Óptimo (C_min) = " << std::fixed << std::setprecision(2) << result.optimal_value << std::endl;
            std::cout << "  Esperado: -18.00" << std::endl;

            for (size_t i = 0; i < result.solution.size(); ++i) {
                std::cout << "  Variable x" << i + 1 << " = " << result.solution[i] << std::endl;
            }
            std::cout << "  Esperado: x1 = 0.00, x2 = 6.00, x3 = 0.00" << std::endl;
        }
    }

    /**
     * @brief Realiza todos los tests del algoritmo símplex
     */
    void test_simplex() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Algoritmo Símplex ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_simplex_problem_1();
        test_simplex_problem_2();
        test_simplex_problem_3();
        test_simplex_problem_4();
    }

}