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

in vec3 ndc;
in vec3 fragNormal;

vec3 norm = normalize(fragNormal);

void main() {
  vec3 result = vec3(0.0);

  for (int i = 0; i < numlights; i++) {
    vec3 lightDir = normalize(lights[i].position.xyz - ndc);
    vec3 viewDir = normalize(viewPos - ndc);
    vec3 reflectLight = reflect(-lightDir, fragNormal);

    float spec = pow(max(dot(viewDir, reflectLight), 0.0), max(specularStrength, 0.1));

    vec3 diffuse = max(dot(lightDir, fragNormal), 0.1) * lights[i].color.xyz;

    vec3 specular = lights[i].color.xyz * spec;

    result += (diffuse + specular);
  }

  result = ambientColor * result;

  gl_FragColor = vec4(result, 1.0);

  // Si ves algo raro o negro, el problema es de alineación/envío.
}
