#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureSampler;
uniform bool useTexture;

void main()
{
    if (useTexture) {
        FragColor = texture(textureSampler, TexCoord);
    } else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}