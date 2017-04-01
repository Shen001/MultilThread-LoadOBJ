#include<gltocdialog.h>
#include "glmainwindow.h"
#include<qwidget.h>

GLTOCDialog::GLTOCDialog(QWidget *parent) : QDockWidget(parent)
{
	mainWindow  = qobject_cast<GLMainWindow *>(parent);

	dockWidgetContents = new QWidget();
	vLayout = new QVBoxLayout(dockWidgetContents);
	treeView = new QTreeWidget(dockWidgetContents);
	infoText = new QPlainTextEdit(dockWidgetContents);
	infoText->setReadOnly(true);
	vLayout->addWidget(treeView);
	vLayout->addWidget(infoText);


	this -> setWidget(dockWidgetContents);
	this->setMinimumWidth(200);
	this->setWindowTitle(QString::fromLocal8Bit("д©б╪йсм╪"));
	this->layout();
}
GLTOCDialog::~GLTOCDialog()
{

}

void GLTOCDialog::ShowCurrentRebackInfo(QString& info)
{
	this->infoText->appendPlainText(info);
	QTextCursor cursor = this->infoText->textCursor();
	cursor.movePosition(QTextCursor::End);
	this->infoText->setTextCursor(cursor);

}