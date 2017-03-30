#ifndef GL_MAINWINDOW_H
#define GL_MAINWINDOW_H

#include <qmainwindow.h>
#include <gltocdialog.h>
#include <glcanvas.h>
#include <_glmodel.h>
#include <gleasymath.h>
#include<_glBackgroundThread.h>

class GLMainWindow :public QMainWindow
{
	Q_OBJECT
public:
	GLMainWindow();
	~GLMainWindow();

public://action对应方法
	void OpenOBJFile();//打开obj路径文件
	void RestoreView();
	void StartPickFace();
	void StartTexture();
public:
	_GLModel* getObjModel();//获取当前的模型
	void initialTextureThread();//初始化纹理线程
		
private:
	void setTextureActionEnable(bool isEnable);

private:

	GLTOCDialog *gltocDialog;
	GLCanvas *glCanvas;

	QAction *importObjAction;
	QAction *restoreMatrixAction;
	QAction *pickFaceAction;
	QAction *textureRenderAction;

	QMenu *fileMenu;
	QToolBar *fileToolBar;

};
#endif // !GL_MAINWINDOW_H
