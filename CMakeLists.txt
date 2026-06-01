cmake_minimum_required(VERSION 3.16)
project(PopBubbles VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# UTF-8 source encoding for MSVC
if(MSVC)
    add_compile_options("/utf-8")
endif()

# ---- SFML (local pre-built) ----
set(SFML_ROOT "D:/SFML-2.6.2/SFML-2.6.2")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR "${SFML_ROOT}/lib")
set(SFML_BIN_DIR "${SFML_ROOT}/bin")

# ---- Collect sources ----
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# ---- Executable ----
add_executable(${PROJECT_NAME} ${SOURCES})

# ---- Include directories ----
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${SFML_INCLUDE_DIR}
)

# ---- SFML linkage ----
# Use debug libs for Debug, release libs for Release
target_link_libraries(${PROJECT_NAME} PRIVATE
    debug "${SFML_LIB_DIR}/sfml-graphics-d.lib"
    optimized "${SFML_LIB_DIR}/sfml-graphics.lib"
    debug "${SFML_LIB_DIR}/sfml-window-d.lib"
    optimized "${SFML_LIB_DIR}/sfml-window.lib"
    debug "${SFML_LIB_DIR}/sfml-system-d.lib"
    optimized "${SFML_LIB_DIR}/sfml-system.lib"
    debug "${SFML_LIB_DIR}/sfml-audio-d.lib"
    optimized "${SFML_LIB_DIR}/sfml-audio.lib"
)

# ---- SFML DLLs to build dir ----
file(GLOB SFML_DLLS "${SFML_BIN_DIR}/*.dll")
foreach(DLL ${SFML_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()

# Copy assets if exists
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    )
endif()

# Windows: console window for debug output
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE OFF
    )
endif()
