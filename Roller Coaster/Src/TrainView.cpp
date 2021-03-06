﻿#include "TrainView.h"  
#include<cmath>
pParticle Particles,Part2,Part3,Part4;
int Fire;
UINT nOfFires[4];
int fire = 0;
UINT Tick1[4], Tick2[4];  
float DTick[4];;
GLfloat grav(0.00003f); 
void DrawParticles();
void ProcessParticles();
GLuint    textureID;
#define MAX_PARTICLES 1000  
#define MAX_FIRES 2  

TrainView::TrainView(QWidget *parent) :  
QGLWidget(parent)  
{  
	resetArcball();
	
}  
TrainView::~TrainView()  
{}  
void TrainView::initializeGL()
{
	initializeOpenGLFunctions();
	//Create a triangle object
	triangle = new Triangle();
	//Initialize the triangle object
	triangle->Init();
	//Create a square object
	water = new Water();
	square = new Square();
	//Initialize the square object
	water->Init();
	square->Init();
	//Initialize texture 
	initializeTexture();
	
	
}
void TrainView::initializeTexture()
{
	//Load and create a texture for square;'stexture
	QOpenGLTexture* texture = new QOpenGLTexture(QImage("./Textures/Tupi.bmp"));
	Textures.push_back(texture);
	//---------------------------------------------------------------------------------
	texture = new QOpenGLTexture(QImage("./Textures/Water.bmp"));
	Textures.push_back(texture);
	//----------------------------------------- 


}
Pnt3f TrainView::GMT(float tau, Pnt3f con1, Pnt3f con2, Pnt3f con3, Pnt3f con4, int type,float t)
{
	tau = tautau;
	float Gmartix[3][4] = {
		con1.x,con2.x,con3.x,con4.x,
		con1.y,con2.y,con3.y,con4.y,
		con1.z,con2.z,con3.z,con4.z
	};
	float Tmartix[4]= {
		t*t*t,
		t*t,
		t,
		1
	};
	float MTmartix[4] = {0};
	float  qt[3];
	//type1=Cardinal
	if (type == 1)
	{
		
		float Mmartix[4][4] = {
		-1, 2, -1, 0,
		2- tau, -3+ tau, 0, 1,
		-2 + tau, 3- 2 *tau,tau,0,
		tau,-tau,0,0
		};
		for (int i = 0; i < 4; i++) {
			MTmartix[i] = 0.0;
			for (int j = 0; j < 4; j++) {
				MTmartix[i] += Mmartix[i][j] * Tmartix[j];
			}
		}
		for (int i = 0; i < 3; i++) {
			qt[i] = 0;
			for (int j = 0; j < 4; j++) {
				qt[i] += Gmartix[i][j] * MTmartix[j];
			}
		}
	}
	//type2=BSpline
	else if (type == 2)
	{
		float Mmartix[4][4] = {
		-(1.0 / 6.0), (1.0 / 2.0), -(1.0 / 2.0), (1.0 / 6.0),
		(1.0 / 2.0), (-1.0) , 0, (2.0 / 3.0),
		-(1.0 / 2.0),(1.0 / 2.0), (1.0 / 2.0),(1.0 / 6.0),
		(1.0 / 6.0), 0,0,0
		};
		for (int i = 0; i < 4; i++) {
			MTmartix[i] = 0.0;
			for (int j = 0; j < 4; j++) {
				MTmartix[i] += Mmartix[i][j] * Tmartix[j];
			}
		}
		for (int i = 0; i < 3; i++) {
			qt[i] = 0;
			for (int j = 0; j < 4; j++) {
				qt[i] += Gmartix[i][j] * MTmartix[j];
			}
		}
	}
	return Pnt3f(qt[0],qt[1],qt[2]);
}
void TrainView::drawTrain(float x)
{
	Pnt3f qt, qt0, qt1, orient_t;
		qt = train_pos[0];
		//pos
		//Pnt3f cp_pos_p1 = train_pos[0];
		//Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;

		// orient
		//Pnt3f cp_orient_p1 = train_dir[0];
		//Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		//switch (type_spline) {
		//case spline_Linear:
		//	// Linear
		//	qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
		//	orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
		//	break;
		//}
	Pnt3f cross_t = train_dir[0]* train_updir[0];
	cross_t.normalize();
	cross_t = cross_t * 4.0f;

	if (isrun)
	{
		trainWheelcount++;
		trainWheelcount = trainWheelcount % 25;
	}
	//前
	PrintTextures(train_pos[0] + cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0],
		train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0], train_pos[0] + cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0]);
	//側
	PrintTextures(train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0],
		train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0]);
	PrintTextures(train_pos[0] + (1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0],
		train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0], train_pos[0] + (1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0]);
	//上下
	PrintTextures(train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0],
		train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0], train_pos[0] + (1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (8) * train_updir[0]);
	PrintTextures(train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0],
		train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (1.0) * cross_t + (5) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0]);
	//後
	PrintTextures(train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0], train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (-3) * train_updir[0],
		train_pos[0] + (1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0], train_pos[0] + (-1.0) * cross_t + (-8) * train_dir[0] + (8) * train_updir[0]);
	PrintCircle(train_pos[0] + (-1.0) * cross_t + (5) * train_dir[0] + (-3) * train_updir[0] , 0);
	PrintCircle(train_pos[0] + (-1.0) * cross_t + (-5) * train_dir[0] + (-3) * train_updir[0], 0);
	PrintCircle(train_pos[0] + (1.0) * cross_t + (5) * train_dir[0] + (-3) * train_updir[0], 0);
	PrintCircle(train_pos[0] + (1.0) * cross_t + (-5) * train_dir[0] + (-3) * train_updir[0],0 );
	if(subcar != 0)
	for (int i = 1; i < subcar + 1; i++) {
		Pnt3f cross_t = train_dir[i] * train_updir[i];
		cross_t.normalize();
		cross_t = cross_t * 4.0f;
		//連結線
		glColor3f(0, 0, 0);
		//glLineWidth(20);
		glBegin(GL_LINES);
		glVertex3f(train_pos[i - 1].x + (-10) * train_dir[i - 1].x, train_pos[i - 1].y + (-10) * train_dir[i - 1].y, train_pos[i - 1].z + (-10) * train_dir[i - 1].z);
		glVertex3f(train_pos[i].x + (10) * train_dir[i].x, train_pos[i].y + (10) * train_dir[i].y, train_pos[i].z + (10) * train_dir[i].z);
		glEnd();
		//車廂
		//前
		PrintTextures(train_pos[i] + cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i],
			train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i], train_pos[i] + cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i]);
		//側
		PrintTextures(train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
			train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i]);
		PrintTextures(train_pos[i] + (1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
			train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i]);
		//上下
		PrintTextures(train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i],
			train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (8) * train_updir[i]);
		PrintTextures(train_pos[i] + (-1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
			train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (3) * train_dir[i] + (4) * train_dir[i] + (-3) * train_updir[i]);
		//後
		PrintTextures(train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
			train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i]);
		PrintCircle(train_pos[i] + (-1.0) * cross_t + (5) * train_dir[i] + (-3) * train_updir[i], i);
		PrintCircle(train_pos[i] + (-1.0) * cross_t + (-5) * train_dir[i] + (-3) * train_updir[i], i);
		PrintCircle(train_pos[i] + (1.0) * cross_t + (5) * train_dir[i] + (-3) * train_updir[i], i);
		PrintCircle(train_pos[i] + (1.0) * cross_t + (-5) * train_dir[i] + (-3) * train_updir[i], i);

		//	//前
		//PrintTextures(train_pos[i] + cross_t + (8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (8) * train_dir[i] + (-3) * train_updir[i],
		//	train_pos[i] + (-1.0) * cross_t + (8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + cross_t + (8) * train_dir[i] + (8) * train_updir[i]);
		////側
		//PrintTextures(train_pos[i] + (-1.0) * cross_t + (8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
		//	train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (5) * train_dir[i] + (8) * train_updir[i]);
		//PrintTextures(train_pos[i] + (1.0) * cross_t + (8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
		//	train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (1.0) * cross_t + (5) * train_dir[i] + (8) * train_updir[i]);
		////上下
		//PrintTextures(train_pos[i] + (-1.0) * cross_t + (8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i],
		//	train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (1.0) * cross_t + (8) * train_dir[i] + (8) * train_updir[i]);
		//PrintTextures(train_pos[i] + (-1.0) * cross_t + (8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
		//	train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (8) * train_dir[i] + (-3) * train_updir[i]);
		////後
		//PrintTextures(train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i], train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (-3) * train_updir[i],
		//	train_pos[i] + (1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i], train_pos[i] + (-1.0) * cross_t + (-8) * train_dir[i] + (8) * train_updir[i]);
	}
	//qt.x += cross_t.x;
	//qt.y += cross_t.y;
	//qt.z += cross_t.z;
	//	glColor3ub(0, 0, 0);
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z - 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z - 5);
	//	glEnd();
	//	
	//	glColor3ub(0, 0, 0);
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z + 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z + 5);
	//	glEnd();

	//	glColor3ub(0, 0, 0);
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z + 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z + 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z + 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z + 5);
	//	glEnd();


	//	glColor3ub(255, 255, 255);//旁邊
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z + 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x + 5, qt.y - 5, qt.z + 5);
	//	glEnd();

	//	glColor3ub(0, 0, 0);
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z + 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x + 5, qt.y + 5, qt.z + 5);
	//	glEnd();

	//	glColor3ub(0, 0, 0);
	//	glBegin(GL_QUADS);
	//	glTexCoord2f(0.0f, 0.0f);//左下
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 0.0f);//右下
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z - 5);
	//	glTexCoord2f(1.0f, 1.0f);//右上
	//	glVertex3f(qt.x - 5, qt.y - 5, qt.z + 5);
	//	glTexCoord2f(0.0f, 1.0f);//左上
	//	glVertex3f(qt.x - 5, qt.y + 5, qt.z + 5);
	//	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT3);
	Pnt3f trainss= train_pos[0] +  (20) * train_dir[0] + (8) * train_updir[0];
	GLfloat lightPosition4[] = { trainss.x, trainss.y, trainss.z, 0.0f };
	GLfloat red[] = { 1.0f, .0f, .0f, 1.0f };
	GLfloat white_light[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition4);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, white_light);
	


}
void TrainView:: resetArcball()
	//========================================================================
{
	// Set up the camera to look at the world
	// these parameters might seem magical, and they kindof are
	// a little trial and error goes a long way
	arcball.setup(this, 40, 250, .2f, .4f, 0);
}

