#ifndef PARTICULE_HPP
#define PARTICULE_HPP

#include "Object.hpp"

#include "mpm_conf.hpp"
#include "Eigen/Dense"
#include "Eigen/Core"

using namespace Eigen;

class Particule : public Object {

private :
  
  VEC3 pos0; //init position
  VEC3 pos; //position
  VEC3 vel; //velocity
  VEC3 forces;

  FLOAT m; //masse

  
public:
  Particule(Shader* shader = NULL);
  Particule(FLOAT mass, VEC3 p, VEC3 vel = VEC3(0, 0, 0), Shader* shader = NULL);

    
  void animate();
  void draw(glm::mat4 m = glm::mat4(1.0f), Shader *s = NULL);


  VEC3 getPosition() const;
  void setPosition(VEC3 p);
  VEC3 getVelocity() const;
  void setVelocity(VEC3 velo);

  FLOAT getMass() const;

  void oneStep();
  void addForce(VEC3 f);
};


#endif
