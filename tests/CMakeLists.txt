set(C_TARGET_TEST ${PROJECT_NAME}-tests)

# source files
set(C_TARGET_TEST_SOURCES ${C_TARGET_TEST}_TEST_SOURCES)
file(GLOB_RECURSE ${C_TARGET_TEST_SOURCES} CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# region INSTALL_TESTING
if (TINYPBRT_INSTALL_TEST)
    message(STATUS TESTING_INSTALL)

    find_package(tinypbrt REQUIRED
            HINTS "${PROJECT_SOURCE_DIR}/dist"
    )

    add_library(tinypbrt::all INTERFACE IMPORTED)
    target_link_libraries(tinypbrt::all INTERFACE tinypbrt::tinypbrt)
endif()
# endregion

# make exec
add_executable(${C_TARGET_TEST} ${${C_TARGET_TEST_SOURCES}})
add_test(NAME ${C_TARGET_TEST}_TEST COMMAND ${C_TARGET_TEST})

# gtest 1.17 i testy (std::filesystem) wymagają C++17
target_compile_features(${C_TARGET_TEST} PRIVATE cxx_std_17)

tinypbrt_enable_sanitizers(${C_TARGET_TEST})
tinypbrt_set_sanitizers_test_environment(${C_TARGET_TEST}_TEST)

# region C_API_TEST
set(C_TARGET_C_API_TEST ${PROJECT_NAME}-c-api-tests)
add_executable(${C_TARGET_C_API_TEST} ${CMAKE_CURRENT_SOURCE_DIR}/c_api/c_api_test.c)
add_test(NAME ${C_TARGET_C_API_TEST}_TEST COMMAND ${C_TARGET_C_API_TEST})
set_target_properties(${C_TARGET_C_API_TEST} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_link_libraries(${C_TARGET_C_API_TEST} PRIVATE $<IF:$<BOOL:${TINYPBRT_INSTALL_TEST}>, tinypbrt::all, tinypbrt::tinypbrt>)
target_compile_options(${C_TARGET_C_API_TEST} PRIVATE
    $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:/W4 /WX>
    $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:-Wall -Wextra -Wpedantic -Werror>
)
tinypbrt_enable_sanitizers(${C_TARGET_C_API_TEST})
tinypbrt_set_sanitizers_test_environment(${C_TARGET_C_API_TEST}_TEST)
# endregion

if (TARGET tinypbrt_static)
    set(C_TARGET_TEST_TINYPBRT tinypbrt_static)
elseif (TINYPBRT_INSTALL_TEST)
    set(C_TARGET_TEST_TINYPBRT tinypbrt::all)
else()
    set(C_TARGET_TEST_TINYPBRT tinypbrt::tinypbrt)
endif()
target_link_libraries(${C_TARGET_TEST} PRIVATE ${C_TARGET_TEST_TINYPBRT} gtest_main)

if (WIN32)
    set(C_TARGET_TESTS_WITH_DLLS)
    if (TINYPBRT_BUILD_SHARED)
        list(APPEND C_TARGET_TESTS_WITH_DLLS ${C_TARGET_C_API_TEST})
    endif()
    if (BUILD_SHARED_LIBS)
        list(APPEND C_TARGET_TESTS_WITH_DLLS ${C_TARGET_TEST})
    endif()
    foreach(TEST_EXE ${C_TARGET_TESTS_WITH_DLLS})
        add_custom_command(TARGET ${TEST_EXE} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${TEST_EXE}> $<TARGET_FILE_DIR:${TEST_EXE}>
            COMMAND_EXPAND_LISTS
        )
    endforeach()
endif()

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
            $<$<NOT:$<BOOL:${TINYPBRT_ENABLE_SANITIZERS}>>:/RTC1>
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
)

target_link_options(${C_TARGET_TEST} PRIVATE
    # MSVC/clang with MSVC frontend
    $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
        $<$<CONFIG:Release>:/LTCG>
        $<$<CONFIG:Debug>:/DEBUG>>
    # GCC/clang with GNU frontend
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
        $<$<CONFIG:Release>:-flto>
        $<$<NOT:$<OR:$<BOOL:${TINYPBRT_ENABLE_SANITIZERS}>,$<BOOL:${BUILD_SHARED_LIBS}>,$<PLATFORM_ID:Darwin>>>:-static>
    >
)

tinypbrt_enable_coverage(${C_TARGET_TEST})
tinypbrt_enable_coverage(${C_TARGET_C_API_TEST})