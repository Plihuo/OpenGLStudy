//
// Created by plihuo on 2022/12/28.
//

#ifndef OPENGLSTUDYDAY4_MODEL_H
#define OPENGLSTUDYDAY4_MODEL_H
#include "Mesh.h"
#include "string"
#include "assimp/scene.h"

using namespace std;
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
class Model
{
public:
    /*  函数   */
    Model(string const &path,bool gamma = false):gammaCorrection(gamma)
    {
        loadModel(path);
    }
    void Draw(Shader shader);
private:
    /*  模型数据  */
    vector<Mesh::Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    /*  函数   */
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         string typeName);
};


#endif //OPENGLSTUDYDAY4_MODEL_H
