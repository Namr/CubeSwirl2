#include "skybox.h"

GLfloat Skyvertices[] = {
    // Positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void Skybox::init()
{
  
    //a place to store all the shader information
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  //varibles and pre loop setup
  
  glGenBuffers(1, &vbo); // Generate 1 buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo); // make that buffer active
  glBufferData(GL_ARRAY_BUFFER, sizeof(Skyvertices), Skyvertices, GL_STATIC_DRAW); //upload our vertices to the buffer

   
  //init shaders for this triangle
  GLuint vertexShader = load_shader("shaders/skyVert.glsl",GL_VERTEX_SHADER);
  GLuint fragmentShader = load_shader("shaders/skyFrag.glsl",GL_FRAGMENT_SHADER);
  
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);

  printf("Vertex Status: %s\n",CompileSuccessful(vertexShader) ? "true" : "false");
  printf("Fragment Status: %s\n",CompileSuccessful(fragmentShader) ? "true" : "false");
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

    //inform the shader programs how the vertices are formatted
  //(2 values per position,floating point values,no space between position values,
  //and 0 offset because there are no other attribs)
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  
  vector<const GLchar*> faces;
  faces.push_back("right.jpg");
  faces.push_back("left.jpg");
  faces.push_back("top.jpg");
  faces.push_back("bottom.jpg");
  faces.push_back("back.jpg");
  faces.push_back("front.jpg");
  
  textures = loadCubemap(faces);
  
}

void Skybox::draw(Camera cam)
{
  glDepthFunc(GL_LEQUAL); 
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBindVertexArray(vao);
  glUseProgram(shaderProgram);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_CUBE_MAP,textures);
  glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 3);  
  
  glm::mat4 view = glm::mat4(glm::mat3(cam.view));
    
  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(cam.proj));

  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_LEQUAL); 
}

GLuint Skybox::load_shader(char *filepath, GLenum type)
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


GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE3);

    int width,height;
    unsigned char* image;
	
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}  

bool Skybox::CompileSuccessful(int obj)
{
  int status;
  glGetShaderiv(obj, GL_COMPILE_STATUS, &status);
  return status == GL_TRUE;
}
