#include <cstdio>
#include <cstdlib>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>
#include <cmath>

#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

class Point{
    public :
        //coordonnées x, y et z du point
        double x;
        double y;
        double z;
        // couleur r, v et b du point
        float r=0.5;
        float g=0.5;
        float b=0.5;
};


//texture
int largimg=450;
int hautimg=300;
unsigned char image[450*300*3];
unsigned char texture[450][300][3];
unsigned char texture2[900][600][3];

char presse;
int anglex=30,angley=20,x,y,xold,yold;
//const float PI = 3.14159265f;
//const int numTriangles = 100;

//déclaration des méthodes de création du lapin et d'animation au clavier/souris
void affichage();
void clavier(unsigned char touche,int x,int y);
void souris(int boutton, int etat,int x,int y);
void sourismouv(int x,int y);
void redim(int l,int h);
void touchesFleches(int key, int x, int y);

//déclaration des méthodes pour créer le lapin
void tete();
void corps();
void oreilles();
void bras();
void jambes()  ;

//angles et booléens pour les animations
double angle = 0;
double deplac = 0;
double angleDeplac =0;
double angleDeplac2=0;
bool bDeplac =true;
bool b = true;
void initLight();

//initialisation du zoom
int zoom=4;

int nps=30; //nombre de parallèles de la sphère
int nms=40; //nombre de méridiens de la sphère
int fSphere[39*30][4]; //faces de la sphère
Point pSphere[30*40];   //points du cylindre

void sphere(double modx , double mody  , double modz ,float r,float v,float b, bool bo ) ;
void cylindre(double ray, double rx , double rz , float h ,int n , float r , float v , float  b ) ;

//déclaration des animations
void animation1();
void animation2();

//chargement de la texture
void loadJpegImage(char *fichier);



int main(int argc,char **argv)

{
  /* Chargement de la texture */
  loadJpegImage("./Pelage.jpg");

  /* Creation de la fenetre OpenGL */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600,600);
  glutCreateWindow("Lapin");

  /* Initialisation de l'etat d'OpenGL */
  glClearColor(0,0,0,0);
  glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

  /* Mise en place de la projection perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,1,1.0,5.0);
  glMatrixMode(GL_MODELVIEW);

    initLight();


  /* Parametrage du placage de textures */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,2*largimg,2*hautimg,0,
	       GL_RGB,GL_UNSIGNED_BYTE,texture2);
  glEnable(GL_TEXTURE_2D);

  /* Mise en place des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutMouseFunc(souris);
  glutMotionFunc(sourismouv);
  glutReshapeFunc(redim);
  glutSpecialFunc(touchesFleches);

  /* Entr�e dans la boucle principale glut */
  glutMainLoop();
  return 0;
}


void initLight()
{
  GLfloat specular[] = { 1,1,1,1};
  GLfloat diffus[] = { 1,1,1,1};
  GLfloat coul1[] = { 0,0,1,1 };
  GLfloat coul2[] = { 0,1,0,1 };

  GLfloat pos0[] = { 1,1,2,0 };
  GLfloat pos1[] = { 1,0,1,0 };

  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);

  glLightfv(GL_LIGHT0,GL_POSITION,pos0);
  glLightfv(GL_LIGHT0,GL_SPECULAR,coul1);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffus) ;

  glLightfv(GL_LIGHT1,GL_POSITION,pos1);
  glLightfv(GL_LIGHT1,GL_SPECULAR,coul2);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,diffus) ;
}


void affichage()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-zoom, zoom, -zoom, zoom, -zoom, zoom);

  gluLookAt(0.0,0.0,2.5,0.0,0.0,0.0,0.0,1.0,0.0);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);

  //Repère
    //axe x en rouge
    /*glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
            glVertex3f(0, 0,0.0);
            glVertex3f(1, 0,0.0);
    glEnd();
    //axe des y en vert
    glBegin(GL_LINES);
            glColor3f(0.0,1.0,0.0);
            glVertex3f(0, 0,0.0);
            glVertex3f(0, 1,0.0);
    glEnd();
    //axe des z en bleu
    glBegin(GL_LINES);
            glColor3f(0.0,0.0,1.0);
            glVertex3f(0, 0,0.0);
            glVertex3f(0, 0,1.0);
    glEnd();
*/

  animation1();
  tete();
  corps();
  oreilles();
  bras();
  jambes() ;

  glutSwapBuffers();
}

