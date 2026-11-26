#version 300 es

precision mediump float;
precision mediump sampler2DArray;

out vec4 FragColor;

in vec2 g_TexCoord;
in float g_Texture;
in vec4 g_Color;

uniform sampler2DArray textureSample;


void main(){
    FragColor = texture(textureSample, vec3(g_TexCoord, g_Texture)) * g_Color;
}
