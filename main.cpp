
#include "Test/TestHashSet.h"
#include "Test/TestGraph.h"
#include "Test/TestAVLTree.h"

#include "Test/TestMatrix.h"
#include "Test/TestFiniteAutomaton.h"
#include "Test/TestSimplex.h"

int main() {

    Test::test_hash_set();
    Test::test_graph();
    Test::test_avl_tree();

    Test::test_matrix();
    Test::test_finite_automaton();
    Test::test_simplex();

    return 0;
}
