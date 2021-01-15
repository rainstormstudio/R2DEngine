#version 410
in vec2 texCoord;
out vec4 gl_FragColor;
uniform sampler2D theTexture;

uniform float t;
uniform int innerWidth;
uniform int innerHeight;

float calcR(float n) {
  n += t;
  n /= innerWidth / 1.2;
  int x = int(round(t) + n);
  x += 255;
  x = x % 500;
  return (abs(255 - x)) / 255.0;
}

float calcG(float n) {
  n += t;
  n /= innerWidth / 1.2;
  int x = int(round(t) + n);
  x = x % 500;
  return (abs(255 - x)) / 255.0;
}

float calcB(float n) {
  n += t;
  n /= innerWidth / 1.2;
  int x = int(round(t) + n);
  x += 128;
  x = x % 500;
  return (abs(255 - x)) / 255.0;
}

void main() { 
    float x = texCoord.x * innerWidth;
    float y = texCoord.y * innerHeight;
    vec3 color = vec3(calcR(x * (innerHeight - y)), calcG(y * (innerWidth - x)), calcB(x * y));
    gl_FragColor = vec4(color, 1); 

}