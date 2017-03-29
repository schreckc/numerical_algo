#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/glm.hpp>
#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>

#endif

#include "Texture.hpp"
#include "Shader.hpp"

class Object {
  
protected:
  Texture* m_texture;
  Shader* m_shader;
  glm::mat4 m_model_view;

public:
Object(Shader* shader = NULL, Texture* texture = NULL);

  virtual void animate() = 0;
  virtual void draw( glm::mat4 m = glm::mat4(1.0f), Shader *s = NULL) = 0;

  Texture* getTexture() const;
  Shader* getShader() const;
  
  glm::mat4 modelView() const;
  glm::mat4 & modelView();

  void setMVP(glm::mat4 m, Shader *s); 
  
  void enableShader();
  void disableShader();
  
};

#endif
