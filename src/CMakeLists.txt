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
file(GLOB_RECURSE ${CPP_TARGET_HEADERS} CONFIGURE_DEPENDS   ${BUILD_PUBLIC_HEADERS_DIR}/*.h
                                                            ${BUILD_PUBLIC_HEADERS_DIR}/*.hpp
                                                            ${BUILD_PRIVATE_HEADERS_DIR}/*.h
                                                            ${BUILD_PRIVATE_HEADERS_DIR}/*.hpp)
# endregion

# region SOURCE_FILES
set(BUILD_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INSTALL_SOURCES_DIR src)

set(C_TARGET_SOURCES ${C_TARGET_NAME}_SOURCES)
file(GLOB_RECURSE ${C_TARGET_SOURCES} CONFIGURE_DEPENDS ${BUILD_SOURCES_DIR}/*.c)

set(CPP_TARGET_SOURCES ${CPP_TARGET_NAME}_SOURCES)
file(GLOB_RECURSE ${CPP_TARGET_SOURCES} CONFIGURE_DEPENDS   ${BUILD_SOURCES_DIR}/*.c
                                                            ${BUILD_SOURCES_DIR}/*.cpp)
# endregion

# region LIBRARY_SETUP
if (TINYPBRT_BUILD_SHARED)
    set(LIBRARY_TYPE SHARED)
else()
    set(LIBRARY_TYPE STATIC)
endif()
add_library(${C_TARGET_NAME} ${LIBRARY_TYPE} ${${C_TARGET_HEADERS}} ${${C_TARGET_SOURCES}})
add_library(${CPP_TARGET_NAME} ${LIBRARY_TYPE} ${${CPP_TARGET_HEADERS}} ${${CPP_TARGET_SOURCES}})

# set library alias
set(NAMESPACE ${C_TARGET_NAME}::)

set(C_TARGET_ALIAS ${NAMESPACE}${C_TARGET_NAME})
add_library(${C_TARGET_ALIAS} ALIAS ${C_TARGET_NAME})

set(CPP_TARGET_ALIAS ${NAMESPACE}${CPP_TARGET_NAME})
add_library(${CPP_TARGET_ALIAS} ALIAS ${CPP_TARGET_NAME})
# endregion

# region CLANG_TIDY
if (TINYPBRT_ENABLE_CLANG_TIDY)
    set(CLANG_TIDY_C_STANDARD c11)

    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${C_TARGET_NAME}/pch.h)
    enable_clang_tidy(
            TARGET ${C_TARGET_NAME}
            CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_C_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )

    set(CLANG_TIDY_CPP_STANDARD c++20)

    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${C_TARGET_NAME}/pch.hpp)
    enable_clang_tidy(
            TARGET ${CPP_TARGET_NAME}
            CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_CPP_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )
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

function(tinypbrt_configure_cpp_library TARGET)
    target_precompile_headers(${TARGET} PRIVATE "$<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}/${C_TARGET_NAME}/pch.hpp>")

    target_compile_features(${TARGET} PRIVATE $<BUILD_INTERFACE:cxx_std_20>)

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

tinypbrt_configure_c_library(${C_TARGET_NAME})
tinypbrt_configure_cpp_library(${CPP_TARGET_NAME})

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

    target_compile_definitions(${CPP_TARGET_NAME} PUBLIC TINYPBRT_SHARED_LIBS)
    set_target_properties(${CPP_TARGET_NAME} PROPERTIES
            DEFINE_SYMBOL TINYPBRT_EXPORTS
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    if (TINYPBRT_BUILD_TESTS OR TINYPBRT_BUILD_COVERAGE)
        add_library(${CPP_TARGET_NAME}_static STATIC ${${CPP_TARGET_HEADERS}} ${${CPP_TARGET_SOURCES}})
        tinypbrt_configure_cpp_library(${CPP_TARGET_NAME}_static)
    endif()
endif()
# endregion

# region INSTALL
if(TINYPBRT_INSTALL)
    # Installing header files
    install(DIRECTORY ${BUILD_PUBLIC_HEADERS_DIR}/${C_TARGET_NAME}/
            DESTINATION ${INSTALL_HEADERS_DIR}/${C_TARGET_NAME}
            FILES_MATCHING PATTERN "*.h"
    )

    # Installing targets
    set(C_TARGET_INSTALL_TARGETS ${C_TARGET_NAME}Targets)
    install(TARGETS ${C_TARGET_NAME} EXPORT ${C_TARGET_NAME}Targets
            RUNTIME DESTINATION bin     # .dll
            LIBRARY DESTINATION lib     # .so / .dylib
            ARCHIVE DESTINATION lib     # .a / .lib
            INCLUDES DESTINATION ${INSTALL_HEADERS_DIR}
    )

    # Generate Targets.cmake
    install(EXPORT ${C_TARGET_INSTALL_TARGETS}
            FILE ${C_TARGET_INSTALL_TARGETS}.cmake
            NAMESPACE ${C_TARGET_NAMESPACE}
            DESTINATION cmake
    )

    # Create Config.cmake
    include(CMakePackageConfigHelpers)

    set(C_TARGET_INSTALL_CONFIG ${C_TARGET_NAME}Config)
    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/${C_TARGET_INSTALL_CONFIG}.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${C_TARGET_INSTALL_CONFIG}.cmake"
            INSTALL_DESTINATION cmake
    )

    # Create Version.cmake
    set(C_TARGET_INSTALL_VERSION ${C_TARGET_NAME}ConfigVersion)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${C_TARGET_INSTALL_VERSION}.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Install config files
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${C_TARGET_INSTALL_CONFIG}.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${C_TARGET_INSTALL_VERSION}.cmake"
            DESTINATION cmake
    )

    # Installing header files
    install(DIRECTORY ${BUILD_PUBLIC_HEADERS_DIR}/${C_TARGET_NAME}/
            DESTINATION ${INSTALL_HEADERS_DIR}/${C_TARGET_NAME}
            FILES_MATCHING PATTERN "*.(h|hpp)"
    )

    # Installing targets
    set(CPP_TARGET_INSTALL_TARGETS ${CPP_TARGET_NAME}Targets)
    install(TARGETS ${CPP_TARGET_NAME} EXPORT ${CPP_TARGET_NAME}Targets
            RUNTIME DESTINATION bin     # .dll
            LIBRARY DESTINATION lib     # .so / .dylib
            ARCHIVE DESTINATION lib     # .a / .lib
            INCLUDES DESTINATION ${INSTALL_HEADERS_DIR}
    )

    # Generate Targets.cmake
    install(EXPORT ${CPP_TARGET_INSTALL_TARGETS}
            FILE ${CPP_TARGET_INSTALL_TARGETS}.cmake
            NAMESPACE ${NAMESPACE}
            DESTINATION cmake
    )

    # Create Config.cmake
    include(CMakePackageConfigHelpers)

    set(CPP_TARGET_INSTALL_CONFIG ${CPP_TARGET_NAME}Config)
    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/${C_TARGET_INSTALL_CONFIG}.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${C_TARGET_INSTALL_CONFIG}.cmake"
            INSTALL_DESTINATION cmake
    )

    # Create Version.cmake
    set(CPP_TARGET_INSTALL_VERSION ${CPP_TARGET_NAME}ConfigVersion)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${CPP_TARGET_INSTALL_VERSION}.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Install config files
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${CPP_TARGET_INSTALL_CONFIG}.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${CPP_TARGET_INSTALL_VERSION}.cmake"
            DESTINATION cmake
    )
endif()
# endregion