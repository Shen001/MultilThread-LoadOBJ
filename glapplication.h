#ifndef GL_APPLICATION_H
#define GL_APPLICATION_H

#include <QApplication>
#include <QString>

class GlApplication :public QApplication
{
public :
	GlApplication(int &argc, char *argv[]) :QApplication(argc, argv){}
	~GlApplication(){}

	static const QString AppName(){ return QString::fromLocal8Bit("ÌùÍ¼0.0~"); }
	static const QString AppVersion(){ return tr("0.0.1"); }

};

#endif