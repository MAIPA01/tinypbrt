cmake_minimum_required(VERSION 3.30)
project(tinypbrt VERSION 1.0.0 LANGUAGES C CXX)

option(BUILD_SHARED_LIBS 					"Build shared library" 			OFF)
option(TINYPBRT_BUILD_TESTS 				"Build tests"			 		${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_BENCHMARK  			"Build benchmark" 				${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_COVERAGE 				"Enable coverage reporting" 	${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_DOCUMENTATION	 		"Build documentation"	 		${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_BUILD_EXAMPLES				"Build examples"				${PROJECT_IS_TOP_LEVEL})

option(TINYPBRT_ENABLE_CLANG_TIDY 			"Enables clang-tidy checks" 	${PROJECT_IS_TOP_LEVEL})

option(TINYPBRT_INSTALL 					"Enables installation of this project"						${PROJECT_IS_TOP_LEVEL})
option(TINYPBRT_INSTALL_TEST				"This is only to test if installation of tinypbrt works" 	OFF)

option(TINYPBRT_DISABLE_ASSERT_ON_RELEASE 		"Disables assert on release builds" 				OFF)

message(STATUS "TINYPBRT_DISABLE_ASSERT_ON_RELEASE: ${TINYPBRT_DISABLE_ASSERT_ON_RELEASE}")

if(${BUILD_SHARED_LIBS})
	set(BUILD_STATIC_LIBS OFF)
else()
	set(BUILD_STATIC_LIBS ON)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/third_party.cmake)

if (TINYPBRT_ENABLE_CLANG_TIDY)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/clang_tidy.cmake)
endif()

if (TINYPBRT_BUILD_DOCUMENTATION)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if (NOT TINYPBRT_INSTALL_TEST)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tinypbrt)
endif()

if (TINYPBRT_BUILD_TESTS OR TINYPBRT_BUILD_COVERAGE)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()

if (TINYPBRT_BUILD_COVERAGE OR TINYPBRT_BUILD_BENCHMARK)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/benchmark)
endif()

if (TINYPBRT_BUILD_EXAMPLES)
	add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

if (TINYPBRT_BUILD_COVERAGE)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coverage_report.cmake)

#	setup_coverage_report(
#			TEST_TARGET tinypbrt-tests
#			FORMAT html
#			OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/coverage_report
#			CLEAN_TEMP
#	)
endif()