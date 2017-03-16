#ifndef GLCANVAS_H
#define GLCANVAS_H

#include<qopenglwidget.h>
#include<QtOpenGL\qgl.h>
#include<qopenglfunctions.h>
//#include <glmodel.h>
#include<QKeyEvent>
#include<gltrackball.h>
#include<_glmodel.h>
class GLMainWindow;
class GLCanvas :public QGLWidget//public QOpenGLWidget
{
	Q_OBJECT
public:
	GLCanvas(QWidget *parent = 0);
	~GLCanvas();
	void InitParameter();
	//GLMmodel* getModel();
	//int RoundUpToTheNextHighestPowerOf2(unsigned int v);
	void BindTexture(_GLModel* model);
	// The OBJ model
	//GLMmodel* pModel;
	_GLModel* pModel;
	double pModelViewMatrix[16];
protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void keyPressEvent(QKeyEvent *e);
	void keyReleaseEvent(QKeyEvent *e);
	void wheelEvent(QWheelEvent *e);

	void initializeGL();
	void resizeGL(int w, int h); //会自动刷新屏幕
	void paintGL();


private:
	GLMainWindow *mainWindow;
	bool leftButtonPress;//当前左键按下
	bool middleButtonPress;
	int oldX;
	int oldY;

	int oldMiddleX;
	int oldMiddleY;

public:
	float scale;

};

#endif // GLCANVAS_H
