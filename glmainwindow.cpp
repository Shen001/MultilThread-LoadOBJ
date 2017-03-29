#include <qaction.h>
#include<qmenubar.h>
#include<qtoolbar.h>
#include<qfiledialog.h>
#include<qdebug.h>
#include<qmessagebox.h>

#include <glmainwindow.h>
#include <transform.h>

GLMainWindow::GLMainWindow()
{
	gltocDialog = new GLTOCDialog(this);
	gltocDialog->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, gltocDialog);

	glCancas = new GLCanvas(this);
	setCentralWidget(glCancas);

	//importObjAction = new QAction(QIcon(":/images/import_mesh.png"), tr("&Open OBJ"), this);//需要引用Qt5Gui.lib或者Qt5Guid.lib
	importObjAction = new QAction(QString::fromLocal8Bit("打开OBJ"), this);
	importObjAction->setShortcutContext(Qt::ApplicationShortcut);
	importObjAction->setShortcut(Qt::CTRL + Qt::Key_O);
	connect(importObjAction, &QAction::triggered, this, &GLMainWindow::OpenOBJFile);
	//还原视图
	restoreMatrixAction = new QAction(QString::fromLocal8Bit("还原视图"), this);
	connect(restoreMatrixAction, &QAction::triggered, this, &GLMainWindow::RestoreView);

	//拾取面
	pickFaceAction = new QAction(QString::fromLocal8Bit("拾取面"),this);
	connect(pickFaceAction, &QAction::triggered, this, &GLMainWindow::StartPickFace);
	pickFaceAction->setCheckable(true);
	pickFaceAction->setChecked(false);


	fileMenu = menuBar()->addMenu(tr("&File"));//需要menubar头文件
	fileMenu->addAction(importObjAction);

	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(importObjAction);
	fileToolBar->addAction(restoreMatrixAction);
	fileToolBar->addAction(pickFaceAction);

}



GLMainWindow::~GLMainWindow()
{

}

void GLMainWindow::RestoreView()
{
	glCancas->ReviewInit();
}

void GLMainWindow::StartPickFace()
{
	if (glCancas->pModel&&this->pickFaceAction->isChecked())
	{
		glCancas->isPickFace = true;
	}
	else
	{
		glCancas->isPickFace = false;
	}
}

void GLMainWindow::OpenOBJFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开OBJ"), "/home", "OBJ Files (*.obj)");
	//如果已经存在了，那么先删除该模型

	if (fileName.isNull())
		return;

	if (glCancas->pModel)
	{
		_glDelete(glCancas->pModel);
		glCancas->pModel = NULL;
	}

	glCancas->pModel = _glReadOBJ(fileName);
	if (!glCancas->pModel)
	{
		qDebug(T_Char2Char("无法打开OBJ文件"));
		return;
	}
	


	_glConstructIndexFromName(glCancas->pModel);
	//绑定textture
	glCancas->BindTexture(glCancas->pModel);
	glCancas->scale = _glUnitize(glCancas->pModel, glCancas->pModel->center);
	_glFacetNormals(glCancas->pModel);


	// Init the modelview matrix as an identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, glCancas->pModelViewMatrix);
	
	glCancas->setFocus();
	glCancas->update();
}








