#include "Texture.h"
#include "iostream"

using namespace std;
unsigned int Texture::LoadTextureFromFile(const char* path)
{
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    int width, height, nr_channels;
    unsigned char *data = stbi_load(path, &width, &height, &nr_channels, 0);
    if (data){
        GLenum format;
        if (nr_channels == 1){
            format = GL_RED;
        }
        else if (nr_channels == 3){
            format = GL_RGB;
        }
        else if (nr_channels == 4){
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture_id;
}
unsigned int Texture::LoadSkyBoxTexture(const std::vector<std::string> pathVector){
    unsigned int texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    int width,height,channels;

    for(int i = 0;i<pathVector.size();i++){
        unsigned char *data = stbi_load(pathVector[i].c_str(), &width, &height, &channels, 0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }else{
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture_id;
}