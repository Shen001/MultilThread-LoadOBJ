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

	glCanvas = new GLCanvas(this);
	setCentralWidget(glCanvas);

	//importObjAction = new QAction(QIcon(":/images/import_mesh.png"), tr("&Open OBJ"), this);//需要引用Qt5Gui.lib或者Qt5Guid.lib

	importObjAction = new QAction(QString::fromLocal8Bit("打开OBJ"), this);
	importObjAction->setShortcutContext(Qt::ApplicationShortcut);
	importObjAction->setShortcut(Qt::CTRL + Qt::Key_O);
	connect(importObjAction, &QAction::triggered, this, &GLMainWindow::OpenOBJFile);
	//还原视图
	restoreMatrixAction = new QAction(QString::fromLocal8Bit("还原视图"), this);
	connect(restoreMatrixAction, &QAction::triggered, this, &GLMainWindow::RestoreView);

	//拾取面
	pickFaceAction = new QAction(QString::fromLocal8Bit("拾取面"), this);
	connect(pickFaceAction, &QAction::triggered, this, &GLMainWindow::StartPickFace);
	pickFaceAction->setCheckable(true);
	pickFaceAction->setChecked(false);

	//开启纹理贴图
	textureRenderAction = new QAction(QString::fromLocal8Bit("启用纹理"), this);
	connect(textureRenderAction, &QAction::triggered, this, &GLMainWindow::StartTexture);
	textureRenderAction->setCheckable(true);
	textureRenderAction->setChecked(false);
	textureRenderAction->setEnabled(false);

	//菜单
	fileMenu = menuBar()->addMenu(tr("&File"));//需要menubar头文件
	fileMenu->addAction(importObjAction);
	//工具条
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(importObjAction);
	fileToolBar->addAction(restoreMatrixAction);
	fileToolBar->addAction(pickFaceAction);
	fileToolBar->addAction(textureRenderAction);


	connect(glCanvas, SIGNAL(SendInfo(QString&)), this, SLOT(ShowInfo(QString&)));//绑定显示信号的槽
}

void GLMainWindow::ShowInfo(QString& info)
{
	this->gltocDialog->ShowCurrentRebackInfo(info);
}


void GLMainWindow::setTextureActionEnable(bool isEnable)
{
	this->textureRenderAction->setEnabled(isEnable);
	this->textureRenderAction->setChecked(true);
	glCanvas->update();
}

//初始化纹理的线程
void GLMainWindow::initialTextureThread()
{
	textureThread = new _TextureThread(glCanvas);
	connect(textureThread, &_TextureThread::loadReady, this, &GLMainWindow::setTextureActionEnable);
	connect(textureThread, &_TextureThread::finished, textureThread, &QObject::deleteLater);
}

GLMainWindow::~GLMainWindow()
{
	//if (textureThread)//研究如何正确有效的释放new出来的对象
	//	delete &textureThread;
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glCanvas->hRC);
	wglDeleteContext(glCanvas->hRCShareing);
}

void GLMainWindow::RestoreView()
{
	glCanvas->ReviewInit();
}

void GLMainWindow::StartPickFace()
{
	if (glCanvas->pModel&&this->pickFaceAction->isChecked())
	{
		glCanvas->isPickFace = true;
	}
	else
	{
		glCanvas->isPickFace = false;
		glCanvas->ClearSelection();
		glCanvas->updateGL();
	}
}

void GLMainWindow::OpenOBJFile()
{
	QFileInfo fileInfo(QString::fromLocal8Bit("F:\\code\\cPP\\doc\\三维数据\\obj文件\\"));
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开OBJ"), fileInfo.absolutePath(), "OBJ Files (*.obj)");
	//如果已经存在了，那么先删除该模型

	if (fileName.isNull())
		return;
	updateMenus(true);
	if (glCanvas->pModel)
	{
		_glDelete(glCanvas->pModel);
		glCanvas->pModel = NULL;
	}

	glCanvas->pModel = _glReadOBJ(fileName);
	if (!glCanvas->pModel)
	{
		qDebug(T_Char2Char("无法打开OBJ文件"));
		return;
	}

	_glReconstructFaceIndexes(glCanvas->pModel);
	glCanvas->InitHDC();
	initialTextureThread();
	wglMakeCurrent(glCanvas->hDC, glCanvas->hRC);
	textureThread->start();
	_glConstructIndexFromName(glCanvas->pModel);
	//绑定textture
	_glFacetNormals(glCanvas->pModel);
	glCanvas->scale = _glUnitize(glCanvas->pModel, glCanvas->pModel->center);

	// Init the modelview matrix as an identity matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, glCanvas->pModelViewMatrix);

	glCanvas->setFocus();
	glCanvas->update();
}

void GLMainWindow::StartTexture()
{
	if (!glCanvas->pModel)
		return;
	if (this->textureRenderAction->isChecked())
	{
		glCanvas->redrawMode = _GL_FLAT | _GL_TEXTURE;
	}
	else
	{
		glCanvas->redrawMode = _GL_FLAT;
	}
	glCanvas->update();
}

void GLMainWindow::closeEvent(QCloseEvent* event)
{
	//以后还要确定贴图后的保存操作
	//伪代码：
	//if(需要保存，显示提示信息)
	/*
		IF(true)
		save()
		ELSE
		quit()
		*/

	event->accept();
}
//刷新菜单栏
void GLMainWindow::updateMenus(bool ifLoadNewOBJ)
{
	pickFaceAction->setChecked(false);
	glCanvas->isPickFace = false;
	if (ifLoadNewOBJ)
		textureRenderAction->setEnabled(false);
	textureRenderAction->setChecked(false);
}


