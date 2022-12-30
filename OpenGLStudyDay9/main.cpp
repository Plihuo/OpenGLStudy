#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
#include "Shader.h"
#include "Texture.h"
#include "Utils.h"
#include "Camera.h"
#include "Model.h"

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
    stbi_set_flip_vertically_on_load(true);
    Shader modeShader("resources/shader/modeVertices.glsl","resources/shader/modeFragment.glsl");
    Model ourModel("resources/model/nanosuit.obj");
    glEnable(GL_DEPTH_TEST);
    while(!glfwWindowShouldClose(wwindow)){
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(wwindow);
        glClearColor(0.2F,0.3F,0.3F,0.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        modeShader.Use();
        glm::mat4 view(1);
        glm::mat4 projection(1);
        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        modeShader.SetMat4("view", view);
        modeShader.SetMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        modeShader.SetMat4("model", model);
        ourModel.Draw(modeShader);
        glfwSwapBuffers(wwindow);
        glfwPollEvents();
    }
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