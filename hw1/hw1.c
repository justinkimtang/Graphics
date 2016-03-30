/*
 *  Lorenz Function
 *
 *  Key bindings:
 *  ESC	  Exit
 *
 *  0    reset parameters
 *  1    view mode 1
 *  2    view mode 2
 *  3    view mode 3
 *  4    decrease s
 *  5    decrease b
 *  6    decrease r
 *  7    increase s
 *  8    increase b
 *  9    increase r
 * 
 *  +    plot graph (viewmode 3)
 *  -    plot graph (viewmode 3)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=45;		 // Azimuth of view angle
int ph=30;		 // Elevation of view angle
int mode=1;		 // View types
double dim=2;	 // Dimension of orthogonal box

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;
int t = 50;

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192	// Maximum length of text string
void Print(const char* format , ...)
{
	char	  buf[LEN];
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
 *  Display the scene
 */
void display()
{
	int i;
	/*  Coordinates  */
        double x = 1;
	double y = 1;
	double z = 1;
	/*  Time step	*/
	double dt = 0.001;


	//  Clear the image
	glClear(GL_COLOR_BUFFER_BIT);
	//  Reset previous transforms
	glLoadIdentity();
	//  Set view angle
	glRotated(ph,1,0,0);
	glRotated(th,0,1,0);

	switch (mode)
	{
	//  Attractor by itself
	case 1:	
		glTranslatef(0,0,-1);
		glColor3f(1,1,1);
		glBegin(GL_LINE_STRIP);

		/*
		 *  Integrate 50,000 steps (50 time units with dt = 0.001)
		 *  Explicit Euler integration
		 */
		for (i=0;i<50000;i++)
		{
			double dx = s*(y-x);
			double dy = x*(r-z)-y;
			double dz = x*y - b*z;
			x += dt*dx;
			y += dt*dy;
			z += dt*dz;
			glVertex3d(x/25,y/25,z/25);
		}
		glEnd();

	break;
	// Draw with axes
	case 3:
		glColor3f(1,1,0);
		glBegin(GL_LINE_STRIP);

		for (i=0;i<50000;i++)
		{
			double dx = s*(y-x);
			double dy = x*(r-z)-y;
			double dz = x*y - b*z;
			x += dt*dx;
			y += dt*dy;
			z += dt*dz;
			glVertex3d(x/40,y/40,z/40);
		}

		         //  Draw axes in white
		         glColor3f(1,1,1);
			 glBegin(GL_LINES);
			 glVertex3d(0,0,0);
			 glVertex3d(1,0,0);
			 glVertex3d(0,0,0);
			 glVertex3d(0,1,0);
			 glVertex3d(0,0,0);
			 glVertex3d(0,0,1);
			 glVertex3d(0,0,0);
			 glVertex3d(-1,0,0);
			 glVertex3d(0,0,0);
			 glVertex3d(0,-1,0);
			 glVertex3d(0,0,0);
			 glVertex3d(0,0,-1);
			 glEnd();
			
			 //  Label axes
			 glRasterPos3f(1.1,0,0);
			 Print("x");
			 glRasterPos3f(0,1.1,0);
			 Print("y");
			 glRasterPos3f(0,0,1.1);
			 Print("z");
			 glRasterPos3f(-1.1,0,0);
			 Print("-x");
			 glRasterPos3f(0,-1.1,0);
			 Print("-y");
			 glRasterPos3f(0,0,-1.1);
			 Print("-z");
			 glEnd();
		glEnd();
	break;
	//Graph in portions
	case 2:
	        glTranslatef(0,0,-1.5);
		glColor3f(1,1,0);
		glBegin(GL_LINE_STRIP);

		for(i=0;i<t;i++)
		{
			double dx = s*(y-x);
			double dy = x*(r-z)-y;
			double dz = x*y - b*z;
			x += dt*dx;
			y += dt*dy;
			z += dt*dz;
			glVertex3d(x/20,y/20,z/20);
		}
		glEnd();
	break;

	}
	//  Display parameters
	glColor3f(1,1,1);
	glWindowPos2i(5,5);
	if (mode != 2)
		Print("s=%8.3f  b=%8.3f r=%8.3f",s,b,r);
	else
	   Print("Press + and - to control graph");

	//  Flush and swap
	glFlush();
	glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void key(unsigned char ch,int x,int y)
{
	//  Exit on ESC
	if (ch == 27)
		exit(0);

	else if (ch == '0')
	{
		t = 50;
		s = 10;
		b = 2.6666;
		r = 28;
		th = 45;
		ph = 30;
	}
	//  Switch mode
	else if ('1'<=ch && ch<='3')
	  mode = ch-'0';
	//  Increase paramaters  by 0.1
	else if (ch == '7')
		s += .01;
	else if (ch == '8')
		b += .1;
	else if (ch == '9')
		r += .1;
	//  Decrease parameters by 0.1
	else if (ch == '4')
		s -= .1;
	else if (ch == '5')
		b -= .1;
	else if (ch == '6')
		r -= .1; 
	//  increase time
	else if (ch == '+')
	      t += 150;
	else if (ch == '-')
	      t -= 150;

	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

void special(int key,int x,int y)
{
	//  Right arrow key - increase azimuth by 5 degrees
	if (key == GLUT_KEY_RIGHT)
		th += 5;
	//  Left arrow key - decrease azimuth by 5 degrees
	else if (key == GLUT_KEY_LEFT)
		th -= 5;
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP)
		ph += 5;
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN)
		ph -= 5;
	//  Keep angles to +/-360 degrees
	th %= 360;
	ph %= 360;
	//  Tell GLUT it is necessary to redisplay the scene
	glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
	//  Ratio of the width to the height of the window
	double w2h = (height>0) ? (double)width/height : 1;
	//  Set the viewport to the entire window
	glViewport(0,0, width,height);
	//  Tell OpenGL we want to manipulate the projection matrix
	glMatrixMode(GL_PROJECTION);
	//  Undo previous transformations
	glLoadIdentity();
	//  Orthogonal projection box adjusted for the
	//  aspect ratio of the window
	glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
	//  Switch to manipulating the model matrix
	glMatrixMode(GL_MODELVIEW);
	//  Undo previous transformations
	glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //	Initialize GLUT and process user parameters
	glutInit(&argc,argv);
	//  Request double buffered, true color window 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	//  Request 500 x 500 pixel window
	glutInitWindowSize(500,500);
	//  Create the window
	glutCreateWindow("Justin_Tang");
	//  Tell GLUT to call "display" when the scene should be drawn
	glutDisplayFunc(display);
  //	Tell GLUT to call "reshape" when the window is resized
	glutReshapeFunc(reshape);
	//  Tell GLUT to call "special" when an arrow key is pressed
	glutSpecialFunc(special);
	//  Tell GLUT to call "key" when a key is pressed
	glutKeyboardFunc(key);
	//  Pass control to GLUT so it can interact with the user
	glutMainLoop();
	//  Return code
	return 0;
}