void tete()
{


      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      sphere(1,1.3,1,0.5,0.5,0.5,true);
      glPopMatrix()  ;

      // Blanc des yeux
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0.4,0.4,0.62);
      glColor3f(1,1,1);
      sphere(0.3,0.4,0.3,1,1,1,false);
      glPopMatrix()  ;
    //-----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(-0.4,0.4,0.62);
      glColor3f(1,1,1);
      sphere(0.3,0.4,0.3,1,1,1,false);
      glPopMatrix()  ;


    // iris
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0.45,0.45,0.74);
      sphere(0.2,0.3,0.2,0.24,0.6,1,false);
      glPopMatrix()  ;
    //-----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(-0.45,0.45,0.74);
      sphere(0.2,0.3,0.2,0.24,0.6,1,false);
      glPopMatrix()  ;

    // pupille
       glPushMatrix()  ;
       glTranslatef(0,0,deplac);
      glTranslatef(0.48,0.48,0.90);
      sphere(0.1,0.15,0.1,0,0,0,false);
      glPopMatrix()  ;
    //----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(-0.48,0.48,0.90);
      sphere(0.1,0.15,0.1,0,0,0,false);
      glPopMatrix()  ;

       glPushMatrix()  ;
       glTranslatef(0,0,deplac);
      glTranslatef(0.51,0.54,0.94);
      glColor3f(1,1,1);
      sphere(0.06,0.06,0.06,1,1,1,false);
      glPopMatrix()  ;
    //----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(-0.51,0.54,0.94);
      glColor3f(1,1,1);
      sphere(0.06,0.06,0.06,1,1,1,false);
      glPopMatrix()  ;

    //nez
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0,0.1,0.9);
      glRotatef(-90,1,0,0);
      glColor3f(1,0,0.87);
      glutSolidCone(0.2,0.3,30,20);
      glPopMatrix()  ;

      //dents
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0.06,-0.2,1);
      glScalef(0.1,0.2,0.1);
      cylindre(0.5,2,1,1,30,1,1,1);
      glColor3f(1,0,0.5);
      glPopMatrix()  ;
    //-----------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(-0.06,-0.2,1);
      glScalef(0.1,0.2,0.1);
      cylindre(0.5,2,1,1,30,1,1,1);
      glColor3f(1,0,0.5);
      glPopMatrix()  ;

}
void oreilles ()
{
      //derrière des oreilles
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glRotatef(angle,0,0,1);
      glRotatef(-30,0,0,1);
      glTranslatef(0,2,0);
      sphere(0.5,1.2,0.5,0.5,0.5,0.5,true) ;
      glPopMatrix()  ;
    //----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glRotatef(angle,0,0,1);
      glRotatef(30,0,0,1);
      glTranslatef(0,2,0);
      sphere(0.5,1.2,0.5,0.5,0.5,0.5,true) ;
      glPopMatrix()  ;

      //rose des oreilles
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glRotatef(-30,0,0,1);
      glRotatef(angle,0,0,1);
      glTranslatef(0,2,0.2);
      sphere(0.4,1.1,0.4,0.9,0.5,0.5,false) ;
      glPopMatrix()  ;
    //----------------------------
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glRotatef(angle,0,0,1);
      glRotatef(30,0,0,1);
      glTranslatef(0,2,0.2);
      sphere(0.4,1.1,0.4,0.9,0.5,0.5,false) ;
      glPopMatrix()  ;

}

void corps()
{
     //corps avec pelage
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0,-3,0);
      sphere(1.5,2,1.5,0.5,0.5,0.5,true) ;
      glPopMatrix()  ;

      //corps sans pelage
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0,-2.7,0.6);
      sphere(1,1.5,1,0.6,0.6,0.6,false) ;
      glPopMatrix();

      //queue
      glPushMatrix()  ;
      glTranslatef(0,0,deplac);
      glTranslatef(0,-3.7,-1.5);
      sphere(0.3,0.3,0.3,1,1,1,false) ;
      glPopMatrix();

}



