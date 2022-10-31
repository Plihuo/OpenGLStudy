#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
using namespace std;
void framebuffer_window_size_callback(GLFWwindow* wwindow,int width,int height);
void processInput(GLFWwindow* wwindow);
const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 600;
float mixValue = 0;
int main() {
    Utils::registerSignal();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* wwindow = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"OpenGLStudyDay3", nullptr, nullptr);
    if(wwindow == nullptr){
        cout<<"Failed to create GLFW window"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(wwindow);
    glfwSetFramebufferSizeCallback(wwindow,framebuffer_window_size_callback);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        cout<<"Failed to initialize GLAD"<<endl;
        return -1;
    }
    Shader shader("resources/shader/vertexShader.glsl","resources/shader/fragmentShader.glsl");
    shader.Use();
    shader.SetInt("textureSky",0);
    shader.SetInt("textureMoon",1);
    float vertices[]{
        0.5f,0.5f,0.0f,           1.0f,0.0f,0.0f,   1.0f,0.0f,
        0.5f,-0.5f,0.0f,         0.0f,1.0f,0.0f,   1.0f,1.0f,
        -0.5f,-0.5f,0.0f,       0.0f,0.0f,1.0f,   0.0f,1.0f,
        -0.5f,0.5f,0.0f,        1.0f,1.0f,0.0f,   0.0f,0.0f,
    };
    unsigned  int indices[]{
        0,1,3,
        1,2,3
    };
    unsigned  int VBO,VAO,EBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8* sizeof(float ),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    Texture textureSky;
    unsigned int textureSky2D = textureSky.LoadTextureFromFile("resources/sky.jpeg");
    Texture textureMoon;
    unsigned int textureMoon2D = textureMoon.LoadTextureFromFile("resources/moon.jpeg");
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model,glm::radians(-45.0f),glm::vec3(1.0f,0.0f,0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view,glm::vec3(0.0f,0.0f,-1.0f));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f),(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1f,100.0f);
    while(!glfwWindowShouldClose(wwindow)){
        processInput(wwindow);
        glClearColor(0.2F,0.3F,0.3F,0.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureSky2D);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,textureMoon2D);
        shader.Use();
        shader.SetFloat("mixValue",mixValue*1.0f/100);
        shader.SetMat4("model",model);
        shader.SetMat4("view",view);
        shader.SetMat4("projection",projection);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glfwSwapBuffers(wwindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glfwTerminate();

    return 0;
}
void framebuffer_window_size_callback(GLFWwindow* wwindow,int width,int height){
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow* wwindow){
    if(glfwGetKey(wwindow,GLFW_KEY_SPACE)==GLFW_PRESS){
        glfwSetWindowShouldClose(wwindow, true);
    }else if(glfwGetKey(wwindow,GLFW_KEY_UP)==GLFW_PRESS){
        mixValue++;
        mixValue = min(mixValue,100.0f);
    }else if(glfwGetKey(wwindow,GLFW_KEY_DOWN)==GLFW_PRESS){
        mixValue--;
        mixValue = max(mixValue,0.0f);
    }
}