attribute vec4 vertexPos;
uniform mat4 transform;
uniform mat4 perspective;
uniform mat4 view;

void main(){
    gl_Position = perspective * view * transform * vec4(vertexPos.x, vertexPos.y, vertexPos.z, 1.0);
}
