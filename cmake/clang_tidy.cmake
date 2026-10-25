if(DEFINED _CLANG_TIDY_HELPER_INCLUDED)
    return()
endif()
set(_CLANG_TIDY_HELPER_INCLUDED TRUE)

set(CLANG_TIDY_RUNNER ${CMAKE_CURRENT_LIST_DIR}/run_clang_tidy.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_GENERATOR MATCHES "Ninja|Makefiles")
    message(WARNING "clang-tidy: generator '${CMAKE_GENERATOR}' does not write compile_commands.json - "
                    "clang_tidy_* targets need a Ninja or Makefile generator")
endif()

# enable_clang_tidy(
#     TARGET <target>                    - target, którego źródła mają być sprawdzane (wymagany)
#     [GROUP <name>]                     - opcjonalna grupa: wszystkie targety z tą samą grupą są sprawdzane
#                                          jednym wywołaniem skryptu (clang_tidy_<name> i clang_tidy_<name>_full)
#                                          ze wspólną konfiguracją; bez GROUP powstają clang_tidy_<target>
#                                          i clang_tidy_<target>_full, jak dotychczas
#     [CONFIG <file>]                    - plik .clang-tidy (domyślnie ${PROJECT_SOURCE_DIR}/.clang-tidy)
#     [CXX_STANDARD <std>]               - przekazywane jako --extra-arg=-std=<std>
#     [HEADER_FILTER <regex>]
#     [EXCLUDE_HEADER_FILTER <regex>])   - pomijane dla clang-tidy < 19 (nie zna tej opcji)
#
# W grupie konfigurację ustala pierwsze wywołanie; kolejne mogą pominąć CONFIG/CXX_STANDARD/filtry
# (zostaną odziedziczone), a jeśli je podadzą, muszą być takie same - inaczej błąd konfiguracji.

function(_clang_tidy_version OUT_MAJOR)
    get_property(VERSION_MAJOR GLOBAL PROPERTY _CLANG_TIDY_VERSION_MAJOR)
    if(NOT VERSION_MAJOR)
        execute_process(COMMAND ${CLANG_TIDY_EXE} --version OUTPUT_VARIABLE VERSION_OUTPUT ERROR_QUIET)
        string(REGEX MATCH "version ([0-9]+)" _ "${VERSION_OUTPUT}")
        set(VERSION_MAJOR ${CMAKE_MATCH_1})
        if(NOT VERSION_MAJOR)
            set(VERSION_MAJOR 0)
        endif()
        set_property(GLOBAL PROPERTY _CLANG_TIDY_VERSION_MAJOR ${VERSION_MAJOR})
    endif()
    set(${OUT_MAJOR} ${VERSION_MAJOR} PARENT_SCOPE)
endfunction()

function(_clang_tidy_collect_sources TARGET OUT_SOURCES)
    get_target_property(TARGET_SOURCES ${TARGET} SOURCES)
    get_target_property(TARGET_SOURCE_DIR ${TARGET} SOURCE_DIR)

    set(RESULT)
    if(TARGET_SOURCES)
        foreach(SRC ${TARGET_SOURCES})
            if(SRC MATCHES "^\\$<")
                continue()
            endif()

            get_source_file_property(TIDY_DISABLED ${SRC} CXX_CLANG_TIDY)
            if(DEFINED TIDY_DISABLED AND TIDY_DISABLED STREQUAL "")
                continue()
            endif()

            if(NOT IS_ABSOLUTE ${SRC})
                set(SRC ${TARGET_SOURCE_DIR}/${SRC})
            endif()
            cmake_path(NORMAL_PATH SRC)
            list(APPEND RESULT ${SRC})
        endforeach()
    endif()
    set(${OUT_SOURCES} ${RESULT} PARENT_SCOPE)
endfunction()

