
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<windows.h>

#include <stdlib.h>
#include<stdio.h>
#include<math.h>

/*
 * The functions implemented in the project are
 * declared below
 */
void init();
void plot(int x, int y);
void line(int x0, int y0, int x1, int y1);
void ray(int x0, int y0, int x1, int y1);
void image(int x0, int y0, int x1, int y1);
void matrixMult();
void makeIdentity();
void rotApoint();
void pixel(int a,int b, int x, int y);
void concaveMirror();
void pixel1(int c,int d, int x, int y);
void convexMirror();
void planeMirror();
void pixel2(int x,int y,int g,int f1);
void midEllipse(int a,int b,int g,int f1);
void pixel3(int x,int y,int g,int f1);
void midEllipse1(int a,int b,int g,int f1);
void concaveLens();
void convexLens();
void scanLine();
void glassSlab();
void display();
static void starting_page();
void second_page();
void keySpecialOperations(void);
void keyOperations(void);
void keySpecialUp (int key, int x, int y);
void keySpecial(int key, int x, int y);
void resetPerspectiveProjection();
void renderBitmapString(float x, float y, void *font,const char *string);
void setOrthographicProjection(int);
//Key pressed is stored in below arrays.
bool* keySpecialStates = new bool[246];
bool* keyStates = new bool[256];

//Transformation declarations
int temp[10],p=0,count=0;
float fi=0;
int theta;                              // This is the angle of incidence in glass slab
int degree;
int i,j;
float pi=2*3.14159265/180;
int u, v, h,he,chanx=0,tempx,glsin,v0,v1;    //object and image distance and height. chanx is used to keep the change in u after rotation
int mm=0;   //Mirror Movement
/*
 * Transform matrix is the general transformation matrix.
 * coor matrix is the matrix storing x and y coordinates.
 */
float transform[3][3],coor[3][3],t[3][3];
/*
 * x1 is the x coordinate of the point of intersection of bottom normal line
 * and the glass slab.
 */
int x1;

/* Declarations for BITMAP
 * wi and hi are the width of the window.
 */
int wi=1376, hi=1024;
const int font = (int)GLUT_BITMAP_TIMES_ROMAN_24;
static void (*drawAll) (void) = starting_page;


/*The declarations for drawing circle*/
int r=700, f=50;
int r1=1000;



void init(){
    glClearColor(0,0,0,0.9);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-800,800,-800,800);
}


//The method (plot) below plots a point
void plot(int x, int y){

    glBegin(GL_POINTS);
    glVertex2f(x,y);
    glEnd();
}

//The function (line) below implements DDA line drawing algorithm
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

    plot(round(x),round(y));

    for(k=0;k<steps;k++)
	{
		x+=xincrement;
		y+=yincrement;

		plot(x,y);
	}

}
// The ray function implemented below is used to draw rays connecting image and object
void ray(int x0, int y0, int x1, int y1){
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

    plot(round(x),round(y));

    for(k=0;k<steps;k++)
	{
		x+=xincrement;
		y+=yincrement;

        if((int)x%5==0)
            plot(x,y);
	}

}
/*
 * The image function is used to draw image.
 * We are using this function separately so that we can get a dotted image.
 */
void image(int x0, int y0, int x1, int y1){
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

    plot(round(x),round(y));

    for(k=0;k<steps;k++)
	{
		x+=xincrement;
		y+=yincrement;

        if((int)y%5==0)
            plot(x,y);
	}

}

/*
 * Transformation is implemented int the following 3 functions
 */


void matrixMult()
{
	for(i=0;i<3;i++)
		t[i][0]=transform[i][0]*coor[0][0]+transform[i][1]*coor[1][0]+transform[i][2]*coor[2][0];
	for(i=0;i<3;i++)
		coor[i][0]=t[i][0];
}

void makeIdentity()
{
	for (i=0; i<3; i++)
		for (j=0; j<3; j++)
 			transform[i][j]=(i==j);
}

void scale(char dir)
{
    float Sx=1,Sy;
    if(h>10)
    {
        if(dir=='u')
            Sy=1.1;
        else
            Sy=(float)1/1.1;

            makeIdentity();
            transform[0][0]=Sx;
            transform[1][1]=Sy;

            coor[0][0]=u;
            coor[1][0]=h;
            coor[2][0]=1;

            matrixMult();

            u=coor[0][0];
            h=coor[1][0];
    }
    else
    {
        if(dir=='u')
            h+=8;
        else
            h-=8;

    }

}

