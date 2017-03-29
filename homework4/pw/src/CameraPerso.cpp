#include "CameraPerso.hpp"


CameraPerso::CameraPerso() : Camera(), m_phi(0.0), m_theta(0.0) {

}

  CameraPerso::CameraPerso(glm::vec3 position, glm::vec3 pointCible, glm::vec3 axeVertical,
			     float sensibilite, float vitesse) :
    Camera(position, pointCible, axeVertical, sensibilite, vitesse), m_phi(0), m_theta(0) {
setPointcible(pointCible);
}

CameraPerso::~CameraPerso() {

}

void CameraPerso::orienter(int xRel, int yRel) {

  m_phi += -yRel * m_sensibilite;
  m_theta += -xRel * m_sensibilite;

  if(m_phi > 89.0) {
    m_phi = 89.0;
  }

  else if(m_phi < -89.0) {
    m_phi = -89.0;
  }

  // Conversion des angles en radian
  float phiRadian = m_phi * M_PI / 180;
  float thetaRadian = m_theta * M_PI / 180;

    
  // Calcul des coordonnées sphériques
  if(m_axeVertical.x == 1.0) {
    m_orientation.x = sin(phiRadian);
    m_orientation.y = cos(phiRadian) * cos(thetaRadian);
    m_orientation.z = cos(phiRadian) * sin(thetaRadian);
  } else if(m_axeVertical.y == 1.0) {
    m_orientation.x = cos(phiRadian) * sin(thetaRadian);
    m_orientation.y = sin(phiRadian);
    m_orientation.z = cos(phiRadian) * cos(thetaRadian);
  } else {
    m_orientation.x = cos(phiRadian) * cos(thetaRadian);
    m_orientation.y = cos(phiRadian) * sin(thetaRadian);
    m_orientation.z = sin(phiRadian);
  }

  m_deplacementLateral = cross(m_axeVertical, m_orientation);
  m_deplacementLateral = normalize(m_deplacementLateral);

  // Calcul du point ciblé pour OpenGL
  m_pointCible = m_position + m_orientation;
}

void CameraPerso::deplacer(Input const &input) {
  // Gestion de l'orientation
  if(input.mouvementSouris()) {
    orienter(input.getXRel(), input.getYRel());
  }

  if(input.getTouche(SDL_SCANCODE_UP)) {
    m_position = m_position + m_orientation * m_vitesse;
    m_pointCible = m_position + m_orientation;
  }

  if(input.getTouche(SDL_SCANCODE_DOWN)) {
    m_position = m_position - m_orientation * m_vitesse;
    m_pointCible = m_position + m_orientation;
  }

  if(input.getTouche(SDL_SCANCODE_LEFT)) {
    m_position = m_position + m_deplacementLateral * m_vitesse;
    m_pointCible = m_position + m_orientation;
  }

  if(input.getTouche(SDL_SCANCODE_RIGHT)) {
    m_position = m_position - m_deplacementLateral * m_vitesse;
    m_pointCible = m_position + m_orientation;
  }
}

void CameraPerso::lookAt(glm::mat4 &modelview) {
  modelview = glm::lookAt(m_position, m_pointCible, m_axeVertical);
}

void CameraPerso::setPointcible(glm::vec3 pointCible) {
m_pointCible = pointCible;
    m_orientation = m_pointCible - m_position;
    m_orientation = normalize(m_orientation);

    if(m_axeVertical.x == 1.0) {
        m_phi = asin(m_orientation.x);
        m_theta = acos(m_orientation.y / cos(m_phi));
if(m_orientation.y < 0) {
            m_theta *= -1;
}
    } else if(m_axeVertical.y == 1.0) {
        m_phi = asin(m_orientation.y);
        m_theta = acos(m_orientation.z / cos(m_phi));
if(m_orientation.z < 0) {
            m_theta *= -1;
}
    } else {
        m_phi = asin(m_orientation.x);
        m_theta = acos(m_orientation.z / cos(m_phi));
if(m_orientation.z < 0) {
            m_theta *= -1;
}
    }

    m_phi = m_phi * 180 / M_PI;
    m_theta = m_theta * 180 / M_PI;
}

 
void CameraPerso::setPosition(glm::vec3 position) {
  m_position = position;
  m_pointCible = m_position + m_orientation;
}
