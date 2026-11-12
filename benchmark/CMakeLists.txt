set(C_TARGET_BENCHMARK ${PROJECT_NAME}-benchmark)

add_executable(${C_TARGET_BENCHMARK}
        ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene_generators.hpp
)
target_compile_features(${C_TARGET_BENCHMARK} PRIVATE cxx_std_17)
target_include_directories(${C_TARGET_BENCHMARK} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${C_TARGET_BENCHMARK} PRIVATE tinypbrt::tinypbrt benchmark::benchmark)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "tinypbrt-benchmark: CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}, use Release for meaningful numbers")
endif()

if (WIN32 AND (TINYPBRT_BUILD_SHARED OR BUILD_SHARED_LIBS))
    add_custom_command(TARGET ${C_TARGET_BENCHMARK} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${C_TARGET_BENCHMARK}> $<TARGET_FILE_DIR:${C_TARGET_BENCHMARK}>
        COMMAND_EXPAND_LISTS
    )
endif()

if (TINYPBRT_BUILD_TESTS)
    add_test(NAME ${C_TARGET_BENCHMARK}_SMOKE
             COMMAND ${C_TARGET_BENCHMARK} --benchmark_min_time=1x --benchmark_filter=/100$|/1000$|/10$)
    set_tests_properties(${C_TARGET_BENCHMARK}_SMOKE PROPERTIES LABELS benchmark)
endif()

# region CPP_BENCHMARK
# C++ wrapper: każdy benchmark bm_cpp_* ma odpowiednik bm_c_* robiący to samo przez API C (narzut wrappera)
if (TINYPBRT_BUILD_CPP_WRAPPER AND TARGET tinypbrt_cpp)
    set(CPP_TARGET_BENCHMARK ${PROJECT_NAME}-cpp-benchmark)

    add_executable(${CPP_TARGET_BENCHMARK}
            ${CMAKE_CURRENT_SOURCE_DIR}/benchmark_cpp.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/scene_generators.hpp
    )
    target_compile_features(${CPP_TARGET_BENCHMARK} PRIVATE cxx_std_20)
    target_include_directories(${CPP_TARGET_BENCHMARK} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${CPP_TARGET_BENCHMARK} PRIVATE tinypbrt::tinypbrt_cpp benchmark::benchmark)

    if (WIN32 AND (TINYPBRT_BUILD_SHARED OR BUILD_SHARED_LIBS))
        add_custom_command(TARGET ${CPP_TARGET_BENCHMARK} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${CPP_TARGET_BENCHMARK}> $<TARGET_FILE_DIR:${CPP_TARGET_BENCHMARK}>
            COMMAND_EXPAND_LISTS
        )
    endif()

    if (TINYPBRT_BUILD_TESTS)
        add_test(NAME ${CPP_TARGET_BENCHMARK}_SMOKE
                 COMMAND ${CPP_TARGET_BENCHMARK} --benchmark_min_time=1x)
        set_tests_properties(${CPP_TARGET_BENCHMARK}_SMOKE PROPERTIES LABELS benchmark)
    endif()
endif()
# endregion

# region CLANG_TIDY
if (TINYPBRT_ENABLE_CLANG_TIDY)
    set(CLANG_TIDY_CXX_STANDARD c++17)

    enable_clang_tidy(
            TARGET ${C_TARGET_BENCHMARK}
            CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_CXX_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )
endif()
# endregion
