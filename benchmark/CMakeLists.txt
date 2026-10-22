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
