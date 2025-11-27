#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
    float w = 500.0;
    
    vec4 worldPos = model * vec4(vertPos, 1.0);
    FragPos = worldPos.xyz / worldPos.w;
    
    Normal = mat3(transpose(inverse(model))) * vertNormal;
    
    gl_Position = projection * view * model * vec4(vertPos * w, w);
}