#version 150
in vec3 TexCoords;
out vec4 outColor;

uniform samplerCube skybox;

void main()
{    
    outColor = texture(skybox, TexCoords);
}
