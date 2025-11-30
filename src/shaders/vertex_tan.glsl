#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNormal;
layout(location = 2) in vec2 vertTexCoords;
layout(location = 3) in vec3 vertTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out mat3 TBN;

void main() {
    float w = 500.0;

    vec4 worldPos = model * vec4(vertPos, 1.0);
    FragPos = worldPos.xyz / worldPos.w;
    
    mat3 normalMat = transpose(inverse(mat3(model)));
    
    // Gram-Schmidt orthonormalization
    vec3 _normal = normalize(vertNormal);
    vec3 _tangent = normalize(vertTangent);
    _tangent = normalize(_tangent - dot(_tangent, _normal) * _normal);
    vec3 _bitangent = cross(_normal, _tangent);
    
    // TBN Matrix
    vec3 T = normalize(normalMat * _tangent);
    vec3 B = normalize(normalMat * _bitangent);
    vec3 N = normalize(normalMat * _normal);
    TBN = mat3(T, B, N);
    
    Normal = N;  // fallbakc
    TexCoords = vertTexCoords;
    
    gl_Position = projection * view * model * vec4(vertPos * w, w);
}