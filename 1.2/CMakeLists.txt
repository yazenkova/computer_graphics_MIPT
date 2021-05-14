cmake_minimum_required(VERSION 3.15)
project(OpenGL)

set(CMAKE_CXX_STANDARD 14)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

# Add header file
set(GLEW_H /Users/yazenkova/Desktop/compgraph/tutorials/external/glew-1.13.0/include/GL)
set(GLFW_H /Users/yazenkova/Desktop/compgraph/tutorials/external/glew-1.13.0/include/GLFW)
set(GLM /Users/yazenkova/Desktop/compgraph/tutorials/external/glew-1.13.0/include/glm) # glm
include_directories(${GLEW_H} ${GLFW_H} ${GLM})


# Add target link
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.4//lib/libglfw.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

# Execute compilation command
set(SOURCE_FILES main.cpp)
add_executable(OpenGL main.cpp)

if (APPLE)
    target_link_libraries(OpenGL "-framework OpenGL")
    target_link_libraries(OpenGL "-framework GLUT")
endif()