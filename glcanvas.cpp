#include<gl\glut.h>
#include <QFileInfo>
#include<qimage.h>

#include <glcanvas.h>
#include<glmainwindow.h>
#include<transform.h>

GLCanvas::GLCanvas(QWidget *parent) :QGLWidget(parent)//QOpenGLWidget(parent)
{
	InitParameter();
	mainWindow = qobject_cast<GLMainWindow *>(parent);
	showFullScreen();
}

GLCanvas::~GLCanvas(){}

void GLCanvas::InitParameter()
{
	pModel = NULL;
	leftButtonPress = false;
	middleButtonPress = false;
	oldX = 0;
	oldY = 0;
}

void GLCanvas::initializeGL()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(128 / (GLclampd)255, 128 / (GLclampd)255, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)this->width() / (float)this->height(), 0.01f, 200.0f);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Setup other misc features. 混合要素
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	// Setup lighting model.//光照模型
	GLfloat light_model_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light0_direction[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_model_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
}

void GLCanvas::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslated(0.0, 0.0, -5.0);

	glMultMatrixd(pModelViewMatrix);
	if (pModel)
	{
		//InitialTextures(true);
		//glmDraw(pModel, GLM_FLAT | GLM_TEXTURE);
		//_glDraw(pModel, _GL_FLAT);
		//_glDraw(pModel, _GL_FLAT|_GL_TEXTURE);
		_glDraw(pModel, _GL_SMOOTH | _GL_TEXTURE);
		//_glDraw(pModel, _GL_SMOOTH);
	}
}

void GLCanvas::resizeGL(int w, int h)
{
	if (0 == h)
		h = 1;
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLCanvas::mousePressEvent(QMouseEvent *e)
{
	if (pModel)
	{
		if (e->button() == Qt::LeftButton)
		{
			leftButtonPress = true;
			oldX = e->x();
			oldY = e->y();
		}
		//中键移动整个图形
		else if (e->button() == Qt::MiddleButton)
		{
			middleButtonPress = true;
			oldMiddleX = e->x();
			oldMiddleY = e->y();
		}
		update();
	}
}

void GLCanvas::mouseReleaseEvent(QMouseEvent *e)
{
	if (pModel)
	{
		if (e->button() == Qt::LeftButton)
		{
			leftButtonPress = false;
		}
		else if (e->button() == Qt::MiddleButton)
		{
			middleButtonPress = false;
		}
		update();
	}
}

void GLCanvas::mouseMoveEvent(QMouseEvent *e)
{
	if (pModel)
	{
		if (leftButtonPress)
		{
			float buffer0 = 3.0f;
			float buffer1 = 1.0f;

			//加buffer的原因？
			float fOldX = buffer0*oldX / (float)this->width() - buffer1;
			float fOldY = -buffer0*oldY / (float)this->height() + buffer1;
			float fNewX = buffer0*e->x() / (float)this->width() - buffer1;
			float fNewY = -buffer0*e->y() / (float)this->height() + buffer1;

			double pMatrix[16];
			trackBall_GL_Matrix(pMatrix, fOldX, fOldY, fNewX, fNewY);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();//加载单位矩阵
			glLoadMatrixd(pMatrix);//加载变形矩阵

			glMultMatrixd(pModelViewMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);

			oldX = e->x();
			oldY = e->y();
		}
		else if (middleButtonPress)
		{
			float buffer = 0.005;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();//加载单位矩阵

			float subX = (float)(e->x() - oldMiddleX)*buffer;
			float subY = (float)(e->y() - oldMiddleY)*buffer;
			glTranslatef(subX, -subY, 0.0);

			glMultMatrixd(pModelViewMatrix);
			glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);

			oldMiddleX = e->x();
			oldMiddleY = e->y();
		}
		update();
	}
}
//需要为widget设置setfocus
void GLCanvas::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_F1://放大
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixd(pModelViewMatrix);
		glScaled(1.05, 1.05, 1.05);
		glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
		break;
	case Qt::Key_F2:
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixd(pModelViewMatrix);
		glScaled(0.95, 0.95, 0.95);
		glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
		break;
	default:
		break;
	}
	update();
}

void GLCanvas::keyReleaseEvent(QKeyEvent *e)
{
	e->ignore();
	setFocus();
}

void GLCanvas::wheelEvent(QWheelEvent *e)
{
	const float WHEEL_STEP = 1200.0;
	int delta = e->delta();
	float notch = delta / WHEEL_STEP;
	GLdouble scale = (GLdouble)(1 - notch);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixd(pModelViewMatrix);
	glScaled(scale, scale, scale);
	glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);

	update();
}

//GLMmodel* GLCanvas::getModel(){ return this->pModel; }

void GLCanvas::BindTexture(_GLModel* model)
{
	if (model->list_ImagePath.length() == 0)
		return;
	for (int i = 0; i < model->list_ImagePath.length(); i++)
	{
		glEnable(GL_TEXTURE_2D);
		GLint MaxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);

		QImage img, imgScaled, imgGL;
		QFileInfo fi(model->list_ImagePath[i]);
		QString imagePath = fi.absoluteFilePath();
		imagePath = imagePath.trimmed();
		bool res = img.load(imagePath);
		if (res)
		{
			int bestW = RoundUpToTheNextHighestPowerOf2(img.width());
			int bestH = RoundUpToTheNextHighestPowerOf2(img.height());
			while (bestW > MaxTextureSize) bestW /= 2;
			while (bestH > MaxTextureSize) bestH /= 2;

			imgScaled = img.scaled(bestW, bestH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
			imgGL = convertToGLFormat(imgScaled);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			//model->textureArray.push_back(0);
			//glGenTextures(1, (GLuint*)&(model->textureArray.back()));
			glGenTextures(1, (GLuint*)&(model->textureArray[i]));
			glBindTexture(GL_TEXTURE_2D, (GLuint)model->textureArray[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, imgGL.width(), imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits());
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgGL.width(), imgGL.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		glDisable(GL_TEXTURE_2D);
	}

}