void TrainView::drawVolcanic()
{

	m->render(false, false);
	n->render(false, false);
	v->render(false, false);

}
void TrainView::paintGL()
{
	
	//*********************************************************************
	//
	// * Set up basic opengl informaiton
	//
	//**********************************************************************
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Set up the view port
	glViewport(0,0,width(),height());
	// clear the window, be sure to clear the Z-Buffer too
	glClearColor(0,0,0.3f,0);
	
	// we need to clear out the stencil buffer since we'll use
	// it for shadows
	glClearStencil(0);
	glEnable(GL_DEPTH);

	// Blayne prefers GL_DIFFUSE
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// prepare for projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setProjection();		// put the code to set up matrices here

	//######################################################################
	// TODO: 
	// you might want to set the lighting up differently. if you do, 
	// we need to set up the lights AFTER setting up the projection
	//######################################################################
	// enable the lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// top view only needs one light
	if (this->camera == 1) {
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	} else {
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	

	//*********************************************************************
	//
	// * set the light parameters
	//
	//**********************************************************************
	GLfloat lightPosition1[]	= {0,1,1,0}; // {50, 200.0, 50, 1.0};
	GLfloat lightPosition2[]	= {1, 0, 0, 0};
	GLfloat lightPosition3[]	= {0, -1, 0, 0};
	
	GLfloat yellowLight[]		= {0.5f, 0.5f, .1f, 1.0};
	GLfloat whiteLight[]		= {1.0f, 1.0f, 1.0f, 1.0};
	GLfloat blueLight[]			= {.1f,.1f,.3f,1.0};
	GLfloat grayLight[]			= {.1f, .3f, .3f, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grayLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, yellowLight);

	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition3);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, blueLight);



	//*********************************************************************
	// now draw the ground plane
	//*********************************************************************
	setupFloor();
	glDisable(GL_LIGHTING);
	//drawFloor(200,10);
	glColor3ub(70, 70, 70);
	glBegin(GL_QUADS);
	glVertex3f(-200, 0, -200);
	glVertex3f(-200, 0, 200);
	glVertex3f(200, 0, 200);
	glVertex3f(200, 0, -200);
	glFlush();
	glEnd();
	/////////////////////////////////////////////////////
	if (!isLoad)
	{
		t_time.push_back(0.0f); 
		train_pos.push_back(Pnt3f(0, 0, 0));
		train_dir.push_back(Pnt3f(0, 0, 0));
		train_updir.push_back(Pnt3f(0, 1, 0));

		/*loadTexture2D("./Textures/grass.png", grass_ID);

		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D, grass_ID);
		glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex2f(-100, -100);
		glTexCoord2f(0, 0); glVertex2f(-100, 0);
		glTexCoord2f(0, 1); glVertex2f(100, 0);
		glTexCoord2f(1, 1); glVertex2f(100, -100);
		glEnd();
		glDisable(GL_TEXTURE_2D);*/
		isLoad = true;
	}
	/////////////////////////////////////////////////////
	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();

	drawStuff();
	
	// this time drawing is for shadows (except for top view)
	if (this->camera != 1) {
		glEnable(GL_DEPTH_TEST);
		setupShadows();
		drawStuff(true);
		unsetupShadows();
	}

	//Get modelview matrix
 	glGetFloatv(GL_MODELVIEW_MATRIX,ModelViewMatrex);
	//Get projection matrix
 	glGetFloatv(GL_PROJECTION_MATRIX,ProjectionMatrex);

	//Call triangle's render function, pass ModelViewMatrex and ProjectionMatrex
 	triangle->Paint(ProjectionMatrex,ModelViewMatrex);
    //----------
	//----------
	//use render texture
	water->Begin();
	//Active Texture
	glActiveTexture(GL_TEXTURE0);
	Textures[1]->bind();
	//Bind square's texture
	//we manage textures by Trainview class, so we modify square's render function
	//Active Texture

	//Bind square's texture
	//pass texture to shader
	water->shaderProgram->setUniformValue("Texture", 0);
	//Call square's render function, pass ModelViewMatrex and ProjectionMatrex
	water->Paint(ProjectionMatrex, ModelViewMatrex);
	water->End();

	square->Begin();
		//Active Texture
		glActiveTexture(GL_TEXTURE0);
		//Bind square's texture
		Textures[0]->bind();
		//pass texture to shader
		square->shaderProgram->setUniformValue("Texture",0);
		//Call square's render function, pass ModelViewMatrex and ProjectionMatrex
		square->Paint(ProjectionMatrex,ModelViewMatrex);
	square->End();
	
	
}

