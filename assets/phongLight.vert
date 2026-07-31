#version 330 core

layout (location = 0) in vec3 vPos; 
layout (location = 1) in vec3 vNormal;

uniform mat4 model;      
uniform mat4 view;       
uniform mat4 projection;


mat3 normalMat = mat3(transpose(inverse(model)));

out vec3 ndc;
out vec3 fragNormal;

void main(){
  gl_Position = projection * view * model * vec4(vPos, 1.0);
  ndc = (model * vec4(vPos, 1.0)).xyz;
  fragNormal = normalMat * vNormal;
}
