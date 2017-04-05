#ifndef GLCANVAS_H
#define GLCANVAS_H

#include<qopenglwidget.h>
#include<QtOpenGL\qgl.h>
#include<QKeyEvent>
#include<process.h>


#include<gltrackball.h>
#include<_glmodel.h>
#include<glpick.h>


class GLMainWindow;
class GLCanvas :public QGLWidget//public QOpenGLWidget
{
	Q_OBJECT
public:
	GLCanvas(QWidget *parent = 0);
	~GLCanvas();
	void InitParameter();
	bool BindTexture();
	void ReviewInit();//恢复初始状态
	void InitHDC();
	void ClearSelection();
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
	//变量
public:
	_GLModel* pModel;
	bool isPickFace;//用来标示当前是否开始拾取面~
	size_t redrawMode;//重绘的模式

	double pModelViewMatrix[16];//模型视图
	double pProjectMatrix[16];//投影视图
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

	_WINDEF_::HDC	hDC;  // Private GDI Device Context
	HGLRC  hRC;  // Permanent Rendering Context
	HGLRC  hRCShareing;// 用于分享hRC的资源

signals:
	void SendInfo(QString&);//发送显示信息的信号

private :
	void sendInfo(Face *f);
};

#endif // GLCANVAS_H
