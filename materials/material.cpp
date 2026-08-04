#include "core/components.h"
#include "core/engine.h"
#include "core/materialHandellers.h"
#include "core/pch.hpp"
#include "gtc/type_ptr.hpp"
#include <iostream>
#include <memory>
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

struct n_PhongMaterial {
  glm::vec3 ambient = glm::vec3(1.0f);
  glm::vec3 diffuse = glm::vec3(1.0f);
  glm::vec3 specular = glm::vec3(1.0f);
  float shininess = 32.0;
};

class n_PhongBinder : public IBinder {

  bool checkBind(entt::entity entity, entt::registry &world) override {
    return defaultCheckBind<n_PhongMaterial>(entity, world);
  }

  void bind(entt::entity entity, entt::registry &world,
            unsigned int shaderID) override {

    auto &n_PhongMat = world.get<n_PhongMaterial>(entity);

    glUniform3fv(glGetUniformLocation(shaderID, "material.ambient"), 1,
                 glm::value_ptr(n_PhongMat.ambient));

    glUniform3fv(glGetUniformLocation(shaderID, "material.diffuse"), 1,
                 glm::value_ptr(n_PhongMat.diffuse));

    glUniform3fv(glGetUniformLocation(shaderID, "material.specular"), 1,
                 glm::value_ptr(n_PhongMat.specular));

    glUniform1f(glGetUniformLocation(shaderID, "material.shininess"),
                n_PhongMat.shininess);
  };
};

int main(int argc, char *argv[]) {

  Engine engine;

  MeshComponent cubeMesh;
  engine.meshMag.loadNormalMesh("cube", cubeVertex, vertexSize, cubeMesh);
  cubeMesh.vertexCount = vertexCount;

  engine.shaderMag.loadShader("basicShader", "assets/basic.vert",
                              "assets/basic.frag");

  engine.shaderMag.loadShader("n_PhongShader",
                              "assets/materialChap/n_phongLight.vert",
                              "assets/materialChap/n_phongLight.frag");
  // binders
  engine.materialManager->addMaterialBinder(std::make_unique<n_PhongBinder>());

  // entity definition
  auto &registry = engine.getWorld();

  auto n_PhongEntity = registry.create();
  registry.emplace<Shader>(n_PhongEntity,
                           engine.shaderMag.getShader("n_PhongShader"));
  registry.emplace<TransformComponent>(n_PhongEntity);
  registry.emplace<MeshComponent>(n_PhongEntity, cubeMesh);
  registry.emplace<n_PhongMaterial>(
      n_PhongEntity, n_PhongMaterial{.ambient = glm::vec3(1.0f, 0.5f, 0.31f),
                                     .diffuse = glm::vec3(1.0f, 0.5f, 0.31f),
                                     .specular = glm::vec3(0.5f, 0.5f, 0.5f)});
  auto lightEntity = registry.create();
  registry.emplace<Shader>(lightEntity,
                           engine.shaderMag.getShader("basicShader"));
  registry.emplace<TransformComponent>(
      lightEntity, TransformComponent{.position = glm::vec3(4.0, 2.0, 0.0)});
  registry.emplace<MeshComponent>(lightEntity, cubeMesh);
  registry.emplace<UnlitMaterial>(lightEntity);
  registry.emplace<LightComponent>(lightEntity);

  engine.start();
  engine.run();
  engine.shutDown();

  return 0;
}
