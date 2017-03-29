#include "Spring.hpp"
#include "error.hpp"

Spring::Spring(Shader* shader) : Object(shader), l0(0) {

}

Spring::Spring(FLOAT l, FLOAT stiff, Particule *part1, Particule *part2, Shader* shader) :
  Object(shader), l0(l), k(stiff), p1(part1), p2(part2) {

}

void Spring::animate() {}

void Spring::draw(glm::mat4 m, Shader *s) {
  VEC3 pos1 = p1->getPosition();
  VEC3 pos2 = p2->getPosition();

  GLfloat vert[6];
  for (uint l = 0; l < 3; ++l) {
    vert[l] = pos1(l);
    vert[l+3] = pos2(l);
  }
  GLfloat col[6];
  for (uint l = 0; l < 6; l+=3) {
    col[l] = 0;
    col[l+1] = 0;
    col[l+2] = 0;
  }
 glLineWidth(5.0f);
  
  enableShader();
  setMVP(m, s);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vert);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, col);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_LINES, 0, 2);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  disableShader();
}
  
FLOAT Spring::getLength() const {
 VEC3 pos1 = p1->getPosition();
 VEC3 pos2 = p2->getPosition();

 return (pos1-pos2).norm();
}


void Spring::updateForces() {
  TEST(l0 != 0);
  VEC3 pos1 = p1->getPosition();
  VEC3 pos2 = p2->getPosition();
 
  VEC3 dir = pos2 - pos1;
  FLOAT l = dir.norm();
  dir /= l;

  VEC3 force = k*(l - l0)/l0 * dir;

  // INFO(3, l<<" "<<l0);
  // INFO(3, force);

  p1->addForce(force);
  p2->addForce(-force);
}
