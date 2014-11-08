cmake_minimum_required (VERSION 2.8.11)
project (language)

set(FILES
    src/lexer.hpp
    src/lexer.cpp
    src/tokenizer.hpp
    src/tokenizer.cpp
    src/main.cpp
)

add_executable(language ${FILES})