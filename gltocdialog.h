#ifndef GL_TOCDIALOG_H
#define GL_TOCDIALOG_H

#include <qdockwidget.h>
#include <qtreewidget.h>
#include<qplaintextedit.h>
#include <qboxlayout.h>
class GLMainWindow;

class GLTOCDialog :public QDockWidget
{
	Q_OBJECT
public:
	GLTOCDialog(QWidget *parent = 0);
	~GLTOCDialog();
private:
	GLMainWindow* mainWindow;
	QWidget *dockWidgetContents;//子空间容器
	QVBoxLayout *vLayout;
	QTreeView *treeView;//目录树
	QPlainTextEdit *infoText;//显示文本信息

public:
	QString infomation;//需要显示的信息
	QString infoHTML;//html格式的信息

public:
	void ShowCurrentRebackInfo(QString& info);//显示当前信息
};


#endif // !TOCDIALOG_H