void rotApoint()
{
    float x;
    float y;
    makeIdentity();
    transform[0][2]= -(u);
    transform[1][2]=0;
    transform[2][2]=1;

    coor[0][0]=(u-chanx);
    coor[1][0]=h;
    coor[2][0]=1;

    matrixMult();

    makeIdentity();

	transform[0][0]=cos(fi);
	transform[0][1]=-sin(fi);
	transform[1][0]=sin(fi);
	transform[1][1]=cos(fi);

	matrixMult();

	makeIdentity();

	transform[0][2]=u;
    transform[1][2]=0;
    transform[2][2]=1;

	matrixMult();
	x=coor[0][0];
	y=coor[1][0];
	chanx=u-(int)x;
	h=(int)y;
}

/*
 * The function pixel is used to plot points for concave mirror.
 */

void pixel(int a,int b, int x, int y){
glColor3f(1,1,0);
glBegin(GL_POINTS);


if(y>600)
{
if(b+x==h && h>0)
{
tempx=a+y;
}
if(b-x==h && h<0)
{
  tempx=a+y;
}
glVertex2f(a+y,b+x);
glVertex2f(a+y,b-x);
}
glColor3f(1,1,1);
glEnd();
glFlush();
}


/* Concave mirror is drawn using the Mid-Point Circle Drawing algorithm.
 * The image is drawn only in 2 octants.
 */


void concaveMirror()
{   f=-50+mm;
    int x, y;
    int p;
	p=1-r;
    x=0;
    y=r;

    glColor3f(0,1,1);
    glPointSize(3.0f);
    plot(f,0);
    glPointSize(1.0f);
    glColor3f(1,1,1);

    while(x<y){
            pixel(-700+mm,0,x,y);

        if(p<0){
            x+=1;
            p=p+2*x+1;
        }
        else{
            x+=1;
            y-=1;
            p=p+2*(x-y)+1;
		}
    }
    line(-800,0,800,0);

    if(h!=0)
   {
    glColor3f(1,0,0);
    if(h>0)
    {
    line(u-chanx-5,h-20,u-chanx,h); //For Arrow
    line(u-chanx+5,h-20,u-chanx,h);
    }
    if(h<0)
    {
    line(u-chanx-5,h+20,u-chanx,h);
    line(u-chanx+5,h+20,u-chanx,h);
    }

   }
    glPointSize(2.0);
    line(u,0,u-chanx,h);//object
    if(u!=-50 )//to avoid divide by zero exception
        v0 = u*f/(u-f);
     if(u-chanx!=-50)
        v1=((u-chanx)*f)/(u-chanx-f); // Upper tip of image
    else if(u==50){
        v=0;
        he=0;
    }
    if((u-chanx)!=0)//to avoid divide by zero exception
        he=-(v1*h/(u-chanx));
    /* Image */

    if(he!=0)
    {
    glColor3f(0,0,1);

    if(he>0)
    {
    line(v1+mm-5,he-20,v1+mm,he);
    line(v1+mm+5,he-20,v1,he);
    }

    if(he<0)
    {
    line(v1+mm-5,he+20,v1+mm,he);
    line(v1+mm+5,he+20,v1+mm,he);
    }
    }
    glPointSize(2.0f);
    image(v0+mm,0,v1+mm,he);
    glColor3f(1,1,1);
    ray(u-chanx,h,tempx,h);
    ray(tempx,h,v1+mm,he);
    if(u<f)
    {
        ray(u-chanx,h,v1+mm,he);
    }
    if(u>=f)
    {
        ray(u-chanx,h,0,0);
        ray(0,0,v1+mm,he);
    }
    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-200,600,(void *)font,"REFLECTION THROUGH CONCAVE MIRROR");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();
}


void pixel1(int c,int d, int x, int y){
glColor3f(1,1,0);
glBegin(GL_POINTS);

if(y>600)
{
 if(d+x==h && h>0)
{
tempx=c-y;
}
if(d-x==h && h<0)
{
  tempx=c-y;
}

glVertex2f(c-y,d-x);
glVertex2f(c-y,d+x);
}
glColor3f(1,1,1);
glEnd();
glFlush();
}

