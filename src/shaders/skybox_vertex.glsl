#version 330 core
layout (location = 0) in vec3 vertPos;

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoord;

void main()
{
    TexCoord = vertPos;
    vec4 pos = projection * view * vec4(vertPos, 1.0);
    gl_Position = pos.xyww;
}