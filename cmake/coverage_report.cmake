# Pokrycie kodu (LLVM source-based coverage: clang + llvm-profdata + llvm-cov).
#
#   cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DTINYPBRT_BUILD_COVERAGE=ON
#   cmake --build build --target coverage          # testy -> merge -> raport HTML + podsumowanie w konsoli
#
# tinypbrt_enable_coverage(<target>)   - instrumentuje cel (wołane dla biblioteki, testów i przykładów)
# setup_coverage_report(
#     TEST_TARGET <exe>                - główny plik testów (wymagany)
#     [EXE_TARGETS <exe>...]           - inne programy, których pokrycie ma być w raporcie (np. test C API)
#     [FORMAT html|text]               - format raportu (domyślnie html)
#     [OUTPUT_DIR <dir>]               - katalog raportu (domyślnie <build>/coverage_report)
#     [IGNORE_REGEX <regex>...]        - pliki pomijane w raporcie (domyślnie testy, przykłady, benchmark, third_party)
#     [CLEAN_TEMP])                    - usuń *.profraw i *.profdata po wygenerowaniu raportu
#
# Uruchamiane są wszystkie testy z ctest (także test C API i przykłady), każdy proces zapisuje własny plik
# .profraw (LLVM_PROFILE_FILE=%p), więc nic się nie nadpisuje. Działa tylko z Clangiem z frontendem GNU
# (clang/clang++, także clang.exe na Windows); dla innych kompilatorów cel `coverage` nie jest tworzony.

if(DEFINED _COVERAGE_REPORT_HELPER_INCLUDED)
    return()
endif()
set(_COVERAGE_REPORT_HELPER_INCLUDED TRUE)

set(_TINYPBRT_COVERAGE_SCRIPTS_DIR ${CMAKE_CURRENT_LIST_DIR})