//************************************************************************
//
// * This sets up both the Projection and the ModelView matrices
//   HOWEVER: it doesn't clear the projection first (the caller handles
//   that) - its important for picking
//========================================================================
void TrainView::
setProjection()
//========================================================================
{
	// Compute the aspect ratio (we'll need it)
	float aspect = static_cast<float>(width()) / static_cast<float>(height());

	// Check whether we use the world camp
	if (this->camera == 0){
		arcball.setProjection(false);
		update();

	// Or we use the top cam
	}
	else if (this->camera == 1)
	{
		glEnable(GL_DEPTH_TEST);
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		} 
		else 
		{
			he = 110;
			wi = he * aspect;
		}

		// Set up the top camera drop mode to be orthogonal and set
		// up proper projection matrix
		glMatrixMode(GL_PROJECTION);
		glOrtho(-wi, wi, -he, he, 200, -200);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef(-90,1,0,0);
		update();
	}
	else if (this->camera == 2)
	{
		//train_pos[0] + cross_t + (8) * train_dir[0] + (4) * train_dir[0] + (-3) * train_updir[0]
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(120.0,1, 1, 2000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		Pnt3f cross_t = train_dir[0] * train_updir[0];
		cross_t.normalize();
		cross_t = cross_t * 4.0f;
		float test = 0.0;
		float testX = train_pos[0].x + cross_t.x + (10) * train_dir[0].x + (8) * train_updir[0].x;
		float testY = train_pos[0].y + cross_t.y + (10) * train_dir[0].y + (8) * train_updir[0].y;
		float testZ = train_pos[0].z + cross_t.z + (10) * train_dir[0].z + (8) * train_updir[0].z;
		gluLookAt(testX, testY, testZ,
			testX + train_dir[0].x*2   , testY + train_dir[0].y*2  , testZ + train_dir[0].z*2 ,
			train_updir[0].x, train_updir[0].y, train_updir[0].z);
		
		update();
	}

	// Or do the train view or other view here
	//####################################################################
	// TODO: 
	// put code for train view projection here!	
	//####################################################################
	else {
#ifdef EXAMPLE_SOLUTION
		trainCamView(this,aspect);
#endif
		update();
	}
}

//************************************************************************
//
// * this draws all of the stuff in the world
//
//	NOTE: if you're drawing shadows, DO NOT set colors (otherwise, you get 
//       colored shadows). this gets called twice per draw 
//       -- once for the objects, once for the shadows
//########################################################################
// TODO: 
// if you have other objects in the world, make sure to draw them
//########################################################################
//========================================================================

