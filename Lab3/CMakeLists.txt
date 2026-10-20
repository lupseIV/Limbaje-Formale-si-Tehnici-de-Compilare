cmake_minimum_required(VERSION 3.20)
project(Lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(lexer main.cpp AutomataScanner.cpp FiniteAutomata.cpp CodesTable.cpp SymbolTable.cpp Fip.cpp)

# The lexer reads code_configs/ from its working directory.
file(COPY code_configs DESTINATION ${CMAKE_BINARY_DIR})
