#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Cube
{
private:
  GLuint vao;
  GLuint vbo;
public:
  GLuint load_shader(char *filepath, GLenum type);
  void draw();
  void init(float x);
};

