set(DOWNLOAD_GTEST ${TINYPBRT_BUILD_TESTS})
set(DOWNLOAD_BENCHMARK ${TINYPBRT_BUILD_BENCHMARK})

# GTEST
if(DOWNLOAD_GTEST)
    CPMAddPackage("gh:google/googletest#v1.17.0")

    foreach(GTEST_TARGET gtest gtest_main gmock gmock_main)
        if (TARGET ${GTEST_TARGET})
            get_target_property(GTEST_TARGET_IMPORTED ${GTEST_TARGET} IMPORTED)
            if (NOT GTEST_TARGET_IMPORTED)
                tinypbrt_enable_sanitizers(${GTEST_TARGET})
            endif()
        endif()
    endforeach()
endif()

# GOOGLE BENCHMARK
if(DOWNLOAD_BENCHMARK)
    CPMAddPackage(
        NAME benchmark
        GITHUB_REPOSITORY google/benchmark
        VERSION 1.9.4
        OPTIONS
            "BENCHMARK_ENABLE_TESTING OFF"
            "BENCHMARK_ENABLE_GTEST_TESTS OFF"
            "BENCHMARK_ENABLE_INSTALL OFF"
            "BENCHMARK_INSTALL_DOCS OFF"
            "BENCHMARK_ENABLE_WERROR OFF"
    )

    foreach(BENCHMARK_TARGET benchmark benchmark_main)
        if (TARGET ${BENCHMARK_TARGET})
            get_target_property(BENCHMARK_TARGET_IMPORTED ${BENCHMARK_TARGET} IMPORTED)
            if (NOT BENCHMARK_TARGET_IMPORTED)
                tinypbrt_enable_sanitizers(${BENCHMARK_TARGET})
            endif()
        endif()
    endforeach()
endif()
