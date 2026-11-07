set(C_TARGET_NAME ${PROJECT_NAME})
set(CPP_TARGET_NAME ${C_TARGET_NAME}_cpp)

# region HEADER_FILES
set(BUILD_PUBLIC_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_PRIVATE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INSTALL_HEADERS_DIR include)

set(C_TARGET_HEADERS ${C_TARGET_NAME}_HEADERS)
file(GLOB_RECURSE ${C_TARGET_HEADERS} CONFIGURE_DEPENDS ${BUILD_PUBLIC_HEADERS_DIR}/*.h
                                                        ${BUILD_PRIVATE_HEADERS_DIR}/*.h)

set(CPP_TARGET_HEADERS ${CPP_TARGET_NAME}_HEADERS)
file(GLOB_RECURSE ${CPP_TARGET_HEADERS} CONFIGURE_DEPENDS   ${BUILD_PUBLIC_HEADERS_DIR}/*.hpp
                                                            ${BUILD_PRIVATE_HEADERS_DIR}/*.hpp)
# endregion

# region SOURCE_FILES
set(BUILD_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INSTALL_SOURCES_DIR src)

set(C_TARGET_SOURCES ${C_TARGET_NAME}_SOURCES)
file(GLOB_RECURSE ${C_TARGET_SOURCES} CONFIGURE_DEPENDS ${BUILD_SOURCES_DIR}/*.c)

set(CPP_TARGET_SOURCES ${CPP_TARGET_NAME}_SOURCES)
file(GLOB_RECURSE ${CPP_TARGET_SOURCES} CONFIGURE_DEPENDS ${BUILD_SOURCES_DIR}/*.cpp)
# endregion

# region LIBRARY_SETUP
if (TINYPBRT_BUILD_SHARED)
    set(LIBRARY_TYPE SHARED)
else()
    set(LIBRARY_TYPE STATIC)
endif()
add_library(${C_TARGET_NAME} ${LIBRARY_TYPE} ${${C_TARGET_HEADERS}} ${${C_TARGET_SOURCES}})

# set library alias
set(NAMESPACE ${C_TARGET_NAME}::)

set(C_TARGET_ALIAS ${NAMESPACE}${C_TARGET_NAME})
add_library(${C_TARGET_ALIAS} ALIAS ${C_TARGET_NAME})

if (TINYPBRT_BUILD_CPP_WRAPPER)
    if (${CPP_TARGET_SOURCES})
        set(CPP_TARGET_HEADER_ONLY FALSE)
        add_library(${CPP_TARGET_NAME} ${LIBRARY_TYPE} ${${CPP_TARGET_HEADERS}} ${${CPP_TARGET_SOURCES}})
        set(CPP_TARGET_SCOPE PUBLIC)
    else()
        set(CPP_TARGET_HEADER_ONLY TRUE)
        add_library(${CPP_TARGET_NAME} INTERFACE ${${CPP_TARGET_HEADERS}})
        set(CPP_TARGET_SCOPE INTERFACE)
    endif()

    set(CPP_TARGET_ALIAS ${NAMESPACE}${CPP_TARGET_NAME})
    add_library(${CPP_TARGET_ALIAS} ALIAS ${CPP_TARGET_NAME})
endif()
# endregion

function(tinypbrt_configure_c_library TARGET)
    target_precompile_headers(${TARGET} PRIVATE "$<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}/${C_TARGET_NAME}/pch.h>")

    target_compile_features(${TARGET} PRIVATE $<BUILD_INTERFACE:c_std_11>)

    # set headers folder
    target_include_directories(${TARGET}    PUBLIC $<BUILD_INTERFACE:${BUILD_PUBLIC_HEADERS_DIR}>
                                            PRIVATE $<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}>
    )

    # region COMPILE_DEFINITIONS
    target_compile_definitions(${TARGET} PRIVATE $<BUILD_INTERFACE:
            $<$<BOOL:${TINYPBRT_DISABLE_ASSERT_ON_RELEASE}>:TINYPBRT_DISABLE_ASSERT_ON_RELEASE>
            >
    )
    # endregion

    # region COMPILE_OPTIONS
    target_compile_options(${TARGET} PRIVATE
            # MSVC/clang with MSVC frontend
            $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:
                $<$<CONFIG:Release>:
                    /O2
                    /fp:fast
                >
                $<$<CONFIG:Debug>:
                    $<$<NOT:$<BOOL:${TINYPBRT_ENABLE_SANITIZERS}>>:/RTC1>
                    /Zi
                >
                /Zc:preprocessor
                /W4
                $<$<AND:$<C_COMPILER_ID:Clang>,$<BOOL:${TINYPBRT_ENABLE_PEDANTIC}>>:-Wpedantic>
                $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:/WX>
            >
            # GCC/clang with GNU frontend
            $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:
                $<$<CONFIG:Release>:
                    -O3
                >
                -Wall
                -Wextra
                -Wvla
                $<$<BOOL:${TINYPBRT_ENABLE_PEDANTIC}>:-Wpedantic>
                $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:-Werror>
                -fno-strict-aliasing
            >
    )
    # endregion

    # region LINK_LIBRARIES
    if (NOT WIN32 AND NOT APPLE)
        target_link_libraries(${TARGET} PUBLIC m)
    endif()
    # endregion

    # region SANITIZERS
    tinypbrt_enable_sanitizers(${TARGET})
    # endregion

    # region COVERAGE (cmake/coverage_report.cmake)
    tinypbrt_enable_coverage(${TARGET})
    # endregion
endfunction()

function(tinypbrt_configure_cpp_compile_options TARGET)
    target_compile_options(${TARGET} PRIVATE
            # MSVC/clang with MSVC frontend
            $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:MSVC>>>:
                $<$<CONFIG:Release>:
                    /O2
                    /fp:fast
                >
                $<$<CONFIG:Debug>:
                    $<$<NOT:$<BOOL:${TINYPBRT_ENABLE_SANITIZERS}>>:/RTC1>
                    /Zi
                >
                /Zc:preprocessor
                /Zc:__cplusplus
                /permissive-
                /W4
                $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${TINYPBRT_ENABLE_PEDANTIC}>>:-Wpedantic>
                $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:/WX>
            >
            # GCC/clang with GNU frontend
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<AND:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_FRONTEND_VARIANT:GNU>>>:
                $<$<CONFIG:Release>:
                    -O3
                >
                -Wall
                -Wextra
                -Wvla
                $<$<BOOL:${TINYPBRT_ENABLE_PEDANTIC}>:-Wpedantic>
                $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:-Werror>
                -fno-strict-aliasing
            >
    )
endfunction()

function(tinypbrt_configure_cpp_library TARGET C_LIBRARY)
    target_link_libraries(${TARGET} ${CPP_TARGET_SCOPE} ${C_LIBRARY})

    target_compile_features(${TARGET} ${CPP_TARGET_SCOPE} cxx_std_20)

    target_include_directories(${TARGET} ${CPP_TARGET_SCOPE} $<BUILD_INTERFACE:${BUILD_PUBLIC_HEADERS_DIR}>)

    if (CPP_TARGET_HEADER_ONLY)
        return()
    endif()

    target_precompile_headers(${TARGET} PRIVATE "$<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}/${C_TARGET_NAME}/pch.hpp>")
    target_include_directories(${TARGET} PRIVATE $<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}>)
    target_compile_definitions(${TARGET} PRIVATE $<BUILD_INTERFACE:
            $<$<BOOL:${TINYPBRT_DISABLE_ASSERT_ON_RELEASE}>:TINYPBRT_DISABLE_ASSERT_ON_RELEASE>
            >
    )
    tinypbrt_configure_cpp_compile_options(${TARGET})
    tinypbrt_enable_sanitizers(${TARGET})
    tinypbrt_enable_coverage(${TARGET})
endfunction()

tinypbrt_configure_c_library(${C_TARGET_NAME})
if (TINYPBRT_BUILD_CPP_WRAPPER)
    tinypbrt_configure_cpp_library(${CPP_TARGET_NAME} ${C_TARGET_ALIAS})
endif()

# region SHARED_LIBRARY
if (TINYPBRT_BUILD_SHARED)
    target_compile_definitions(${C_TARGET_NAME} PUBLIC TINYPBRT_SHARED_LIBS)
    set_target_properties(${C_TARGET_NAME} PROPERTIES
            DEFINE_SYMBOL TINYPBRT_EXPORTS
            C_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    if (TINYPBRT_BUILD_TESTS OR TINYPBRT_BUILD_COVERAGE)
        add_library(${C_TARGET_NAME}_static STATIC ${${C_TARGET_HEADERS}} ${${C_TARGET_SOURCES}})
        tinypbrt_configure_c_library(${C_TARGET_NAME}_static)
    endif()

    if (TINYPBRT_BUILD_CPP_WRAPPER AND NOT CPP_TARGET_HEADER_ONLY)
        set_target_properties(${CPP_TARGET_NAME} PROPERTIES
                DEFINE_SYMBOL TINYPBRT_CPP_EXPORTS
                CXX_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION_MAJOR}
        )
        if (APPLE)
            set_target_properties(${CPP_TARGET_NAME} PROPERTIES INSTALL_RPATH "@loader_path")
        elseif (UNIX)
            set_target_properties(${CPP_TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN")
        endif()
    endif()
endif()
# endregion

# region CPP_HEADER_CHECK
if (TINYPBRT_BUILD_CPP_WRAPPER AND PROJECT_IS_TOP_LEVEL)
    set(CPP_HEADER_CHECK_TARGET ${CPP_TARGET_NAME}_header_check)
    set(CPP_HEADER_CHECK_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CPP_HEADER_CHECK_TARGET})

    set(CPP_HEADER_CHECK_SOURCES)
    foreach(HEADER ${${CPP_TARGET_HEADERS}})
        cmake_path(IS_PREFIX BUILD_PUBLIC_HEADERS_DIR "${HEADER}" NORMALIZE IS_PUBLIC)
        if (NOT IS_PUBLIC)
            continue()
        endif()
        file(RELATIVE_PATH HEADER_REL ${BUILD_PUBLIC_HEADERS_DIR} ${HEADER})
        string(MAKE_C_IDENTIFIER ${HEADER_REL} HEADER_ID)
        set(CHECK_SOURCE ${CPP_HEADER_CHECK_DIR}/${HEADER_ID}.cpp)
        file(CONFIGURE OUTPUT ${CHECK_SOURCE} CONTENT "#include <${HEADER_REL}>\n")
        list(APPEND CPP_HEADER_CHECK_SOURCES ${CHECK_SOURCE})
    endforeach()

    add_library(${CPP_HEADER_CHECK_TARGET} OBJECT EXCLUDE_FROM_ALL ${CPP_HEADER_CHECK_SOURCES})
    target_link_libraries(${CPP_HEADER_CHECK_TARGET} PRIVATE ${CPP_TARGET_ALIAS})
    tinypbrt_configure_cpp_compile_options(${CPP_HEADER_CHECK_TARGET})
endif()
# endregion

# region CLANG_TIDY
if (TINYPBRT_ENABLE_CLANG_TIDY)
    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${C_TARGET_NAME}/pch.h)
    enable_clang_tidy(
            TARGET ${C_TARGET_NAME}
            CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD c11
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )

    if (TINYPBRT_BUILD_CPP_WRAPPER)
        if (CPP_TARGET_HEADER_ONLY)
            set(CLANG_TIDY_CPP_TARGET ${CPP_HEADER_CHECK_TARGET})
        else()
            set(CLANG_TIDY_CPP_TARGET ${CPP_TARGET_NAME})
            disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${C_TARGET_NAME}/pch.hpp)
        endif()

        if (TARGET ${CLANG_TIDY_CPP_TARGET})
            enable_clang_tidy(
                    TARGET ${CLANG_TIDY_CPP_TARGET}
                    CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
                    CXX_STANDARD c++20
                    HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*\\.hpp$"
                    EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
            )
        endif()
    endif()
endif()
# endregion

# region INSTALL
if(TINYPBRT_INSTALL)
    include(CMakePackageConfigHelpers)

    function(tinypbrt_install_package TARGET HEADER_PATTERN)
        install(DIRECTORY ${BUILD_PUBLIC_HEADERS_DIR}/${C_TARGET_NAME}/
                DESTINATION ${INSTALL_HEADERS_DIR}/${C_TARGET_NAME}
                FILES_MATCHING PATTERN "${HEADER_PATTERN}"
        )

        install(TARGETS ${TARGET} EXPORT ${TARGET}Targets
                RUNTIME DESTINATION bin     # .dll
                LIBRARY DESTINATION lib     # .so / .dylib
                ARCHIVE DESTINATION lib     # .a / .lib
                INCLUDES DESTINATION ${INSTALL_HEADERS_DIR}
        )

        install(EXPORT ${TARGET}Targets
                FILE ${TARGET}Targets.cmake
                NAMESPACE ${NAMESPACE}
                DESTINATION cmake
        )

        configure_package_config_file(
                "${PROJECT_SOURCE_DIR}/cmake/${TARGET}Config.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}Config.cmake"
                INSTALL_DESTINATION cmake
        )

        write_basic_package_version_file(
                "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}ConfigVersion.cmake"
                VERSION ${PROJECT_VERSION}
                COMPATIBILITY AnyNewerVersion
        )

        install(FILES
                "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}Config.cmake"
                "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}ConfigVersion.cmake"
                DESTINATION cmake
        )
    endfunction()

    tinypbrt_install_package(${C_TARGET_NAME} "*.h")
    if (TINYPBRT_BUILD_CPP_WRAPPER)
        tinypbrt_install_package(${CPP_TARGET_NAME} "*.hpp")
    endif()
endif()
# endregion
