#include "TrainView.h"  
pParticle Particles;
UINT nOfFires;

UINT Tick1, Tick2( GetTickCount());  
float DTick(float(Tick2 - Tick1) );  
GLfloat grav(0.00003f); 
void DrawParticles();
void ProcessParticles();
GLuint    textureID;
#define MAX_PARTICLES 1000  
#define MAX_FIRES 5  

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
	square = new Square();
	//Initialize the square object
	square->Init();
	//Initialize texture 
	initializeTexture();
	
	
}
void TrainView::initializeTexture()
{
	//Load and create a texture for square;'stexture
	QOpenGLTexture* texture = new QOpenGLTexture(QImage("./Textures/Tupi.bmp"));
	Textures.push_back(texture);

}
Pnt3f TrainView::GMT(float tau, Pnt3f con1, Pnt3f con2, Pnt3f con3, Pnt3f con4, int type,float t)
{
	
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
	if (!isLoad)
	{
		qt = m_pTrack->points[0].pos;
		t_time = 0;
		isLoad = true;
	}
	else
	{
		qt = train_pos;
	}
		//pos
		//Pnt3f cp_pos_p1 = train_pos;
		//Pnt3f cp_pos_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].pos;

		// orient
		//Pnt3f cp_orient_p1 = train_dir;
		//Pnt3f cp_orient_p2 = m_pTrack->points[(i + 1) % m_pTrack->points.size()].orient;
		//switch (type_spline) {
		//case spline_Linear:
		//	// Linear
		//	qt = (1 - t) * cp_pos_p1 + t * cp_pos_p2;
		//	orient_t = (1 - t) * cp_orient_p1 + t * cp_orient_p2;
		//	break;
		//}
	Pnt3f cross_t = train_dir* train_updir;
	cross_t.normalize();
	cross_t = cross_t * 4.0f;

	//前
	PrintTextures(train_pos + cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir, train_pos + (-1.0) * cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir,
		train_pos + (-1.0) * cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir, train_pos + cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir);
	//側
	PrintTextures(train_pos + (-1.0) * cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir, train_pos + (-1.0) * cross_t + (-8) * train_dir + (-3) * train_updir,
		train_pos + (-1.0) * cross_t + (-8) * train_dir + (8) * train_updir, train_pos + (-1.0) * cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir);
	PrintTextures(train_pos + (1.0) * cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir, train_pos + (1.0) * cross_t + (-8) * train_dir + (-3) * train_updir,
		train_pos + (1.0) * cross_t + (-8) * train_dir + (8) * train_updir, train_pos + (1.0) * cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir);
	//上下
	PrintTextures(train_pos + (-1.0) * cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir, train_pos + (-1.0) * cross_t + (-8) * train_dir + (8) * train_updir,
		train_pos + (1.0) * cross_t + (-8) * train_dir + (8) * train_updir, train_pos + (1.0) * cross_t + (5) * train_dir + (4) * train_dir + (8) * train_updir);
	PrintTextures(train_pos + (-1.0) * cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir, train_pos + (-1.0) * cross_t + (-8) * train_dir + (-3) * train_updir,
		train_pos + (1.0) * cross_t + (-8) * train_dir + (-3) * train_updir, train_pos + (1.0) * cross_t + (8) * train_dir + (4) * train_dir + (-3) * train_updir);
	//後
	PrintTextures(train_pos + (-1.0) * cross_t + (-8) * train_dir + (-3) * train_updir, train_pos + (1.0) * cross_t + (-8) * train_dir + (-3) * train_updir,
		train_pos + (1.0) * cross_t + (-8) * train_dir + (8) * train_updir, train_pos + (-1.0) * cross_t + (-8) * train_dir + (8) * train_updir);

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
	GLfloat grayLight[]			= {.3f, .3f, .3f, 1.0};

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
	drawFloor(200,10);


	//*********************************************************************
	// now draw the object and we need to do it twice
	// once for real, and then once for shadows
	//*********************************************************************
	glEnable(GL_LIGHTING);
	setupObjects();

	drawStuff();
	
	// this time drawing is for shadows (except for top view)
	if (this->camera != 1) {
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
    
	//we manage textures by Trainview class, so we modify square's render function
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
	}else if (this->camera == 1) {
		float wi, he;
		if (aspect >= 1) {
			wi = 110;
			he = wi / aspect;
		} 
		else {
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
	DrawParticles();
	ProcessParticles();
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
	track_lengh = 0.0; //printf("X:%f Y:%f Z:%f \n\n", m_pTrack->points[0].pos.x, m_pTrack->points[0].pos.y, m_pTrack->points[0].pos.z);
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
			glEnd();

			

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
	if (isrun)
	{
		if (clock() - lastRedraw > CLOCKS_PER_SEC / 15000) {
			//	Arc Length Parameterization 
			float time = t_time;
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
				t_time += (1.0 / m_pTrack->points.size() / DIVIDE_LINE) * rate;
			//
			if (t_time > 1.0f)
				t_time -= 1.0f;
			if (t_time < 0.0f)
				t_time += 1.0f;
			lastRedraw = clock();
		}
	}
	//temp data
	float time = t_time;
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
	train_updir = orient_t;
	orient_t = orient_t * 5;
	train_pos = qt + orient_t;
	train_dir = qt2 + (-1.0 * qt);
	train_dir.normalize();
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
		Particles = *p;
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

void DeleteAll(pParticle* Part)
{
	while ((*Part))
		DeleteParticle(Part);
}

void InitParticle(Particle& ep)
{
	ep.b = float(rand() % 100) / 60.0f;//顏色隨機
	ep.g = float(rand() % 100) / 60.0f;
	ep.r = float(rand() % 100) / 60.0f;
	ep.life = 1.0f;//初始壽命
	ep.fade = 0.005f + float(rand() % 21) / 10000.0f;//衰减速度
	ep.size = 1;//大小  
	ep.xpos = 80.0f + float(rand() % 301) / 10.0f;//位置 
	ep.ypos = 5.0f;
	ep.zpos = -100.0f + float(rand() % 201) / 10.0f;
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
	nOfFires++;//粒子數+1 
	AddParticle(ep);//加入粒子列表    
}

void Explosion1(Particle* par)
{
	Particle ep;
	for (int i = 0; i < 100; i++)
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
	for (int i = 0; i < 1000; i++)
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
	for (int i = 0; i < 30; i++) {
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
	for (int i = 0; i < 30; i++) {
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
	for (int i = 0; i < 30; i++) {
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
	for (int i = 0; i < 100; i++) {
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
	par = Particles;
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
	Tick1  = Tick2;
	Tick2  = GetTickCount();
	DTick  = float(Tick2 - Tick1);
	DTick *= 0.5f;
	Particle ep;
	if (nOfFires < MAX_FIRES)
	{
		InitParticle(ep);
	}
	pParticle par;
	par = Particles;
	while(par) {
		par->life -= par->fade * (float(DTick) * 0.1f);//Particle壽命衰減 
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
				nOfFires--;
			DeleteParticle(&par);
		}
		else{
				par->xpos += par->xspeed * DTick;
				par->ypos += par->yspeed * DTick;
				par->zpos += par->zspeed * DTick;
				par->yspeed -= grav * DTick;
				if (par->bAddParts) {//假如有尾巴  
					par->AddCount += 0.01f * DTick;//AddCount變化愈慢，尾巴粒子愈小  
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
}





