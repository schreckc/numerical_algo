#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object.hpp"

#include <glm/gtx/transform.hpp>

class Cube : public Object {

private:
  float size;
  const static float vertices[108];
  const static float normals[108];
  // float couleurs[108];
  const static float coordTexture[72];


public:
  Cube(float s = 1, Shader* shader = NULL, Texture* texture = NULL);

  void animate();
  void draw(glm::mat4 m = glm::mat4(1.0f), Shader *s = NULL);

};

#endif