void bras()
{


    //  bras gauche
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(1.5,-1.5,0.2);
    glRotatef(90,0,0,1);
    cylindre(0.2,2,1,2,30,0.5,0.5,0.5) ;
    glPopMatrix()  ;
    //----------------------------
    glPushMatrix( );
    glTranslatef(0,0,deplac);
    glTranslatef(1.5,-1.5,0.35);
    glRotatef(90,0,0,1);
    cylindre(0.15,2,1,2,30,0.6,0.6,0.6) ;
    glPopMatrix() ;

    // bras droit
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(-1.5,-1.5,0.2);
    glRotatef(-90,0,0,1);
    cylindre(0.2,2,1,2,30,0.5,0.5,0.5) ;
    glPopMatrix()  ;
    //----------------------------
    glPushMatrix( );
    glTranslatef(0,0,deplac);
    glTranslatef(-1.5,-1.5,0.35);
    glRotatef(-90,0,0,1);
    cylindre(0.15,2,1,2,30,0.6,0.6,0.6) ;
    glPopMatrix() ;


    // main gauche
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(2.6,-1.5,0.2);
    sphere(0.8,0.2,0.3,0.4,0.4,0.4,true) ;
    glPopMatrix() ;

    //main  droite
     glPushMatrix() ;
     glTranslatef(0,0,deplac);
     glTranslatef(-2.6,-1.5,0.2);
     sphere(0.8,0.2,0.3,0.4,0.4,0.4,true) ;
    glPopMatrix() ;



}


void jambes()
{

    //jambe gauche
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(0.5,-5.3,0);
    glRotatef(angleDeplac,1,0,0);
    cylindre(0.2,3,3,2,30,0.4,0.4,0.4) ;
    glPopMatrix()  ;


    //jambe  droite
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(-0.5,-5.3,0);
    glRotatef(angleDeplac2,1,0,0);
    cylindre(0.2,3,3,2,30,0.4,0.4,0.4) ;
    glPopMatrix() ;



    //pied gauche
    glPushMatrix() ;
    glTranslatef(0,0,deplac);
    glTranslatef(0.5,-6.3,0.1);
    glRotatef(angleDeplac,1,0,0);
    sphere(0.3,0.2,0.7,0.5,0.5,0.5,true) ;
    glPopMatrix()  ;



     // pied droit
     glPushMatrix() ;
     glTranslatef(0,0,deplac);
     glTranslatef(-0.5,-6.3,0.1);
     glRotatef(angleDeplac2,1,0,0);
     sphere(0.3,0.2,0.7,0.5,0.5,0.5,true) ;
     glPopMatrix() ;



}


void animation1()
{
    if (angle == 10)
        b = false;

    else if (angle == -10)
        b = true;

    if(b ==  true)
        angle += 0.25 ;
    else if (b == false)
        angle -= 0.25;
    glutPostRedisplay();
}


void animation2()
{
    if (deplac >= -7)
    {
        deplac-=0.025;

        if (angleDeplac == 15)
            bDeplac = false;

        else if (angleDeplac == -15)
            bDeplac = true;

        if(bDeplac ==  true)
        {
            angleDeplac += 0.5 ;
            angleDeplac2 -= 0.5;
        }
        else if (bDeplac == false)
        {
            angleDeplac -= 0.5;
            angleDeplac2 +=0.5;
        }
    }
    else
    {
        deplac=0;
    }
    glutPostRedisplay();
}

//fonction pour créer un cylindre
void cylindre(double ray , double rx , double rz, float h ,int n, float r , float v , float  b )
{
// attribution des coordonnées des points
    Point pCyl[2*n];
    int fCyl[n][4];
    for(int i=0;i<n;i++)
    {
        pCyl[i].x=ray*cos(2*i*M_PI/n)*rx;
        pCyl[i].y=h/2;
        pCyl[i].z=ray*sin(2*i*M_PI/n)*rz;
        pCyl[n+i].x=ray*cos(2*i*M_PI/n);
        pCyl[n+i].y=-h/2;
        pCyl[n+i].z=ray*sin(2*i*M_PI/n);
    }

//attribution des points par faces
    for(int i=0;i<n;i++)
    {
        fCyl[i][0]=n+i;
        fCyl[i][1]=n+((i+1)%n);
        fCyl[i][2]=(i+1)%n;
        fCyl[i][3]=i;
    }

     // Dessin du cylindre
    for(int i=0;i<n;i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(r,v,b);
        for ( int k=0 ; k< 4  ; k++)
           glVertex3d(pCyl[fCyl[i][k]].x,pCyl[fCyl[i][k]].y,pCyl[fCyl[i][k]].z);

           //glEnd();
    }
    glEnd();
}


