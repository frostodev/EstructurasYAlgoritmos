
#ifndef SIMPLEX_H
#define SIMPLEX_H

#include "../Math/Matrix.h"

#include <stdexcept>
#include <limits>
#include <iostream>

namespace Algorithms {

    /**
     * @enum ProblemType
     * @brief Define si el problema Símplex es de Maximización o Minimización.
     * Esto cambia la regla para seleccionar la columna pivote.
     */
    enum class ProblemType {
        MAXIMIZE,
        MINIMIZE
    };

    /**
     * @struct SimplexResult
     * @brief Contiene la solución de un problema Símplex.
     */
    struct SimplexResult {
        bool is_bounded;                // true si se encontró una solución óptima, false si no es acotada.
        double optimal_value;           // El valor óptimo de la función objetivo.
        DS::Vector<double> solution;    // Vector con los valores de las variables de decisión (x1, x2, ...)
    };

    /**
     * @brief Encuentra la columna pivote (variable entrante).
     * @param tableau La matriz del tablero Símplex.
     * @param type El tipo de problema (MAXIMIZE o MINIMIZE).
     * @return El índice de la columna pivote, o -1 si se alcanzó la condición de optimalidad.
     * @note Complejidad: O(n), donde n es el número de columnas del tablero.
     */
    inline int find_pivot_column(const Math::Matrix<double>& tableau, ProblemType type) {
        size_t num_cols = tableau.cols();
        size_t obj_row = tableau.rows() - 1;

        int pivot_col = -1;
        double best_val = 0.0;

        if (type == ProblemType::MAXIMIZE) {
            // Maximización: Buscar el coeficiente más negativo.
            // (Usamos -epsilon para ignorar pequeños errores de punto flotante cerca de cero)
            best_val = -std::numeric_limits<double>::epsilon();
            for (size_t j = 0; j < num_cols - 1; ++j) {
                if (tableau(obj_row, j) < best_val) {
                    best_val = tableau(obj_row, j);
                    pivot_col = static_cast<int>(j);
                }
            }
        } else {
            // Minimización: Buscar el coeficiente más positivo.
            // (Usamos +epsilon para ignorar pequeños errores de punto flotante cerca de cero)
            best_val = std::numeric_limits<double>::epsilon();
            for (size_t j = 0; j < num_cols - 1; ++j) {
                if (tableau(obj_row, j) > best_val) {
                    best_val = tableau(obj_row, j);
                    pivot_col = static_cast<int>(j);
                }
            }
        }

        return pivot_col;
    }

    /**
     * @brief Encuentra la fila pivote (variable saliente) usando la prueba del ratio mínimo.
     * @param tableau La matriz del tablero Símplex.
     * @param pivot_col El índice de la columna pivote seleccionada.
     * @return El índice de la fila pivote, o -1 si el problema no es acotado.
     * @note Complejidad: O(m), donde m es el número de filas del tablero.
     */
    inline int find_pivot_row(const Math::Matrix<double>& tableau, int pivot_col) {
        size_t num_rows = tableau.rows();
        size_t rhs_col = tableau.cols() - 1;
        int pivot_row = -1;
        double min_ratio = std::numeric_limits<double>::infinity();

        for (size_t i = 0; i < num_rows - 1; ++i) { // Iterar sobre las filas de restricción
            double a_ij = tableau(i, pivot_col);
            double b_i = tableau(i, rhs_col);

            // Prueba del ratio: solo considerar elementos > 0 en la columna pivote
            if (a_ij > std::numeric_limits<double>::epsilon()) {
                double ratio = b_i / a_ij;

                // (Permitir ratios de 0, p.ej. en casos degenerados)
                if (ratio >= 0 && ratio < min_ratio) {
                    min_ratio = ratio;
                    pivot_row = static_cast<int>(i);
                }
            }
        }
        return pivot_row;
    }

    /**
     * @brief Realiza la operación de pivoteo de Gauss-Jordan en el tablero.
     * @param tableau El tablero Símplex (se modifica in-place).
     * @param pivot_row El índice de la fila pivote.
     * @param pivot_col El índice de la columna pivote.
     * @note Complejidad: O(m * n), donde m es el número de filas y n el de columnas.
     */
    inline void do_pivot(Math::Matrix<double>& tableau, int pivot_row, int pivot_col) {
        size_t num_rows = tableau.rows();
        double pivot_element = tableau(pivot_row, pivot_col);

        // Hacer el elemento pivote igual a 1
        // (Divide toda la fila pivote por el elemento pivote)
        tableau.scale_row(pivot_row, 1.0 / pivot_element);

        // Hacer todos los demás elementos en la columna pivote iguales a 0
        for (size_t i = 0; i < num_rows; ++i) {
            if (i == static_cast<size_t>(pivot_row)) {
                continue; // Omitir la fila pivote
            }

            double factor = tableau(i, pivot_col);
            if (factor != 0.0) {
                // R_i = R_i - factor * R_pivote
                tableau.add_scaled_row(i, pivot_row, -factor);
            }
        }
    }

