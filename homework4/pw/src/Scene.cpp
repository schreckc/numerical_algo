#include "Scene.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "error.hpp"
#include "Times.hpp"
#include "mpm_conf.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <thread>


using namespace glm;

Scene* Scene::SCENE(new Scene("test", 2400, 1800));

void Scene::animateScene() {
  SCENE->animationLoop();
}

Scene::Scene() {
}

Scene::Scene(std::string titreFenetre, int largeurFenetre, int hauteurFenetre) :
  m_titreFenetre(titreFenetre), m_largeurFenetre(largeurFenetre),
  m_hauteurFenetre(hauteurFenetre), m_fenetre(0), m_contexteOpenGL(0), m_input() {
}

Scene::~Scene() {
   std::list<Object*>::iterator it;
  for (it = l_objects.begin(); it != l_objects.end(); ++it) {
    delete (*it);
  }
   std::vector<Shader*>::iterator its;
  for (its = l_shaders.begin(); its != l_shaders.end(); ++its) {
    delete (*its);
  }
  std::vector<Texture*>::iterator itt;
  for (itt = l_textures.begin(); itt != l_textures.end(); ++itt) {
    delete (*itt);
  }
  
  l_objects.clear();
  l_shaders.clear();
  l_textures.clear();

    for (auto &s : springs) {
      delete s;
    }
     for (auto& p : particules) {
       delete p;
     }
  
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}

