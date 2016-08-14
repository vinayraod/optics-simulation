#include<windows.h>
#include<stdio.h>
#include<math.h>


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

int h=400,w=750;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-800,800,-800,800);
}

void setPixel(int x,int y)
{
    glColor3f(1,0,0);
    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}

/*DDA line drawing algorithm*/
void line(int x0, int y0, int x1, int y1){
    int k, steps, dx, dy;
    float x, y, xincrement, yincrement;

    x=x0;
    y=y0;

    dx= x1 - x0;
    dy= y1 - y0;

    if(abs(dx) > abs(dy)) steps = abs(dx);
    else steps = abs(dy);

    xincrement = (float)dx/(float)steps;
    yincrement = (float)dy/(float)steps;

    setPixel(round(x),round(y));

    for(k=0;k<steps;k++)
	{
		x+=xincrement;
		y+=yincrement;

		setPixel(x,y);
	}

}
/*DDA line drawing algorithm Ends*/

void a(int x,int y)
{
line(x,y,x+h/2,y+w);
line(x+h/2,y+w,x+h,y);
line(x+h/4,y+w/4,x+3*h/4,y+w/4);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    a(0,0);
    a(100,400);
    glFlush();
    glutSwapBuffers();
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(1376,1024);
    glutCreateWindow("Font");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
}