void TrainView::drawStuff(bool doingShadows)
{
	drawVolcanic();
	for (int z = 0; z < 4; z++)
	{
		Fire = z;
		DrawParticles();
		ProcessParticles();
	}
	Printtunnel();
	//--------------------------------------------------------------------
	// Draw the control points
	// don't draw the control points if you're driving 
	// (otherwise you get sea-sick as you drive through them)
	if (this->camera != 2) {
		for (size_t i = 0; i < this->m_pTrack->points.size(); ++i) {
			if (!doingShadows) {
				if (((int)i) != selectedCube)
					glColor3ub(240, 60, 60);
				else
					glColor3ub(240, 240, 30);
			}
			this->m_pTrack->points[i].draw();
		}
		update();
	}
	// draw the track
	//####################################################################
	// TODO: 
	// call your own track drawing code
	//####################################################################
	float percent = 1.0f / DIVIDE_LINE;
	spline_t type_spline = (spline_t) curve;
	//跑每個控制點
	track_lengh = 0.0; 
	for (size_t i = 0; i < m_pTrack->points.size(); ++i) {
		//positioon
		Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
		Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		track_lengh += pow(pow(cp_pos_p2.x - cp_pos_p1.x, 2) + pow(cp_pos_p2.y - cp_pos_p1.y, 2) + pow(cp_pos_p2.z - cp_pos_p1.z, 2), 0.5);
	}
	for (size_t i = 0; i < m_pTrack->points.size(); ++i)
	{
		// pos
		Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
		Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		Pnt3f cp_pos_p3 , cp_pos_p4;
		//如果有4個點
	
			 cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
			 cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;
		
		// orient
		Pnt3f cp_orient_p1 = m_pTrack->points[i].orient;
		Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		Pnt3f cp_orient_p3, cp_orient_p4;
		//如果有4個點
		
			 cp_orient_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].orient;
			 cp_orient_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].orient;
		
		glLineWidth(4);
		Pnt3f qt, qt0, qt1, orient_t;
		float t = 0;
		//選擇直線 曲線....
		
		switch (type_spline)
		{
			case spline_Linear:
				qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
				break;
			case spline_CardinalCubic:
				qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 1, 0);
				break;
			case spline_CubicB_Spline :
				qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 2, 0);
				break;
		} 
		//�e�u�a
		for (size_t j = 0; j < DIVIDE_LINE; j++) {
			qt0 = qt;
			switch (type_spline) {
			case spline_Linear:
				orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
				break;
			case spline_CardinalCubic:
				orient_t = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 1, 0);
				break;
			case spline_CubicB_Spline:
				orient_t = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 2, 0);
				break;
			}
			t += percent;
			
			switch (type_spline)
			{
			case spline_Linear:
				qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
				break;
			case spline_CardinalCubic:
				qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 1, t);
				
				break;
			case spline_CubicB_Spline:
				qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 2, t);
				break;
			}
			qt1 = qt;
			
			glLineWidth(0.5);
			glBegin(GL_LINES);
			//glVertex3f(qt0.x, qt0.y, qt0.z);
			//glVertex3f(qt1.x, qt1.y, qt1.z);
			if (!doingShadows) {
				glColor3ub(32, 32, 64);
			}
			// cross
			Pnt3f forward = Pnt3f(qt1.x - qt0.x, qt1.y - qt0.y, qt1.z - qt0.z);
			forward.normalize();
			Pnt3f cross_t = forward * orient_t;
			cross_t.normalize();
			cross_t = cross_t * 2.5f;
			orient_t.normalize();
			glVertex3f(qt0.x + cross_t.x, qt0.y + cross_t.y, qt0.z + cross_t.z);
			glVertex3f(qt1.x + cross_t.x, qt1.y + cross_t.y, qt1.z + cross_t.z);
			
			glVertex3f(qt0.x - cross_t.x, qt0.y - cross_t.y, qt0.z - cross_t.z);
			glVertex3f(qt1.x - cross_t.x, qt1.y - cross_t.y, qt1.z - cross_t.z);
			glEnd();
			//�e�伵
			Pnt3f Girder = (cross_t.y > 0) ? 3 * cross_t : (-3) * cross_t;//�٦V�q
			//if (j == 0) {
			//	if (!doingShadows)
			//		glColor3ub(130, 130, 130);
			//	//樑
			//	drawCube(qt0 + Pnt3f(-1, 0, -1) + Pnt3f(Girder.x, 0, Girder.z), qt0 + Pnt3f(1, 0, -1) + Pnt3f(Girder.x, 0, Girder.z), qt0 + Pnt3f(1, 0, 1), qt0 + Pnt3f(-1, 0, 1), qt0 + Pnt3f(-1, (cp_orient_p1.y < 0) ? 2 : -2, -1) + Pnt3f(Girder.x, 0, Girder.z));
			//	//柱
			//	drawCube(qt0 + Pnt3f(-1, 0, -1) + Pnt3f(Girder.x, 0, Girder.z), qt0 + Pnt3f(1, 0, -1) + Pnt3f(Girder.x, 0, Girder.z), qt0 + Pnt3f(1, 0, 1) + Pnt3f(Girder.x, 0, Girder.z), qt0 + Pnt3f(-1, 0, 1) + Pnt3f(Girder.x, 0, Girder.z), Pnt3f(qt.x - 1, 0, qt.z - 1) + Pnt3f(Girder.x, 0, Girder.z));
			//}
			//��y�D
			track_cumulative_dist += pow(pow(qt1.x - qt0.x, 2) + pow(qt1.y - qt0.y, 2) + pow(qt1.z - qt0.z, 2), 0.5);
			if (track_cumulative_dist >= track_spacing) {
				track_cumulative_dist -= track_spacing;
				if (track == 1) {
					glBegin(GL_QUADS);
					if (!doingShadows)
						glColor3ub(64, 32, 0);//咖啡
					glVertex3f(qt0.x + cross_t.x * 1.6 - forward.x * 1.5, qt0.y + cross_t.y * 1.6 - forward.y * 1.5, qt0.z + cross_t.z * 1.6 - forward.z * 1.5);
					glVertex3f(qt0.x + cross_t.x * 1.6 + forward.x * 1.5, qt0.y + cross_t.y * 1.6 + forward.y * 1.5, qt0.z + cross_t.z * 1.6 + forward.z * 1.5);
					glVertex3f(qt0.x - cross_t.x * 1.6 + forward.x * 1.5, qt0.y - cross_t.y * 1.6 + forward.y * 1.5, qt0.z - cross_t.z * 1.6 + forward.z * 1.5);
					glVertex3f(qt0.x - cross_t.x * 1.6 - forward.x * 1.5, qt0.y - cross_t.y * 1.6 - forward.y * 1.5, qt0.z - cross_t.z * 1.6 - forward.z * 1.5);
					glEnd();
				}
			}
			//ROAD
			if (track == 2) {
				glBegin(GL_QUADS);
				if (!doingShadows)
					glColor3ub(80, 80, 80);
				glVertex3f(qt0.x + cross_t.x * 2.0, qt0.y + cross_t.y * 2.0, qt0.z + cross_t.z * 2.0);
				glVertex3f(qt1.x + cross_t.x * 2.0, qt1.y + cross_t.y * 2.0, qt1.z + cross_t.z * 2.0);
				glVertex3f(qt1.x - cross_t.x * 2.0, qt1.y - cross_t.y * 2.0, qt1.z - cross_t.z * 2.0);
				glVertex3f(qt0.x - cross_t.x * 2.0, qt0.y - cross_t.y * 2.0, qt0.z - cross_t.z * 2.0);
				glEnd();
			}
			//glEnd();
		}
		track_cumulative_dist = track_spacing;
		
	}
