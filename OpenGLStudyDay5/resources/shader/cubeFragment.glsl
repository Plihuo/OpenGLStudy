#version 330 core
out vec4 fragColor;

in vec2 mTextCoord;

uniform sampler2D textureSky;

void main(){
    fragColor = texture(textureSky,mTextCoord);
}