/* Convex mirror is drawn using the Mid-Point Circle Drawing algorithm.
 * The image is drawn only in 2 octants.
 */
void convexMirror()
{
    int x,y;
    int p;
	p=1-r;
    x=0;
    y=r;
    f=50+mm;

    glColor3f(0,1,0);
    glPointSize(5.0f);
    plot(f,0);
    glPointSize(1.0f);
    glColor3f(1,1,1);

    while(x<y){
            pixel1(700+mm,0,x,y);

        if(y==h)
        {
            tempx=x;
        }
        if(p<0){
            x+=1;
            p=p+2*x+1;
        }
        else{
            x+=1;
            y-=1;
            p=p+2*(x-y)+1;
		}
    }
    line(-800,0,800,0);//principal axis
                        //To draw arrows on object
   if(h!=0)
   {
    glColor3f(1,0,0);
    if(h>0)
    {
    line(u-chanx-5,h-20,u-chanx,h);
    line(u-chanx+5,h-20,u-chanx,h);
    }
    if(h<0)
    {
     line(u-chanx-5,h+20,u-chanx,h);
    line(u-chanx+5,h+20,u-chanx,h);

    }
   }
    glPointSize(2.0);
    line(u,0,u-chanx,h);              //object
    if(u!=50)                   //to avoid divide by zero exception
        v0 = (u)*f/(u-f);
     if(u-chanx!=50)
        v1=(u-chanx)*f/(u-chanx-f);          //mirror formula
    if((u-chanx)!=0)                    //to avoid divide by zero exception
        he=-(v1*h)/(u-chanx);            //magnification

//    To draw arrows on the image
    if(he!=0)
    {
    glColor3f(0,0,1);
    glPointSize(2.0f);
    if(he>0)
    {
    line(v1+mm-5,he-20,v1+mm,he);
    line(v1+mm+5,he-20,v1+mm,he);
    }
    if(he<0)
    {
        line(v1+mm-5,he+20,v1+mm,he);
        line(v1+mm+5,he+20,v1+mm,he);

    }
    }
    image(v0+mm,0,v1+mm,he);//image
    glColor3f(1,1,1);
    ray(u-chanx,h,tempx,h);
    ray(tempx,h,v1+mm,he);
    ray(u-chanx,h,v1+mm,he);

    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-200,600,(void *)font,"REFLECTION THROUGH CONVEX MIRROR");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();


}

/*
 * This function is used to draw plane mirror, it's axis, object and it's image.
 * The object and image are drawn using equation v=-u, i.e. xi = -xo where xi is the image distance from mirror
 * and xo is the object distance from the mirror.
 * The object movement will be incorporated by manipulating this equation.
 * The rotation of object is also implemented.
*/
void planeMirror(){

    glColor3f(1,1,1);
    line(-800, 0, 800, 0);  //principal axis
    line(0+mm, -600, 0+mm, 600);  //mirror

    // for silvering of mirror
    int x=0, x1=10, y= 600, y1=580;
    for(y=600;y>=-600;){
        line(x+mm,y,x1+mm,y1);
        y1=y-40;
        y =y-20;
    }

    v=-u;     //This is the mirror formula for plane mirror.
    glColor3f(1,0,0);
    glPointSize(2.0f);
    //To draw arrows on object
    line(u-chanx-5,h-20,u-chanx,h);
    line(u-chanx+5,h-20,u-chanx,h);
    line(u,0, u-chanx, h);    //object

    //To draw arrows on image
    glColor3f(0,0,1);
    glPointSize(1.0f);
    line(v+mm+chanx-5,h-20,v+mm+chanx,h);
    line(v+mm+chanx+5,h-20,v+mm+chanx,h);
    image(v+mm,0,v+chanx+mm,h); //image
    glColor3f(1,1,1);
    ray(u-chanx,h,v+chanx+mm,h);
    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-200,600,(void *)font,"REFLECTION THROUGH PLANE MIRROR");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();

}
/******************************************************REFRACTION***************************************************************/

/**************************************************** Concave Lens**************************************************************/
//pixel2() and pixel3() are used to draw concave lens
void pixel2(int x,int y,int g,int f1)
{
    glColor3f(1,1,1);

    if(x>0)
    {
    plot(g+x,f1+y);
    plot(g+x,f1-y);
    }
}
/*
 * midEllipse, midEllipse1 are used to draw the two surfaces of the lens.
 */