#ifdef EXAMPLE_SOLUTION
	drawTrack(this, doingShadows);
#endif
	size_t i;
	Pnt3f qt, qt2, orient_t;
	//positioon
	Pnt3f cp_pos_p1;
	Pnt3f cp_pos_p2;
	Pnt3f cp_pos_p3;
	Pnt3f cp_pos_p4;
	// orient
	Pnt3f cp_orient_p1;
	Pnt3f cp_orient_p2;
	Pnt3f cp_orient_p3;
	Pnt3f cp_orient_p4;
	for (int num = 0; num < subcar + 1; num++)
	{
		if (isrun && num == 0)
		{
			if (clock() - lastRedraw > CLOCKS_PER_SEC / trainSpeed) {
				//	Arc Length Parameterization 
				float time = t_time[0];
				time *= m_pTrack->points.size();
				for (i = 0; time > 1; time -= 1)
					i++;
				/*//positioon
				Pnt3f cp_pos_p1;
				Pnt3f cp_pos_p2;*/
				switch (type_spline) {
				case spline_Linear:
					cp_pos_p1 = m_pTrack->points[i].pos;
					cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
					break;
				case spline_CardinalCubic:
					cp_pos_p1 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
					cp_pos_p2 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
					break;
				case spline_CubicB_Spline:
					cp_pos_p1 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
					cp_pos_p2 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
					break;
				}
				float l = pow(pow(cp_pos_p2.x - cp_pos_p1.x, 2) + pow(cp_pos_p2.y - cp_pos_p1.y, 2) + pow(cp_pos_p2.z - cp_pos_p1.z, 2), 0.5);
				float rate = track_lengh / m_pTrack->points.size() / l;
				//
				t_time[0] += (1.0 / m_pTrack->points.size() / DIVIDE_LINE) * rate;
				//
				if (t_time[0] > 1.0f)
					t_time[0] -= 1.0f;
				if (t_time[0] < 0.0f)
					t_time[0] += 1.0f;
				lastRedraw = clock();
			}
		}
		else if(num != 0)
		{
			t_time[num] = t_time[num - 1];
			float multiple = 1;
			switch (type_spline) {
			case spline_Linear:
				multiple = 4;
				break;
			case spline_CardinalCubic:
				multiple = 18;
				break;
			case spline_CubicB_Spline:
				multiple = 18;
				break;
			}
			Pnt3f qtTemp;
			for (float d = 0.0; d < 15.9 * multiple;) {
				t_time[num] += -0.01f * 280.0 / track_lengh;
				if (t_time[num] < 0.0f)
					t_time[num] += 1.0f;
				float time = t_time[num];
				time *= m_pTrack->points.size();
				for (i = 0; time > 1; time -= 1)
					i++;
				Pnt3f qt;
				//positioon
				Pnt3f cp_pos_p1 = m_pTrack->points[i].pos;
				Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
				Pnt3f cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
				Pnt3f cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;
				//dt
				float t = time;
				int intt = time;
				t -= intt;

				switch (type_spline) {
				case spline_Linear:
					qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
					break;
				case spline_CardinalCubic:
					qt = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 1, t);
					break;
				case spline_CubicB_Spline:
					qt = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 2, t);
					break;
				}
					d += pow(pow(train_pos[num - 1].x - qt.x, 2) + pow(train_pos[num - 1].y - qt.y, 2) + pow(train_pos[num - 1].z - qt.z, 2), 0.5f);
			}
		}
		//temp data
		float time = t_time[num];
		//Pnt3f qt, qt2, orient_t;
		time *= m_pTrack->points.size();
		for (i = 0; time > 1; time -= 1)
			i++;
		//positioon
		cp_pos_p1 = m_pTrack->points[i].pos;
		cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;
		cp_pos_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].pos;
		cp_pos_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].pos;
		// orient
		cp_orient_p1 = m_pTrack->points[i].orient;
		cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		cp_orient_p3 = m_pTrack->points[(i + 2) % m_pTrack->points.size()].orient;
		cp_orient_p4 = m_pTrack->points[(i + 3) % m_pTrack->points.size()].orient;
		//dt
		float t = time;
		int intt = time;
		t -= intt;
		float T = t;
		switch (type_spline) {
		case spline_Linear:
			qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
			orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
			t += 1.0f / DIVIDE_LINE;
			qt2 = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
			break;
		case spline_CardinalCubic:
			qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 1, T);
			orient_t = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 1, T);
			T = t + 1.0f / DIVIDE_LINE;
			qt2 = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 1, T);
			break;
		case spline_CubicB_Spline:
			qt = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 2, T);
			orient_t = GMT(2, cp_orient_p1, cp_orient_p2, cp_orient_p3, cp_orient_p4, 2, T);
			T = t + 1.0f / DIVIDE_LINE;
			qt2 = GMT(2, cp_pos_p1, cp_pos_p2, cp_pos_p3, cp_pos_p4, 2, T);
			break;
		}
		//set train
		orient_t.normalize();
		train_updir[num] = orient_t;
		orient_t = orient_t * 5;
		train_pos[num] = qt + orient_t;
		train_dir[num] = qt2 + (-1.0 * qt);
		train_dir[num].normalize();
	}
	/////////////////////////////////////draw Pillar
	glColor3f(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	for (int e = 0; e <= 360; e += 15)
	{
		float p = e * 3.14 / 180;
		glVertex3f(100 + sin(p),0 , 155 + cos(p));
		glVertex3f(84 + sin(p), 25,164 + cos(p));
	}
	glFlush();
	glEnd();
	glColor3f(0, 0, 0);
	glBegin(GL_QUAD_STRIP);
	for (int e = 0; e <= 360; e += 15)
	{
		float p = e * 3.14 / 180;
		glVertex3f(72 + sin(p), 0, 182 + cos(p));
		glVertex3f(84 + sin(p), 25, 164 + cos(p));
	}
	glFlush();
	glEnd();
	////////////////////////////////////
	
	// draw the train
	drawTrain(0);
	//####################################################################
	// TODO: 
	//	call your own train drawing code
	//####################################################################
#ifdef EXAMPLE_SOLUTION
	// don't draw the train if you're looking out the front window
	if (!tw->trainCam->value())
		drawTrain(this, doingShadows);
#endif

}

