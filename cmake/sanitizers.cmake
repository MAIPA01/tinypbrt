# Sanitizery dla biblioteki i testów.
#
#   cmake -DTINYPBRT_ENABLE_SANITIZERS=ON ...                       -> address + undefined
#   cmake -DTINYPBRT_ENABLE_SANITIZERS=ON -DTINYPBRT_SANITIZERS="thread" ...
#
# GCC / Clang (także clang.exe na Windows): address, undefined, leak, memory (tylko Clang/Linux), thread.
# MSVC / clang-cl: tylko address (/fsanitize=address); pozostałe są pomijane z ostrzeżeniem.
# LeakSanitizer jest częścią AddressSanitizera na Linuksie; na Windows nie istnieje.

if (TINYPBRT_ENABLE_SANITIZERS)
	message(STATUS "tinypbrt sanitizers: ${TINYPBRT_SANITIZERS}")

	if ("thread" IN_LIST TINYPBRT_SANITIZERS AND ("address" IN_LIST TINYPBRT_SANITIZERS OR "leak" IN_LIST TINYPBRT_SANITIZERS))
		message(FATAL_ERROR "ThreadSanitizer cannot be combined with AddressSanitizer/LeakSanitizer")
	endif()
	if ("memory" IN_LIST TINYPBRT_SANITIZERS AND NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
		message(FATAL_ERROR "MemorySanitizer is only available in Clang")
	endif()
endif()

# MSVC albo clang-cl (frontend MSVC)
function(_tinypbrt_is_msvc_frontend OUT_VAR)
	if (MSVC OR (CMAKE_C_COMPILER_ID STREQUAL "Clang" AND CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
		set(${OUT_VAR} TRUE PARENT_SCOPE)
	else()
		set(${OUT_VAR} FALSE PARENT_SCOPE)
	endif()
endfunction()

# Dodaje flagi sanitizerów do celu. PUBLIC: biblioteka statyczna przekazuje je dalej do testów,
# dzięki czemu kod C i C++ jest zinstrumentowany jednakowo, a linker dostaje runtime sanitizera.
function(tinypbrt_enable_sanitizers TARGET)
	if (NOT TINYPBRT_ENABLE_SANITIZERS)
		return()
	endif()

	_tinypbrt_is_msvc_frontend(IS_MSVC)
	if (IS_MSVC)
		if ("address" IN_LIST TINYPBRT_SANITIZERS)
			target_compile_options(${TARGET} PUBLIC /fsanitize=address /Zi)
			target_link_options(${TARGET} PUBLIC /INCREMENTAL:NO /DEBUG)
		endif()
		set(UNSUPPORTED ${TINYPBRT_SANITIZERS})
		list(REMOVE_ITEM UNSUPPORTED address)
		if (UNSUPPORTED)
			message(WARNING "Sanitizers not supported by the MSVC frontend, skipped: ${UNSUPPORTED}")
		endif()
		return()
	endif()

	list(JOIN TINYPBRT_SANITIZERS "," SANITIZERS_LIST)
	target_compile_options(${TARGET} PUBLIC
		-fsanitize=${SANITIZERS_LIST}
		-fno-sanitize-recover=all   # każdy błąd UBSan kończy test (zamiast tylko wypisać ostrzeżenie)
		-fno-omit-frame-pointer     # czytelne stosy wywołań
		-g
	)
	target_link_options(${TARGET} PUBLIC -fsanitize=${SANITIZERS_LIST})
endfunction()

# Zmienne środowiskowe dla ctest: zatrzymanie na pierwszym błędzie i pełne stosy.
function(tinypbrt_set_sanitizers_test_environment TEST_NAME)
	if (NOT TINYPBRT_ENABLE_SANITIZERS)
		return()
	endif()

	set(ASAN_OPTIONS "abort_on_error=1:strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1")
	if (NOT WIN32)
		string(APPEND ASAN_OPTIONS ":detect_leaks=1")
	endif()

	set(TEST_ENVIRONMENT
		"ASAN_OPTIONS=${ASAN_OPTIONS}"
		"UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1"
		"TSAN_OPTIONS=halt_on_error=1"
	)
	set_tests_properties(${TEST_NAME} PROPERTIES ENVIRONMENT "${TEST_ENVIRONMENT}")

	# clang.exe / clang-cl na Windows: runtime ASan to DLL z katalogu zasobów Clanga -> dopisz go do PATH
	if (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		execute_process(
			COMMAND "${CMAKE_CXX_COMPILER}" --print-resource-dir
			OUTPUT_VARIABLE CLANG_RESOURCE_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
			ERROR_QUIET
		)
		if (CLANG_RESOURCE_DIR)
			file(TO_NATIVE_PATH "${CLANG_RESOURCE_DIR}/lib/windows" CLANG_RUNTIME_DIR)
			set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${CLANG_RUNTIME_DIR}")
		endif()
	endif()
endfunction()
