///////////////////////////////////////////////////////////////////////////////
// Imagina
// ----------------------------------------------------------------------------
// IN - Synthèse d'images - Modélisation géométrique
// Auteur : Gilles Gesquière
// ----------------------------------------------------------------------------
// Base du TP 1
// programme permettant de créer des formes de bases.
// La forme représentée ici est un polygone blanc dessiné sur un fond rouge
///////////////////////////////////////////////////////////////////////////////  

#include "point.hpp"
#include "vector.hpp"
#include "courbe.hpp"

#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include <utility>


/* Dans les salles de TP, vous avez généralement accès aux glut dans C:\Dev. Si ce n'est pas le cas, téléchargez les .h .lib ...
Vous pouvez ensuite y faire référence en spécifiant le chemin dans visual. Vous utiliserez alors #include <glut.h>. 
Si vous mettez glut dans le répertoire courant, on aura alors #include "glut.h" 
*/

// #include <glut.h> 
#include <GL/glut.h>
//#include "GL/glut.h"

// Définition de la taille de la fenêtre
#define WIDTH  800
#define HEIGHT 800

// Définition de la couleur de la fenêtre
#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1


// Touche echap (Esc) permet de sortir du programme
#define KEY_ESC 27


// Entêtes de fonctions
void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height); 
GLvoid window_key(unsigned char key, int x, int y); 
void timerFunc(int te);
void mouseFunc(int button, int state,int x, int y);
void motionFunc(int x, int y);
std::pair<double,double> mouseToWorldPosition(double x , double y);
void majSquelette(); 

double angleX = 0, angleY = 0, angleZ = 0;
int nbPoints = 5, numPointSelected = 0, numInterSelected = 18, lastPositionMouseX, lastPositionMouseY;
double xOrtho = 3, yOrtho = 3, zOrtho = 3, zoom = 1;
Point* tabPoints = new Point[nbPoints];
Point* pointToMove;
bool mouseRotating = false, mouseDragging = false, toggleAffichagePointsDeControles = true;
Courbe squelette;

int main(int argc, char **argv) 
{  
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);

  // définition et création de la fenêtre graphique, ainsi que son titre
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Premier exemple : carré");

  // initialisation de OpenGL et de la scène
  initGL();
  init_scene();

  // choix des procédures de callback pour 
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);

  //glutTimerFunc(2000,timerFunc,0);

  glutMouseFunc(mouseFunc);
  glutMotionFunc(motionFunc);

  // la boucle prinicipale de gestion des événements utilisateur
  glutMainLoop();  

  return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL() 
{
  glClearColor(RED, GREEN, BLUE, ALPHA);       
}

// Initialisation de la scene. Peut servir à stocker des variables de votre programme
void init_scene()
{
  tabPoints[0] = Point(-1,0,0);
  tabPoints[1] = Point(-0.5,1,0);
  tabPoints[2] = Point(0.5,-1,0);
  tabPoints[3] = Point(1,0,0);
  tabPoints[4] = Point(1.3,1,-0.5);

  majSquelette();
}

