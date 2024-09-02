cmake_minimum_required(VERSION 3.22)

project(donsus_test)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(donsus_test
        # typecheck
        #symbol table
        #codegen_test
        #parser
        parser/test_var.cc
        parser/test_array.cc
        parser/test_assignments.cc
        parser/test_multi_var_def.cc
        parser/test_range_expression.cc
        parser/test_var.cc
        parser/test_functions.cc
        parser/test_param_decl.cc
        parser/test_alias.cc
        parser/test_case.cc
        parser/test_class.cc
        parser/test_expr.cc
        parser/test_generics.cc
        parser/test_if_statement.cc
        parser/test_indices.cc
        parser/test_instance.cc
        parser/test_language_extension.cc
        parser/test_loop.cc
        parser/test_pointer_ref.cc
        parser/test_range_expression.cc
        parser/test_tuple.cc
        parser/test_float.cc
)


include(GoogleTest)
gtest_discover_tests(donsus_test)

target_link_libraries(donsus_test PUBLIC
        GTest::gtest_main
        donsus_lib
)