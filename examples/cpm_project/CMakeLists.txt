cmake_minimum_required(VERSION 3.30)
project(my_renderer LANGUAGES C)

set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
if (NOT EXISTS ${CPM_DOWNLOAD_LOCATION})
    file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()
include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
    NAME tinypbrt
    GITHUB_REPOSITORY MAIPA01/tinypbrt
    GIT_TAG v1.0.0
    OPTIONS
        "TINYPBRT_BUILD_SHARED OFF"     # ON -> tinypbrt.dll / libtinypbrt.so
)

add_executable(my_renderer main.c)
target_link_libraries(my_renderer PRIVATE tinypbrt::tinypbrt)

if (WIN32 AND TINYPBRT_BUILD_SHARED)
    add_custom_command(TARGET my_renderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:my_renderer> $<TARGET_FILE_DIR:my_renderer>
        COMMAND_EXPAND_LISTS)
endif()
