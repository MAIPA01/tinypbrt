# region HEADER_FILES
set(PROJECT_HEADERS ${PROJECT_NAME}_HEADERS)
set(BUILD_PUBLIC_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_PRIVATE_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INSTALL_HEADERS_DIR include)
file(GLOB_RECURSE ${PROJECT_HEADERS} CONFIGURE_DEPENDS ${BUILD_PUBLIC_HEADERS_DIR}/*.h ${BUILD_PRIVATE_HEADERS_DIR}/*.h)
# endregion

# region SOURCE_FILES
set(PROJECT_SOURCES ${PROJECT_NAME}_SOURCES)
set(BUILD_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INSTALL_SOURCES_DIR src)
file(GLOB_RECURSE ${PROJECT_SOURCES} CONFIGURE_DEPENDS ${BUILD_SOURCES_DIR}/*.c)
# endregion

# region LIBRARY_SETUP
add_library(${PROJECT_NAME} ${${PROJECT_HEADERS}} ${${PROJECT_SOURCES}})

# set library alias
set(PROJECT_NAMESPACE ${PROJECT_NAME}::)
set(PROJECT_ALIAS ${PROJECT_NAMESPACE}${PROJECT_NAME})
add_library(${PROJECT_ALIAS} ALIAS ${PROJECT_NAME})
# endregion

# region CLANG_TIDY
if (TINYPBRT_ENABLE_CLANG_TIDY)
    set(CLANG_TIDY_C_STANDARD c11)

    disable_clang_tidy_for_file(${CMAKE_CURRENT_SOURCE_DIR}/src/${PROJECT_NAME}/pch.h)
    enable_clang_tidy(
            TARGET ${PROJECT_NAME}
            CONFIG "${CMAKE_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_C_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${CMAKE_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )
endif()
# endregion

target_precompile_headers(${PROJECT_NAME} PRIVATE "$<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}/${PROJECT_NAME}/pch.h>")

target_compile_features(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:c_std_11>)

# set headers folder
target_include_directories(${PROJECT_NAME}  PUBLIC $<BUILD_INTERFACE:${BUILD_PUBLIC_HEADERS_DIR}>
                                            PRIVATE $<BUILD_INTERFACE:${BUILD_PRIVATE_HEADERS_DIR}>
)

# region COMPILE_DEFINITIONS
target_compile_definitions(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:
        $<$<BOOL:${TINYPBRT_DISABLE_ASSERT_ON_RELEASE}>:TINYPBRT_DISABLE_ASSERT_ON_RELEASE>
        $<$<BOOL:${BUILD_SHARED_LIBS}>:TINYPBRT_SHARED_LIBS>
        >
)
# endregion

# region COMPILE_OPTIONS
target_compile_options(${PROJECT_NAME} INTERFACE
        # MSVC/clang with MSVC frontend
        $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:
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
        $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:
            $<$<CONFIG:Release>:
                -O3
                -flto
            >
            -Wall
            -Wextra
            -Werror
            -fno-strict-aliasing
        >
        $<BUILD_INTERFACE:$<$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${TINYPBRT_BUILD_COVERAGE}>>:
            --coverage
            -fcoverage-mcdc
            -fprofile-instr-generate
            -fcoverage-mapping
        >>
)
# endregion

# region LINK_OPTIONS
target_link_options(${PROJECT_NAME} INTERFACE
        # MSVC/clang with MSVC frontend
        $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:
            $<$<CONFIG:Release>:/LTCG>
            $<$<CONFIG:Debug>:/DEBUG>
        >
        # GCC/clang with GNU frontend
        $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:
            $<$<CONFIG:Release>:-flto>
            -static
        >
        $<BUILD_INTERFACE:$<$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>,$<BOOL:${TINYPBRT_BUILD_COVERAGE}>>:
            --coverage
            -fprofile-instr-generate
        >>
)
# endregion

# region INSTALL
if(TINYPBRT_INSTALL)
    # Installing header files
    install(DIRECTORY ${BUILD_HEADERS_DIR}/${PROJECT_NAME}/
            DESTINATION ${INSTALL_HEADERS_DIR}/${PROJECT_NAME}
            FILES_MATCHING PATTERN "*.h"
    )

    # Installing targets
    set(PROJECT_INSTALL_TARGETS ${PROJECT_NAME}Targets)
    install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
            INCLUDES DESTINATION ${INSTALL_HEADERS_DIR}
    )

    # Generate Targets.cmake
    install(EXPORT ${PROJECT_INSTALL_TARGETS}
            FILE ${PROJECT_INSTALL_TARGETS}.cmake
            NAMESPACE ${PROJECT_NAMESPACE}
            DESTINATION cmake
    )

    # Create Config.cmake
    include(CMakePackageConfigHelpers)

    set(PROJECT_INSTALL_CONFIG ${PROJECT_NAME}Config)
    configure_package_config_file(
            "${CMAKE_SOURCE_DIR}/cmake/${PROJECT_INSTALL_CONFIG}.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            INSTALL_DESTINATION cmake
    )

    # Create Version.cmake
    set(PROJECT_INSTALL_VERSION ${PROJECT_NAME}Version)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    # Install config files
    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_CONFIG}.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_INSTALL_VERSION}.cmake"
            DESTINATION cmake
    )
endif()
# endregion