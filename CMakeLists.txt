cmake_minimum_required(VERSION 3.31)
project(EstructurasYAlgoritmos VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# Configuración de Flags y Perfiles de Optimización
# ==============================================================================
# -------------------------
# GCC / MinGW
# -------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Configurando flags específicos para GCC...")

    # MinGW no soporta LTO
    if (NOT MINGW)
        set(LTO "-flto")
    else()
        set (LTO "")
    endif()

    # Perfil Debug
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -D_DEBUG")

    # Perfil Release
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

    # Perfil Aggressive (con LTO)
    set(CMAKE_CXX_FLAGS_AGGRESSIVE "-O3 -march=native -fno-math-errno -fno-trapping-math ${LTO} -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_AGGRESSIVE "${LTO} -s")

    # Registrar Aggressive como build válido
    set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Elegir tipo de build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Aggressive")

# -------------------------
# MSVC
# -------------------------
elseif(MSVC)
    message(STATUS "Configurando flags específicos para MSVC...")

    # Perfil Debug
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /D_DEBUG")

    # Perfil Release
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")

    # Perfil Aggressive
    set(CMAKE_CXX_FLAGS_AGGRESSIVE "/O2 /GL /DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_AGGRESSIVE "/LTCG")

    # Registrar Aggressive como build válido
    set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Elegir tipo de build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "Aggressive")
endif()

# ==============================================================================
# Definición del Ejecutable y Fuentes
# ==============================================================================
add_executable(EstructurasYAlgoritmos
        Math/MathUtils.cpp
        Math/Trigonometry.cpp

        Test/TestHashSet.cpp
        Test/TestGraph.cpp
        Test/TestAVLTree.cpp
        Test/TestMatrix.cpp
        Test/TestTensor3D.cpp
        Test/TestFiniteAutomaton.cpp
        Test/TestSimplex.cpp

        main.cpp
)
