#version 330 core

struct Lightmaterial {
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};

uniform Lightmaterial material;

void main() {
  gl_FragColor = vec4(material.diffuse.xyz, 1.0);
}
