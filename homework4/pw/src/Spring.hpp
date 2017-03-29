#ifndef SPRING_HPP
#define SPRING_HPP

#include "Particule.hpp"


class Spring : public Object {
private:
  FLOAT l0;
  FLOAT k;
  Particule *p1, *p2;

public:
  Spring(Shader* shader = NULL);
  Spring(FLOAT l, FLOAT stiff, Particule *part1, Particule *part2, Shader* shader = NULL);

  void animate();
  void draw(glm::mat4 m = glm::mat4(1.0f), Shader *s = NULL);
  
  FLOAT getLength() const;
  void updateForces();

};

#endif
