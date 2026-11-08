cmake_minimum_required(VERSION 3.20)
project(Lab6)

set(CMAKE_CXX_STANDARD 17)

add_executable(lexer main.cpp Scanner.cpp FiniteAutomaton.cpp CodesTable.cpp SymbolTable.cpp Fip.cpp)

# The lexer reads code_configs/ and automate/ from its working directory.
file(COPY code_configs automate DESTINATION ${CMAKE_BINARY_DIR})
