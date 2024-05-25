cmake_minimum_required(VERSION 3.14)
project(file-director)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/vertex_buffer.cpp 
    src/elements_buffer.cpp 
    src/vertex_array.cpp 
    src/shader.cpp 
    src/texture.cpp
    src/stb.cpp
    src/camera.cpp
    src/mesh.cpp
    src/skybox.cpp
    src/shader.h)

set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")

set(GLM_BUILD_INSTALL ON CACHE INTERNAL "Generate installation target")
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")
find_package(glm CONFIG REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")
target_link_libraries(${PROJECT_NAME} "glm" glm::glm-header-only)

target_include_directories(${PROJECT_NAME} PRIVATE "${LIB_DIR}/stb")

if (APPLE)
    message(STATUS "Platform is Apple")
    add_compile_definitions(PLATFORM_APPLE)
endif ()

if (WIN32)
    message(STATUS "Platform is Windows")
    add_compile_definitions(PLATFORM_WINDOWS)
endif ()