void TrainView::
	doPick(int mx, int my)
	//========================================================================
{
	// since we'll need to do some GL stuff so we make this window as 
	// active window
	makeCurrent();

	// get the viewport - most reliable way to turn mouse coords into GL coords
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity ();

	gluPickMatrix((double)mx, (double)(viewport[3] - my), 
		5, 5, viewport);

	// now set up the projection
	setProjection();

	// now draw the objects - but really only see what we hit
	GLuint buf[100];
	glSelectBuffer(100,buf);
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);


	// draw the cubes, loading the names as we go
	for(size_t i=0; i<m_pTrack->points.size(); ++i) {
		glLoadName((GLuint) (i+1));
		m_pTrack->points[i].draw();
	}

	// go back to drawing mode, and see how picking did
	int hits = glRenderMode(GL_RENDER);
	if (hits) {
		// warning; this just grabs the first object hit - if there
		// are multiple objects, you really want to pick the closest
		// one - see the OpenGL manual 
		// remember: we load names that are one more than the index
		selectedCube = buf[3]-1;
	} else // nothing hit, nothing selected
		selectedCube = -1;
}

void AddParticle(Particle ex)
{
	pParticle p;
	p = new Particle;//new particle   
	p->pNext = NULL;
	p->pPrev = NULL;
	p->b = ex.b;
	p->g = ex.g;
	p->r = ex.r;
	p->fade = ex.fade;
	p->life = ex.life;
	p->size = ex.size;
	p->xpos = ex.xpos;
	p->ypos = ex.ypos;
	p->zpos = ex.zpos;
	p->xspeed = ex.xspeed;
	p->yspeed = ex.yspeed;
	p->zspeed = ex.zspeed;
	p->AddCount = ex.AddCount;
	p->AddSpeed = ex.AddSpeed;
	p->bAddParts = ex.bAddParts;
	p->bFire = ex.bFire;
	p->nExpl = ex.nExpl;
	if (Fire == 0)
	{
		if (!Particles)//當目前的Particle列表為空時
		{
			Particles = p;
			return;
		}
		else
		{
			Particles->pPrev = p;//插入Particle   
			p->pNext = Particles;
			Particles = p;
		}
	}
	else if (Fire == 1)
	{
		if (!Part2)//當目前的Particle列表為空時
		{
			Part2 = p;
			return;
		}
		else
		{
			Part2->pPrev = p;//插入Particle   
			p->pNext = Part2;
			Part2 = p;
		}
	}
	else if (Fire == 2)
	{
		if (!Part3)//當目前的Particle列表為空時
		{
			Part3 = p;
			return;
		}
		else
		{
			Part3->pPrev = p;//插入Particle   
			p->pNext = Part3;
			Part3 = p;
		}
	}
	else if (Fire == 3)
	{
		if (!Part4)//當目前的Particle列表為空時
		{
			Part4 = p;
			return;
		}
		else
		{
			Part4->pPrev = p;//插入Particle   
			p->pNext = Part4;
			Part4 = p;
		}
	}

}

void DeleteParticle(pParticle* p)
{
	if (!(*p))//假如Particle列表為空
		return;
	if (!(*p)->pNext && !(*p)->pPrev)//假如只有一個Particle，直接删除  
	{
		delete(*p);
		*p = NULL;
		return;
	}
	pParticle tmp;
	if (!(*p)->pPrev)//假如是首節點  
	{
		tmp = (*p);
		*p = (*p)->pNext;
		if (Fire == 0)Particles = *p;
		else if (Fire == 1)Part2 = *p;
		else if (Fire == 2)Part3 = *p;
		else if (Fire == 3)Part4 = *p;
		(*p)->pPrev = NULL;
		delete tmp;
		return;
	}
	if (!(*p)->pNext)//末節點  
	{
		(*p)->pPrev->pNext = NULL;
		delete(*p);
		*p = NULL;
		return;
	}
	else
	{
		//一般情况  
		tmp = (*p);
		(*p)->pPrev->pNext = (*p)->pNext;
		(*p)->pNext->pPrev = (*p)->pPrev;
		*p = (*p)->pNext;
		delete tmp;
	}
}

void InitParticle(Particle& ep)
{
	ep.b = float(rand() % 100) / 60.0f;//顏色隨機
	ep.g = float(rand() % 100) / 60.0f;
	ep.r = float(rand() % 100) / 60.0f;
	ep.life = 1.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1;//大小  
	if (Fire == 0)
	{
		ep.xpos = 120.0f + float(rand() % 301) / 10.0f;//位置 
		ep.ypos = 8.0f;
		ep.zpos = -140.0f + float(rand() % 201) / 10.0f;
	}
	else if (Fire == 1)
	{
		ep.xpos = 40.0f + float(rand() % 301) / 10.0f;//位置 
		ep.ypos = 8.0f;
		ep.zpos = -140.0f + float(rand() % 201) / 10.0f;
	}
	else if (Fire == 2)
	{
		ep.xpos = -40.0f + float(rand() % 301) / 10.0f;//位置 
		ep.ypos = 8.0f;
		ep.zpos = -140.0f + float(rand() % 201) / 10.0f;
	}
	else if (Fire == 3)
	{
		ep.xpos = -120.0f + float(rand() % 301) / 10.0f;//位置 
		ep.ypos = 8.0f;
		ep.zpos = -140.0f + float(rand() % 201) / 10.0f;
	}
	if (!int(ep.xpos))//x方向速度(z方向相同)
	{
		ep.xspeed = 0.0f;
	}
	else
	{
		if (ep.xpos < 0)
		{
			ep.xspeed = (rand() % int(-ep.xpos)) / 1500.0f;
		}
		else
		{
			ep.xspeed = -(rand() % int(-ep.xpos)) / 1500.0f;
		}
	}
	ep.yspeed = 0.04f + float(rand() % 11) / 1000.0f;//y方向速度(向上)
	ep.bFire = 1;
	ep.nExpl = 1 + rand() % 6;//粒子效果  
	ep.bAddParts = 1;//設定有尾巴 
	ep.AddCount = 0.0f;
	ep.AddSpeed = 0.2f;
	nOfFires[Fire]++;//粒子數+1 
	AddParticle(ep);//加入粒子列表    
}

