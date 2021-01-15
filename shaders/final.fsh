#version 410
in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D theTexture;

void main() {
    gl_FragColor = vec4(1, 0, 0, 1);
}