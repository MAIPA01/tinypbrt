set(DOWNLOAD_GTEST ${TINYPBRT_BUILD_TESTS})

# GTEST
if(DOWNLOAD_GTEST)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()