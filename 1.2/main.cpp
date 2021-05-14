#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

GLFWwindow* window;

int main() {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "HW 2", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }


    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  

    // Create and compile our GLSL program from the shaders
    GLuint leftProgramID = LoadShaders("VertexShader.vertexshader", "FragmentShader1.fragmentshader");
    GLuint rightProgramID = LoadShaders("VertexShader.vertexshader", "FragmentShader2.fragmentshader");
  
    GLuint MatrixID1 = glGetUniformLocation(leftProgramID, "MVP");
    GLuint MatrixID2 = glGetUniformLocation(rightProgramID, "MVP");
  
    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    
    static const GLfloat g_vertex_buffer_data[] = {
            0.f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,

            0.f, -0.7f, 0.0f,
            -0.5f, 0.3f, 0.0f,
            0.5f, 0.3f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    // переспектива
    glm::mat4 Projection = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);

    // Model matrix : переносит точку отсчета координат из фигуры во вне
    glm::mat4 Model = glm::mat4(1.f);
  
    glm::mat4 View;
    glm::mat4 MVP;

    const float radius = 4.0f;

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);
      
        float camera_x = sin(glfwGetTime()) * radius;
        float camera_y = sin(glfwGetTime()) * radius;
        float camera_z = cos(glfwGetTime()) * radius;
      
      
        glm::mat4 View = glm::lookAt(
                glm::vec3(camera_x, camera_y, camera_z), // координаты камеры
                glm::vec3(0, 0, 0), // смотрим в центр
                glm::vec3(0, 1, 0)  // вектор вверх
        );
      
        MVP = Projection * View * Model;

        // Use our shader
        glUseProgram(leftProgramID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID1, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                vertexPosition_modelspaceID,
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

        glUseProgram(rightProgramID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID2, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
              vertexPosition_modelspaceID,
              3,                  // size
              GL_FLOAT,           // type
              GL_FALSE,           // normalized?
              0,                  // stride
              (void *) 0            // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 3, 6);
      
        glDisableVertexAttribArray(vertexPosition_modelspaceID);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(leftProgramID);
    glDeleteProgram(rightProgramID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    

    return 0;
}
