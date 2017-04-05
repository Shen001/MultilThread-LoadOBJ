#ifndef GL_MAINWINDOW_H
#define GL_MAINWINDOW_H

#include <qmainwindow.h>
#include <gltocdialog.h>
#include <glcanvas.h>
#include <_glmodel.h>
#include <gleasymath.h>
#include<_glBackgroundThread.h>
//#include<qevent.h>
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
	void closeEvent(QCloseEvent *event);
	void updateMenus(bool ifLoadNewOBJ);//更新菜单状态

private:
	GLTOCDialog *gltocDialog;
	GLCanvas *glCanvas;

	QAction *importObjAction;
	QAction *restoreMatrixAction;
	QAction *pickFaceAction;
	QAction *textureRenderAction;

	QMenu *fileMenu;
	QToolBar *fileToolBar;
	//多线程
	_TextureThread* textureThread;

	public slots:
	void ShowInfo(QString&);//接收信号的槽
};
#endif // !GL_MAINWINDOW_H
