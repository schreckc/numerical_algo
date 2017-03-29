#include "Sphere.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cassert>

#include "Scene.hpp"

// uint Sphere::meridians_count = 4;
// uint Sphere::parallels_count = 1;
// uint Sphere::size_array = 18*Sphere::meridians_count + 18*Sphere::meridians_count*(Sphere::parallels_count-1);
 GLfloat* Sphere::vertices = new GLfloat[Sphere::size_array];
 GLfloat* Sphere::normals = new GLfloat[Sphere::size_array];
// GLfloat* Sphere::colors_sphere = new GLfloat[Sphere::size_array];

Sphere::Sphere(float s, Shader* shader): Object(shader) {
    m_size = s;
    // m_model_view = translate(m_model_view, glm::vec3(0.0, 5.0, 0.0));
    m_model_view = scale(m_model_view, glm::vec3(s, s, s));
    m_texture = NULL;
    // create_array();
    // for (uint i = 0; i < 18*meridians_count + 18*meridians_count* (parallels_count-1); i+= 3) {
    //   std::cout <<"("<<vertices_sphere[i]<<", "<<vertices_sphere[i+1]<<", "<<vertices_sphere[i+2]<<") "<< std::endl;
    // }
    setColor(1, 1, 1);
  }

 Sphere::~Sphere() {
   //  delete[] colors;
  }

void Sphere::setColor(float r, float g, float b) {
   cr = r;
   cg = g;
   cb = b;
  // colors = new GLfloat[size_array];
  // for (uint i = 0; i < size_array; i+= 3) {
  //   colors[i] = cr;
  //   colors[i+1] = cg;
  //   colors[i+2] = cb;
  //   //   std::cout <<"("<<vertices_sphere[i]<<", "<<vertices_sphere[i+1]<<", "<<vertices_sphere[i+2]<<") "<< std::endl;
  // }
}
  
  void Sphere::animate() {
  }

void Sphere::draw(glm::mat4 m, Shader *s) {
  GLfloat colors[size_array];
  for (uint i = 0; i < size_array; i+= 3) {
    colors[i] = cr;
    colors[i+1] = cg;
    colors[i+2] = cb;
    //    std::cout <<"("<<vertices[i]<<", "<<vertices[i+1]<<", "<<vertices[i+2]<<") "<< std::endl;
    // std::cout <<"n ("<<normals_sphere[i]<<", "<<normals_sphere[i+1]<<", "<<normals_sphere[i+2]<<") "<< std::endl;
  }
  //  std::cout<<"draw sph"<<std::endl;
   enableShader();
   setMVP(m, s);
  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, colors);
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, size_array/3);

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    disableShader();
  }

void Sphere::create_array() {
    GLfloat phi_prec = 0;
    GLfloat phi_step = M_PI / (parallels_count + 1);
    GLfloat theta_prec = 0;
    GLfloat theta_step = 2*M_PI / (meridians_count);

    GLfloat phi = phi_step;
    GLfloat theta = 0;
    uint index = 0;
 
    // colors_sphere = new GLfloat[18*meridians_count + 18*meridians_count* (parallels_count-1)];
    for (uint i = 0; i < meridians_count; ++i) {
      theta_prec = theta;
      theta += theta_step;
      create_vertex(theta_prec, phi, index);
      create_vertex(theta, phi, index);
      create_vertex(theta, phi_prec, index);
      //   std::cout<<"theta ;"<<theta<<std::endl;
    }
    for (uint j = 0; j < parallels_count-1; ++j) {
      phi_prec = phi;
      phi += phi_step;
      theta = 0;
      for (uint i = 0; i < meridians_count; ++i) {
	theta_prec = theta;
	theta += theta_step;
	create_vertex(theta, phi_prec, index);
	create_vertex(theta_prec, phi_prec, index);
	create_vertex(theta, phi, index);
	create_vertex(theta_prec, phi, index);
	create_vertex(theta, phi, index);
	create_vertex(theta_prec, phi_prec, index);
      }
    }
    phi_prec = phi;
    phi = M_PI;
    theta = 0;
    for (uint i = 0; i < meridians_count; ++i) {
      theta_prec = theta;
      theta += theta_step;
      create_vertex(theta, phi_prec, index);
      create_vertex(theta_prec, phi_prec, index);
      create_vertex(theta, phi, index);
    }
    std::cout<<index<<" "<<18*meridians_count + 18*meridians_count* (parallels_count-1)<<std::endl;
  }

void Sphere::create_vertex(float theta, float phi, uint &index) {
  vertices[index] = cos(theta)*sin(phi);
  vertices[index+1] = sin(theta)*sin(phi);
  vertices[index+2] = cos(phi);
  // float n =  cos(theta)*sin(phi)*cos(theta)*sin(phi) + sin(theta)*sin(phi)*sin(theta)*sin(phi) + cos(phi)*cos(phi);
  // n = sqrt(n);
  //  std::cout<<"n  "<<n<<std::endl;
  normals[index] = cos(theta)*sin(phi);
  normals[index+1] = sin(theta)*sin(phi);
  normals[index+2] = cos(phi);
  // normals_sphere[index] = 1;
  // normals_sphere[index+1] = 0;
  // normals_sphere[index+2] = 0;
  // colors_sphere[index] = 1;
  // colors_sphere[index+1] = 1;
  // colors_sphere[index+2] = 1;
  index += 3;
}

// void Sphere::delete_array() {

// }
