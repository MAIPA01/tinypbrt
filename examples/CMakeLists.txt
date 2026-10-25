set(TINYPBRT_EXAMPLES quickstart scene_info load_from_memory materials lights export_obj)
set(TINYPBRT_EXAMPLE_SCENES ${CMAKE_CURRENT_SOURCE_DIR}/scenes)

add_library(${PROJECT_NAME}-example-utils STATIC ${CMAKE_CURRENT_SOURCE_DIR}/common/example_utils.c
                                                 ${CMAKE_CURRENT_SOURCE_DIR}/common/example_utils.h)
target_include_directories(${PROJECT_NAME}-example-utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common)
target_link_libraries(${PROJECT_NAME}-example-utils PUBLIC tinypbrt::tinypbrt)
set_target_properties(${PROJECT_NAME}-example-utils PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
target_compile_options(${PROJECT_NAME}-example-utils PRIVATE
    $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:/W4>
    $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:-Wall -Wextra -Wpedantic>
    $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:$<IF:$<C_COMPILER_FRONTEND_VARIANT:MSVC>,/WX,-Werror>>
)
tinypbrt_enable_sanitizers(${PROJECT_NAME}-example-utils)
tinypbrt_enable_coverage(${PROJECT_NAME}-example-utils)

foreach(EXAMPLE ${TINYPBRT_EXAMPLES})
    set(EXAMPLE_TARGET ${PROJECT_NAME}-example-${EXAMPLE})
    add_executable(${EXAMPLE_TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE}/main.c)
    target_link_libraries(${EXAMPLE_TARGET} PRIVATE ${PROJECT_NAME}-example-utils)
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)
    target_compile_options(${EXAMPLE_TARGET} PRIVATE
        $<$<OR:$<C_COMPILER_ID:MSVC>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:MSVC>>>:/W4>
        $<$<OR:$<C_COMPILER_ID:GNU>,$<AND:$<C_COMPILER_ID:Clang>,$<C_COMPILER_FRONTEND_VARIANT:GNU>>>:-Wall -Wextra -Wpedantic>
        $<$<BOOL:${TINYPBRT_WARNINGS_AS_ERRORS}>:$<IF:$<C_COMPILER_FRONTEND_VARIANT:MSVC>,/WX,-Werror>>
    )
    if (NOT WIN32 AND NOT APPLE)
        target_link_libraries(${EXAMPLE_TARGET} PRIVATE m)
    endif()
    tinypbrt_enable_sanitizers(${EXAMPLE_TARGET})
    tinypbrt_enable_coverage(${EXAMPLE_TARGET})

    if (WIN32 AND (TINYPBRT_BUILD_SHARED OR BUILD_SHARED_LIBS))
        add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${EXAMPLE_TARGET}> $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>
            COMMAND_EXPAND_LISTS
        )
    endif()
endforeach()

if (TINYPBRT_BUILD_TESTS)
    set(EXAMPLE_ARGS_quickstart        ${TINYPBRT_EXAMPLE_SCENES}/cornell_box.pbrt)
    set(EXAMPLE_ARGS_scene_info        ${TINYPBRT_EXAMPLE_SCENES}/forest.pbrt)
    set(EXAMPLE_ARGS_load_from_memory  ${TINYPBRT_EXAMPLE_SCENES})
    set(EXAMPLE_ARGS_materials         ${TINYPBRT_EXAMPLE_SCENES}/cornell_box.pbrt gold)
    set(EXAMPLE_ARGS_lights            ${TINYPBRT_EXAMPLE_SCENES}/forest.pbrt)
    set(EXAMPLE_ARGS_export_obj        ${TINYPBRT_EXAMPLE_SCENES}/forest.pbrt ${CMAKE_CURRENT_BINARY_DIR}/forest.obj)

    foreach(EXAMPLE ${TINYPBRT_EXAMPLES})
        add_test(NAME ${PROJECT_NAME}-example-${EXAMPLE}_TEST COMMAND ${PROJECT_NAME}-example-${EXAMPLE} ${EXAMPLE_ARGS_${EXAMPLE}})
        set_tests_properties(${PROJECT_NAME}-example-${EXAMPLE}_TEST PROPERTIES LABELS examples)
        tinypbrt_set_sanitizers_test_environment(${PROJECT_NAME}-example-${EXAMPLE}_TEST)
    endforeach()
endif()

# region CLANG_TIDY
if (TINYPBRT_ENABLE_CLANG_TIDY)
    set(CLANG_TIDY_C_STANDARD c11)

    enable_clang_tidy(
            TARGET ${PROJECT_NAME}-example-utils
            CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
            CXX_STANDARD ${CLANG_TIDY_C_STANDARD}
            HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
            EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
    )

    foreach(EXAMPLE ${TINYPBRT_EXAMPLES})
        set(EXAMPLE_TARGET ${PROJECT_NAME}-example-${EXAMPLE})

        enable_clang_tidy(
                TARGET ${EXAMPLE_TARGET}
                CONFIG "${PROJECT_SOURCE_DIR}/.clang-tidy"
                CXX_STANDARD ${CLANG_TIDY_C_STANDARD}
                HEADER_FILTER "^${CMAKE_CURRENT_SOURCE_DIR}[\\\\\/\\\\].*"
                EXCLUDE_HEADER_FILTER "^${PROJECT_SOURCE_DIR}[\\\\\/\\\\]third_party[\\\\\/\\\\].*"
        )
    endforeach()
endif()
# endregion
