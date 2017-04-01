#include<gl\glut.h>
#include <QFileInfo>
#include<qimage.h>

#include <glcanvas.h>
#include<glmainwindow.h>
#include<transform.h>

#define PICK_BUFFER_SIZE 1024
#define N_PICK 10

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
	isPickFace = false;
	redrawMode = _GL_FLAT|_GL_TEXTURE;
	hDC = NULL;
	hRC = NULL;
	hRCShareing = NULL;
	
}

void GLCanvas::initializeGL()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(128 / (GLclampd)255, 128 / (GLclampd)255, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)this->width() / (float)this->height(), 0.01f, 100.0f);


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_COLOR_MATERIAL);//设置了才显示面颜色
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);//不剔除内部显示
	// Setup other misc features. 混合要素
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);//启用发走样之后可以设置小数线宽

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.0f);
	//
	glShadeModel(GL_FLAT);//颜色模式，flat使用计算法向量，smooth使用自带法向量

	// Setup lighting model.//光照模型
	//GLfloat light_model_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat light0_direction[] = { 0.0f, 0.0f, 10.0f, 0.0f };
	//GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	GLfloat light_model_ambient[] = { 0.125f, 0.125f, 0.125f, 1.0f };
	GLfloat light0_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light0_direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
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
	glTranslated(0.0, 0.0, -5.0);//实际调用了glMultMatrixd
	glMultMatrixd(pModelViewMatrix);
	if (pModel)
	{
		_glDraw(pModel, this->redrawMode);

		//_glDraw(pModel, _GL_FLAT);
		//_glDraw(pModel, _GL_SMOOTH | _GL_TEXTURE);
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

//清除选择集
void ClearSelect(_GLModel* model)
{
	if (model)
	{
		for (size_t i = 0; i < model->num_Faces; i++)
		{
			model->list_Faces[i].isS = false;
		}
	}
}

void GLCanvas::mousePressEvent(QMouseEvent *e)
{
	if (pModel)
	{
		if (e->button() == Qt::LeftButton)
		{
			if (isPickFace)//拾取面操作
			{
				ClearSelect(pModel);//先清除选择集
				int x = e->x();
				int y = e->y();

				//****************TEST_1*****************
				/*float viewportF[4];
				QMatrix4x4 matrix;
				QList<Point3> pointList;
				matrix = glGetMatrixAndViewport(viewportF);
				pointList = FillProjectedVetor(pModel, matrix, viewportF);

				float xx = x;
				float yy = this->height() - y;

				for (size_t j = 0; j < pModel->num_Faces; j++)
				{
				isIn = PtInPolygon(xx, yy, pModel->list_Faces[j], pointList);
				if (isIn)
				{
				pModel->list_Faces[j].isS = true;
				}
				else
				{
				pModel->list_Faces[j].isS = false;
				}
				}*/

				//*******************TEST_2******************
				/*GLint viewport[4];
				GLdouble mvmatrix[16], projmatrix[16];
				GLdouble wx1, wy1, wz1, wx2, wy2, wz2, realy;
				GLfloat p1[3], p2[3];

				glGetIntegerv(GL_VIEWPORT, viewport);
				glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
				glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);


				realy = viewport[3] - y;
				gluUnProject((GLdouble)x, realy, 0.0, mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1);
				p1[0] = wx1;
				p1[1] = wy1;
				p1[2] = wz1;
				gluUnProject((GLdouble)x, (GLdouble)realy, 1.0, mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);
				p2[0] = wx2;
				p2[1] = wy2;
				p2[2] = wz2;

				float xx = p1[0] - p1[2] * (p2[0] - p1[0]) / (p2[2] - p1[2]);
				float yy = p1[1] - p1[2] * (p2[1] - p1[1]) / (p2[2] - p1[2]);

				for (size_t i = 0; i < pModel->num_Faces; i++)
				{
				isIn = PtInPolygon(xx, yy, pModel->list_Faces[i], pModel->list_Vertices);
				if (isIn)
				{
				pModel->list_Faces[i].isS = true;
				}
				else
				{
				pModel->list_Faces[i].isS = false;
				}
				}*/

				//*******************TEST_3*****************
				GLuint selectBuffer[PICK_BUFFER_SIZE];
				GLint hits;
				GLuint nearHit = 0;
				GLint viewport[4];

				glGetIntegerv(GL_VIEWPORT, viewport);
				glSelectBuffer(PICK_BUFFER_SIZE, selectBuffer);

				glRenderMode(GL_SELECT);
				glInitNames();
				glPushName(0);//如果是空的会报错，所以需要先添加一个0

				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadIdentity();
				gluPickMatrix(x, viewport[3] - y + viewport[1], N_PICK, N_PICK, viewport);
				gluPerspective(45.0f, (float)viewport[2] / (float)viewport[3], 0.1, 100.0);

				_glDraw(pModel, _GL_FLAT | _GL_SELECT);
				glPopMatrix();

				hits = glRenderMode(GL_RENDER);
				if (hits > 0)
				{
					int n = 0;
					GLuint minz = selectBuffer[1];//最小z
					for (GLint i = 1; i < hits; i++)
					{
						if (selectBuffer[1 + i * 4] < minz)
						{
							n = i;
							minz = selectBuffer[1 + i * 4];
						}
					}
					nearHit = selectBuffer[3 + n * 4];
				}
				if (nearHit > 0)
				{
					pModel->list_Faces[nearHit - 1].isS = true;
					pModel->currentSelectedFace = nearHit - 1;
					sendInfo(&pModel->list_Faces[nearHit - 1]);
				}
			}
			else
			{
				leftButtonPress = true;
				oldX = e->x();
				oldY = e->y();
			}
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


			//fOldX = this->devicePixelRatio()*oldX;
			//fOldY = this->devicePixelRatio()*oldY;
			//fNewX = this->devicePixelRatio()*e->x();
			//fNewY = this->devicePixelRatio()*e->y();


			double pMatrix[16];
			trackBall_GL_Matrix(pMatrix, fOldX, fOldY, fNewX, fNewY);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();//加载单位矩阵
			glLoadMatrixd(pMatrix);//将栈顶矩阵变成该旋转矩阵

			glMultMatrixd(pModelViewMatrix);//栈顶矩阵左乘该矩阵，即旋转矩阵左乘当前矩阵
			glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);//获得处理过后的当前矩阵，然后在刷新中调用单位矩阵再次左乘该矩阵

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

bool GLCanvas::BindTexture()
{
	if (this->pModel->list_ImagePath.length() == 0)
		return false;
	for (int i = 0; i < this->pModel->list_ImagePath.length(); i++)
	{
		glEnable(GL_TEXTURE_2D);
		GLint MaxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);

		QImage img, imgScaled, imgGL;
		QFileInfo fi(this->pModel->list_ImagePath[i]);
		QString imagePath = fi.absoluteFilePath();
		imagePath = imagePath.trimmed();
		bool res = img.load(imagePath);
		if (!res)
			return false;

		//成功开始绑定纹理
		int bestW = RoundUpToTheNextHighestPowerOf2(img.width());//计算最接近宽度的2的幂，如1024
		int bestH = RoundUpToTheNextHighestPowerOf2(img.height());
		while (bestW > MaxTextureSize) bestW /= 2;
		while (bestH > MaxTextureSize) bestH /= 2;

		imgScaled = img.scaled(bestW, bestH, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		imgGL = convertToGLFormat(imgScaled);//该方法是QGLWidget插件独有的方法，所以该方法是在你的QGLWidget中实现的

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//对齐像素字节函数

		//pModel->textureArray_Fake;
		glGenTextures(1, (GLuint*)&(this->pModel->textureArray[i]));//创建
		glBindTexture(GL_TEXTURE_2D, (GLuint)this->pModel->textureArray[i]);//绑定
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, imgGL.width(), imgGL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits());
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imgGL.width(), imgGL.height(), GL_RGBA, GL_UNSIGNED_BYTE, imgGL.bits());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		glDisable(GL_TEXTURE_2D);
	}
	return true;
}
//恢复初始视图
void GLCanvas::ReviewInit()
{
	//恢复初始视图
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glLoadMatrixd(pModelViewMatrix);
	glScaled(1.05, 1.05, 1.05);//为何有还原视图的效果
	glGetDoublev(GL_MODELVIEW_MATRIX, pModelViewMatrix);
	update();
}
//初始化多线程句柄
void GLCanvas::InitHDC()
{
	hDC = wglGetCurrentDC();
	//hRC = wglGetCurrentDC();
	hRC = wglGetCurrentContext();
	hRCShareing = wglCreateContext(hDC);

	wglShareLists(hRCShareing, hRC);//第一个rc是分享别人资源，第二个是共线资源给别人分享
}

void GLCanvas::sendInfo(Face *f)
{
	emit SendInfo(QString("index of selected face: %1\n").arg(pModel->currentSelectedFace));

	QString str;
	for (int i = 0; i < f->list_index_Points.size(); i++)
	{
		float x = pModel->list_Vertices[f->list_index_Points[i]]._X;
		float y = pModel->list_Vertices[f->list_index_Points[i]]._Y;
		float z = pModel->list_Vertices[f->list_index_Points[i]]._Z;

		str.append(QString("X:%1 \n").arg(x));
		str.append(QString("Y:%1 \n").arg(y));
		str.append(QString("Z:%1 \n").arg(z));
		str.append("\n");
	}
	emit SendInfo(str);
}
