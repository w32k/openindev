precision mediump float;
varying vec2 g_TexCoord;
uniform sampler2D texture;

void main(){
    gl_FragColor = texture2D(texture, g_TexCoord);
}