void midEllipse(int a,int b,int g,int f1)
 {
   int p,q,x,y;
   x=0;
   y=b;

   int n=b*b;
   p=b*b-a*a*b+a*a/4;
   while(2*x*b*b<2*y*a*a)
   {

       if(p>=0)
       {
           x=x+1;
           y=y-1;
           p=p+2*x*b*b-2*y*a*a+b*b;
       }
       else
       {
           x=x+1;
           p=p+2*x*b*b+b*b;
       }
       pixel2(x,y,g,f1);
   }
    q=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;
   while(y>0)
   {


       if(q>=0)
       {
           y=y-1;
           q=q-2*a*a*y+a*a;
       }
       else
       {
           x=x+1;
           y=y-1;
           q=q+2*b*b*x-2*y*a*a+a*a;
       }
        pixel2(x,y,g,f1);
   }

}

void pixel3(int x,int y,int g,int f1)
{
    plot(g-x,f1-y);
    plot(g-x,f1+y);

}

void midEllipse1(int a,int b,int g,int f1)
{
   int p,q,x,y;
   x=0;
   y=b;

   int n=b*b;
   p=b*b-a*a*b+a*a/4;
   while(2*x*b*b<2*y*a*a)
   {

       if(p>=0)
       {
           x=x+1;
           y=y-1;
           p=p+2*x*b*b-2*y*a*a+b*b;
       }
       else
       {
           x=x+1;
           p=p+2*x*b*b+b*b;
       }
       pixel3(x,y,g,f1);
   }
    q=b*b*(x+0.5)*(x+0.5)+a*a*(y-1)*(y-1)-a*a*b*b;
   while(y>0)
   {


       if(q>=0)
       {
           y=y-1;
           q=q-2*a*a*y+a*a;
       }
       else
       {
           x=x+1;
           y=y-1;
           q=q+2*b*b*x-2*y*a*a+a*a;
       }
        pixel3(x,y,g,f1);
   }
}

