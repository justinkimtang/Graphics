/*
 *  Hw2
 *
 *  Draw 3d scene
 *
 *  Key bindings:
 *  m          Toggle between perspective and orthogonal
 *  +/-        Changes field of view for perspective
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
double Pi = 3.1415927;
//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */

static void circle(double x,double y,double z,double r)
{   
   int i;
   glPushMatrix();
   glBegin(GL_TRIANGLE_FAN);
      //  Center of circle
      glVertex3f(x, y,z);
      for(i = 0; i <= 20;i++)
      { 
         glVertex3f(
            x + (r * cos(i *  2*Pi / 20)), 
            y + (r * sin(i *  2*Pi / 20)),
            z
         );
      }
   glEnd();
   glPopMatrix();
}

static void cylinder(double x,double y,double z,
                     double h,double r,double s,double th)
{
   int i;
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,1,0,0);
   glScaled(r,r,h);
   
   //  Cylinder
   //  Top Cirlce
   glColor3f(0,0,1);
   circle(x,y,z,r);
   //  Bottom Circle
   glColor3f(0,1,0);
   circle(x,y,z+h,s);
   //  Sides
   glColor3f(0,0,1);
   glBegin(GL_QUAD_STRIP);
   for(i = 0; i <= 20;i++)
   {
     
      glVertex3f(
         x + (r * cos(i *  2*Pi / 20)), 
         y + (r * sin(i *  2*Pi / 20)),
         z);
      glVertex3f(
         x + (s * cos(i *  2*Pi / 20)), 
         y + (s * sin(i *  2*Pi / 20)),
         z + h);
   }
   glEnd();
   glPopMatrix();
}

static void rupee(double x,double y, double z,
                  double dx, double dy, double dz, double th)
{
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   glBegin(GL_POLYGON);
   glColor3f(0,1,0);
   glVertex3f(-.5,.5,0);
   glVertex3f(0,.904,0);
   glVertex3f(.5,.5,0);
   glVertex3f(.5,-.5,0);
   glVertex3f(0,-.904,0);
   glVertex3f(-.5,-.5,0);
   glVertex3f(-.5,.5,0);
   glEnd();

   glBegin(GL_POLYGON);
   glColor3f(1,0,0);
   glVertex3f(-.5,.5,-.5);
   glVertex3f(0,.904,-.5);
   glVertex3f(.5,.5,-.5);
   glVertex3f(.5,-.5,-.5);
   glVertex3f(0,-.904,-.5);
   glVertex3f(-.5,-.5,-.5);
   glVertex3f(-.5,.5,-.5);
   glEnd();

   glBegin(GL_QUADS);
   //  Sides
   glColor3f(1,0,1);
   glVertex3f(-.5,.5,0);
   glVertex3f(-.5,-.5,0);
   glVertex3f(-.65,-.5,-.25);
   glVertex3f(-.65,.5,-.25);
  
   glColor3f(1,1,0); 
   glVertex3f(-.5,.5,-.5);
   glVertex3f(-.5,-.5,-.5);
   glVertex3f(-.65,-.5,-.25);
   glVertex3f(-.65,.5,-.25);
  
   glColor3f(1,0,1);
   glVertex3f(.5,.5,0);
   glVertex3f(.5,-.5,0);
   glVertex3f(.65,-.5,-.25);
   glVertex3f(.65,.5,-.25);

   glColor3f(1,1,0);
   glVertex3f(.5,.5,-.5);
   glVertex3f(.5,-.5,-.5);
   glVertex3f(.65,-.5,-.25);
   glVertex3f(.65,.5,-.25);
   
   //  Corners
   glColor3f(0,0,1);
   glVertex3f(-.5,.5,0);
   glVertex3f(0,.904,0);
   glVertex3f(0,1.052,-.25);
   glVertex3f(-.65,.5,-.25);
 
   glColor3f(0,1,1);
   glVertex3f(-.5,.5,-.5);
   glVertex3f(0,.904,-.5);
   glVertex3f(0,1.052,-.25);
   glVertex3f(-.65,.5,-.25);
 
   glColor3f(0,1,1);
   glVertex3f(.5,.5,0);
   glVertex3f(0,.904,0);
   glVertex3f(0,1.052,-.25);
   glVertex3f(.65,.5,-.25);
 
   glColor3f(0,0,1);
   glVertex3f(.5,.5,-.5);
   glVertex3f(0,.904,-.5);
   glVertex3f(0,1.052,-.25);
   glVertex3f(.65,.5,-.25);
 
   glColor3f(0,1,1);
   glVertex3f(-.5,-.5,0);
   glVertex3f(0,-.904,0);
   glVertex3f(0,-1.052,-.25);
   glVertex3f(-.65,-.5,-.25);
 
   glColor3f(0,0,1);
   glVertex3f(-.5,-.5,-.5);
   glVertex3f(0,-.904,-.5);
   glVertex3f(0,-1.052,-.25);
   glVertex3f(-.65,-.5,-.25);
 
   glColor3f(0,0,1);
   glVertex3f(.5,-.5,0);
   glVertex3f(0,-.904,0);
   glVertex3f(0,-1.052,-.25);
   glVertex3f(.65,-.5,-.25);
 
   glColor3f(0,1,1);
   glVertex3f(.5,-.5,-.5);
   glVertex3f(0,-.904,-.5);
   glVertex3f(0,-1.052,-.25);
   glVertex3f(.65,-.5,-.25);
 

   glEnd();

   glPopMatrix();

}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=3;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }
   //  Draw Circle
   cylinder(0,0,0,1,1,1.5,0);
  // cylinder(1,0,0,1,1,1,270,.25,2);
  // rupee(0,1,0,1,1,1,0);
   //rupee(1,1,1,.5,.5,.5,240);

   //  Draw axes
   glColor3f(1,1,1);
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("hw2_justin_tang");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