// fonction de call-back pour l´affichage dans la fenêtre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();

  // Zoom (fonctionne avec gluPerspective, dans window_reshape())
  // gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0);


  // C'est l'endroit où l'on peut dessiner. On peut aussi faire appel
  // à une fonction (re20nder_scene() ici) qui contient les informations 
  // que l'on veut dessiner
  render_scene();
  


  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
  //glutSwapBuffers();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{  
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // ici, vous verrez pendant le cours sur les projections qu'en modifiant les valeurs, il est
  // possible de changer la taille de l'objet dans la fenêtre. Augmentez ces valeurs si l'objet est 
  // de trop grosse taille par rapport à la fenêtre.
  glOrtho(-xOrtho, xOrtho, -yOrtho, yOrtho, -zOrtho, zOrtho);

  // Zoom (fonctionne avec gluLookAt, dans window_display())
  //gluPerspective(60, (float)1, 1, 1000);
  
  //gluPerspective(zoom,  1,  1,  20);

  // toutes les transformations suivantes s´appliquent au modèle de vue 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y) 
{   
  if (key == KEY_ESC) {
    exit(1); 
  } else if (key == 'z') {
    angleY--;
  } else if (key == 's') {
    angleY++;
  } else if (key == 'q') {
    angleX++;
  } else if (key == 'd') {
    angleX--;
  } else if (key == 'a') {
    
  } else if (key == 'e') {
    
  } else if (key == 'w') {
    numPointSelected--;
    if (numPointSelected < 0) {
      numPointSelected = nbPoints-1;
    }
    printf("Now selecting %d\n", numPointSelected);
  } else if (key == 'x') {
    numPointSelected++;
    if (numPointSelected > nbPoints-1) {
      numPointSelected = 0;
    }
    printf("Now selecting %d\n", numPointSelected);
  } else if (key == 'c') {
    numInterSelected--;
  } else if (key == 'v') {
    numInterSelected++;
  } else if (key == 'r') {
    double a = tabPoints[numPointSelected].getZ() - 0.1;
    tabPoints[numPointSelected].setZ(a);
    printf("%f\n", tabPoints[numPointSelected].getZ());
    majSquelette();
  } else if (key == 't') {
    double a = tabPoints[numPointSelected].getY() + 0.1;
    tabPoints[numPointSelected].setY(a);
    printf("%f\n", tabPoints[numPointSelected].getY());
    majSquelette();
  } else if (key == 'y') {
    double a = tabPoints[numPointSelected].getZ() + 0.1;
    tabPoints[numPointSelected].setZ(a);
    printf("%f\n", tabPoints[numPointSelected].getZ());
    majSquelette();
  } else if (key == 'f') {
    double a = tabPoints[numPointSelected].getX() - 0.1;
    tabPoints[numPointSelected].setX(a);
    printf("%f\n", tabPoints[numPointSelected].getX());
    majSquelette();
  } else if (key == 'g') {
    double a = tabPoints[numPointSelected].getY() - 0.1;
    tabPoints[numPointSelected].setY(a);
    printf("%f\n", tabPoints[numPointSelected].getY());
    majSquelette();
  } else if (key == 'h') {
    double a = tabPoints[numPointSelected].getX() + 0.1;
    tabPoints[numPointSelected].setX(a);
    printf("%f\n", tabPoints[numPointSelected].getX());
    majSquelette();
  } else if (key == 'p') {
    toggleAffichagePointsDeControles = !toggleAffichagePointsDeControles;
  } else {
    printf ("La touche %d n´est pas active.\n", key);
  } 
  glutPostRedisplay();    
}



