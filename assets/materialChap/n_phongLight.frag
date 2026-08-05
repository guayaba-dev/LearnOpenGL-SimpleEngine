#version 330 core

uniform vec3 ambientColor;

uniform vec3 viewPos;
uniform float specularStrength;

#define MAX_LIGHTS 16
struct Light {
  vec4 position;
  vec4 color;
}; // vec4, no vec3 — ver nota abajo
layout(std140) uniform LightBlock {
  int numlights;
  //Padding for int
  Light lights[MAX_LIGHTS];
};

struct n_Light { // DATA SENDED TO THE GPU
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
};
layout(std140) uniform n_LightBlock {
  int n_numlights;
  //Padding for int
  n_Light n_lights[MAX_LIGHTS];
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

in vec3 ndc;
in vec3 fragNormal;

void main() {
  vec3 result = vec3(0.0);

  for (int i = 0; i < n_numlights; i++) {

    //ambient
    vec3 ambient = n_lights[i].ambient.xyz * material.ambient;

    //diffuse
    vec3 lightDir = normalize(n_lights[i].position.xyz - ndc);
    vec3 norm = normalize(fragNormal);
    float diff = max(dot(lightDir, norm), 0.1);
    vec3 diffuse = n_lights[i].diffuse.xyz * (diff * material.diffuse);

    //specular
    vec3 viewDir = normalize(viewPos - ndc);
    vec3 reflectLight = reflect(-lightDir, fragNormal);
    float spec = pow(max(dot(viewDir, reflectLight), 0.0), max(material.shininess, 0.1));

    vec3 specular = n_lights[i].specular.xyz * (spec * material.specular);

    result += (diffuse + specular + ambient);
  }

  gl_FragColor = vec4(result, 1.0);
}
