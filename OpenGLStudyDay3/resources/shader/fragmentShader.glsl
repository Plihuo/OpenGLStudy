#version 330 core
out vec4 fragColor;
in vec3 ourColor;
in vec2 textCoord;
uniform float mixValue = 1.0f;

uniform sampler2D textureSky;
uniform sampler2D textureMoon;
void main(){
    fragColor=mix(texture(textureSky,textCoord),texture(textureMoon,textCoord),mixValue);
}