#version 300 es

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec4 color;
layout (location = 3) in float textureIndex;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 view;

out vec2 g_TexCoord;
out vec4 g_Color;
out float g_Texture;

void main(){
    g_TexCoord = texCoord;
    g_Color = color;
    g_Texture = textureIndex;
    gl_Position = perspective * view * transform * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
