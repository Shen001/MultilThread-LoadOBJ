#ifndef _GL_BACKGROUND_THREAD_H
#define _GL_BACKGROUND_THREAD_H
/*定义后台线程*/
#include<qthread.h>
#include<_glmodel.h>

class _TextureThread :public QThread
{
	Q_OBJECT
	void run() Q_DECL_OVERRIDE
	{
		bool isReady;
		//开始执行绑定纹理

		emit loadReady(isReady);//发出信号
	}
signals:

	void loadReady(const bool isEnable);


};




#endif // !_GL_BACKGROUND_THREAD_H
