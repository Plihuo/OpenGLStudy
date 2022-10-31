#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
#include "Camera.h"

using namespace std;
void framebuffer_window_size_callback(GLFWwindow* wwindow,int width,int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* wwindow);
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera camera(glm::vec3(1.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int main() {
    Utils::registerSignal();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* wwindow = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"OpenGLStudyDay3", nullptr, nullptr);
    if(wwindow == nullptr){
        cout<<"Failed to create GLFW window"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(wwindow);
    glfwSetFramebufferSizeCallback(wwindow,framebuffer_window_size_callback);
    glfwSetCursorPosCallback(wwindow, mouse_callback);
    glfwSetScrollCallback(wwindow, scroll_callback);
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        cout<<"Failed to initialize GLAD"<<endl;
        return -1;
    }
    Utils utils;
    Shader cubeShader("resources/shader/cubeVertices.glsl","resources/shader/cubeFragment.glsl");
    cubeShader.Use();
    cubeShader.SetInt("textureSky",GL_TEXTURE0);

    Shader skyBoxShader("resources/shader/skyboxVertices.glsl","resources/shader/skyboxFragment.glsl");
    skyBoxShader.Use();
    skyBoxShader.SetInt("textureSkybox",GL_TEXTURE0);

    unsigned  int cubeVBO,cubeVAO;
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1,&cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Utils::CUBE_VERTICES),Utils::CUBE_VERTICES,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5* sizeof(float ),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5* sizeof(float ),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int textureSky2D = Texture::LoadTextureFromFile("resources/sky.jpeg");
    unsigned int textureMoon2D = Texture::LoadTextureFromFile("resources/moon.jpeg");


    unsigned  int skyBoxVBO,skyBoxVAO;
    glGenVertexArrays(1,&skyBoxVAO);
    glGenBuffers(1,&skyBoxVBO);

    glBindVertexArray(skyBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER,skyBoxVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Utils::SKYBOX_VERTICES),&Utils::SKYBOX_VERTICES,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3* sizeof(float ),(void*)0);


    unsigned int textureSkyBox = Texture::LoadSkyBoxTexture(utils.SKYBOX_TEXTURE);
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(wwindow)){
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(wwindow);
        glClearColor(0.2F,0.3F,0.3F,0.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model(1);
        glm::mat4 view(1);
        glm::mat4 projection(1);
        for(int i = 0;i<10;i++){
            model = glm::translate(model,Utils::CUBEPOSITION[i]);
            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,textureSky2D);
            cubeShader.Use();
            cubeShader.SetMat4("model",model);
            cubeShader.SetMat4("view",view);
            cubeShader.SetMat4("projection",projection);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES,0,36);
            glBindVertexArray(0);
        }

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        skyBoxShader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyBoxShader.SetMat4("view", view);
        skyBoxShader.SetMat4("projection", projection);

        glBindVertexArray(skyBoxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP,textureSkyBox);
        glDrawArrays(GL_TRIANGLES,0,36);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(wwindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&cubeVAO);
    glDeleteBuffers(1,&cubeVBO);
    glDeleteVertexArrays(1,&skyBoxVAO);
    glDeleteBuffers(1,&skyBoxVBO);
    glDeleteTextures(1,&textureSkyBox);
    glDeleteTextures(1,&textureSky2D);
    glDeleteTextures(1,&textureMoon2D);
    glfwTerminate();

    return 0;
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
        glDisable(GL_BLEND);
    }
}

void framebuffer_window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}