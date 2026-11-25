#version 300 es

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 texCoord;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 view;

out vec2 g_TexCoord;

void main(){
    g_TexCoord = texCoord;
    gl_Position = perspective * view * transform * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
