
#include "TestFiniteAutomaton.h"
#include "../Automata/FiniteAutomaton.h"

namespace Test {

    /**
     * @brief Realiza tests de un AFD básico
     */
    void test_dfa_ends_with_one() {
        std::cout << "\n--- Prueba 1: Autómata simple ---" << std::endl;
        // Usamos <int, char> (Estados como enteros, Alfabeto como caracteres)
        Automata::FiniteAutomaton<int, char> dfa;

        // Definir Q (Estados)
        dfa.add_state(0);
        dfa.add_state(1);

        // Definir Σ (Alfabeto)
        dfa.add_symbol('0');
        dfa.add_symbol('1');

        // Definir q₀ (Estado Inicial)
        dfa.set_start_state(0);

        // Definir F (Estados Finales)
        dfa.add_final_state(1);

        // Definir δ (Transiciones)
        dfa.add_transition(0, '0', 0);
        dfa.add_transition(0, '1', 1);
        dfa.add_transition(1, '0', 0);
        dfa.add_transition(1, '1', 1);

        // Imprimir la definición del autómata
        dfa.print_automaton();

        // --- Probar cadenas ---
        std::cout << "Probando cadenas:" << std::endl;

        std::string s1 = "10101";
        std::string s2 = "10100";
        std::string s3 = "1";
        std::string s4 = "0";
        std::string s5 = "1111";
        std::string s6 = "0000";
        std::string s7 = ""; // Cadena vacía

        std::cout << "  '" << s1 << "': " << (dfa.accepts(s1) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s2 << "': " << (dfa.accepts(s2) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s3 << "': " << (dfa.accepts(s3) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s4 << "': " << (dfa.accepts(s4) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s5 << "': " << (dfa.accepts(s5) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s6 << "': " << (dfa.accepts(s6) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s7 << "': " << (dfa.accepts(s7) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
    }

    /**
     * @brief Realiza tests en un AFD un poco más complejo
     */
    void test_dfa_number_recognizer() {
        std::cout << "\n--- Prueba 2: Reconocedor de Números Flotantes ---" << std::endl;

        // Usamos <std::string, char> (Estados como strings, Alfabeto como caracteres)
        Automata::FiniteAutomaton<std::string, char> dfa;

        // Definir Q (Estados)
        std::string s_start = "start";
        std::string s_int = "integer";
        std::string s_dot_no_int = "dot_no_int";
        std::string s_frac = "fraction";
        std::string s_end_dot = "end_dot";
        std::string s_trap = "trap";

        dfa.add_state(s_start);
        dfa.add_state(s_int);
        dfa.add_state(s_dot_no_int);
        dfa.add_state(s_frac);
        dfa.add_state(s_end_dot);
        dfa.add_state(s_trap); // Estado trampa (no final)

        // Definir Σ (Alfabeto)
        dfa.add_symbol('.');
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_symbol(c);
        }

        // Definir q₀ (Estado Inicial)
        dfa.set_start_state(s_start);

        // Definir F (Estados Finales)
        dfa.add_final_state(s_int);       // Acepta "123"
        dfa.add_final_state(s_frac);      // Acepta "123.45" o ".45"
        dfa.add_final_state(s_end_dot);   // Acepta "123."

        // Definir δ (Transiciones)
        // Transiciones desde 'start'
        dfa.add_transition(s_start, '.', s_dot_no_int);
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_start, c, s_int);
        }

        // Transiciones desde 'integer' (hemos visto dígitos)
        dfa.add_transition(s_int, '.', s_end_dot);
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_int, c, s_int);
        }

        // Transiciones desde 'dot_no_int' (hemos visto un '.', ej: "._")
        dfa.add_transition(s_dot_no_int, '.', s_trap); // Inválido: ".."
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_dot_no_int, c, s_frac); // Válido: ".4"
        }

        // Transiciones desde 'fraction' (hemos visto dígitos después de un punto)
        dfa.add_transition(s_frac, '.', s_trap); // Inválido: "1.2.3"
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_frac, c, s_frac); // Válido: "1.234"
        }

        // Transiciones desde 'end_dot' (hemos visto "123.")
        dfa.add_transition(s_end_dot, '.', s_trap); // Inválido: "123.."
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_end_dot, c, s_frac); // Válido: "123.4"
        }

        // Transiciones desde 'trap' (estado de error, no se puede salir)
        dfa.add_transition(s_trap, '.', s_trap);
        for (char c = '0'; c <= '9'; ++c) {
            dfa.add_transition(s_trap, c, s_trap);
        }


        // Imprimir la definición del autómata
        dfa.print_automaton();

        // --- Probar cadenas ---
        std::cout << "Probando cadenas:" << std::endl;
        std::string s1 = "123";
        std::string s2 = "123.45";
        std::string s3 = ".45";
        std::string s4 = "123.";
        std::string s5 = ".";       // Inválido
        std::string s6 = "1.2.3";   // Inválido
        std::string s7 = "12a34";  // Inválido (símbolo 'a' no en alfabeto)
        std::string s8 = "..";      // Inválido

        std::cout << "  '" << s1 << "': " << (dfa.accepts(s1) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s2 << "': " << (dfa.accepts(s2) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s3 << "': " << (dfa.accepts(s3) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s4 << "': " << (dfa.accepts(s4) ? "Aceptada" : "Rechazada") << " (Esperado: Aceptada)" << std::endl;
        std::cout << "  '" << s5 << "': " << (dfa.accepts(s5) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s6 << "': " << (dfa.accepts(s6) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s7 << "': " << (dfa.accepts(s7) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
        std::cout << "  '" << s8 << "': " << (dfa.accepts(s8) ? "Aceptada" : "Rechazada") << " (Esperado: Rechazada)" << std::endl;
    }

    /**
     * @brief Ejecuta todas las pruebas de AFD.
     */
    void test_finite_automaton() {
        std::cout << "\n=============================================" << std::endl;
        std::cout << "--- Pruebas de Autómata Finito Determinista ---" << std::endl;
        std::cout << "=============================================" << std::endl;
        test_dfa_ends_with_one();
        test_dfa_number_recognizer();
    }

}