
#include "CSCIx229.h"
float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
int axes=1;       //  Display axes



void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0);  //rotate our camera on teh x-axis (left and right)
    glRotatef(yrot,0.0,1.0,0.0);  //rotate our camera on the y-axis (up and down)
    glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
}

void display (void) {
    const double len=2.5;  //  Length of axes
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //   glEnable(GL_CULL_FACE);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    glLoadIdentity();  
    camera();

if (axes)
   {
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
   }

    glutSwapBuffers(); //swap the buffers
    angle++; //increase the angle
}


 
void reshape (int w, int h) {
    glViewport (0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
    glMatrixMode (GL_PROJECTION); //set the matrix to projection
    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0
); //set the perspective (angle of sight, width, height, , depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model

}

void keyboard (unsigned char key, int x, int y) {
    if(key=='q')
    {
    xrot += 1;
    if(xrot >360) xrot -= 360;
    }

    if(key=='z')
    {
    xrot -= 1;
    if(xrot < -360) xrot += 360;
    }

    if(key=='w')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos += (sin(yrotrad)) ;
    zpos -= (cos(yrotrad)) ;
    ypos -= (sin(xrotrad)) ;
    }

    if(key=='s')
    {
    float xrotrad, yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xrotrad = (xrot / 180 * 3.141592654f); 
    xpos -= (sin(yrotrad));
    zpos += (cos(yrotrad)) ;
    ypos += (sin(xrotrad));
    }

    if(key=='d')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos += (cos(yrotrad)) * 0.2;
    zpos += (sin(yrotrad)) * 0.2;
    }

    if(key=='a')
    {
    float yrotrad;
    yrotrad = (yrot / 180 * 3.141592654f);
    xpos -= (cos(yrotrad)) * 0.2;
    zpos -= (sin(yrotrad)) * 0.2;
    }

    if(key==27)
    {
    exit(0);
    }
}

void mouseMovement(int x, int y) {
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    xrot += diffy; //set the xrot to xrot with the additionof the difference in the y position
    yrot += diffx;    //set the xrot to yrot with the additionof the difference in the x position
}

int main (int argc, char **argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); 
    glutInitWindowSize (600, 600); 
    glutCreateWindow ("Justin Tang Final"); 
    glutDisplayFunc (display); 
    glutIdleFunc (display); 
    glutReshapeFunc (reshape);

    glutPassiveMotionFunc(mouseMovement); //check for mousemovement

    glutKeyboardFunc (keyboard); 
    glutMainLoop (); 
    return 0;
}
