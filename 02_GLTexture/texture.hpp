#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image.h>

class Texture
{
public:
    unsigned int texture;
    int width, height, nrChannels;
    unsigned char* data;

    Texture(const char* tex_dir);

    void use();
};

#endif