function(_tinypbrt_coverage_supported OUT_VAR)
    if (TINYPBRT_BUILD_COVERAGE AND CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        set(${OUT_VAR} TRUE PARENT_SCOPE)
    else()
        set(${OUT_VAR} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(tinypbrt_enable_coverage TARGET)
    _tinypbrt_coverage_supported(SUPPORTED)
    if (NOT SUPPORTED)
        return()
    endif()

    # -fcoverage-mcdc (pokrycie warunków MC/DC) jest dostępne od Clang 18
    set(COVERAGE_FLAGS -fprofile-instr-generate -fcoverage-mapping)
    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 18)
        list(APPEND COVERAGE_FLAGS -fcoverage-mcdc)
    endif()

    target_compile_options(${TARGET} PRIVATE ${COVERAGE_FLAGS})
    # biblioteka statyczna z instrumentacją wymaga runtime'u profilowania w programie, który ją linkuje
    get_target_property(TARGET_TYPE ${TARGET} TYPE)
    if (TARGET_TYPE STREQUAL "STATIC_LIBRARY")
        target_link_options(${TARGET} INTERFACE $<BUILD_INTERFACE:-fprofile-instr-generate>)
    else()
        target_link_options(${TARGET} PRIVATE -fprofile-instr-generate)
    endif()
endfunction()

function(setup_coverage_report)
    _tinypbrt_coverage_supported(SUPPORTED)
    if (NOT SUPPORTED)
        if (TINYPBRT_BUILD_COVERAGE)
            message(STATUS "tinypbrt coverage: needs Clang with the GNU frontend, 'coverage' target not created")
        endif()
        return()
    endif()

    set(options CLEAN_TEMP)
    set(oneValueArgs TEST_TARGET FORMAT OUTPUT_DIR)
    set(multiValueArgs EXE_TARGETS IGNORE_REGEX)
    cmake_parse_arguments(PARSE_ARGV 0 COVERAGE "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if (NOT DEFINED COVERAGE_TEST_TARGET)
        message(FATAL_ERROR "setup_coverage_report: TEST_TARGET is required")
    endif()
    foreach(EXE_TARGET ${COVERAGE_TEST_TARGET} ${COVERAGE_EXE_TARGETS})
        if (NOT TARGET ${EXE_TARGET})
            message(FATAL_ERROR "setup_coverage_report: '${EXE_TARGET}' is not a target")
        endif()
        get_target_property(TARGET_TYPE ${EXE_TARGET} TYPE)
        if (NOT TARGET_TYPE STREQUAL "EXECUTABLE")
            message(FATAL_ERROR "setup_coverage_report: '${EXE_TARGET}' must be an executable")
        endif()
    endforeach()

    if (NOT DEFINED COVERAGE_FORMAT)
        set(COVERAGE_FORMAT html)
    endif()
    if (NOT COVERAGE_FORMAT MATCHES "^(html|text)$")
        message(FATAL_ERROR "setup_coverage_report: FORMAT must be html or text")
    endif()
    if (NOT DEFINED COVERAGE_OUTPUT_DIR)
        set(COVERAGE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/coverage_report)
    endif()
    if (NOT DEFINED COVERAGE_IGNORE_REGEX)
        set(COVERAGE_IGNORE_REGEX "[\\\\/](tests|examples|benchmark|third_party|_deps)[\\\\/]")
    endif()

    # narzędzia LLVM w wersji kompilatora (format .profraw zależy od wersji)
    string(REGEX MATCH "^[0-9]+" CLANG_MAJOR ${CMAKE_C_COMPILER_VERSION})
    get_filename_component(CLANG_BIN_DIR ${CMAKE_C_COMPILER} DIRECTORY)
    find_program(TINYPBRT_LLVM_PROFDATA NAMES llvm-profdata-${CLANG_MAJOR} llvm-profdata HINTS ${CLANG_BIN_DIR} REQUIRED)
    find_program(TINYPBRT_LLVM_COV NAMES llvm-cov-${CLANG_MAJOR} llvm-cov HINTS ${CLANG_BIN_DIR} REQUIRED)

    set(PROFRAW_DIR ${CMAKE_BINARY_DIR}/coverage_profraw)
    set(PROFDATA_FILE ${CMAKE_BINARY_DIR}/coverage.profdata)

    # pierwszy plik binarny jest argumentem pozycyjnym llvm-cov, kolejne przez -object
    set(COV_OBJECTS $<TARGET_FILE:${COVERAGE_TEST_TARGET}>)
    foreach(EXE_TARGET ${COVERAGE_EXE_TARGETS})
        list(APPEND COV_OBJECTS -object $<TARGET_FILE:${EXE_TARGET}>)
    endforeach()
    # biblioteka współdzielona ma własną mapę pokrycia
    if (TARGET tinypbrt)
        get_target_property(LIB_TYPE tinypbrt TYPE)
        if (LIB_TYPE STREQUAL "SHARED_LIBRARY")
            list(APPEND COV_OBJECTS -object $<TARGET_FILE:tinypbrt>)
        endif()
    endif()

    set(IGNORE_ARGS)
    foreach(REGEX ${COVERAGE_IGNORE_REGEX})
        list(APPEND IGNORE_ARGS -ignore-filename-regex=${REGEX})
    endforeach()

    set(SHOW_ARGS -show-line-counts-or-regions -show-branches=count -show-expansions)
    if (CLANG_MAJOR VERSION_GREATER_EQUAL 18)
        list(APPEND SHOW_ARGS -show-mcdc)
    endif()
    if (COVERAGE_FORMAT STREQUAL "html")
        list(APPEND SHOW_ARGS -show-directory-coverage)
    endif()

    set(CLEAN_COMMANDS)
    if (COVERAGE_CLEAN_TEMP)
        set(CLEAN_COMMANDS
            COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROFRAW_DIR}
            COMMAND ${CMAKE_COMMAND} -E rm -f ${PROFDATA_FILE})
    endif()

    set(COVERAGE_TARGET_NAME coverage_report_${COVERAGE_TEST_TARGET})
    add_custom_target(${COVERAGE_TARGET_NAME}
        # 1. czyste dane z poprzedniego uruchomienia
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${PROFRAW_DIR} ${COVERAGE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROFRAW_DIR} ${COVERAGE_OUTPUT_DIR}
        # 2. wszystkie testy; %p = PID, %m = sygnatura binarki -> osobny plik na proces
        COMMAND ${CMAKE_COMMAND} -E env LLVM_PROFILE_FILE=${PROFRAW_DIR}/%p-%m.profraw
                ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        # 3. scalenie .profraw (glob w skrypcie CMake - bez powłoki, działa też na Windows)
        COMMAND ${CMAKE_COMMAND} -DLLVM_PROFDATA=${TINYPBRT_LLVM_PROFDATA} -DPROFRAW_DIR=${PROFRAW_DIR}
                -DOUTPUT=${PROFDATA_FILE} -P ${_TINYPBRT_COVERAGE_SCRIPTS_DIR}/coverage_merge.cmake
        # 4. raport
        COMMAND ${TINYPBRT_LLVM_COV} show ${COV_OBJECTS} -instr-profile=${PROFDATA_FILE} -format=${COVERAGE_FORMAT}
                -output-dir=${COVERAGE_OUTPUT_DIR} ${IGNORE_ARGS} ${SHOW_ARGS}
        # 5. podsumowanie w konsoli
        COMMAND ${TINYPBRT_LLVM_COV} report ${COV_OBJECTS} -instr-profile=${PROFDATA_FILE} ${IGNORE_ARGS}
        ${CLEAN_COMMANDS}
        COMMAND ${CMAKE_COMMAND} -E echo "Coverage report: ${COVERAGE_OUTPUT_DIR}/index.${COVERAGE_FORMAT}"
        DEPENDS ${COVERAGE_TEST_TARGET} ${COVERAGE_EXE_TARGETS}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating coverage report (${COVERAGE_FORMAT}) in ${COVERAGE_OUTPUT_DIR}"
        VERBATIM
    )

    if (NOT TARGET coverage)
        add_custom_target(coverage DEPENDS ${COVERAGE_TARGET_NAME})
    endif()
endfunction()
