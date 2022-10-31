#ifndef __TEXTURE_H__
#define __TEXTURE_H__


#include "stb_image.h"

#include <glad/glad.h>
#include <vector>
#include <string>

class Texture
{
public:
    static unsigned int LoadTextureFromFile(const char* path);
    static unsigned int LoadSkyBoxTexture(std::vector<std::string> pathVector);
};

#endif // !__TEXTURE_H__
