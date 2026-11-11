attribute vec3 vertexPos;
attribute vec2 texCoord;

uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 view;

varying vec2 g_TexCoord;

void main(){
    g_TexCoord = texCoord;
    gl_Position = perspective * view * transform * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
