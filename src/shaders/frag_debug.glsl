#version 330 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;

struct Light {
    vec3 position;
    vec3 color;
    float ambient;
    float diffuse;
};

uniform Light light;
uniform vec3 objectColor;

void main() {
    // green means the shader is working
    // fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    
    vec3 ambient = light.ambient * light.color;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * light.color;
    vec3 result = max(ambient + diffuse, vec3(0.1)) * objectColor;
    
    fragColor = vec4(result, 1.0);
}