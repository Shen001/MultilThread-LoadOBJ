#ifndef GL_TOCDIALOG_H
#define GL_TOCDIALOG_H

#include <qdockwidget.h>
#include <qtreewidget.h>

class GLMainWindow;

class GLTOCDialog :public QDockWidget
{
	Q_OBJECT
public:
	GLTOCDialog(QWidget *parent = 0);
	~GLTOCDialog();
private:
	GLMainWindow* mainWindow;
	QTreeView *treeView;//Ä¿Â¼Ê÷

};


#endif // !TOCDIALOG_H