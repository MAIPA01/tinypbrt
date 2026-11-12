# The version of tinypbrt is set in exactly one place: project(tinypbrt VERSION x.y.z) in the main CMakeLists.txt.
# Everything built by CMake reads it from there (PROJECT_VERSION: library VERSION/SOVERSION, package version files,
# Doxygen PROJECT_NUMBER). This file keeps the version written in plain text files (README, CPM example) equal to it.
#
# Recognized forms (x.y.z = full version, x.y = major.minor):
#   tinypbrt@vx.y.z             CPM shorthand "gh:MAIPA01/tinypbrt@vx.y.z"
#   git tag `vx.y.z`            README text
#   GIT_TAG vx.y.z              CPMAddPackage / FetchContent_Declare (GIT_TAG main or a commit hash is left alone)
#   find_package(tinypbrt x.y   also find_package(tinypbrt_cpp x.y
#
# Usage:
#   include(cmake/version_sync.cmake)
#   tinypbrt_sync_version(SOURCE_DIR <dir> VERSION <x.y.z> [CHECK_ONLY])
# or as a script (used by the tinypbrt-sync-version target and the tinypbrt-version-check test):
#   cmake -DTINYPBRT_SOURCE_DIR=<dir> -DTINYPBRT_VERSION=<x.y.z> [-DTINYPBRT_CHECK_ONLY=ON] -P cmake/version_sync.cmake

include_guard(GLOBAL)

# files (relative to the source directory) that mention the version
set(TINYPBRT_VERSIONED_FILES
        README.md
        examples/cpm_project/CMakeLists.txt
)

function(tinypbrt_sync_version)
    cmake_parse_arguments(PARSE_ARGV 0 ARG "CHECK_ONLY" "SOURCE_DIR;VERSION" "")
    if (NOT ARG_VERSION MATCHES "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$")
        message(FATAL_ERROR "tinypbrt_sync_version: VERSION must be x.y.z, got '${ARG_VERSION}'")
    endif ()
    set(FULL "${ARG_VERSION}")
    set(MAJOR_MINOR "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}")
    set(V "[0-9]+\\.[0-9]+\\.[0-9]+")

    set(OUTDATED "")
    foreach (FILE IN LISTS TINYPBRT_VERSIONED_FILES)
        set(PATH "${ARG_SOURCE_DIR}/${FILE}")
        if (NOT EXISTS "${PATH}")
            continue()
        endif ()
        file(READ "${PATH}" OLD)
        set(NEW "${OLD}")
        string(REGEX REPLACE "(tinypbrt@v)${V}" "\\1${FULL}" NEW "${NEW}")
        string(REGEX REPLACE "(git tag `v)${V}" "\\1${FULL}" NEW "${NEW}")
        string(REGEX REPLACE "(GIT_TAG[ \t]+v)${V}" "\\1${FULL}" NEW "${NEW}")
        string(REGEX REPLACE "(find_package\\(tinypbrt(_cpp)?[ \t]+)[0-9]+\\.[0-9]+(\\.[0-9]+)?" "\\1${MAJOR_MINOR}" NEW "${NEW}")

        if (NOT "${NEW}" STREQUAL "${OLD}")
            list(APPEND OUTDATED "${FILE}")
            if (NOT ARG_CHECK_ONLY)
                # written only when something changed, so the file timestamp stays untouched otherwise
                file(WRITE "${PATH}" "${NEW}")
                message(STATUS "tinypbrt: version ${FULL} written to ${FILE}")
            endif ()
        endif ()
    endforeach ()

    if (ARG_CHECK_ONLY AND OUTDATED)
        list(JOIN OUTDATED ", " OUTDATED)
        message(FATAL_ERROR "tinypbrt: version in ${OUTDATED} differs from ${FULL} "
                "(run: cmake --build <build> --target tinypbrt-sync-version)")
    endif ()
endfunction()

# script mode: cmake -P cmake/version_sync.cmake
if (CMAKE_SCRIPT_MODE_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)
    if (TINYPBRT_CHECK_ONLY)
        tinypbrt_sync_version(SOURCE_DIR "${TINYPBRT_SOURCE_DIR}" VERSION "${TINYPBRT_VERSION}" CHECK_ONLY)
    else ()
        tinypbrt_sync_version(SOURCE_DIR "${TINYPBRT_SOURCE_DIR}" VERSION "${TINYPBRT_VERSION}")
    endif ()
endif ()
