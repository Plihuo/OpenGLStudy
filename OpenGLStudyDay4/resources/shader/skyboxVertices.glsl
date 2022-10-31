#version 330 core
layout (location=0) in vec3 aPosition;
uniform mat4 view;
uniform mat4 projection;
out vec3 mTextCoord;
void main(){
    mTextCoord = aPosition;
    vec4 position = projection * view  * vec4(aPosition,1.0);
    gl_Position = position.xyww;
}