bool Scene::initialiserFenetre() {
    // Initialisation de la SDL
	
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
	
    // Version d'OpenGL
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
    // Double Buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
    // Création de la fenêtre
    m_fenetre = SDL_CreateWindow(m_titreFenetre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_largeurFenetre, m_hauteurFenetre, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (m_fenetre == 0) {
        std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // Création du contexte OpenGL
    m_contexteOpenGL = SDL_GL_CreateContext(m_fenetre);
    if (m_contexteOpenGL == 0) {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_fenetre);
        SDL_Quit();
        return false;
    }

    return true;
}

bool Scene::initGL() {
  srand (time(NULL));
#ifdef WIN32
  GLenum initialisationGLEW( glewInit() );
  if(initialisationGLEW != GLEW_OK) {
    std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;
    SDL_GL_DeleteContext(m_contexteOpenGL);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
    return false;
  }
#endif
  glEnable( GL_DEPTH_TEST );
  glDepthFunc(GL_LESS);
  return true;
}

void Scene::init() {
  Sphere::create_array();
  
  l_shaders = std::vector<Shader*>(3);
  l_shaders[0] = new Shader("shaders/simple.vert", "shaders/simple.frag");
  l_shaders[1] = new Shader("shaders/texture.vert", "shaders/texture.frag");
  l_shaders[2] = new Shader("shaders/point.vert", "shaders/point.frag");

  l_textures = std::vector<Texture*>(1);
  l_textures[0] = new Texture("Textures/boulet.jpg");
  l_textures[0]->charger();
  
     
    // // Camera matrix
    // glm::mat4 view = glm::lookAt(glm::vec3(-10, -10, 0), // Camera is at (4,3,3), in World Space
    // 				 glm::vec3(0, 0, 0), // and looks at the origin
    // 				 glm::vec3(0, 0, 1)  // Head is up (set to 0,-1,0 to look upside-down)
  //);
  m_camera = CameraObject(vec3(1, 0, -0.2), vec3(0.0, 0.0, -0.2), vec3(0, 0, 1), 0.005, 0.005);
    // m_input.afficherPointeur(false);
    // m_input.capturerPointeur(true);
    
    // // Model matrix : an identity matrix (model will be at the origin)
  //  m_model = glm::mat4(1.0f);
  m_projection = glm::perspective(glm::radians(45.0f), (float) m_largeurFenetre/ (float) m_hauteurFenetre, 0.1f, 100.0f);
    // // Our ModelViewProjection : multiplication of our 3 matrices
    // glm::mat4 mvp = projection * view * model; 
  m_frameRate = 1000 / 50;
  m_debutBoucle = 0;
  m_finBoucle = 0;
  m_tempsEcoule = 0;

  m_input_rate = 1000 / 10;
  m_input_loop0 = 0;
  m_input_loop1 = 0;
  m_input_loop_time = 0;

  running = false;
  step_by_step = 0;
  end_ = false;
  re_init = false;
  back = 0;

  t = 0;
  
  Times::TIMES->init();

  /*
   Particule *p0 = new Particule(0.0, VEC3(0, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p1 = new Particule(0.01, VEC3(0, 0.1, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p2 = new Particule(0.01, VEC3(0, 0.2, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p3 = new Particule(0.0, VEC3(0, 0.3, -0.0), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p4 = new Particule(0.01, VEC3(0, 0, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p5 = new Particule(0.01, VEC3(0, 0.1, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p6 = new Particule(0.01, VEC3(0, 0.2, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p7 = new Particule(0.01, VEC3(0, 0.3, -0.1), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p8 = new Particule(0.01, VEC3(0, 0, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p9 = new Particule(0.01, VEC3(0, 0.1, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p10 = new Particule(0.01, VEC3(0, 0.2, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p11 = new Particule(0.01, VEC3(0, 0.3, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  
  Spring *s1 = new Spring(0.1, 1, p0, p1, l_shaders[2]);
  Spring *s2 = new Spring(0.1, 1, p1, p2, l_shaders[2]);
  Spring *s3 = new Spring(0.1, 1, p2, p3, l_shaders[2]);
  
  Spring *s4 = new Spring(0.1, 1, p4, p5, l_shaders[2]);
  Spring *s5 = new Spring(0.1, 1, p5, p6, l_shaders[2]);
  Spring *s6 = new Spring(0.1, 1, p6, p7, l_shaders[2]);

  Spring *s7 = new Spring(0.1, 1, p8, p9, l_shaders[2]);
  Spring *s8 = new Spring(0.1, 1, p9, p10, l_shaders[2]);
  Spring *s9 = new Spring(0.1, 1, p10, p11, l_shaders[2]);

  Spring *s10 = new Spring(0.1, 1, p0, p4, l_shaders[2]);
  Spring *s11 = new Spring(0.1, 1, p1, p5, l_shaders[2]);
  Spring *s12 = new Spring(0.1, 1, p2, p6, l_shaders[2]);
  Spring *s13 = new Spring(0.1, 1, p3, p7, l_shaders[2]);
  
  Spring *s14 = new Spring(0.1, 1, p8, p4, l_shaders[2]);
  Spring *s15 = new Spring(0.1, 1, p9, p5, l_shaders[2]);
  Spring *s16 = new Spring(0.1, 1, p10, p6, l_shaders[2]);
  Spring *s17 = new Spring(0.1, 1, p11, p7, l_shaders[2]);

  particules.push_back(p0);
  particules.push_back(p1);
  particules.push_back(p2);
  particules.push_back(p3);
  particules.push_back(p4);
  particules.push_back(p5);
  particules.push_back(p6);
  particules.push_back(p7);
  particules.push_back(p8);
  particules.push_back(p9);
  particules.push_back(p10);
  particules.push_back(p11);
  
  springs.push_back(s1);
  springs.push_back(s2);
  springs.push_back(s3);
  springs.push_back(s4);
  springs.push_back(s5);
  springs.push_back(s6);
  springs.push_back(s7);
  springs.push_back(s8);
  springs.push_back(s9);
  springs.push_back(s10);
  springs.push_back(s11);
  springs.push_back(s12);
  springs.push_back(s13);
  springs.push_back(s14);
  springs.push_back(s15);
  springs.push_back(s16);
  springs.push_back(s17);

   Particule *p12 = new Particule(0.0, VEC3(0.1, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
   Particule *p13 = new Particule(0.01, VEC3(0.1, 0.1, -0.0), VEC3(0, 0, 0), l_shaders[0]);
   Particule *p14 = new Particule(0.01, VEC3(0.1, 0.2, -0.0), VEC3(0, 0, 0), l_shaders[0]);
   Particule *p15 = new Particule(0.0, VEC3(0.1, 0.3, -0.0), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p16 = new Particule(0.01, VEC3(0.1, 0, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p17 = new Particule(0.01, VEC3(0.1, 0.1, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p18 = new Particule(0.01, VEC3(0.1, 0.2, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p19 = new Particule(0.01, VEC3(0.1, 0.3, -0.1), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p20 = new Particule(0.01, VEC3(0.1, 0, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p21 = new Particule(0.01, VEC3(0.1, 0.1, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p22 = new Particule(0.01, VEC3(0.1, 0.2, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p23 = new Particule(0.01, VEC3(0.1, 0.3, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  
   Spring *s18 = new Spring(0.1, 1, p12, p13, l_shaders[2]);
   Spring *s19 = new Spring(0.1, 1, p13, p14, l_shaders[2]);
   Spring *s20 = new Spring(0.1, 1, p14, p15, l_shaders[2]);
  
  Spring *s21 = new Spring(0.1, 1, p16, p17, l_shaders[2]);
  Spring *s22 = new Spring(0.1, 1, p17, p18, l_shaders[2]);
  Spring *s23 = new Spring(0.1, 1, p18, p19, l_shaders[2]);

  Spring *s24 = new Spring(0.1, 1, p20, p21, l_shaders[2]);
  Spring *s25 = new Spring(0.1, 1, p21, p22, l_shaders[2]);
  Spring *s26 = new Spring(0.1, 1, p22, p23, l_shaders[2]);

  Spring *s27 = new Spring(0.1, 1, p12, p16, l_shaders[2]);
  Spring *s28 = new Spring(0.1, 1, p13, p17, l_shaders[2]);
  Spring *s29 = new Spring(0.1, 1, p14, p18, l_shaders[2]);
  Spring *s30 = new Spring(0.1, 1, p15, p19, l_shaders[2]);
  
  Spring *s31 = new Spring(0.1, 1, p16, p20, l_shaders[2]);
  Spring *s32 = new Spring(0.1, 1, p17, p21, l_shaders[2]);
  Spring *s33 = new Spring(0.1, 1, p18, p22, l_shaders[2]);
  Spring *s34 = new Spring(0.1, 1, p19, p23, l_shaders[2]);


   Spring *s35 = new Spring(0.1, 1, p0, p12, l_shaders[2]);
   Spring *s36 = new Spring(0.1, 1, p1, p13, l_shaders[2]);
   Spring *s37 = new Spring(0.1, 1, p2, p14, l_shaders[2]);
   Spring *s38 = new Spring(0.1, 1, p3, p15, l_shaders[2]);
  Spring *s39 = new Spring(0.1, 1, p4, p16, l_shaders[2]);
  Spring *s40 = new Spring(0.1, 1, p5, p17, l_shaders[2]);
  Spring *s41 = new Spring(0.1, 1, p6, p18, l_shaders[2]);
  Spring *s42 = new Spring(0.1, 1, p7, p19, l_shaders[2]);
  Spring *s43 = new Spring(0.1, 1, p8, p20, l_shaders[2]);
  Spring *s44 = new Spring(0.1, 1, p9, p21, l_shaders[2]);
  Spring *s45 = new Spring(0.1, 1, p10, p22, l_shaders[2]);
  Spring *s46 = new Spring(0.1, 1, p11, p23, l_shaders[2]);
  

  particules.push_back(p12);
  particules.push_back(p13);
  particules.push_back(p14);
  particules.push_back(p15);
  particules.push_back(p16);
  particules.push_back(p17);
  particules.push_back(p18);
  particules.push_back(p19);
  particules.push_back(p20);
  particules.push_back(p21);
  particules.push_back(p22);
  particules.push_back(p23);
  

  springs.push_back(s18);
  springs.push_back(s19);
  springs.push_back(s20);
  springs.push_back(s21);
  springs.push_back(s22);
  springs.push_back(s23);
  springs.push_back(s24);
  springs.push_back(s25);
  springs.push_back(s26);
  springs.push_back(s27);
  springs.push_back(s28);
  springs.push_back(s29);
  springs.push_back(s30);
  springs.push_back(s31);
  springs.push_back(s32);
  springs.push_back(s33);
  springs.push_back(s34);

  springs.push_back(s35);
  springs.push_back(s36);
  springs.push_back(s37);
  springs.push_back(s38);
  springs.push_back(s39);
  springs.push_back(s40);
  springs.push_back(s41);
  springs.push_back(s42);
  springs.push_back(s43);
  springs.push_back(s44);
  springs.push_back(s45);
  springs.push_back(s46);

  /**/

  
  Particule *p0 = new Particule(0.0, VEC3(0, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p1 = new Particule(0.01, VEC3(0, 0.1, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p2 = new Particule(0.01, VEC3(0, 0.2, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p3 = new Particule(0.0, VEC3(0, 0.3, -0.0), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p4 = new Particule(0.01, VEC3(0, 0, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p5 = new Particule(0.01, VEC3(0, 0.1, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p6 = new Particule(0.01, VEC3(0, 0.2, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p7 = new Particule(0.01, VEC3(0, 0.3, -0.1), VEC3(0, 0, 0), l_shaders[0]);

  Particule *p8 = new Particule(0.01, VEC3(0, 0, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p9 = new Particule(0.01, VEC3(0, 0.1, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p10 = new Particule(0.01, VEC3(0, 0.2, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p11 = new Particule(0.01, VEC3(0, 0.3, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  
  Spring *s1 = new Spring(0.1, 1, p0, p1, l_shaders[2]);
  Spring *s2 = new Spring(0.1, 1, p1, p2, l_shaders[2]);
  Spring *s3 = new Spring(0.1, 1, p2, p3, l_shaders[2]);
  
  Spring *s4 = new Spring(0.1, 1, p4, p5, l_shaders[2]);
  Spring *s5 = new Spring(0.1, 1, p5, p6, l_shaders[2]);
  Spring *s6 = new Spring(0.1, 1, p6, p7, l_shaders[2]);

  Spring *s7 = new Spring(0.1, 1, p8, p9, l_shaders[2]);
  Spring *s8 = new Spring(0.1, 1, p9, p10, l_shaders[2]);
  Spring *s9 = new Spring(0.1, 1, p10, p11, l_shaders[2]);

  Spring *s10 = new Spring(0.1, 1, p0, p4, l_shaders[2]);
  Spring *s11 = new Spring(0.1, 1, p1, p5, l_shaders[2]);
  Spring *s12 = new Spring(0.1, 1, p2, p6, l_shaders[2]);
  Spring *s13 = new Spring(0.1, 1, p3, p7, l_shaders[2]);
  
  Spring *s14 = new Spring(0.1, 1, p8, p4, l_shaders[2]);
  Spring *s15 = new Spring(0.1, 1, p9, p5, l_shaders[2]);
  Spring *s16 = new Spring(0.1, 1, p10, p6, l_shaders[2]);
  Spring *s17 = new Spring(0.1, 1, p11, p7, l_shaders[2]);

  particules.push_back(p0);
  particules.push_back(p1);
  particules.push_back(p2);
  particules.push_back(p3);
  particules.push_back(p4);
  particules.push_back(p5);
  particules.push_back(p6);
  particules.push_back(p7);
  particules.push_back(p8);
  particules.push_back(p9);
  particules.push_back(p10);
  particules.push_back(p11);
  
  springs.push_back(s1);
  springs.push_back(s2);
  springs.push_back(s3);
  springs.push_back(s4);
  springs.push_back(s5);
  springs.push_back(s6);
  springs.push_back(s7);
  springs.push_back(s8);
  springs.push_back(s9);
  springs.push_back(s10);
  springs.push_back(s11);
  springs.push_back(s12);
  springs.push_back(s13);
  springs.push_back(s14);
  springs.push_back(s15);
  springs.push_back(s16);
  springs.push_back(s17);
  /**/

  /*
  Particule *p0 = new Particule(0.0, VEC3(0, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p1 = new Particule(0.01, VEC3(0, 0.1, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p2 = new Particule(0.01, VEC3(0, 0.2, -0.0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p3 = new Particule(0.01, VEC3(0, 0.3, -0.0), VEC3(0, 0, 0), l_shaders[0]);

  Spring *s1 = new Spring(0.1, 1, p0, p1, l_shaders[2]);
  Spring *s2 = new Spring(0.1, 1, p1, p2, l_shaders[2]);
  Spring *s3 = new Spring(0.1, 1, p2, p3, l_shaders[2]);

  particules.push_back(p0);
  particules.push_back(p1);
  particules.push_back(p2);
  particules.push_back(p3);

  springs.push_back(s1);
  springs.push_back(s2);
  springs.push_back(s3);

  /**/

  /*
  Particule *p0 = new Particule(0.0, VEC3(0, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p1 = new Particule(0.01, VEC3(0, 0.0, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p2 = new Particule(0.01, VEC3(0, 0.0, -0.2), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p3 = new Particule(0.01, VEC3(0, 0.0, -0.3), VEC3(0, 0, 0), l_shaders[0]);

  Spring *s1 = new Spring(0.1, 1, p0, p1, l_shaders[2]);
  Spring *s2 = new Spring(0.1, 1, p1, p2, l_shaders[2]);
  Spring *s3 = new Spring(0.1, 1, p2, p3, l_shaders[2]);

  particules.push_back(p0);
  particules.push_back(p1);
  particules.push_back(p2);
  particules.push_back(p3);

  springs.push_back(s1);
  springs.push_back(s2);
  springs.push_back(s3);

  /**/

  /*
  Particule *p0 = new Particule(0.0, VEC3(0, 0, 0), VEC3(0, 0, 0), l_shaders[0]);
  Particule *p1 = new Particule(0.01, VEC3(0, 0.0, -0.1), VEC3(0, 0, 0), l_shaders[0]);
  Spring *s1 = new Spring(0.1, 0.1, p0, p1, l_shaders[2]);
  particules.push_back(p0);
  particules.push_back(p1);
  springs.push_back(s1);
  /**/
}

void Scene::animate() {
  m_camera.lookAt(m_view);
  m_vp = m_projection * m_view;
  
  if (running || step_by_step > 0) {
      for (auto &s : springs) {
        s->updateForces();
      }
    for (auto& p : particules) {
       p->oneStep();
    }

    std::list<Object*>::iterator it;
    for (it = l_objects.begin(); it != l_objects.end(); ++it) {
      (*it)->animate();
    }

    
  }
  if (step_by_step > 0) {
    --step_by_step;
  }
}

void Scene::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    for (auto &s : springs) {
      s->draw();
    }
     for (auto& p : particules) {
       p->draw();
     }
  
  std::list<Object*>::iterator it;
  
  for (it = l_objects.begin(); it != l_objects.end(); ++it) {
      (*it)->draw();
  }
}


void Scene::animationLoop() {
  while(!end_) {
    //INFO(3,"input");
    m_input_loop0 = SDL_GetTicks();
    m_input.updateEvenements();

    if(m_input.getTouche(SDL_SCANCODE_ESCAPE) || m_input.terminer()) {
	end_ = true;
	break;
    }
    if(m_input.getTouche(SDL_SCANCODE_RETURN)) {
      if (running) {
	INFO(1, "STOP ANIMATION");
      } else {
	INFO(1, "RUN ANIMATION");
      }
      running = !running;
    }
    if(m_input.getTouche(SDL_SCANCODE_SPACE)) {
      --back;
      ++step_by_step;
    }
    if(m_input.getTouche(SDL_SCANCODE_BACKSPACE)) {
      re_init = true;
    }
    if(m_input.getTouche(SDL_SCANCODE_B) ) {
      ++step_by_step;
      ++back;
    }
    if(m_input.getTouche(SDL_SCANCODE_S) ) {
      mpm_conf::display_sphere_ = !mpm_conf::display_sphere_;
    }
     if(m_input.getTouche(SDL_SCANCODE_KP_PLUS) ) {
       if (mpm_conf::replay_speed_ >= 1) {
	 mpm_conf::replay_speed_ *= 2;
       } else if (mpm_conf::replay_speed_ < -1) {
	 mpm_conf::replay_speed_ /= 2;
       } else {
	 assert( mpm_conf::replay_speed_ == -1);
	 mpm_conf::replay_speed_ = 1;
       }
    }
     if(m_input.getTouche(SDL_SCANCODE_KP_MINUS) ) {
         if (mpm_conf::replay_speed_ <= -1) {
	 mpm_conf::replay_speed_ *= 2;
       } else if (mpm_conf::replay_speed_ > 1) {
	 mpm_conf::replay_speed_ /= 2;
       } else {
	 assert( mpm_conf::replay_speed_ == 1);
	 mpm_conf::replay_speed_ = -1;
       }
    }
    m_camera.deplacer(m_input);

    m_input_loop1 = SDL_GetTicks();
    m_input_loop_time = m_input_loop1 - m_input_loop0;
    if(m_input_loop_time < m_input_rate) {
      SDL_Delay(m_input_rate - m_input_loop_time);
    }
  }
}

void Scene::bouclePrincipale() {
  std::thread t1(animateScene);
  FLOAT spacing =  mpm_conf::grid_spacing_;
  while(!end_) {
    m_debutBoucle = SDL_GetTicks();
    ++t;
    if (re_init) {
      Times::TIMES->init();
      re_init = false;
    }
    Times::TIMES->tick(Times::total_time_);
    animate();
    draw();

    if (t > 20000) {
        end_ = true;
      }

    Times::TIMES->tock(Times::total_time_);
     SDL_GL_SwapWindow(m_fenetre);
     m_finBoucle = SDL_GetTicks();
     m_tempsEcoule = m_finBoucle - m_debutBoucle;
     if(m_tempsEcoule < m_frameRate) {
      SDL_Delay(m_frameRate - m_tempsEcoule);
     }

  }
 
  t1.join();
}

void Scene::getProjView(glm::mat4 &vp) {
  vp = m_vp;
}

Shader* Scene::getShader(uint i) {
  return l_shaders[i];
}

Texture* Scene::getTexture(uint i) {
  return l_textures[i];
}

void Scene::setLoad(std::string s) {
}

void Scene::setExport(std::string s) {
}

void Scene::setImport(std::string s) {
}


void Scene::setRun(bool run) {
  if (running) {
    INFO(1, "STOP ANIMATION");
  } else {
    INFO(1, "RUN ANIMATION");
  }
  running = run;
}

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(SDLWindow);
    if (infoSurface == NULL) {
        std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
    } else {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
        if (pixels == 0) {
            std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
            return false;
        } else {
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
                std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
                pixels = NULL;
                return false;
            } else {
                saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
                if (saveSurface == NULL) {
                    std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
                    return false;
                }
                SDL_SaveBMP(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }
            delete[] pixels;
        }
        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }
    return true;
}