void concaveLens()
{
    f=-200;


    /*MAKES MIRROR*/
    midEllipse(100,300,-150+mm,0);
    midEllipse1(100,300,150+mm,0);
    line(-150+mm,300,150+mm,300);
    line(-150+mm,-300,150+mm,-300);
    /*MAKES PRINCIPAL AXIS*/
    line(-800,0,800,0);

    /*makes the object*/
    glColor3f(1,0,0);
    glPointSize(2.0f);
    line(u,0,u-chanx,h);

    if(h!=0)
   {
    glColor3f(1,0,0);
    if(h>0)
    {
    line(u-chanx-5,h-20,u-chanx,h);
    line(u-chanx+5,h-20,u-chanx,h);
    }
    if(h<0)
    {
     line(u-chanx-5,h+20,u-chanx,h);
    line(u-chanx+5,h+20,u-chanx,h);

    }
   }

    if(u!=-f)//to avoid divide by zero exception
    {
    v0=((u)*f)/((u)+f);
    }
    if(u-chanx!=-f)
    {
        v1=((u-chanx)*f)/((u-chanx)+f);
    }
    if(u-chanx!=0)//to avoid divide by zero exception
    he=(h*v1)/(u-chanx);

 if(he!=0)
    {
    glColor3f(0,0,1);
    glPointSize(2.0f);
    if(he>0)
    {
    line(v1+mm-5,he-20,v1+mm,he);
    line(v1+mm+5,he-20,v1+mm,he);
    }
    if(he<0)
    {
        line(v1+mm-5,he+20,v1+mm,he);
        line(v1+mm+5,he+20,v1+mm,he);

    }
    }


    glColor3f(0,0,1);
    glPointSize(2.0f);
    image(v0+mm,0,v1+mm,he);
    glColor3f(1,1,1);


    glPointSize(2.0f);
    ray(u-chanx,h,0+mm,h);
    ray(0+mm,h,v1+mm,he);
    ray(v1+mm,he,f,0);
    ray(u-chanx,h,v1+mm,he);
    ray(v1+mm,he,0+mm,0);

    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-200,600,(void *)font,"REFRACTION THROUGH CONCAVE LENS");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();

}
/*******************************************************CONVEX LENS****************************************************************/
void convexLens()
{
    f=200;
    int x,y;
    int p;
	p=1-r1;
    x=0;
    y=r1;
    while(x<y){
            if(x<=300)
                pixel1(955+mm,0,x,y);
            if(x<=300)
                pixel(-955+mm,0,x,y);
        if(p<0){
            x+=1;
            p=p+2*x+1;
        }
        else{
            x+=1;
            y-=1;
            p=p+2*(x-y)+1;
		}
    }
    glColor3f(0,0,1);
    glPointSize(3.0f);
    plot(f,0);
    glPointSize(1.0f);
    glColor3f(1,1,1);
    line(-800,0,800,0);//principal axis
    //To draw arrows on object
    if(h!=0)
   {
    glPointSize(2.0f);
    glColor3f(1,0,0);
    if(h>0)
    {
    line(u-chanx-5,h-20,u-chanx,h);
    line(u-chanx+5,h-20,u-chanx,h);
    }
    if(h<0)
    {
     line(u-chanx-5,h+20,u-chanx,h);
    line(u-chanx+5,h+20,u-chanx,h);

    }
   }glColor3f(1,0,0);
    glPointSize(2.0);
    line(u,0,u-chanx,h);//object
    //lens formula
    if(u!=-f)//to avoid divide by zero exception
        v0=u*f/(u+f);

    if(u-chanx!=-f)//to avoid divide by zero exception
        v1=(u-chanx)*f/(u-chanx+f);
    else
        {
            v0=0;
            v1=0;
            he=0;
        }
    //magnification
    if(u-chanx!=0)//to avoid divide by zero exception
        he=(v1*h/(u-chanx));
    //To draw arrows on the image
    glPointSize(2.0f);
    glColor3f(0,0,1);
    image(v0+mm,0,v1+mm,he);//image
    glColor3f(1,1,1);
    if(he!=0)
    {
    glColor3f(0,0,1);
    glPointSize(2.0f);
    if(he>0)
    {
    line(v1+mm-5,he-20,v1+mm,he);
    line(v1+mm+5,he-20,v1+mm,he);
    }
    if(he<0)
    {
        line(v1+mm-5,he+20,v1+mm,he);
        line(v1+mm+5,he+20,v1+mm,he);

    }
    }
    glColor3f(1,1,1);
    glPointSize(2.0f);
    ray(u-chanx,h,0+mm,h);
    ray(0+mm,h,v1+mm,he);
    ray(v1+mm,he,f+mm,0);
    ray(u-chanx,h,v1+mm,he);
    ray(v1+mm,he,0+mm,0);

    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-200,600,(void *)font,"REFRACTION THROUGH CONVEX LENS");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();

}

void scanLine(){
    glColor3f(0,1,1);
    int x,y;
    for(y=400; y>-400;y--){
        line(-400,y,400,y);
    }
    glColor3f(1,1,1);
}

 void glassSlab()
 {
//     int theta,u;


  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


     line(-400,400,400,400);
     line(-400,400,-400,-400);
     line(-400,-400,400,-400);
     line(400,-400,400,400);
     scanLine();
     glColor3f(1,0,0);
/*
 *   Declarations for the initials of ray and normal
 */
 if(theta!=90){
     int yi;
     int xi=-400+glsin-50;
     float rad = 3.14*(90+theta)/180;
     yi=(-50)*tan(rad)+400;

/*    Now we have the initial coordinates of the incident ray.
 *    i.e. ray starts from (xi, yi) to (u, 400)
 *    Also, x co-ordinate of normal is -400+glsin.
 *    Incident ray starts a distance (x coordinate) 50 from left of normal
 *    i.e. the x coordinate is xi defined above.
 */

      line(-400+glsin,350,-400+glsin,450);//normal
      line(xi,yi,-400+glsin,400);//ray

/*
 *     Implementation of refraction.
 */
        float u1= 1, u2= 1.5;
        float radR, sin_thetaR, tan_thetaR;//Angles for refration
        radR= theta*3.14/180;
        sin_thetaR =sin(radR)/u2;
        tan_thetaR = sin_thetaR/sqrt(1-pow(sin_thetaR,2));
         x1 = 800*tan_thetaR;

        line(-400+glsin,400,x1-400+glsin,-400);//point of incidence of refracted ray
        line(-400+x1+glsin,-450,-400+x1+glsin,-350);//normal 2

        int yf;
        yf = 50/tan(radR);//final position where ray should stop
        line(-400+x1+glsin,-400,-400+x1+glsin+50,-400-yf);//final ray
        glColor3f(1,1,1);
    }

    else{
        glColor3f(1,0,0);
         line(-400+glsin,350,-400+glsin,450);//normal
         line(-500,400,500,400);
    }

    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(1);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-300,600,(void *)font,"REFRACTION THROUGH GLASS SLAB :");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();



}
void display(){
    keyOperations();
    keySpecialOperations();
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    (*drawAll)();
    glPopMatrix();
    glutSwapBuffers();
}
/*
 * BITMAP
 * Functions used to write on the window.
 */
