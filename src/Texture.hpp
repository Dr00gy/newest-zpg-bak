#pragma once
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>

class Texture {
public:
    Texture(const std::string& path);
    Texture(const std::vector<std::string>& faces);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    GLuint getID() const { return textureID; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isCubemap() const { return cubemap; }

private:
    bool load(const std::string& path);
    bool loadCubemap(const std::vector<std::string>& faces);

    GLuint textureID;
    int width;
    int height;
    int channels;
    std::string filepath;
    bool cubemap;
};