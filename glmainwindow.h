#ifndef GL_MAINWINDOW_H
#define GL_MAINWINDOW_H

#include <qmainwindow.h>
#include <gltocdialog.h>
#include <glcanvas.h>
#include <_glmodel.h>
#include <gleasymath.h>

class GLMainWindow :public QMainWindow
{
	Q_OBJECT
public:
	GLMainWindow();
	~GLMainWindow();

public:
	void OpenOBJFile();//打开obj路径文件
	_GLModel* getObjModel();//获取当前的模型

private:

	GLTOCDialog *gltocDialog;
	GLCanvas *glCancas;

	QAction *importObjAction;
	QMenu *fileMenu;
	QToolBar *fileToolBar;

public:

};
#endif // !GL_MAINWINDOW_H
