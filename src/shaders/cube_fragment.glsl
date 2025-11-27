#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D textureSampler;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        FragColor = texture(textureSampler, TexCoords);
    } else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}