//////////////////////////////////////////////////////////////////////////////////////////
// Fonction que vous allez modifier afin de dessiner
/////////////////////////////////////////////////////////////////////////////////////////
void render_scene()
{
  glRotatef(angleX, 0.0, 1.0, 0.0);
  glRotatef(angleY, 1.0, 0.0, 0.0);
  glRotatef(angleZ, 0.0, 0.0, 1.0);

  // Test courbe de Bézier 1
  glColor3f(1.0, 1.0, 1.0);

  if (toggleAffichagePointsDeControles) {
    tabPoints[0].draw();
    tabPoints[1].draw();
    tabPoints[2].draw();
    tabPoints[3].draw();
    tabPoints[4].draw();
    tabPoints[0].drawLine(tabPoints[1]);
    tabPoints[1].drawLine(tabPoints[2]);
    tabPoints[2].drawLine(tabPoints[3]);
    tabPoints[3].drawLine(tabPoints[4]);
  }

  glColor3f(1.0, 0.0, 0.0);

  //Courbe cCasteljau(tabPoints, 5, 30);
  squelette.drawCurve();

  glColor3f(0.0, 1.0, 0.0);
  //cCasteljau.drawIntermediary(numInterSelected);

  
  //Courbe cHermite(tabPoints[0], tabPoints[3], Vector(tabPoints[0], tabPoints[1]), Vector(tabPoints[3], tabPoints[2]), 30);
  //cHermite.drawCurve();



  // Test courbe de Bézier 2
  /*int nbPoints = 16;
  Point tabPoints[17];
  tabPoints[0] = Point(0,0,0);
  tabPoints[1] = Point(0,1,0);
  tabPoints[2] = Point(1,1,0);
  tabPoints[3] = Point(1,0,0);
  tabPoints[4] = Point(1,-1,0);
  tabPoints[5] = Point(0,-1,0);
  tabPoints[6] = Point(-1,-1,0);
  tabPoints[7] = Point(-1,0,0);
  tabPoints[8] = Point(-1,1,0);
  tabPoints[9] = Point(-1,2,0);
  tabPoints[10] = Point(0,2,0);
  tabPoints[11] = Point(1,2,0);
  tabPoints[12] = Point(2,2,0);
  tabPoints[13] = Point(2,1,0);
  tabPoints[14] = Point(2,0,0);
  tabPoints[15] = Point(2,-1,0);
  tabPoints[16] = Point(2,-2,0);
  glBegin(GL_LINE_STRIP);
    for(int i = 0; i < nbPoints; i++)
      glVertex3f(tabPoints[i].getX(), tabPoints[i].getY(), tabPoints[i].getZ());
  glEnd();
  for(int i = 0; i < nbPoints; i++)
    tabPoints[i].draw();

  glColor3f(1.0, 0.0, 0.0);
  Courbe cBezier(tabPoints, nbPoints, 30);
  cBezier.drawCurve();*/

  //Courbe cCasteljau;
  //cCasteljau.genererBezierCasteljau(tabPoints, nbPoints, 30);
  //cCasteljau.drawCurve();
}


void timerFunc(int te) {
  /// code to update your objects

  glutPostRedisplay(); // Redraw screen with new object data.

  glutTimerFunc(10, timerFunc, 1);// timer is a one shoot must be reset after being called. By using a timed event, your application should run about the same speed on any machine.
}

void mouseFunc(int button, int state,int x, int y) {
  lastPositionMouseX = x;
  lastPositionMouseY = y;
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) { // Droit relaché (roation)
    mouseRotating = false;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { // Droit enfoncé (rotation)
    mouseRotating = true;
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) { // Gauche relaché (sélection point, déplacement point)
    mouseDragging = false;
  } else if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { // Gauche enfoncé (sélection point, déplacement point)
    std::pair<double,double> mousePosition = mouseToWorldPosition(x,y);
    printf("x:%f, y:%f\n",mousePosition.first, mousePosition.second);
    for(unsigned int i = 0; i < nbPoints; i++) {
      if(tabPoints[i].near(0.1,mousePosition.first, mousePosition.second)) {
        pointToMove = &tabPoints[i];
        mouseDragging = true;
        numPointSelected = i;
        printf("selected %d\n", i);
      }
    }
    majSquelette();
  } else if (button == 3) { // scroll up (zoomer)
    zoom -= 0.05;
    printf("%f\n", zoom);
  } else if (button == 4) { // scroll down (dézoomer)
    zoom += 0.05;
    printf("%f\n", zoom);
  }
  window_display(); 
}

std::pair<double,double> mouseToWorldPosition(double x , double y){
  std::pair <double,double> position;
  double mouseX = ((x/WIDTH) * (2*xOrtho))+(-xOrtho);
  double mouseY = (((y/HEIGHT) * (2*yOrtho))+(-yOrtho))*-1;
  position.first = mouseX;
  position.second = mouseY;
  return position;
}

void motionFunc(int x, int y) {
  if(mouseRotating){
    angleX += (x - lastPositionMouseX)/10;
    angleY += (y - lastPositionMouseY)/10;
  } 
  if(mouseDragging) {
    std::pair<double,double> mousePosition = mouseToWorldPosition(x,y);
    pointToMove->setX(mousePosition.first);
    pointToMove->setY(mousePosition.second);
  }  
  lastPositionMouseX = x;
  lastPositionMouseY = y;
  window_display();
}

void majSquelette() {
  // Met à jour la courbe squelette
  // Doit être appelée à chaque modification d'un point de tabPoints.
  squelette = Courbe(tabPoints, 5, 30);
}