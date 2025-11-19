#version 330 core
out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Light {
    vec3 position;
    vec3 color;
    float ambient;
    float diffuse;
};

uniform Light light;
uniform sampler2D textureSampler;

uniform bool isSun;
uniform float sunRadius;
uniform float sunGlow;

void main() {
    vec3 textureColor = texture(textureSampler, TexCoords).rgb;

    if (isSun) {
        float distFromCenter = length(FragPos - light.position);

        // inside sun sphere → pure emission
        if (distFromCenter < sunRadius) {
            fragColor = vec4(textureColor * sunGlow, 1.0);
            return;
        }
    }

    vec3 norm = normalize(Normal);

    vec3 ambient = light.ambient * light.color;

    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * light.color;

    vec3 result = (ambient + diffuse) * textureColor;

    fragColor = vec4(result, 1.0);
}