void Explosion1(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 30; i++)
	{
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion2(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 100; i++)
	{
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.0f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion3(Particle* par)
{
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 10; i++) {
		float angle = float(rand() % 360) * PIAsp;
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle) * 0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle) * 0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion4(Particle* par) {
	Particle ep;
	float PIAsp = 3.1415926 / 180;
	for (int i = 0; i < 10; i++) {
		float angle = float(rand() % 360) * PIAsp;
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 1.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = (float)sin(angle) * 0.01f;
		ep.yspeed = 0.01f + float(rand() % 11) / 1000.0f;
		ep.zspeed = (float)cos(angle) * 0.01f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 1;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.2f;
		AddParticle(ep);
	}
}

void Explosion5(Particle* par) {
	Particle ep;
	for (int i = 0; i < 10; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion6(Particle* par) {
	Particle ep;
	for (int i = 0; i < 10; i++) {
		ep.b = float(rand() % 100) / 60.0f;
		ep.g = float(rand() % 100) / 60.0f;
		ep.r = float(rand() % 100) / 60.0f;
		ep.life = 0.8f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.8f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.yspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.zspeed = 0.01f - float(rand() % 21) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 7;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void Explosion7(Particle* par) {
	Particle ep;
	for (int i = 0; i < 10; i++) {
		ep.b = par->b;
		ep.g = par->g;
		ep.r = par->r;
		ep.life = 0.5f;
		ep.fade = 0.01f + float(rand() % 31) / 10000.0f;
		ep.size = 0.6f;
		ep.xpos = par->xpos;
		ep.ypos = par->ypos;
		ep.zpos = par->zpos;
		ep.xspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.yspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.zspeed = 0.02f - float(rand() % 41) / 1000.0f;
		ep.bFire = 0;
		ep.nExpl = 0;
		ep.bAddParts = 0;
		ep.AddCount = 0.0f;
		ep.AddSpeed = 0.0f;
		AddParticle(ep);
	}
}

void DrawParticles() {
	pParticle par;
	int t = fire;
	if (Fire == 0)par = Particles;
	else if (Fire == 1)par = Part2;
	else if (Fire == 2)par = Part3;
	else if (Fire == 3)par = Part4;
	while(par)
	{
		glColor4f(par->r, par->g, par->b, par->life);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1, 1);
		glVertex3f(par->xpos + par->size, par->ypos + par->size, par->zpos);
		glTexCoord2d(0, 1);
		glVertex3f(par->xpos - par->size, par->ypos + par->size, par->zpos);
		glTexCoord2d(1, 0);
		glVertex3f(par->xpos + par->size, par->ypos - par->size, par->zpos);
		glTexCoord2d(0, 0);
		glVertex3f(par->xpos - par->size, par->ypos - par->size, par->zpos);
		glEnd();
		par = par->pNext;
	}
}

void ProcessParticles()
{
	int t = fire;
	Tick1[Fire]  = Tick2[Fire];
	Tick2[Fire] = GetTickCount();
	DTick[Fire] = float(Tick2[Fire] - Tick1[Fire]);
	DTick[Fire] *= 0.5f;
	Particle ep;
	if (nOfFires[Fire] < MAX_FIRES)
	{
		InitParticle(ep);
	}
	pParticle par;
	if (Fire == 0)par = Particles;
	else if (Fire == 1)par = Part2;
	else if (Fire == 2)par = Part3;
	else if (Fire == 3)par = Part4;
	while(par) {
		par->life -= par->fade * (float(DTick[Fire]) * 0.1f);//Particle壽命衰減 
		if (par->life <= 0.05f) {//當壽命小於一定值          
			if (par->nExpl) {//爆炸效果
				switch (par->nExpl) {
				case 1:
					Explosion1(par);
					break;
				case 2:
					Explosion2(par);
					break;
				case 3:
					Explosion3(par);
					break;
				case 4:
					Explosion4(par);
					break;
				case 5:
					Explosion5(par);
					break;
				case 6:
					Explosion6(par);
					break;
				case 7:
					Explosion7(par);
					break;
				default:
					break;
				}
			}
			if (par->bFire)
				nOfFires[Fire]--;
			DeleteParticle(&par);
		}
		else{
				par->xpos += par->xspeed * DTick[Fire] * 0.2;
				par->ypos += par->yspeed * DTick[Fire];
				par->zpos += par->zspeed * DTick[Fire] * 0.5;
				par->yspeed -= grav * DTick[Fire];
				if (par->bAddParts) {//假如有尾巴  
					par->AddCount += 0.01f * DTick[Fire];//AddCount變化愈慢，尾巴粒子愈小  
					if (par->AddCount > par->AddSpeed) {//AddSpeed愈大，尾巴粒子愈小  
							par->AddCount = 0;
							ep.b = par->b;
							ep.g = par->g;
							ep.r = par->r;
							ep.life = par->life * 0.9f;//壽命變短  
							ep.fade = par->fade * 7.0f;//衰减快一些  
							ep.size = 0.6f;//粒子尺寸小一些  
							ep.xpos = par->xpos;
							ep.ypos = par->ypos;
							ep.zpos = par->zpos;
							ep.xspeed = 0.0f;
							ep.yspeed = 0.0f;
							ep.zspeed = 0.0f;
							ep.bFire = 0;
							ep.nExpl = 0;
							ep.bAddParts = 0;//尾巴粒子没有尾巴  
							ep.AddCount = 0.0f;
							ep.AddSpeed = 0.0f;
							AddParticle(ep);
					}
				}
			par = par->pNext;//更新下一粒子    
		}
	}
}

void TrainView::PrintTextures(Pnt3f p00, Pnt3f p10, Pnt3f p11, Pnt3f p01) {
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3d(p00.x, p00.y, p00.z);
	glTexCoord2f(1.0f, 0.0f); glVertex3d(p10.x, p10.y, p10.z);
	glTexCoord2f(1.0f, 1.0f); glVertex3d(p11.x, p11.y, p11.z);
	glTexCoord2f(0.0f, 1.0f); glVertex3d(p01.x, p01.y, p01.z);
	glEnd();
	//glColor3f(1, 1, 1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, textures[index]);
	//glBegin(GL_QUADS);
	//glTexCoord2d(0, 0); glVertex3d(p00.x, p00.y, p00.z);
	//glTexCoord2d(1, 0); glVertex3d(p10.x, p10.y, p10.z);
	//glTexCoord2d(1, 1); glVertex3d(p11.x, p11.y, p11.z);
	//glTexCoord2d(0, 1); glVertex3d(p01.x, p01.y, p01.z);
	//glEnd();
	//glDisable(GL_TEXTURE_2D);
}

void TrainView::PrintCircle(Pnt3f p11,int num)
{
	glColor3f(255, 0, 0);
	//glPointSize(50.0f);
	//glEnable(GL_POINT_SMOOTH);//圓形
	//glBegin(GL_POINTS);
	//glVertex3f(p00.x, p00.y, p00.z);
	//glEnd();
	//glDisable(GL_POINT_SMOOTH);//！！！！！
	//glFlush();
	//glPushMatrix();
	//glRotatef(5.0, 0.0, 1.0, 0.0);
	Pnt3f cross_t = train_dir[num] * train_updir[num];
	cross_t.normalize();
	glBegin(GL_POLYGON);
	glEnable(GL_POLYGON_SMOOTH);
	for (int i = 0; i < 100; i++)
	{
		if(cross_t.z < 0 && cross_t.x < 0)
			glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * i) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * i) , p11.z + cross_t.z  * cos(2 * 3.14159 / 100 * i));
		else if(cross_t.z > 0 && cross_t.x < 0)
			glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * i) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * i) , p11.z + cross_t.z  * cos(2 * 3.14159 / 100 * i));
		else if (cross_t.z < 0 && cross_t.x > 0)
			glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * i) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * i) , p11.z - cross_t.z  * cos(2 * 3.14159 / 100 * i));
		else if (cross_t.z > 0 && cross_t.x > 0)
			glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * i) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * i) , p11.z - cross_t.z  * cos(2 * 3.14159 / 100 * i));
	}	
	glDisable(GL_POLYGON_SMOOTH);//！！！！！
	glFlush();
	glEnd();

	glColor3f(255, 255, 255);
	glBegin(GL_LINES);
	if (cross_t.z < 0 && cross_t.x < 0 || cross_t.z > 0 && cross_t.x < 0)
	{
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * trainWheelcount) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * trainWheelcount), p11.z + cross_t.z * cos(2 * 3.14159 / 100 * trainWheelcount));
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 50)) , p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 50)), p11.z + cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 50)));

		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 25)), p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 25)), p11.z + cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 25)));
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 75)), p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 75)), p11.z + cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 75)));
	}
	else
	{
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * trainWheelcount), p11.y + 2.0 * sin(2 * 3.14159 / 100 * trainWheelcount), p11.z - cross_t.z * cos(2 * 3.14159 / 100 * trainWheelcount));
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 50)), p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 50)), p11.z - cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 50)));

		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 25)), p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 25)), p11.z - cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 25)));
		glVertex3f(p11.x + 1.0 * cos(2 * 3.14159 / 100 * (trainWheelcount + 75)), p11.y + 2.0 * sin(2 * 3.14159 / 100 * (trainWheelcount + 75)), p11.z - cross_t.z * cos(2 * 3.14159 / 100 * (trainWheelcount + 75)));
	}
	glFlush();
	glEnd();
	
	//for (int i = 0; i < 100; i++)
	//{
	//	float plus = 0;
	//	if (i < 25)
	//		plus = (p01.z - p00.z) / 25;
	//	else if (i < 50 && i >= 25)
	//		plus = (p11.z - p01.z) / 25;
	//	else if (i >= 50 && i < 75)
	//		plus = (p01.z - p11.z) / 25;
	//	else
	//		plus = (p00.z - p01.z) / 25;
	//	glVertex3f(p00.x + 5.0 * cos(2 * 3.14159 / 100 * i), p00.y + 5.0 * sin(2 * 3.14159 / 100 * i), plus * (i % 25) + (p00.z + p01.z) / 2);//計算坐標
	//}
	//for (int i = 0; i < 100; i++)
	//{
	//	if(i<25)
	//		glVertex3f(p00.x + 5.0 * cos(2 * 3.14159 / 100 * i), p00.y + 5.0 * sin(2 * 3.14159 / 100 * i), p00.z);//計算坐標
	//	else if(i<50 && i>=25)
	//		glVertex3f(p10.x + 5.0 * cos(2 * 3.14159 / 100 * i), p10.y + 5.0 * sin(2 * 3.14159 / 100 * i), p10.z);//計算坐標	
	//	else if(i>=50 && i<75)
	//		glVertex3f(p11.x + 5.0 * cos(2 * 3.14159 / 100 * i), p11.y + 5.0 * sin(2 * 3.14159 / 100 * i), p11.z);//計算坐標
	//	else
	//		glVertex3f(p01.x + 5.0 * cos(2 * 3.14159 / 100 * i), p01.y + 5.0 * sin(2 * 3.14159 / 100 * i), p01.z);//計算坐標
	//}

}

