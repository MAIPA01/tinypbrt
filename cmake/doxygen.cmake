# Dokumentacja (Doxygen): cmake --build <build> --target docs  ->  <build>/docs/html/index.html
#
# Konfiguracja jest w docs/Doxyfile.in (wersja i ścieżki wstawiane przez CMake), strony opisowe
# w docs/pages/*.md, strona główna to README.md. Komentarze w nagłówkach: /** ... */ (styl Javadoc).
find_package(Doxygen OPTIONAL_COMPONENTS dot)

set(TINYPBRT_DOCUMENTATION_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs" CACHE PATH "Output directory of the Doxygen documentation")
option(TINYPBRT_DOCUMENTATION_WARN_UNDOCUMENTED "Doxygen warnings for undocumented API" OFF)

if (DOXYGEN_FOUND)
    if (TINYPBRT_DOCUMENTATION_WARN_UNDOCUMENTED)
        set(TINYPBRT_DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    else()
        set(TINYPBRT_DOXYGEN_WARN_IF_UNDOCUMENTED NO)
    endif()
    if (DOXYGEN_DOT_FOUND)
        set(TINYPBRT_DOXYGEN_HAVE_DOT YES)
    else()
        set(TINYPBRT_DOXYGEN_HAVE_DOT NO)
    endif()

    set(TINYPBRT_DOXYFILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${PROJECT_SOURCE_DIR}/docs/Doxyfile.in ${TINYPBRT_DOXYFILE} @ONLY)

    add_custom_target(documentation_${PROJECT_NAME}
                      COMMAND ${CMAKE_COMMAND} -E make_directory ${TINYPBRT_DOCUMENTATION_OUTPUT_DIR}
                      COMMAND ${DOXYGEN_EXECUTABLE} ${TINYPBRT_DOXYFILE}
                      COMMAND ${CMAKE_COMMAND} -E echo "Documentation: ${TINYPBRT_DOCUMENTATION_OUTPUT_DIR}/html/index.html"
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                      COMMENT "Generating documentation..."
                      VERBATIM)
    if (NOT TARGET docs)
        add_custom_target(docs DEPENDS documentation_${PROJECT_NAME})
    endif()
else()
    message(STATUS "Doxygen not found, documentation target will not be available.")
endif()
