#-------------------------------------------------------------------------------------------
# Copyright (c) 2024-present, SkillerRaptor
#
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------------------

function(hyperengine_group_source SOURCE)
    foreach (item IN ITEMS ${SOURCE})
        get_filename_component(src_path "${item}" PATH)
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" group_path "${src_path}")
        string(REPLACE ".." "\\" group_path "${group_path}")
        source_group("${group_path}" FILES "${item}")
    endforeach ()
endfunction()

function(hyperengine_define_executable target)
    hyperengine_group_source(${SOURCES})
    if (HEADERS)
        hyperengine_group_source(${HEADERS})
    endif ()

    add_executable(${target} ${SOURCES} ${HEADERS})
    target_link_libraries(${target} PRIVATE ProjectOptions ProjectWarnings)
    target_include_directories(${target} PUBLIC include)

    if (WIN32)
        target_compile_definitions(
                ${target}
                PUBLIC
                HE_WINDOWS=1
                _CRT_SECURE_NO_WARNINGS
                NOMINMAX
                WIN32_LEAN_AND_MEAN)
    else ()
        target_compile_definitions(${target} PUBLIC HE_LINUX=1)
    endif ()
endfunction()

function(hyperengine_define_library target)
    if (NOT SOURCES)
        hyperengine_define_interface_library(${target})
        return()
    endif ()

    hyperengine_group_source(${SOURCES})
    if (HEADERS)
        hyperengine_group_source(${HEADERS})
    endif ()

    add_library(${target} STATIC ${SOURCES} ${HEADERS})
    target_link_libraries(${target} PRIVATE ProjectOptions ProjectWarnings)
    target_include_directories(${target} PUBLIC include)

    if (WIN32)
        target_compile_definitions(
                ${target}
                PUBLIC
                HE_WINDOWS=1
                _CRT_SECURE_NO_WARNINGS
                NOMINMAX
                WIN32_LEAN_AND_MEAN)
    else ()
        target_compile_definitions(${target} PUBLIC HE_LINUX=1)
    endif ()
endfunction()

function(hyperengine_define_interface_library target)
    hyperengine_group_source(${HEADERS})

    add_library(${target} INTERFACE ${SOURCES} ${HEADERS})
    target_link_libraries(${target} INTERFACE ProjectOptions ProjectWarnings)
    target_include_directories(${target} INTERFACE include)

    if (WIN32)
        target_compile_definitions(
                ${target}
                INTERFACE
                HE_WINDOWS=1
                _CRT_SECURE_NO_WARNINGS
                NOMINMAX
                WIN32_LEAN_AND_MEAN)
    else ()
        target_compile_definitions(${target} INTERFACE HE_LINUX=1)
    endif ()
endfunction()

function(hyperengine_download_and_extract URL DESTINATION FOLDER_NAME)
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip)
        message(STATUS "Downloading ${URL} and unpacking to ${DESTINATION}/${FOLDER_NAME}.")
        file(
                DOWNLOAD
                ${URL}
                ${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip)
    else ()
        message(STATUS "${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip already exists. No download required.")
    endif ()

    if (NOT EXISTS ${DESTINATION}/${FOLDER_NAME})
        message(STATUS "Extracting ${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip to ${DESTINATION}/${FOLDER_NAME}.")
        file(
                ARCHIVE_EXTRACT
                INPUT ${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip
                DESTINATION ${DESTINATION}/${FOLDER_NAME})
    else ()
        message(STATUS "${CMAKE_BINARY_DIR}/download/${FOLDER_NAME}.zip is already extracted to ${DESTINATION}/${FOLDER_NAME}.")
    endif ()
endfunction()


function(hyperengine_deploy_files SOURCE DESTINATION)
    set(DEPLOY_FILES_DESTINATION ${CMAKE_BINARY_DIR}/${DESTINATION})
    message(STATUS "Copying ${SOURCE} to ${DEPLOY_FILES_DESTINATION}")
    file(COPY ${SOURCE} DESTINATION ${DEPLOY_FILES_DESTINATION})
endfunction()