set(C_TARGET_TEST ${PROJECT_NAME}-tests)

# source files
set(C_TARGET_TEST_SOURCES ${C_TARGET_TEST}_TEST_SOURCES)
file(GLOB_RECURSE ${C_TARGET_TEST_SOURCES} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

enable_testing()

# region INSTALL_TESTING
if (TINYPBRT_INSTALL_TEST)
    message(STATUS TESTING_INSTALL)

    find_package(tinypbrt REQUIRED
            HINTS "${CMAKE_SOURCE_DIR}/dist"
    )

    add_library(tinypbrt::all INTERFACE IMPORTED)
    target_link_libraries(tinypbrt::all INTERFACE tinypbrt::tinypbrt)
endif()
# endregion

# make exec
add_executable(${C_TARGET_TEST} ${${C_TARGET_TEST_SOURCES}})
add_test(${C_TARGET_TEST}_TEST ${C_TARGET_TEST})

target_link_libraries(${C_TARGET_TEST} PRIVATE   $<IF:$<BOOL:${TINYPBRT_INSTALL_TEST}>, tinypbrt::all, tinypbrt::tinypbrt>
        gtest_main
)

if(TINYPBRT_INSTALL_TEST)
    get_target_property(TINYPBRT_SOURCE_DIR tinypbrt::all SOURCE_DIR)
else()
    get_target_property(TINYPBRT_SOURCE_DIR tinypbrt::tinypbrt SOURCE_DIR)
endif()

target_include_directories(${C_TARGET_TEST} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} "${TINYPBRT_SOURCE_DIR}/src")

# Konfiguracja PCH
target_precompile_headers(${C_TARGET_TEST} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp")

# Optimizations
target_compile_options(${C_TARGET_TEST} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:
            /O2
            /GL
            /fp:fast
        >
        $<$<CONFIG:Debug>:
            /RTC1
            /Zi
        >
        /Zc:preprocessor
        /Zc:__cplusplus
        /W4
        /WX
    >
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:
            -O3
            -flto
        >
        -Wall
        -Wextra
        -Werror
        -fno-strict-aliasing
    >
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>>:
        --coverage
        -fcoverage-mcdc
        -fprofile-instr-generate
        -fcoverage-mapping
    >
)

target_link_options(${C_TARGET_TEST} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>>
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        -static
    >
    $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${PCRE2CPP_BUILD_COVERAGE}>>:
        --coverage
        -fprofile-instr-generate
    >
)