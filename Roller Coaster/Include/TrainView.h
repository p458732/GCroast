#ifndef TRAINVIEW_H  
#define TRAINVIEW_H  
#include <QGLWidget> 
#include <QtGui>  
#include <QtOpenGL>  
#include <GL/GLU.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib") 
#include "Utilities/ArcBallCam.H"
#include "Utilities/3DUtils.H"
#include "Track.H"
#include "Triangle.h"
#include "Square.h"
#include "Model.h"
#include "3DSLoader.h"
#include "Water.h"

typedef struct tag_PARTICLE {
	GLfloat xpos;//(xpos,ypos,zpos)為particle的position
	GLfloat ypos;
	GLfloat zpos;
	GLfloat xspeed;//(xspeed,yspeed,zspeed)為particle的speed 
	GLfloat yspeed;
	GLfloat zspeed;
	GLfloat r;//(r,g,b)為particle的color
	GLfloat g;
	GLfloat b;
	GLfloat life;// particle的壽命 
	GLfloat fade;// particle的衰減速度
	GLfloat size;// particle的大小  
	GLbyte bFire;
	GLbyte nExpl;//哪種particle效果  
	GLbyte bAddParts;// particle是否含有尾巴
	GLfloat AddSpeed;//尾巴粒子的加速度  
	GLfloat AddCount;//尾巴粒子的增加量  
	tag_PARTICLE* pNext;//下一particle 
	tag_PARTICLE* pPrev;//上一particle   
} Particle, * pParticle;


class CTrack;

//#######################################################################
// TODO
// You might change the TrainView in order to add different objects to
// be drawn, or adjust the lighting, or ...
//#######################################################################


class TrainView : public QGLWidget, protected QOpenGLFunctions_4_3_Core
{  
	Q_OBJECT  
public:  
	friend class AppMain;
	explicit TrainView(QWidget *parent = 0);  
	~TrainView();  

public:

	// overrides of important window things
	//virtual int handle(int);
	virtual void paintGL();
	//Model* m = new Model("D://volcano02_subdiv_02.obj", 0, Point3d(0, 0, 0));火山
	Model* m = new Model("./volcano02_subdiv_02.obj", 500, Point3d(250000, 20000, 250000));
	Model* n = new Model("./gargoyle.obj", 70, Point3d(-500, 90, -650));
	Model* v = new Model("./IronMan.obj", 70, Point3d(300, 120, -300));
	// all of the actual drawing happens in this routine
	// it has to be encapsulated, since we draw differently if
	// we're drawing shadows (no colors, for example)
	void drawStuff(bool doingShadows=false);

	// setup the projection - assuming that the projection stack has been
	// cleared for you
	void setProjection();

	// Reset the Arc ball control
	void resetArcball();
	void drawVolcanic();
	// pick a point (for when the mouse goes down)
	void doPick(int mx, int my);

	void initializeGL();
	void initializeTexture();

	void PrintTextures(Pnt3f p00, Pnt3f p10, Pnt3f p11, Pnt3f p01);
	void PrintCircle(Pnt3f p00 , int num);
	void Printtunnel();
	void loadTexture2D(QString, GLuint&);
public:
	ArcBallCam		arcball;			// keep an ArcBall for the UI
	int				selectedCube;  // simple - just remember which cube is selected

	CTrack*			m_pTrack;		// The track of the entire scene
	float track_cumulative_dist = 8.0f;
	float track_spacing = 8.0;
	int camera;
	int curve;
	int track;
	bool isrun;
	float track_lengh;
	float trainSpeed = 500;
	float tautau = 2;
	int subcar = 0;
	int trainWheelcount = 0;
	Pnt3f GMT(float tau, Pnt3f con1, Pnt3f con2, Pnt3f con3, Pnt3f con4, int type, float t);
	Triangle* triangle;
	Square* square;
	Water* water;
	GLfloat ProjectionMatrex[16];
	GLfloat ModelViewMatrex[16];
	QVector<QOpenGLTexture*> Textures;

	vector<float> t_time;
	unsigned long lastRedraw;
	vector<Pnt3f> train_pos;
	vector<Pnt3f> train_dir;
	vector<Pnt3f> train_updir;
	bool isLoad = false;
	unsigned int DIVIDE_LINE = 250;
	typedef enum {

		spline_Linear = 0,
		spline_CardinalCubic = 1,
		spline_CubicB_Spline = 2
	} spline_t;
	void drawTrain(float x);
	GLuint grass_ID;
};  
#endif // TRAINVIEW_H  