void setOrthographicProjection(int i) {
    if(i!=1)
        gluOrtho2D(0, wi, 0, hi);
    glScalef(1, -1, 1);
    glTranslatef(0, -hi, 0);
}

void resetPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void renderBitmapString(float x, float y, void *font,const char *string){
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// To display the Main Menu
void starting_page()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(0);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-250,600,(void *)font,"OPTICS SIMULAT0R: CODE THE LIGHT");
    renderBitmapString(-100,300,(void *)font,"Atif Ahmed");
    renderBitmapString(-100,200,(void *)font,"Anmol Varma");
    renderBitmapString(-100,100,(void *)font,"Anmol Vijaywargiya");
    renderBitmapString(-100,0,(void *)font,"Jatin Sood");
    renderBitmapString(-100,-100,(void *)font,"Nachiketh P");
    renderBitmapString(-100,-200,(void *)font,"Sandeep D");
    renderBitmapString(-100,-500,(void *)font,"Press Enter to continue...");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();

}

void second_page()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    setOrthographicProjection(0);
    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(-250,400,(void *)font,"Double click on the desired option :");
    renderBitmapString(-250,200,(void *)font,"1. Reflection");
    renderBitmapString(-250,100,(void *)font,"2. Refraction");
    glPopMatrix();
    glColor3f(1,1,1);
    resetPerspectiveProjection();
}

//void third_page()
//
//{
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glColor3f(1.0, 0.0, 0.0);
//    setOrthographicProjection();
//    glPushMatrix();
//    glLoadIdentity();
//    //renderBitmapString(-250,0,(void *)font,"Reflection");
//    renderBitmapString(-250,0,(void *)font,"Refaction");
//    glPopMatrix();
//    glColor3f(1,1,1);
//    resetPerspectiveProjection();
//}


/*
 * The following functions implement mouse events.
 */
void setReflection(int type)
{
    glutDetachMenu(GLUT_LEFT_BUTTON);
    switch(type)
    {
        case 1: fi=0;
                printf("Enter the object distance (Negative for Real Objects) : ");
                scanf("%d",&u);
                printf("Enter the height of the object : ");
                scanf("%d",&h);
                drawAll= planeMirror;
                break;

        case 2: fi=0;
                printf("Enter the object distance (Negative for Real Objects) : ");
                scanf("%d",&u);
                printf("Enter the height of the object : ");
                scanf("%d",&h);
                drawAll= convexMirror;
                break;


        case 3: fi=0;
                printf("Enter the object distance (Negative for Real Objects) : ");
                scanf("%d",&u);
                printf("Enter the height of the object : ");
                scanf("%d",&h);
                drawAll= concaveMirror;
                break;
        case 4: exit(0);
                break;
    }
    glutPostRedisplay();

}
void setRefraction(int type)
{
     glutDetachMenu(GLUT_LEFT_BUTTON);
     switch(type)
     {
         case 1:    fi=0;
                    printf("Enter object distance : ");
                    scanf("%d",&u);
                    printf("Enter object height : ");
                    scanf("%d",&h);
                    drawAll=convexLens;
                    break;

         case 2:    fi=0;
                    printf("Enter object distance : ");
                    scanf("%d",&u);
                    printf("Enter object height : ");
                    scanf("%d",&h);
                    drawAll=concaveLens;
                    break;

         again:  case 3:printf("Enter angle from normal : ");
                    scanf("%d",&theta);
                    if(theta>90)
                        goto again;
                    printf("Enter the distance from the edge of slab : ");
                    scanf("%d",&glsin);
                    drawAll=glassSlab;
                    break;
         case 4: exit(0);
     }

    glutPostRedisplay();
}
void processMenu(int type)
{
    switch(type)
    {
        case 1: exit(0);
    }

}

