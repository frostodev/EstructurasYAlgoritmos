cmake_minimum_required(VERSION 3.31)
project(EstructurasYAlgoritmos)

set(CMAKE_CXX_STANDARD 20)

add_executable(EstructurasYAlgoritmos
        Algorithms/Graphs.h
        Algorithms/Sorting.h
        Algorithms/Simplex.h

        Automata/FiniteAutomaton.h

        DataStructures/Trees/BinarySearchTree.h
        DataStructures/Trees/AVLTree.h
        DataStructures/DoublyLinkedList.h
        DataStructures/Graph.h
        DataStructures/HashMap.h
        DataStructures/HashSet.h
        DataStructures/LinkedList.h
        DataStructures/PriorityQueue.h
        DataStructures/Queue.h
        DataStructures/Stack.h
        DataStructures/Vector.h

        Math/Constants.h
        Math/MathUtils.h
        Math/MathUtils.cpp
        Math/Matrix.h
        Math/Trigonometry.h
        Math/Trigonometry.cpp
        Math/Vector2.h
        Math/Vector3.h

        Test/TestHashSet.h
        Test/TestHashSet.cpp
        Test/TestGraph.h
        Test/TestGraph.cpp
        Test/TestAVLTree.h
        Test/TestAVLTree.cpp
        Test/TestMatrix.cpp
        Test/TestMatrix.h
        Test/TestFiniteAutomaton.h
        Test/TestFiniteAutomaton.cpp
        Test/TestSimplex.h
        Test/TestSimplex.cpp

        main.cpp
)
