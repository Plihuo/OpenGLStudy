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

    Shader cubeShader("resources/shader/cubeVertices.glsl","resources/shader/cubeFragment.glsl");
    cubeShader.Use();
    cubeShader.SetInt("material.diffuse",0);
    cubeShader.SetInt("material.specular",1);
    Shader lightShader("resources/shader/lightVertices.glsl","resources/shader/lightFragment.glsl");

    unsigned  int VBO,cubeVAO;
    glGenVertexArrays(1,&cubeVAO);
    glBindVertexArray(cubeVAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Utils::CUBE_VERTICES_WITH_NORMAL),Utils::CUBE_VERTICES_WITH_NORMAL,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8* sizeof(float ), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8* sizeof(float ),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int diffuseMap = Texture::LoadTextureFromFile("resources/container2.png");
    unsigned int specularMap = Texture::LoadTextureFromFile("resources/container2_specular.png");

    unsigned  int lightVAO;
    glGenVertexArrays(1,&lightVAO);

    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(Utils::CUBE_VERTICES_WITH_NORMAL),Utils::CUBE_VERTICES_WITH_NORMAL,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8* sizeof(float ),nullptr);

    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(wwindow)){
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(wwindow);
        glClearColor(0.2F,0.3F,0.3F,0.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        cubeShader.Use();
        glm::mat4 model(1);
        glm::mat4 view(1);
        glm::mat4 projection(1);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // directional light
        cubeShader.SetVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeShader.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        cubeShader.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        cubeShader.SetVec3("pointLights[0].position",Utils::pointLightPositions[0]);
        cubeShader.SetVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[0].constant", 1.0f);
        cubeShader.SetFloat("pointLights[0].linear", 0.09f);
        cubeShader.SetFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        cubeShader.SetVec3("pointLights[1].position",Utils::pointLightPositions[1]);
        cubeShader.SetVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[1].constant", 1.0f);
        cubeShader.SetFloat("pointLights[1].linear", 0.09f);
        cubeShader.SetFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        cubeShader.SetVec3("pointLights[2].position",Utils::pointLightPositions[2]);
        cubeShader.SetVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[2].constant", 1.0f);
        cubeShader.SetFloat("pointLights[2].linear", 0.09f);
        cubeShader.SetFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        cubeShader.SetVec3("pointLights[3].position",Utils::pointLightPositions[3]);
        cubeShader.SetVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeShader.SetVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeShader.SetVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("pointLights[3].constant", 1.0f);
        cubeShader.SetFloat("pointLights[3].linear", 0.09f);
        cubeShader.SetFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        cubeShader.SetVec3("spotLight.position", camera.Position);
        cubeShader.SetVec3("spotLight.direction", camera.Forward);
        cubeShader.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeShader.SetVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubeShader.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeShader.SetFloat("spotLight.constant", 1.0f);
        cubeShader.SetFloat("spotLight.linear", 0.09f);
        cubeShader.SetFloat("spotLight.quadratic", 0.032f);
        cubeShader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        cubeShader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        cubeShader.SetMat4("view",view);
        cubeShader.SetMat4("projection",projection);
        cubeShader.SetMat4("model", model);
        cubeShader.SetVec3("viewPos", camera.Position);
        cubeShader.SetFloat("material.shininess", 32.0f);
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,specularMap);
        for(int i = 0;i < 10;i++){
            model = glm::mat4 (1.0f);
            model = glm::translate(model,Utils::CUBEPOSITION[i]);
            float angle = 20.0f * (float)i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        lightShader.Use();
        lightShader.SetMat4("view", view);
        lightShader.SetMat4("projection", projection);
        for(auto pointLightPosition:Utils::pointLightPositions){
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPosition);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightShader.SetMat4("model", model);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES,0,36);
        }

        glfwSwapBuffers(wwindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&cubeVAO);
    glDeleteVertexArrays(1,&lightVAO);
    glDeleteBuffers(1,&VBO);
    glDeleteTextures(1,&diffuseMap);
    glDeleteTextures(1,&specularMap);
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