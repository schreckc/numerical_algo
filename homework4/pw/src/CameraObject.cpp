#include "CameraObject.hpp"
#include <iostream>

CameraObject::CameraObject() : Camera() {
}
CameraObject::CameraObject(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical,
			 float sensibilite, float vitesse) :
  Camera(position, pointCible, axeVertical, sensibilite, vitesse) {
  setPointcible(pointCible);
}

CameraObject::~CameraObject() {

}

void CameraObject::orienterPerso(int xRel, int yRel) {
  // float phi = yRel * m_sensibilite * (float)0.1;
  // float theta = xRel * m_sensibilite * (float)0.1;
 
  // glm::vec4 orient(m_orientation.x, m_orientation.y, m_orientation.z, 1);
  // // glm::vec4 vert(m_axeVertical.x, m_axeVertical.y, m_axeVertical.z, 1);
 
  // glm::mat4 rotation = glm::rotate(theta, m_axeVertical);
  // orient = rotation * orient;

  // m_orientation = glm::vec3(orient.x, orient.y, orient.z);
  // m_orientation /= orient.w;
 
  // m_deplacementLateral = cross(m_axeVertical, m_orientation);
  // m_deplacementLateral = normalize(m_deplacementLateral);

  // rotation = glm::rotate(phi, m_deplacementLateral);
  // orient = rotation * orient;
  // //  vert = rotation * vert;

  // m_orientation = glm::vec3(orient.x, orient.y, orient.z);
  // m_orientation /= orient.w;

  // m_axeVertical = cross( m_orientation, m_deplacementLateral);
  // m_axeVertical = normalize(m_axeVertical); 

  // // Calcul du point ciblé pour OpenGL
  // m_pointCible = m_position + m_orientation;
}

void CameraObject::orienter(int xRel, int yRel) {
  float phi = yRel * m_sensibilite;
  float theta = -xRel * m_sensibilite;

  glm::vec4 pos(m_position.x, m_position.y, m_position.z, 1);
  glm::vec4 orient(m_orientation.x, m_orientation.y, m_orientation.z, 1);
  // glm::vec4 vert(m_axeVertical.x, m_axeVertical.y, m_axeVertical.z, 1);
 
  glm::mat4 rotation = glm::rotate(theta, m_axeVertical);
  pos = rotation * pos;
  orient = rotation * orient;

  m_orientation = glm::vec3(orient.x, orient.y, orient.z);
  m_orientation /= orient.w;
 
  m_deplacementLateral = cross(m_axeVertical, m_orientation);
  m_deplacementLateral = normalize(m_deplacementLateral);

  rotation = glm::rotate(phi, m_deplacementLateral);
  pos = rotation * pos;
  orient = rotation * orient;
  //  vert = rotation * vert;

  m_orientation = glm::vec3(orient.x, orient.y, orient.z);
  m_orientation /= orient.w;
  m_position = glm::vec3(pos.x, pos.y, pos.z);
  m_position /= pos.w;

  m_axeVertical = cross( m_orientation, m_deplacementLateral);
  m_axeVertical = normalize(m_axeVertical); 
  
  m_pointCible = m_position + m_orientation;
}

void CameraObject::deplacer(Input const &input) {
  if (input.getBoutonSouris(SDL_BUTTON_LEFT)) {
    if(input.mouvementSouris()) {
      // if(input.getTouche(SDL_SCANCODE_LSHIFT)) {
      // 	  std::cout << "orienter perso" << std::endl;
      // orienterPerso(input.getXRel(), input.getYRel());
      // } else {
      //	std::cout << "orienter pas perso" << std::endl;
	orienter(input.getXRel(), input.getYRel());
	// }
    }
  }
  if (input.getBoutonSouris(SDL_BUTTON_RIGHT)) {
    if(input.mouvementSouris()) {
      glm::vec3 depl = m_vitesse*input.getXRel()* m_deplacementLateral + m_vitesse*input.getYRel()*m_axeVertical;
      m_position += depl;
      m_pointCible += depl;
    }
  }
  if (input.getBoutonSouris(SDL_BUTTON_MIDDLE)) {
    alignAxis();
  }
  if (input.getWheel() != 0) {
    //std::cout << "MOUSE : WHEEL DOWN" << std::endl;
    glm::vec3 depl =  (float)10*m_vitesse*(float)input.getWheel()*m_orientation;
    m_position += depl;
    m_pointCible += depl;
  }
    if(input.getTouche(SDL_SCANCODE_UP)) {
      m_position = m_position + m_orientation * (float)10*m_vitesse;
      m_pointCible = m_position + m_orientation;
  }

  if(input.getTouche(SDL_SCANCODE_DOWN)) {
    m_position = m_position - m_orientation * (float)10*m_vitesse;
    m_pointCible = m_position + m_orientation;
  }

  if(input.getTouche(SDL_SCANCODE_LEFT)) {
    m_position = m_position + m_deplacementLateral * (float)10*m_vitesse;
    m_pointCible = m_position + m_orientation;
  }
  if(input.getBoutonSouris(SDL_BUTTON_MIDDLE)) {
    setPointcible(glm::vec3(0, 0, 0));
  }
  if(input.getTouche(SDL_SCANCODE_RIGHT)) {
    m_position = m_position - m_deplacementLateral * (float)10*m_vitesse;
    m_pointCible = m_position + m_orientation;
  }
}

void CameraObject::lookAt(glm::mat4 &modelview) {
  modelview = glm::lookAt(m_position, m_pointCible, m_axeVertical);
}

void CameraObject::setPointcible(glm::vec3 pointCible) {
  m_pointCible = pointCible;
  m_orientation = m_pointCible - m_position;
  m_orientation = normalize(m_orientation);
  
  m_deplacementLateral = cross(m_axeVertical, m_orientation);
  m_deplacementLateral = normalize(m_deplacementLateral);
  
  m_axeVertical = cross( m_orientation, m_deplacementLateral);
  m_axeVertical = normalize(m_axeVertical); 
}

void CameraObject::setPosition(glm::vec3 position) {

}

void CameraObject::alignAxis() {
  float c0 = dot(m_axeVertical, glm::vec3(0, 0, 1));
  float c1 = dot(m_axeVertical, glm::vec3(0, 1, 0));
  float c2 = dot(m_axeVertical, glm::vec3(1, 0, 0));
  
  float c;
  glm::vec3 axe;

  if (fabs(c0) < fabs(c1)) {
    if (fabs(c0) < fabs(c2)) {
      c = c0;
      axe = glm::vec3(0, 0, 1);
    } else {
      c = c2;
      axe = glm::vec3(1, 0, 0);
    }
  } else {
    if (fabs(c1) < fabs(c2)) {
         c = c1;
	 axe = glm::vec3(0, 1, 0);
    } else {
       c = c2;
       axe = glm::vec3(1, 0, 0);
    }
  }
  if (c < 0) {
    axe = -axe;
  }

  m_axeVertical = axe;
  
  m_deplacementLateral = cross(m_axeVertical, m_orientation);
  m_deplacementLateral = normalize(m_deplacementLateral);

  m_orientation = cross(m_deplacementLateral, m_axeVertical);
  m_orientation = normalize(m_orientation);
}
