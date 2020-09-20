#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double ctr_x, ctr_y;
double center_change_speed = 0.05;
double dx, dy;
double move_x, move_y;
double move_point_t = 0;


struct point
{
	double x,y,z;
};

void drawPoint(double x, double y){
    glBegin(GL_QUADS);{
        glVertex3f(x + 0.5, y + 0.5, 0);
        glVertex3f(x + 0.5, y - 0.5, 0);
        glVertex3f(x - 0.5, y - 0.5, 0);
        glVertex3f(x - 0.5, y + 0.5, 0);
    }glEnd();
}

void drawHypoPoint(double x, double y){
    glBegin(GL_QUADS);{
        glVertex3f(x + 5, y + 5, 0);
        glVertex3f(x + 5, y - 5, 0);
        glVertex3f(x - 5, y - 5, 0);
        glVertex3f(x - 5, y + 5, 0);
    }glEnd();
}

void drawHypo(double a, double b, double d){
   for(double t=0;t<=1;t+=0.001){
        double x = (a-b)*cos(6*pi*t) + d*cos(((a-b)/b)*(6*pi*t));
        double y = (a-b)*sin(6*pi*t) - d*sin(((a-b)/b)*(6*pi*t));
        drawPoint(x,y);
   }
}

void drawHypoMovingPointCounterClockWise(){
    glColor3f(0, 1, 1);
    drawHypoPoint(move_x, move_y);
}

void drawHypoMovingPointClockWise(){
    glColor3f(1, 0, 1);
    drawHypoPoint(move_x, -move_y);
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}
void hypoRotation(){
    //rotating clockwise
    glPushMatrix();{
        glRotatef(angle, 0, 0, 1);
        glTranslatef(90, 0, 0);
        glRotatef(2*angle, 0, 0, 1);
        glColor3f(1,1,0);
        drawHypo(36.0, 6.0, 18.0);
        drawHypoMovingPointCounterClockWise();
        drawHypoMovingPointClockWise();
    }glPopMatrix();

    //rotating counter-clockwise
    glPushMatrix();{
        glRotatef(-angle, 0, 0, 1);
        glTranslatef(-90, 0, 0);
        glRotatef(3*angle, 0, 0, 1);
        glColor3f(1,0,0);
        drawHypo(36.0, 6.0, 18.0);
        drawHypoMovingPointCounterClockWise();
        drawHypoMovingPointClockWise();
    }glPopMatrix();

}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
    hypoRotation();
	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    move_x = (36.0-6.0)*cos(6*pi*move_point_t) + 18.0*cos(((36.0-6.0)/6.0)*(6*pi*move_point_t));
    move_y = (36.0-6.0)*sin(6*pi*move_point_t) - 18.0*sin(((36.0-6.0)/6.0)*(6*pi*move_point_t));
    move_point_t += 0.0001;
	//codes for any changes in Models, Camera
	angle+=0.05;
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
    ctr_x = ctr_y = 0;
    angle = 0;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
