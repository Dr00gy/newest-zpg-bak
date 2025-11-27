#version 330 core

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
    int type; // 0: POINT, 1: DIRECTIONAL, 2: REFLECTOR
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    float shininess;
    float ambient;
    float diffuse;
    float specular;
};

#define MAX_LIGHTS 10
uniform Light lights[MAX_LIGHTS];
uniform int numLights;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Material material;

in vec3 FragPos;
in vec3 Normal;

out vec4 fragColor;

vec3 calcPointLight(Light light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * light.color * material.ambient;
    vec3 diffuse = light.diffuse * diff * light.color * material.diffuse;
    vec3 specular = light.specular * spec * light.color * material.specular;
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcDirectionalLight(Light light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * light.color * material.ambient;
    vec3 diffuse = light.diffuse * diff * light.color * material.diffuse;
    vec3 specular = light.specular * spec * light.color * material.specular;
    
    return ambient + diffuse + specular;
}

vec3 calcReflLight(Light light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if (theta < light.outerCutOff) {
        return light.ambient * light.color * material.ambient * intensity;
    }
    
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * light.color * material.ambient;
    vec3 diffuse = light.diffuse * diff * light.color * material.diffuse;
    vec3 specular = light.specular * spec * light.color * material.specular;
    
    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    if (numLights == 0) {
        result = vec3(0.3) * objectColor * material.ambient;
        fragColor = vec4(result, 1.0);
        return;
    }
    
    for (int i = 0; i < numLights && i < MAX_LIGHTS; i++) {
        if (lights[i].type == 0) {
            result += calcPointLight(lights[i], norm, viewDir);
        } else if (lights[i].type == 1) {
            result += calcDirectionalLight(lights[i], norm, viewDir);
        } else if (lights[i].type == 2) {
            result += calcReflLight(lights[i], norm, viewDir);
        }
    }
    
    result *= objectColor;
    fragColor = vec4(result, 1.0);
}