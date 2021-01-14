cmake_minimum_required(VERSION 3.9)

project(R2DEngine)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(GLEW REQUIRED)
    include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIRS}
    )
endif()

file(GLOB SRC_CPP_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_executable(
    ${PROJECT_NAME}
    ${SRC_CPP_FILES}
    ${HEADER_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC "${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}"
)

if(UNIX)
    target_link_libraries(
        ${PROJECT_NAME}
        OpenGL::GL
        ${GLEW_LIBRARIES}
        glfw
    )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

