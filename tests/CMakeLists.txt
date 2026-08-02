# This file is part of a project that is licensed under Apache-2.0.
# See `LICENSE` for details.

include(FetchContent)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(which_tests
    construction.cc
    void_type.cc
    accessors.cc
    structured_bindings.cc
    move_semantics.cc
    same_type.cc
)
target_link_libraries(which_tests PRIVATE mhx::which GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(which_tests)

# Negative compile-time checks.
# Each source below must fail to compile because it violates
# one of `which<T, E>`'s static_asserts. Building the target is
# the test; ctest expects the build itself to fail.
file(GLOB WHICH_COMPILE_FAIL_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/compile_fail/*.cc)

foreach(fail_source ${WHICH_COMPILE_FAIL_SOURCES})
    get_filename_component(fail_name ${fail_source} NAME_WE)
    set(fail_target compile_fail_${fail_name})
    add_executable(${fail_target} EXCLUDE_FROM_ALL ${fail_source})
    target_link_libraries(${fail_target} PRIVATE mhx::which)
    add_test(NAME ${fail_target}
             COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
                     --target ${fail_target} --config $<CONFIG>)
    set_tests_properties(${fail_target} PROPERTIES WILL_FAIL TRUE)
endforeach()
