#include <stdio.h>
#include <stdlib.h>

#include <glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include <common/shader.hpp>

int main( void )
{
  // Initialise GLFW
  if( !glfwInit() )
  {
    fprintf( stderr, "Failed to initialize GLFW\n" );
    getchar();
    return -1;
  }

    glfwWindowHint(GLFW_SAMPLES, 4); // сглаживание 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow( 1024, 768, "Task 1.1", NULL, NULL);
    if( window == NULL ){
      fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
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

    // Включаем режим отслеживания нажатия клавиш, чтобы проверять нажатие при отображении окна
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Белый фон
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    // создание и компиляция шейдерных программ 
    GLuint leftProgramID = LoadShaders("VertexShader.vertexshader", "FragmentShader1.fragmentshader");
    GLuint rightProgramID = LoadShaders("VertexShader.vertexshader", "FragmentShader2.fragmentshader");

    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

    // вершины треугольников
    static const GLfloat g_vertex_buffer_data[] = {
            0.f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,

            0.f, -0.7f, 0.0f,
            -0.5f, 0.3f, 0.0f,
            0.5f, 0.3f, 0.0f,
    };


	
    //Идентификатор буфера вершин
    GLuint vertexbuffer;
    // Создаем буффер
    glGenBuffers(1, &vertexbuffer);
    // Делаем его текущим
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Передаем информацию о вершинах 
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
  
  
    do{
      // очищаем экран 
      glClear( GL_COLOR_BUFFER_BIT );
      
      // Указываем, что первым буфером атрибутов будут вершины
      glEnableVertexAttribArray(vertexPosition_modelspaceID);
      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
      glVertexAttribPointer(
         vertexPosition_modelspaceID,              
         3,                  			// Размер
         GL_FLOAT,           			// Тип
         GL_FALSE,           			// Указывает, что значения не нормализованы
         0,                  			// Шаг
         (void*)0            			// Смещение массива в буфере
      );

      // устанавливаем шейдер
      glUseProgram(leftProgramID);

      // выводим треугольник (первые три точки)
      glDrawArrays(GL_TRIANGLES, 0, 3);
      

      // устанавливаем второй шейдер
      glUseProgram(rightProgramID);

      // выводим треугольник (вторая группа из трех точек)
      glDrawArrays(GL_TRIANGLES, 3, 6); 

      glDisableVertexAttribArray(vertexPosition_modelspaceID);
        
      // Сбрасываем буфферы
      glfwSwapBuffers(window);
      glfwPollEvents();

    } // проверка нажатия ESC или закрытия окна
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );
  
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteProgram(leftProgramID);
    glDeleteProgram(rightProgramID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
