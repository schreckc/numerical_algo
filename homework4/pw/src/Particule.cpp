#include "Particule.hpp"

#include "Sphere.hpp"
#include "Scene.hpp"
#include "error.hpp"
#include "mpm_conf.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Particule::Particule(Shader* shader) : Object(shader) {
  pos = VEC3(0, 0, 0);
}

Particule::Particule(FLOAT mass, VEC3 p, VEC3 velo, Shader* shader) :
  Object(shader), m(mass), pos(p), vel(velo) {
}



void Particule::animate() {
  oneStep();
}

void Particule::draw(glm::mat4 m, Shader *s) {
  /* draw spheres */
  m_model_view = translate(glm::mat4(1.0f), glm::vec3(pos(0), pos(1), pos(2)));
  Sphere sp(0.005, m_shader);
  Shader *cur_shader = m_shader;
  if (m_shader == NULL) {
    cur_shader = s;
  }
  glm::mat4 cur_model = m * m_model_view;
  sp.draw(cur_model, cur_shader);
  
  // /* draw points */
  GLfloat vertex[3] = {0, 0, 0};
  GLfloat color[3] = {0, 0, 0};
  GLfloat normal[3] = {1, 0, 0};
  glLineWidth(5.0f);

  enableShader();
  setMVP(m, s);
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertex);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, color);
  glEnableVertexAttribArray(1);

  glDrawArrays(GL_POINTS, 0, 1);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
 
  disableShader();
}

VEC3 Particule::getPosition() const {
  return pos;
}

void Particule::setPosition(VEC3 p) {
  pos = p;
}

VEC3 Particule::getVelocity() const {
  return vel;
}

void Particule::setVelocity(VEC3 velo) {
  vel = velo;
}

FLOAT Particule::getMass() const {
  return m;
}


void Particule::oneStep() {
  FLOAT dt = 0.0001;
  FLOAT damping = 0.001;
  FLOAT gravity = 9.81;
  if (m != 0) {
    forces += m*VEC3(0, 0, -gravity) - damping*vel;
  pos += dt*vel;
  vel += dt*forces/m;

  forces = VEC3(0, 0, 0);
  }
}

void Particule::addForce(VEC3 f) {
  forces += f;
}