    /**
     * @brief Extrae la solución final del tablero.
     * @param tableau El tablero Símplex final.
     * @param num_vars El número de variables de decisión (x1, x2, ...).
     * @return Un vector con los valores de la solución.
     * @note Complejidad: O(m * v), donde m es el número de filas y v es num_vars (el número de variables de decisión).
     */
    inline DS::Vector<double> extract_solution(const Math::Matrix<double>& tableau, size_t num_vars) {
        size_t num_rows = tableau.rows();
        size_t rhs_col = tableau.cols() - 1;
        DS::Vector<double> solution(num_vars); // Inicializa a 0

        for (size_t j = 0; j < num_vars; ++j) { // Para cada variable x_j
            int basic_row = -1;
            bool is_basic = true;

            for (size_t i = 0; i < num_rows; ++i) { // Buscar en la columna j
                double val = tableau(i, j);
                if (Math::fabs(val - 1.0) < std::numeric_limits<double>::epsilon()) {
                    if (basic_row != -1) {
                        is_basic = false; // Más de un '1', no es básica
                        break;
                    }
                    basic_row = static_cast<int>(i);
                } else if (Math::fabs(val) > std::numeric_limits<double>::epsilon()) {
                    is_basic = false; // Un valor que no es '0' ni '1', no es básica
                    break;
                }
            }

            if (is_basic && basic_row != -1) {
                // Si la variable es básica (columna es un vector unitario),
                // su valor es el RHS de la fila donde está el '1'
                solution[j] = tableau(basic_row, rhs_col);
            }
            // Si no es básica, su valor permanece 0 (como se inicializó)
        }
        return solution;
    }

    /**
     * @brief Resuelve un problema de Programación Lineal usando el Algoritmo Símplex (Fase II).
     * @param tableau El tablero Símplex (se modifica in-place).
     * @param num_vars El número de variables de decisión (x1, x2, ...).
     * @param type El tipo de problema (MAXIMIZE o MINIMIZE).
     * @return Un SimplexResult con la solución.
     * @note Complejidad: O(k * m * n), donde k es el número de iteraciones (pivotes),
     * m es el número de filas y n el de columnas. El peor caso teórico del
     * algoritmo Símplex es exponencial (k puede ser O(2^n)), pero en la
     * práctica k suele estar acotado (ej. O(m)).
     */
    inline SimplexResult solve_simplex(Math::Matrix<double>& tableau, size_t num_vars, ProblemType type = ProblemType::MAXIMIZE) {
        constexpr int max_iterations = 100; // Límite de seguridad
        int iterations = 0;
        size_t obj_row = tableau.rows() - 1;
        size_t rhs_col = tableau.cols() - 1;

        SimplexResult result;

        while (iterations++ < max_iterations) {
            // Encontrar columna pivote (variable entrante)
            int pivot_col = find_pivot_column(tableau, type);

            // Condición de parada (Optimalidad)
            if (pivot_col == -1) {
                result.is_bounded = true;
                result.solution = extract_solution(tableau, num_vars);

                // El valor óptimo está en (obj_row, rhs_col)
                // Para maximización, P = val. Para minimización, C = val.
                result.optimal_value = tableau(obj_row, rhs_col);
                return result;
            }

            // Encontrar fila pivote (variable saliente)
            int pivot_row = find_pivot_row(tableau, pivot_col);

            // Condición de parada (No Acotado)
            if (pivot_row == -1) {
                result.is_bounded = false;
                result.optimal_value = (type == ProblemType::MAXIMIZE) ?
                                       std::numeric_limits<double>::infinity() :
                                       -std::numeric_limits<double>::infinity();
                result.solution.resize(num_vars); // Solución vacía/cero
                return result;
            }

            // Realizar pivoteo
            do_pivot(tableau, pivot_row, pivot_col);
        }

        // Si se excede el límite de iteraciones (posible ciclo)
        throw std::runtime_error("solve_simplex(): Se excedió el límite de iteraciones.");
    }

}

#endif //SIMPLEX_H

