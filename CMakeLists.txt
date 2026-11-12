cmake_minimum_required(VERSION 3.30)
# The only place where the version of tinypbrt is set. README.md and examples/cpm_project are updated from it
# (TINYPBRT_SYNC_VERSION, cmake/version_sync.cmake), everything else reads PROJECT_VERSION.
project(tinypbrt VERSION 1.1.1 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS 					"Build shared library" 			OFF)
option(TINYPBRT_BUILD_SHARED 				"Build tinypbrt as a shared library" 	${BUILD_SHARED_LIBS})
option(TINYPBRT_BUILD_CPP_WRAPPER 			"Build the C++ wrapper (tinypbrt::tinypbrt_cpp)" 	ON)
option(TINYPBRT_BUILD_TESTS 				"Build tests"			 		${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_BENCHMARK  			"Build benchmark" 				${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_COVERAGE 				"Enable coverage reporting" 	${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_DOCUMENTATION	 		"Build documentation"	 		${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_EXAMPLES				"Build examples"				${PROJECT_IS_TOP_LEVEL})

option(TINYPBRT_ENABLE_CLANG_TIDY 			"Enables clang-tidy checks" 	${PROJECT_IS_TOP_LEVEL})

option(TINYPBRT_WARNINGS_AS_ERRORS 			"Treat warnings as errors when building tinypbrt" 	${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_ENABLE_PEDANTIC 			"Warn about everything that is not ISO C11 (-Wpedantic) when building tinypbrt" 	ON)

option(TINYPBRT_ENABLE_SANITIZERS 			"Build tinypbrt and its tests with sanitizers" OFF)
set(TINYPBRT_SANITIZERS "address;undefined" CACHE STRING "Sanitizers used when TINYPBRT_ENABLE_SANITIZERS=ON (address, undefined, leak, memory, thread)")

option(TINYPBRT_INSTALL 					"Enables installation of this project"						${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_INSTALL_TEST				"This is only to test if installation of tinypbrt works" 	OFF)

option(TINYPBRT_DISABLE_ASSERT_ON_RELEASE 		"Disables assert on release builds" 				OFF)

option(TINYPBRT_SYNC_VERSION 				"Write PROJECT_VERSION into README.md and examples/cpm_project when configuring" 	${PROJECT_IS_TOP_LEVEL})

message(STATUS "TINYPBRT_DISABLE_ASSERT_ON_RELEASE: ${TINYPBRT_DISABLE_ASSERT_ON_RELEASE}")

# region VERSION
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_sync.cmake)
if (TINYPBRT_SYNC_VERSION)
	tinypbrt_sync_version(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} VERSION ${PROJECT_VERSION})
endif()

if (PROJECT_IS_TOP_LEVEL)
	# cmake --build <build> --target tinypbrt-sync-version  (when TINYPBRT_SYNC_VERSION=OFF)
	add_custom_target(tinypbrt-sync-version
			COMMAND ${CMAKE_COMMAND} -DTINYPBRT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DTINYPBRT_VERSION=${PROJECT_VERSION}
					-P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_sync.cmake
			COMMENT "Writing version ${PROJECT_VERSION} into README.md and examples"
			VERBATIM
	)
endif()
# endregion

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/sanitizers.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_cpm.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

if (TINYPBRT_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()

if (TINYPBRT_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if (NOT TINYPBRT_INSTALL_TEST)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()

if (TINYPBRT_BUILD_TESTS OR TINYPBRT_BUILD_COVERAGE)
	enable_testing()
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

	if (PROJECT_IS_TOP_LEVEL)
		# fails when README.md or examples mention another version than project() (e.g. TINYPBRT_SYNC_VERSION=OFF in CI)
		add_test(NAME tinypbrt-version-check
				COMMAND ${CMAKE_COMMAND} -DTINYPBRT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DTINYPBRT_VERSION=${PROJECT_VERSION}
						-DTINYPBRT_CHECK_ONLY=ON -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version_sync.cmake
		)
	endif()
endif()

if (TINYPBRT_BUILD_BENCHMARK)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/benchmark)
endif()

if (TINYPBRT_BUILD_EXAMPLES)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

if (TINYPBRT_BUILD_COVERAGE AND TARGET tinypbrt-tests)
	# cmake --build <build> --target coverage  -> <build>/coverage_report/index.html
	set(TINYPBRT_COVERAGE_EXE_TARGETS tinypbrt-c-api-tests)
	if (TINYPBRT_BUILD_EXAMPLES)
		foreach(EXAMPLE quickstart scene_info load_from_memory materials lights export_obj)
			list(APPEND TINYPBRT_COVERAGE_EXE_TARGETS tinypbrt-example-${EXAMPLE})
		endforeach()
	endif()

	setup_coverage_report(
			TEST_TARGET tinypbrt-tests
			EXE_TARGETS ${TINYPBRT_COVERAGE_EXE_TARGETS}
			FORMAT html
			OUTPUT_DIR ${CMAKE_BINARY_DIR}/coverage_report
			CLEAN_TEMP
	)
endif()
