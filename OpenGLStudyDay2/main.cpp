#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "stb_image.h"
using namespace std;
void framebuffer_window_size_callback(GLFWwindow* wwindow,int width,int height);
void processInput(GLFWwindow* wwindow);
const unsigned int WINDOW_WIDTH = 600;
const unsigned int WINDOW_HEIGHT = 600;
const char* verticalShaderSource="#version 330 core\n"
                                 "layout (location=0) in vec3 position;\n"
                                 "layout (location=1) in vec3 color;\n"
                                 "layout (location=2) in vec2 mTextCoord;\n"
                                 "out vec3 ourColor;\n"
                                 "out vec2 textCoord;\n"
                                 "void main(){\n"
                                 "    gl_Position=vec4(position,1.0);\n"
                                 "    ourColor = color;\n"
                                 "    textCoord = mTextCoord;\n"
                                 "}\n";
const char* fragmentShaderSOurce="#version 330 core\n"
                                 "out vec4 fragColor;\n"
                                 "in vec3 ourColor;\n"
                                 "in vec2 textCoord;\n"
                                 "uniform sampler2D ourTexture;\n"
                                 "void main(){\n"
                                 "    fragColor=texture(ourTexture,textCoord);\n"
                                 "}\n";
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* wwindow = glfwCreateWindow(WINDOW_WIDTH,WINDOW_HEIGHT,"OpenGLStudyDay2", nullptr, nullptr);
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
    int verticalShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(verticalShader,1,&verticalShaderSource, nullptr);
    glCompileShader(verticalShader);
    int success;
    char infoLog[512];
    glGetShaderiv(verticalShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(verticalShader,512, nullptr,infoLog);
        cout<<"Error:Compile vertical shader error:"<<infoLog<<endl;
    }
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSOurce, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragmentShader,512, nullptr,infoLog);
        cout<<"Error:Compile fragment shader error:"<<infoLog<<endl;
    }
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,verticalShader);
    glAttachShader(shaderProgram,fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(shaderProgram,512, nullptr,infoLog);
        cout<<"Error:Link program error:"<<infoLog<<endl;
    }
    glDeleteShader(verticalShader);
    glDeleteShader(fragmentShader);

    float vertices[]{
        0.5f,0.5f,0.0f,           1.0f,0.0f,0.0f,   1.0f,1.0f,
        0.5f,-0.5f,0.0f,         0.0f,1.0f,0.0f,   1.0f,0.0f,
        -0.5f,-0.5f,0.0f,       0.0f,0.0f,1.0f,   0.0f,0.0f,
        -0.5f,0.5f,0.0f,        1.0f,1.0f,0.0f,   0.0f,1.0f,
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

    unsigned int texture2D;
    glGenTextures(1,&texture2D);
    glBindTexture(GL_TEXTURE_2D,texture2D);
    glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    int width,height,channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned  char *data = stbi_load("/home/plihuo/CLionProjects/OpenGLStudyDay2/xingkong.jpeg",&width,&height,&channels,0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        cout<<"Failed to load texture"<<endl;
    }
    stbi_image_free(data);
    while(!glfwWindowShouldClose(wwindow)){
        processInput(wwindow);
        glClearColor(0.2F,0.3F,0.3F,0.0F);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D,texture2D);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glfwSwapBuffers(wwindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}
void framebuffer_window_size_callback(GLFWwindow* wwindow,int width,int height){
    glViewport(0,0,width,height);
}
void processInput(GLFWwindow* wwindow){
    if(glfwGetKey(wwindow,GLFW_KEY_SPACE)==GLFW_PRESS){
        glfwSetWindowShouldClose(wwindow, true);
    }
}