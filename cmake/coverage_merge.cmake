# cmake -DLLVM_PROFDATA=<llvm-profdata> -DPROFRAW_DIR=<dir> -DOUTPUT=<file.profdata> -P coverage_merge.cmake

file(GLOB PROFRAW_FILES "${PROFRAW_DIR}/*.profraw")
if (NOT PROFRAW_FILES)
    message(FATAL_ERROR "No .profraw files in ${PROFRAW_DIR} - were the tests built with TINYPBRT_BUILD_COVERAGE=ON?")
endif()

execute_process(
    COMMAND ${LLVM_PROFDATA} merge -sparse ${PROFRAW_FILES} -o ${OUTPUT}
    RESULT_VARIABLE RESULT
)
if (NOT RESULT EQUAL 0)
    message(FATAL_ERROR "llvm-profdata merge failed (${RESULT})")
endif()
list(LENGTH PROFRAW_FILES COUNT)
message(STATUS "Merged ${COUNT} .profraw file(s) into ${OUTPUT}")
