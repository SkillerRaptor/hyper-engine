#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

function(enable_doxygen)
    if ((NOT DOXYGEN_USE_MDFILE_AS_MAINPAGE) AND EXISTS "${PROJECT_SOURCE_DIR}/README.md")
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    endif ()

    set(DOXYGEN_QUIET NO)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_DOT_TRANSPARENT YES)

    if (NOT DOXYGEN_EXCLUDE_PATTERNS)
        set(DOXYGEN_EXCLUDE_PATTERNS "${PROJECT_SOURCE_DIR}/third_party" "${CMAKE_CURRENT_BINARY_DIR}/_deps/*")
    endif ()

    find_package(Doxygen OPTIONAL_COMPONENTS dot)

    if (Doxygen_FOUND)
        include(FetchContent)

        FetchContent_Declare(_doxygen_theme URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v2.3.4.zip)
        FetchContent_MakeAvailable(_doxygen_theme)
        set(DOXYGEN_HTML_EXTRA_STYLESHEET "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")

        message(STATUS "Adding `doxygen-docs` target that builds the documentation.")
        doxygen_add_docs(
                doxygen-docs
                ALL
                ${PROJECT_SOURCE_DIR}
                COMMENT
                "Generating documentation - entry file: ${CMAKE_CURRENT_BINARY_DIR}/html/index.html")
    endif ()
endfunction()