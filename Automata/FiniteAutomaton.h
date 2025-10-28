
#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

#include "../DataStructures/HashMap.h"

#include <set>
#include <stdexcept>
#include <iostream>
#include <sstream>

namespace Automata {

    /**
     * @class FiniteAutomaton
     * @brief Implementación de un Autómata Finito Determinista (AFD).
     *
     * Un AFD se define por (Q, Σ, δ, q₀, F):
     * 1. Q (states_): Un conjunto finito de estados.
     * 2. Σ (alphabet_): Un conjunto finito de símbolos (el alfabeto).
     * 3. δ (transitions_): La función de transición (Q x Σ) -> Q.
     * 4. q₀ (start_state_): El estado inicial.
     * 5. F (final_states_): Un conjunto de estados de aceptación/finales.
     *
     * @tparam StateType El tipo usado para representar un estado (ej. int, std::string).
     * @tparam SymbolType El tipo usado para representar un símbolo del alfabeto (ej. char).
     */
    template <typename StateType = int, typename SymbolType = char>
    class FiniteAutomaton {
    public:

        // -----------------------------------------------------------------
        // Constructores
        // -----------------------------------------------------------------

        /**
         * @brief Constructor por defecto.
         * @note Complejidad: O(1)
         */
        FiniteAutomaton() : start_state_() {}

        // -----------------------------------------------------------------
        // Métodos de construcción
        // -----------------------------------------------------------------

        /**
         * @brief Añade un estado al conjunto Q.
         * @param state El estado a añadir.
         * @note Complejidad: O(log N_states)
         */
        void add_state(const StateType& state) {
            states_.insert(state);
        }

        /**
         * @brief Añade un símbolo al alfabeto Σ.
         * @param symbol El símbolo a añadir.
         * @note Complejidad: O(log N_symbols)
         */
        void add_symbol(const SymbolType& symbol) {
            alphabet_.insert(symbol);
        }

        /**
         * @brief Define el estado inicial q₀.
         * @param state El estado inicial.
         * @throws std::invalid_argument si el estado no existe en Q.
         * @note Complejidad: O(log N_states)
         */
        void set_start_state(const StateType& state) {
            if (states_.find(state) == states_.end()) {
                throw std::invalid_argument("set_start_state(): El estado inicial debe existir en el conjunto de estados.");
            }
            start_state_ = state;
        }

        /**
         * @brief Marca un estado como final (lo añade a F).
         * @param state El estado a marcar como final.
         * @throws std::invalid_argument si el estado no existe en Q.
         * @note Complejidad: O(log N_states)
         */
        void add_final_state(const StateType& state) {
            if (states_.find(state) == states_.end()) {
                throw std::invalid_argument("add_final_state(): El estado final debe existir en el conjunto de estados.");
            }
            final_states_.insert(state);
        }

        /**
         * @brief Añade una transición a la función δ.
         * δ(from_state, on_symbol) = to_state
         * @param from_state El estado de origen.
         * @param on_symbol El símbolo que dispara la transición.
         * @param to_state El estado de destino.
         * @throws std::invalid_argument si los estados no están en Q o el símbolo no está en Σ.
         * @note Complejidad: O(1) promedio (gracias a DS::HashMap).
         */
        void add_transition(const StateType& from_state, const SymbolType& on_symbol, const StateType& to_state) {
            if (states_.find(from_state) == states_.end() || states_.find(to_state) == states_.end()) {
                throw std::invalid_argument("add_transition(): Los estados 'from' y 'to' deben existir en el conjunto de estados.");
            }
            if (alphabet_.find(on_symbol) == alphabet_.end()) {
                throw std::invalid_argument("add_transition(): El símbolo debe existir en el alfabeto.");
            }
            
            // transitions_ es un DS::HashMap<StateType, DS::HashMap<SymbolType, StateType>>
            // operator[] en el HashMap inserta si no existe
            transitions_[from_state][on_symbol] = to_state;
        }

        // -----------------------------------------------------------------
        // Métodos de ejecución
        // -----------------------------------------------------------------

        /**
         * @brief Procesa una cadena de entrada y determina si el autómata la acepta.
         * @param input La cadena de símbolos a procesar.
         * @return true si la cadena es aceptada, false en caso contrario.
         * @note Complejidad: O(L) donde L es la longitud de la cadena 'input'
         * (asumiendo O(1) promedio para DS::HashMap::at()/contains()).
         */
        template <typename StringType>
        bool accepts(const StringType& input) const {
            StateType current_state = start_state_;

            for (const SymbolType& symbol : input) {
                // Verificar que el símbolo esté en el alfabeto
                if (alphabet_.find(symbol) == alphabet_.end()) {
                    // Símbolo no reconocido, rechazar
                    return false; 
                }

                // Buscar la transición
                // Usamos .contains() y .at() del HashMap
                if (!transitions_.contains(current_state) || !transitions_.at(current_state).contains(symbol)) {
                    // No hay transición definida (el autómata "crasha")
                    // En un AFD, esto significa rechazo
                    return false; 
                }
                
                // Realizar la transición
                current_state = transitions_.at(current_state).at(symbol);
            }

            // Al final de la cadena, verificar si el estado actual es un estado final
            // final_states_ es un std::set, .count() es O(log N_states) pero N_states es << L
            return final_states_.count(current_state) > 0;
        }

        /**
         * @brief Imprime la definición formal del autómata.
         * @note Complejidad: O(N_states * N_symbols)
         */
        void print_automaton() const {
            std::cout << "--- Autómata Finito Determinista (AFD) ---" << std::endl;
            
            // Imprimir Q (Estados)
            std::cout << "Q = { ";
            for (const auto& state : states_) std::cout << state << " ";
            std::cout << "}" << std::endl;
            
            // Imprimir Σ (Alfabeto)
            std::cout << "Σ = { ";
            for (const auto& symbol : alphabet_) std::cout << "'" << symbol << "' ";
            std::cout << "}" << std::endl;
            
            // Imprimir q₀ (Estado Inicial)
            std::cout << "q₀ = " << start_state_ << std::endl;

            // Imprimir F (Estados Finales)
            std::cout << "F = { ";
            for (const auto& state : final_states_) std::cout << state << " ";
            std::cout << "}" << std::endl;

            // Imprimir δ (Transiciones)
            // Nota: Se itera sobre estados y símbolos conocidos ya que DS::HashMap no expone iteradores
            std::cout << "δ (Transiciones):" << std::endl;
            for (const auto& from_state : states_) {
                if (transitions_.contains(from_state)) {
                    for (const auto& symbol : alphabet_) {
                        if (transitions_.at(from_state).contains(symbol)) {
                            StateType to_state = transitions_.at(from_state).at(symbol);
                            std::cout << "  δ(" << from_state << ", '" << symbol << "') = " << to_state << std::endl;
                        }
                    }
                }
            }
            std::cout << "------------------------------------------" << std::endl;
        }

    private:
        // Q (Estados)
        std::set<StateType> states_;
        
        // Σ (Alfabeto)
        std::set<SymbolType> alphabet_;
        
        // δ (Función de Transición)
        DS::HashMap<StateType, DS::HashMap<SymbolType, StateType>> transitions_;

        // q₀ (Estado Inicial)
        StateType start_state_;

        // F (Estados Finales)
        std::set<StateType> final_states_;
    };

}

#endif // FINITE_AUTOMATON_H
