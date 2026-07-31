#version 330 core

in vec3 colorOut;

void main() {
  gl_FragColor = vec4(colorOut, 1.0);
}
