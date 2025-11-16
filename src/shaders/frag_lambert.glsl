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
    vec3 norm = normalize(Normal);
    if (length(Normal) < 0.01) { // does normal make sense?
        // if not, show red
        fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        return;
    }
    
    vec3 ambient = light.ambient * light.color;
    
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * light.color;
    
    vec3 result = (ambient + diffuse) * objectColor;
    fragColor = vec4(result, 1.0);
}