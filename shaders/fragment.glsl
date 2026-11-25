#version 300 es

precision mediump float;

out vec4 FragColor;
in vec2 g_TexCoord;
in vec4 g_Color;

uniform sampler2D texture2D;

void main(){
    FragColor = texture(texture2D, g_TexCoord) * g_Color;
}
