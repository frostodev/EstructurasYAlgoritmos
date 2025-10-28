cmake_minimum_required(VERSION 3.31)
project(EstructurasYAlgoritmos)

set(CMAKE_CXX_STANDARD 20)

add_executable(EstructurasYAlgoritmos
        Algorithms/Graphs.h
        Algorithms/Sorting.h

        Automata/FiniteAutomaton.h

        DataStructures/Trees/BinarySearchTree.h
        DataStructures/DoublyLinkedList.h
        DataStructures/Graph.h
        DataStructures/HashMap.h
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

        Test/TestMatrix.cpp
        Test/TestMatrix.h
        main.cpp
        Test/TestFiniteAutomaton.h
        Test/TestFiniteAutomaton.cpp
)