function(_clang_tidy_build_args OUT_ARGS CXX_STANDARD HEADER_FILTER EXCLUDE_HEADER_FILTER)
    set(ARGS
        "--quiet"
        "--system-headers=false"
        "--extra-arg=-Wno-macro-redefined"
    )

    if(NOT CXX_STANDARD STREQUAL "")
        list(APPEND ARGS "--extra-arg=-std=${CXX_STANDARD}")
    endif()

    if(NOT HEADER_FILTER STREQUAL "")
        list(APPEND ARGS "--header-filter=${HEADER_FILTER}")
    endif()

    if(NOT EXCLUDE_HEADER_FILTER STREQUAL "")
        _clang_tidy_version(TIDY_MAJOR)
        if(TIDY_MAJOR GREATER_EQUAL 19)
            list(APPEND ARGS "--exclude-header-filter=${EXCLUDE_HEADER_FILTER}")
        else()
            get_property(WARNED GLOBAL PROPERTY _CLANG_TIDY_EXCLUDE_WARNED)
            if(NOT WARNED)
                message(STATUS "clang-tidy ${TIDY_MAJOR} does not support --exclude-header-filter (needs 19+) - option skipped")
                set_property(GLOBAL PROPERTY _CLANG_TIDY_EXCLUDE_WARNED TRUE)
            endif()
        endif()
    endif()

    set(${OUT_ARGS} ${ARGS} PARENT_SCOPE)
endfunction()

function(_clang_tidy_set_target_property TARGET CONFIG ARGS)
    string(JOIN ";" ARGS_STR ${ARGS})
    set_target_properties(${TARGET} PROPERTIES CXX_CLANG_TIDY
                          "${CLANG_TIDY_EXE};--config-file=${CONFIG};-p=${CMAKE_BINARY_DIR};${ARGS_STR}"
    )
endfunction()

function(_clang_tidy_add_run_targets NAME DESCRIPTION CONFIG ARGS SOURCES_FILE CACHE_FILE WORKING_DIR)
    set(TIDY_TARGET clang_tidy_${NAME})

    foreach(FULL OFF ON)
        if(FULL)
            set(TARGET_NAME ${TIDY_TARGET}_full)
            set(COMMENT_TEXT "Running full clang-tidy for ${DESCRIPTION}")
        else()
            set(TARGET_NAME ${TIDY_TARGET})
            set(COMMENT_TEXT "Running clang-tidy for ${DESCRIPTION}")
        endif()

        add_custom_target(
            ${TARGET_NAME}
            COMMAND ${CMAKE_COMMAND}
                -DFORCE_TIDY=${FULL}
                -DCLANG_TIDY_EXE="${CLANG_TIDY_EXE}"
                -DCLANG_TIDY_ARGS="${ARGS}"
                -DCLANG_COMPILE_COMMANDS_DIR="${CMAKE_BINARY_DIR}"
                -DCLANG_TIDY_CONFIG="${CONFIG}"
                -DSOURCES_FILE="${SOURCES_FILE}"
                -DCACHE_FILE="${CACHE_FILE}"
                -P "${CLANG_TIDY_RUNNER}"
            WORKING_DIRECTORY ${WORKING_DIR}
            USES_TERMINAL
            COMMENT "${COMMENT_TEXT}"
        )
    endforeach()

    message(STATUS "clang-tidy target added: ${TIDY_TARGET}")
endfunction()

function(_clang_tidy_add_dependency NAME TARGET)
    add_dependencies(clang_tidy_${NAME} ${TARGET})
    add_dependencies(clang_tidy_${NAME}_full ${TARGET})
endfunction()

macro(_clang_tidy_group_setting GROUP KEY VAR)
    get_property(_GROUP_VALUE_${KEY} GLOBAL PROPERTY _CLANG_TIDY_GROUP_${GROUP}_${KEY})
    if(NOT DEFINED ${VAR})
        set(${VAR} "${_GROUP_VALUE_${KEY}}")
    elseif(NOT "${${VAR}}" STREQUAL "${_GROUP_VALUE_${KEY}}")
        message(FATAL_ERROR
            "enable_clang_tidy: target '${CLANG_TIDY_TARGET}' uses ${KEY} '${${VAR}}', but clang-tidy group '${GROUP}' "
            "is configured with '${_GROUP_VALUE_${KEY}}' (first target: ${_GROUP_FIRST_TARGET}). "
            "All targets in a group share one configuration - use the same value, omit it, or use another GROUP.")
    endif()
