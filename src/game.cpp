#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "cube.h"

using namespace std;


static GLuint load_shader(char *filepath, GLenum type);


int main()
{
  //init settings
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "CubeSwirl 2: Electric Boogalo", nullptr, nullptr); // Windowed

  glfwMakeContextCurrent(window);
  
  //init OpenGL and link functions in a dynamic way
  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  Cube cube;
  Cube cube2;
  cube.init(0.0f);
  cube2.init(1.0f);
  //main loop
  while(!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    //draw code
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube.draw();
    cube2.draw();
    glfwPollEvents();
  }
  
  glfwTerminate();

  return 0;
}

static GLuint load_shader(char *filepath, GLenum type)
{
  FILE *file = fopen(filepath, "rb");
  if(!file) {
    return 0;
  }
 
  long len;
  if(fseek(file, 0, SEEK_END) != 0 ||
     (len = ftell(file)) == -1L) {
    fclose(file);
    return 0;
  }
  rewind(file);
 
  char *buffer = (char *) malloc(len);
  if(fread(buffer, 1, len, file) != len) {
    fclose(file);
    free(buffer);
    return 0;
  }
  fclose(file);
 
  GLuint shader = glCreateShader(type);
  if(shader == 0) {
    free(buffer);
    return 0;
  }
 
  glShaderSource(shader, 1,(const char * const *) &buffer, (GLint *) &len);
  free(buffer);
  return shader;
}