void TrainView::Printtunnel()
{
	int color[7][3] = { {255,0,0},{255,165,0},{255,255,0},{0,255,0},{0,191,255},{0,0,255},{106,90,205} };
	for (int i = 0; i < 15; i++)
	{
		float num = 5 * i;
		float value = 5 * (i + 1);
		if(i%2 == 0)
			glColor3ub(0, 0, 0);
		else
			glColor3ub(color[i/2][0], color[i / 2][1], color[i / 2][2]);
		glBegin(GL_QUADS);
		glVertex3f(-20 + num, 0, -80);
		glVertex3f(-20 + num, 30, -80);
		glVertex3f(-20 + value, 30, -80);
		glVertex3f(-20 + value, 0, -80);

		glVertex3f(-20 + num, 0, -60);
		glVertex3f(-20 + num, 30, -60);
		glVertex3f(-20 + value, 30, -60);
		glVertex3f(-20 + value, 0, -60);

		glVertex3f(-20 + value, 30, -60);
		glVertex3f(-20 + num, 30, -60);
		glVertex3f(-20 + num, 30, -80);
		glVertex3f(-20 + value, 30, -80);
		glEnd();
	}

}

void TrainView::loadTexture2D(QString str, GLuint&)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	QImage img(str);
	QImage opengl_grass = QGLWidget::convertToGLFormat(img);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, opengl_grass.width(), opengl_grass.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, opengl_grass.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glDisable(GL_TEXTURE_2D);
}





