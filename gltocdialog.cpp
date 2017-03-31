#include<gltocdialog.h>
#include "glmainwindow.h"

GLTOCDialog::GLTOCDialog(QWidget *parent) : QDockWidget(parent)
{
	mainWindow  = qobject_cast<GLMainWindow *>(parent);

	this->setMinimumWidth(200);
	//this->setMaximumWidth(60);
	this->setWindowTitle(QString::fromLocal8Bit("д©б╪йсм╪"));
	this->layout();
}
GLTOCDialog::~GLTOCDialog()
{

}