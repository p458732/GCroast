
#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtOpenGL/QtOpenGL> 
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QFileInfo>
#include <QOpenGLTexture>
#include <QImage>
#include <QDebug>
#include <QString>


class Square 
{
public:
	QOpenGLShaderProgram* shaderProgram;
	QOpenGLShader* vertexShader;
	QOpenGLShader* fragmentShader;
	QVector<QVector3D> vertices;
	QVector<QVector2D> uvs;
	QOpenGLVertexArrayObject vao;
	QOpenGLBuffer vvbo;
	QOpenGLBuffer uvbo;
public:
	Square();
	void Init();
	void InitVAO();
	void InitVBO();
	void InitShader(QString vertexShaderPath,QString fragmentShaderPath);
	void DimensionTransformation(GLfloat source[],GLfloat target[][4]);
	void Begin();
	void Paint(GLfloat* ProjectionMatrix, GLfloat* ModelViewMatrix);
	void End();

	float Divided = 30.0f;
	float x1 = 230.0f, x2 = 270.0f, z1 = 215.0f, z2 = 275.0f;
	float dx = (x2 - x1) / Divided, dz = (z2 - z1) / Divided;
	float duv = 1.0f / Divided;
	//wave
	//振幅
	float Amplitude = 2.0f;
	//波長
	float Wavelength = 1000.0f;
	//速度
	float Speed = 1.5f;
	//基準高度
	float H = 38.0f;
	//time
	float t;

};