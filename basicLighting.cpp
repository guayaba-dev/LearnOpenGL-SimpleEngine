#include "fwd.hpp"
#include <core/components.h>
#include <core/engine.h>

float cubeVertex[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

int vertexSize = sizeof(cubeVertex);
int vertexCount = vertexSize / (sizeof(float) * 6);

int main(int argc, char *argv[]) {

  Engine engine;

  MeshComponent cubeMesh;
  engine.meshMag.loadNormalMesh("CUBE_MESH", cubeVertex, vertexSize, cubeMesh);
  cubeMesh.vertexCount = vertexCount;
  engine.shaderMag.loadShader("PhongShader", "assets/phongLight.vert",
                              "assets/phongLight.frag");

  engine.shaderMag.loadShader("GouraudShader", "assets/gouraudLight.vert",
                              "assets/gouraudLight.frag");

  engine.shaderMag.loadShader("basicShader", "assets/basic.vert",
                              "assets/basic.frag");

  auto &registry = engine.getWorld();

  auto lightEntity = registry.create();
  registry.emplace<Shader>(lightEntity,
                           engine.shaderMag.getShader("basicShader"));
  registry.emplace<TransformComponent>(lightEntity);
  registry.emplace<MeshComponent>(lightEntity, cubeMesh);
  registry.emplace<UnlitMaterial>(lightEntity);
  registry.emplace<LightComponent>(lightEntity);

  auto phongCube = registry.create();
  registry.emplace<Shader>(phongCube,
                           engine.shaderMag.getShader("PhongShader"));
  registry.emplace<TransformComponent>(
      phongCube, TransformComponent{.position = glm::vec3(2, 0, 0)});
  registry.emplace<MeshComponent>(phongCube, cubeMesh);
  registry.emplace<UnlitMaterial>(phongCube);
  registry.emplace<PhongMaterial>(
      phongCube, PhongMaterial{.ambientColor = glm::vec3(0.6, 0.1, 0.2)});

  auto gouraudCube = registry.create();
  registry.emplace<Shader>(gouraudCube,
                           engine.shaderMag.getShader("GouraudShader"));
  registry.emplace<TransformComponent>(
      gouraudCube, TransformComponent{.position = glm::vec3(-2, 0, 0)});
  registry.emplace<PhongMaterial>(
      gouraudCube, PhongMaterial{.shininess = 2.0,
                                 .ambientColor = glm::vec3(0.6, 0.1, 0.2)});
  registry.emplace<MeshComponent>(gouraudCube, cubeMesh);

  engine.start();

  engine.run();

  engine.shutDown();

  return 0;
}