void mouse(int btn,int state,int x,int y)
{

   if(drawAll == second_page)
   {
       glutDetachMenu(GLUT_LEFT_BUTTON);
       //printf("X is %d\n",x);
       //printf("Y is %d\n",y);
       if(x>=460 && x<=600 && y<=280 && y>=260)
       {
        glutCreateMenu(setReflection);
        glutAddMenuEntry("Plane Mirror", 1);
        glutAddMenuEntry("Convex Mirror", 2);
        glutAddMenuEntry("Concave Mirror", 3);
        glutAddMenuEntry("EXIT", 4);
        glutAttachMenu(GLUT_LEFT_BUTTON);
       }
       else if(x>=470 && x<=590 && y>=310 && y<=330)
       {
        glutCreateMenu(setRefraction);
    	glutAddMenuEntry("Convex Lens", 1);
        glutAddMenuEntry("Concave Lens", 2);
        glutAddMenuEntry("Glass Slab", 3);
        glutAddMenuEntry("EXIT", 4);
        glutAttachMenu(GLUT_LEFT_BUTTON);

       }
   }
       else if(drawAll==planeMirror || drawAll==concaveMirror || drawAll==convexMirror || drawAll==concaveLens || drawAll==convexLens || drawAll == glassSlab)
       {
        int reflectionMenu=glutCreateMenu(setReflection);
        glutAddMenuEntry("Plane Mirror", 1);
        glutAddMenuEntry("Convex Mirror", 2);
        glutAddMenuEntry("Concave Mirror", 3);

        int refractionMenu=glutCreateMenu(setRefraction);
        glutAddMenuEntry("Convex Lens", 1);
        glutAddMenuEntry("Concave Lens", 2);
        glutAddMenuEntry("Glass Slab", 3);

        glutCreateMenu(processMenu);
        glutAddSubMenu("Reflection",reflectionMenu);
        glutAddSubMenu("Refraction",refractionMenu);
        glutAddMenuEntry("Exit",1);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
       }

        if(state == GLUT_UP)
        {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }

}

void mouseDrag(int x,int y)
{
    if(drawAll!=second_page)
    {
    glutSetCursor(GLUT_CURSOR_LEFT_RIGHT);
    temp[p]=x;
    p++;
    if(count!=0)
    {
        if(temp[1]>temp[0])
        {
            u+=5;
        }
        else if(temp[1]<temp[0])
        {
            u-=5;
        }
        temp[0]=temp[1];
        p=1;
    }
    count=1;
    }
    glutPostRedisplay();
}


/*
 *  Below function are meant for handling various keyboard operations.
 *  KeySpecial Operations are meant for arrow keys and shift keys
 */

void keyOperations (void) {
if (keyStates[13])
{
    drawAll=second_page;
}
 if(keyStates['r'])
{
    fi+=pi/16;
    //printf("%f",fi);
    rotApoint();
}
 if(keyStates['l'])
{
    fi-=pi/16;
    //printf("%f",fi);
  rotApoint();
}
 if(keyStates['j'])
 {
     mm+=10;
 }
 if(keyStates['h'])
 {
     mm-=10;
 }

 glutPostRedisplay();
}

void keySpecialOperations(void){
    if(keySpecialStates[GLUT_KEY_LEFT]){
        u-= 10;
        if(glsin>=10 && glsin<=800-x1)
        glsin-= 10;
        //printf("debug");
    }

    if(keySpecialStates[GLUT_KEY_RIGHT]){
        u+=10;
        if(glsin>=0 && glsin<=800-x1-10)
        glsin+=10;
    }

    if(keySpecialStates[GLUT_KEY_UP]){
        scale('u');
        if(chanx!=0)
        {
            chanx+=1;
        }
    }

    if(keySpecialStates[GLUT_KEY_DOWN]){
        scale('d');
        if(chanx!=0)
        {
            chanx-=1;
        }
    }

    glutPostRedisplay();
}


void keyPressed(unsigned char key, int x, int y) {
keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp(unsigned char key, int x, int y) {
keyStates[key] = false; // Set the state of the current key to not pressed
}

void keySpecial (int key, int x, int y){
    keySpecialStates[key]= true;
}

void keySpecialUp (int key, int x, int y){
    keySpecialStates[key] = false;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1376,1024);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow("Optics Simulator");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(keySpecial);
    glutSpecialUpFunc(keySpecialUp);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseDrag);
    glutMainLoop();


}
