#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <vector>
#include <stdlib.h>

#include "Camera.h"

using namespace std;

class Skybox
{
private:
  GLuint vao;
  GLuint vbo;
  GLuint shaderProgram;
  GLuint textures;
  bool CompileSuccessful(int obj);
public:
  GLuint load_shader(char *filepath, GLenum type);
  GLuint loadCubemap(vector<const GLchar*> faces);
  void draw(Camera cam);
  void init();  
};
