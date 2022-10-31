#version 330 core
out vec4 fragColor;
in vec3 mTextCoord;

uniform  samplerCube textureSkybox;

void main(){
    fragColor = texture(textureSkybox,mTextCoord);
    //fragColor = vec4(0.3,0.7,0.8,1.0);
}