//fonction pour créer une sphère
void sphere(double modx , double mody , double modz, float r,float v,float b, bool bo ){
//attribution des points par faces

int k=0;
for(int j=0; j < nps-1; j++){
    for(int i=0; i < nms; i++){
        fSphere[k][0]= i+j*nms;
        fSphere[k][1]= ((i+1)%(nms))+j*nms;
        fSphere[k][2]= ((i+1)%(nms))+(j+1)*nms;
        fSphere[k][3]= i+(j+1)*nms;
        k++;
    }
}



// attribution des coordonnées des points
int p=0;

for(int j=0; j < nps; j++){
    for(int i=0; i < nms; i++){
                pSphere[p].x =  cos(M_PI/12 + i * (2 * M_PI / nms)) * cos((M_PI / 12) + j * (M_PI / (nps /2)))*modx ;
                pSphere[p].y = sin(M_PI/12 + i * (2 * M_PI / nms)) * cos((M_PI / 12) + j * (M_PI / (nps /2)))*mody;
                pSphere[p].z = sin((M_PI / 12) + j * (M_PI / (nps /2)))*modz ;
                p++;
            }
    }



 // Dessin de la sphere
   for (int j=0;j<nps-1 ;j++){
        for (int i=0;i<nms;i++){
            for (int k=0;k<4;k++){



                glBegin(GL_POLYGON);
                glColor3f(r,v,b);
                glVertex3f(pSphere[fSphere[i+j*nms][k]].x,pSphere[fSphere[i+j*nms][k]].y,pSphere[fSphere[i+j*nms][k]].z);
                if (bo)
                    glTexCoord2f(pSphere[fSphere[i+j*nms][k]].x,pSphere[fSphere[i+j*nms][k]].y);
            }
        }

   }
   glEnd();
}










void clavier(unsigned char touche,int x,int y)
{
  switch (touche)
    {
    case 'p': /* affichage du carre plein */
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
      glutPostRedisplay();
      break;
    case 'f': /* affichage en mode fil de fer */
      glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
      glutPostRedisplay();
      break;
    case 's' : /* Affichage en mode sommets seuls */
      glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
      glutPostRedisplay();
      break;
    case 'd':
      glEnable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'D':
      glDisable(GL_DEPTH_TEST);
      glutPostRedisplay();
      break;
    case 'z': // zoom le lapin
        zoom++;
        glutPostRedisplay();
        break;
    case 'Z': // dézoom le lapin
        zoom--;
        glutPostRedisplay();
        break;
    case 'a' :          //active l'animation 2
        glutIdleFunc(animation2);
        break;
    case 'l':       //active la lumière
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        break;
    case 'L':                   //désactive la lumière
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        break;
    case 'q' : /*la touche 'q' permet de quitter le programme */
      exit(0);
    }
}

void souris(int bouton, int etat,int x,int y)
{
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
  {
    presse = 1;
    xold = x;
    yold=y;
  }
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
    presse=0;
}

//fonction pour bouger la caméra avec la souris
void sourismouv(int x,int y)
  {
    if (presse)
    {
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay();
    }

    xold=x;
    yold=y;
  }

void redim(int l,int h)
{
  if (l<h)
    glViewport(0,(h-l)/2,l,l);
  else
    glViewport((l-h)/2,0,h,h);
}

//fonction pour bouger la caméra autour du lapin avec les touches directionnelles du clavier
void touchesFleches(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        angley = angley + y*0.01;
        glutPostRedisplay();
        break;

    case GLUT_KEY_DOWN:
        angley = angley - y*0.01;
        glutPostRedisplay();
        break;

    case GLUT_KEY_LEFT:
        anglex = anglex - x*0.01;
        glutPostRedisplay();
        break;

    case GLUT_KEY_RIGHT:
        anglex = anglex + x*0.01;
        glutPostRedisplay();
        break;
    }
}

//fonction pour charger le fichier de texture
void loadJpegImage(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=largimg)||(cinfo.image_height!=hautimg)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*largimg*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

    for (int i=0;i<largimg;i++)
    for (int j =0;j<hautimg;j++){
        texture [i][j][0] = image [i*largimg*3+j*3];
        texture [i][j][1] = image [i*largimg*3+j*3+1];
        texture [i][j][2] = image [i*largimg*3+j*3+2];
    }

  for (int i=0;i<largimg*2;i++)
    for(int j=0;j<hautimg*2;j++){
        if(i<largimg&&j<hautimg){
            texture2[i][j][0]=texture[i][j][0];
            texture2[i][j][1]=texture[i][j][1];
            texture2[i][j][2]=texture[i][j][2];
        }
        else if(i>largimg&&j>hautimg){
            texture2[i][j][0]=texture[i-largimg][j-hautimg][0];
            texture2[i][j][1]=texture[i-largimg][j-hautimg][1];
            texture2[i][j][2]=texture[i-largimg][j-hautimg][2];
        }
        else{
            texture2[i][j][0]=255;
            texture2[i][j][1]=255;
            texture2[i][j][2]=255;
        }
    }

}