endmacro()

function(enable_clang_tidy)
    find_program(CLANG_TIDY_EXE clang-tidy REQUIRED)

    set(options "")
    set(oneValueArgs TARGET GROUP CONFIG HEADER_FILTER EXCLUDE_HEADER_FILTER CXX_STANDARD)
    set(multiValueArgs "")

    cmake_parse_arguments(PARSE_ARGV 0 CLANG_TIDY "${options}" "${oneValueArgs}" "${multiValueArgs}")

    if(NOT DEFINED CLANG_TIDY_TARGET)
        message(FATAL_ERROR "Please provide explicitly TARGET.")
    endif()

    if(NOT TARGET ${CLANG_TIDY_TARGET})
        message(FATAL_ERROR "Target '${CLANG_TIDY_TARGET}' does not exist.")
    endif()

    if(DEFINED CLANG_TIDY_GROUP AND NOT CLANG_TIDY_GROUP MATCHES "^[A-Za-z0-9_.+-]+$")
        message(FATAL_ERROR "enable_clang_tidy: invalid GROUP name '${CLANG_TIDY_GROUP}'")
    endif()

    # pierwszy target grupy ustala konfigurację, kolejne ją dziedziczą
    set(GROUP_EXISTS FALSE)
    if(DEFINED CLANG_TIDY_GROUP)
        get_property(_GROUP_FIRST_TARGET GLOBAL PROPERTY _CLANG_TIDY_GROUP_${CLANG_TIDY_GROUP}_FIRST_TARGET)
        if(_GROUP_FIRST_TARGET)
            set(GROUP_EXISTS TRUE)
            _clang_tidy_group_setting(${CLANG_TIDY_GROUP} CONFIG CLANG_TIDY_CONFIG)
            _clang_tidy_group_setting(${CLANG_TIDY_GROUP} CXX_STANDARD CLANG_TIDY_CXX_STANDARD)
            _clang_tidy_group_setting(${CLANG_TIDY_GROUP} HEADER_FILTER CLANG_TIDY_HEADER_FILTER)
            _clang_tidy_group_setting(${CLANG_TIDY_GROUP} EXCLUDE_HEADER_FILTER CLANG_TIDY_EXCLUDE_HEADER_FILTER)
        endif()
    endif()

    if(NOT DEFINED CLANG_TIDY_CONFIG)
        set(CLANG_TIDY_CONFIG ${PROJECT_SOURCE_DIR}/.clang-tidy)
        message(NOTICE "No .clang-tidy config file provided using default path ${CLANG_TIDY_CONFIG}")
    endif()
    foreach(VAR CXX_STANDARD HEADER_FILTER EXCLUDE_HEADER_FILTER)
        if(NOT DEFINED CLANG_TIDY_${VAR})
            set(CLANG_TIDY_${VAR} "")
        endif()
    endforeach()

    _clang_tidy_collect_sources(${CLANG_TIDY_TARGET} TIDY_SOURCES)
    if(NOT TIDY_SOURCES)
        message(NOTICE "Target ${CLANG_TIDY_TARGET} has no sources - skipping clang-tidy...")
        return()
    endif()

    _clang_tidy_build_args(CLANG_TIDY_ARGS "${CLANG_TIDY_CXX_STANDARD}" "${CLANG_TIDY_HEADER_FILTER}"
                           "${CLANG_TIDY_EXCLUDE_HEADER_FILTER}")
    _clang_tidy_set_target_property(${CLANG_TIDY_TARGET} "${CLANG_TIDY_CONFIG}" "${CLANG_TIDY_ARGS}")

    # --- bez grupy: osobny target, tak jak dotychczas
    if(NOT DEFINED CLANG_TIDY_GROUP)
        get_target_property(BINARY_DIR ${CLANG_TIDY_TARGET} BINARY_DIR)
        set(CACHE_DIR ${BINARY_DIR}/clang-tidy-cache)
        set(SOURCES_FILE ${CACHE_DIR}/${CLANG_TIDY_TARGET}.sources)
        file(MAKE_DIRECTORY ${CACHE_DIR})
        string(JOIN "\n" SOURCES_LINES ${TIDY_SOURCES})
        file(WRITE ${SOURCES_FILE} "${SOURCES_LINES}\n")

        _clang_tidy_add_run_targets(${CLANG_TIDY_TARGET} "target ${CLANG_TIDY_TARGET}" "${CLANG_TIDY_CONFIG}"
            "${CLANG_TIDY_ARGS}" "${SOURCES_FILE}" "${CACHE_DIR}/${CLANG_TIDY_TARGET}.tidy" ${CMAKE_CURRENT_SOURCE_DIR})
        _clang_tidy_add_dependency(${CLANG_TIDY_TARGET} ${CLANG_TIDY_TARGET})
        return()
    endif()

    # --- grupa: jeden plik ze źródłami wszystkich targetów i jeden przebieg skryptu
    set(GROUP ${CLANG_TIDY_GROUP})
    set(CACHE_DIR ${CMAKE_BINARY_DIR}/clang-tidy-cache)
    set(SOURCES_FILE ${CACHE_DIR}/group_${GROUP}.sources)
    string(JOIN "\n" SOURCES_LINES ${TIDY_SOURCES})

    if(NOT GROUP_EXISTS)
        foreach(KEY CONFIG CXX_STANDARD HEADER_FILTER EXCLUDE_HEADER_FILTER)
            set_property(GLOBAL PROPERTY _CLANG_TIDY_GROUP_${GROUP}_${KEY} "${CLANG_TIDY_${KEY}}")
        endforeach()
        set_property(GLOBAL PROPERTY _CLANG_TIDY_GROUP_${GROUP}_FIRST_TARGET ${CLANG_TIDY_TARGET})

        file(MAKE_DIRECTORY ${CACHE_DIR})
        file(WRITE ${SOURCES_FILE} "${SOURCES_LINES}\n") # nadpisywany przy każdej konfiguracji

        _clang_tidy_add_run_targets(${GROUP} "group ${GROUP}" "${CLANG_TIDY_CONFIG}" "${CLANG_TIDY_ARGS}"
            "${SOURCES_FILE}" "${CACHE_DIR}/group_${GROUP}.tidy" ${CMAKE_BINARY_DIR})
    else()
        file(APPEND ${SOURCES_FILE} "${SOURCES_LINES}\n")
    endif()

    _clang_tidy_add_dependency(${GROUP} ${CLANG_TIDY_TARGET})
    set_property(GLOBAL APPEND PROPERTY _CLANG_TIDY_GROUP_${GROUP}_TARGETS ${CLANG_TIDY_TARGET})
    message(STATUS "clang-tidy: ${CLANG_TIDY_TARGET} added to group ${GROUP} (clang_tidy_${GROUP})")
endfunction()

# lista targetów w grupie (np. do własnych zależności): clang_tidy_get_group_targets(<group> <out_var>)
function(clang_tidy_get_group_targets GROUP OUT_VAR)
    get_property(TARGETS GLOBAL PROPERTY _CLANG_TIDY_GROUP_${GROUP}_TARGETS)
    set(${OUT_VAR} ${TARGETS} PARENT_SCOPE)
endfunction()

function(disable_clang_tidy_for_file FILE)

    set_source_files_properties(
        "${FILE}"
        PROPERTIES
        CXX_CLANG_TIDY ""
    )

    message(STATUS "clang-tidy disabled for: ${FILE}")
endfunction()
