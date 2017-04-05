#ifndef _GL_BACKGROUND_THREAD_H
#define _GL_BACKGROUND_THREAD_H
/*定义后台线程*/
#include<qthread.h>
#include<_glmodel.h>
#include<glcanvas.h>

//绑定纹理线程类
class _TextureThread :public QThread
{
	Q_OBJECT
	void run() Q_DECL_OVERRIDE
	{
		bool isReady;
		wglMakeCurrent(canvas->hDC, canvas->hRCShareing);
		//开始执行绑定纹理
		isReady = canvas->BindTexture();

		wglMakeCurrent(NULL, NULL);
		emit loadReady(isReady);//发出信号
	}
	signals:

	void loadReady(const bool isEnable);

public:
	_TextureThread(GLCanvas* canvas){ 
		this->canvas = canvas;
	};

	GLCanvas* canvas;//当前的模型对象
};




#endif // !_GL_BACKGROUND_THREAD_H
