#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

//PHONG MATERIAL
uniform vec3 ambientColor;
uniform float specularStrength;

//VIEW CONTEXT
uniform vec3 viewPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform buffer object
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

out vec3 colorOut;
vec4 fragCoord = vec4(0.0);
vec4 vertNormal = vec4(0.0);

void main() {
  mat4 normalMat = transpose(inverse(model));
  gl_Position = projection * view * model * vec4(vPos, 1.0);
  fragCoord = model * vec4(vPos, 1.0);
  vertNormal = normalize(normalMat * vec4(vNormal, 0.0));
  vec3 lightOut = vec3(0.0);

  for (int i = 0; i < numlights; i++) {

    //LIGHT CALC
    vec3 lightDir = normalize(lights[i].position.xyz - fragCoord.xyz);
    vec3 lightReflect = normalize(reflect(-lightDir, vertNormal.xyz));

    //VIEW
    vec3 viewDir = normalize(viewPos - fragCoord.xyz);

    float ambientStrenght = 0.2;

    float diffuseStrenght = max(dot(vertNormal.xyz, lightDir), 0.0);

    float reflectStrenght = pow(max(dot(lightReflect, viewDir), 0.0), max(specularStrength, 0.1));

    vec3 phongLight = (ambientStrenght + diffuseStrenght + reflectStrenght) * lights[i].color.xyz;

    lightOut += phongLight;
  }

  colorOut = lightOut * ambientColor;
}
