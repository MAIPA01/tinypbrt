set(DOWNLOAD_GTEST ${TINYPBRT_BUILD_TESTS})

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