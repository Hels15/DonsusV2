cmake_minimum_required(VERSION 3.22)
project(donsus)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    message("using clang")
    set(DONSUS_USED_COMPILER "clang")
    add_compile_options(-Wunused-variable -Werror=unused-variable)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    message("using gcc")
    set(DONSUS_USED_COMPILER "gcc")
    add_compile_options(-Wunused-variable -Werror=unused-variable)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    message("using msvc")
    set(DONSUS_USED_COMPILER "msvc")
    add_compile_options(/W4 /WX)
endif ()


add_library(donsus_lib STATIC
        # Entry
        src/main.cc
        src/runner.cc

        # Lexer
        src/handle_file.cc
        src/lexer.cc
        src/token.cc

        # Parser
        src/parser.cc

        # AST
        src/ast/node.cc
        src/ast/tree.cc

        # utility
        src/utility/memory_alloc.cc
        src/utility/exception.cc
        src/utility/parser/print_ast.cc
)

add_compile_options(-fsanitize=undefined) # ASAN
add_link_options(-fsanitize=undefined) # ASAN

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_compile_definitions(DEBUG=0)
endif ()
if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_compile_definitions(DEBUG=1)
    add_compile_definitions(DEBUG_SYMBOL_TABLE=1)
    add_definitions(-DDEBUG_MODE)
endif ()

target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include")
target_include_directories(donsus_lib PUBLIC "${PROJECT_SOURCE_DIR}/Include/Internal")

add_executable(donsus src/main.cc)
target_link_libraries(donsus PUBLIC donsus_lib)

if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_subdirectory(donsus_test)
endif ()
