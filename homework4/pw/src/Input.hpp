#ifndef INPUT_HPP
#define INPUT_HPP

#include <SDL2/SDL.h>

class Input {

public:

  Input();
  ~Input();

  void updateEvenements();  
  bool terminer() const;

  bool getTouche(const SDL_Scancode touche) const;
  bool getBoutonSouris(const Uint8 bouton) const;
  bool mouvementSouris() const;

  int getX() const;
  int getY() const;
  int getXRel() const;
  int getYRel() const;
  int getWheel() const;
  bool stopShift() const;

  void afficherPointeur(bool reponse) const;
  void capturerPointeur(bool reponse) const; //prevent pointer from going out of the windows
private:

    SDL_Event m_evenements;
    bool m_touches[SDL_NUM_SCANCODES];
    bool m_boutonsSouris[8];

    int m_x;
    int m_y;
    int m_xRel;
    int m_yRel;
  int m_wheel;
  bool m_stop_shift;
    bool m_terminer;

};


#endif
