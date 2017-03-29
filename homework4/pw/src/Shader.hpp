#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>

#endif

#include <SDL2/SDL.h>

class Shader {

private:
  std::string m_vertex_shader;
  std::string m_fragment_shader;
  
  GLuint m_id;
  
  bool load();

public:
  Shader();
  Shader(std::string vertex_file_path, std::string fragment_file_path);
  ~Shader();
  
  void enable();
  void disable();

  GLuint getProgramID() const;
};

#endif

  
