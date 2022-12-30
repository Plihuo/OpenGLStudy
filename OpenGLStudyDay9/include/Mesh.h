//
// Created by plihuo on 2022/12/28.
//

#ifndef OPENGLSTUDYDAY4_MESH_H
#define OPENGLSTUDYDAY4_MESH_H


#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "string"
#include "vector"
#include "Shader.h"
#define MAX_BONE_INFLUENCE 4
using namespace std;
class Mesh {
public:
    struct Texture {
        unsigned int id;
        string type;
        string path;
    };
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };
    /*  网格数据  */
    vector<Mesh::Vertex> vertices;
    vector<unsigned int> indices;
    vector<Mesh::Texture> textures;
    /*  函数  */
    Mesh(vector<Mesh::Vertex> vertices, vector<unsigned int> indices, vector<Mesh::Texture> textures);

    void Draw(Shader shader);
private:
    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
    /*  函数  */
    void setupMesh();
};


#endif //OPENGLSTUDYDAY4_MESH_H
