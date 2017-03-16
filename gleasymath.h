#ifndef GL_EASYMATH
#define GL_EASYMATH

#include<cmath>
#include<assert.h>
#include<gl\glut.h>
using namespace std;

#define X_1 0
#define Y_1 1
#define Z_1 2
#define W_1 3

static void vZero(float *v)
{
	v[X_1] = 0.0;
	v[Y_1] = 0.0;
	v[Z_1] = 0.0;
}

static void vSet(float *v, float x, float y, float z)
{
	v[X_1] = x;
	v[Y_1] = y;
	v[Z_1] = z;
}

static void vSub(const float *src1, const float *src2, float *dst)
{
	dst[X_1] = src1[X_1] - src2[X_1];
	dst[Y_1] = src1[Y_1] - src2[Y_1];
	dst[Z_1] = src1[Z_1] - src2[Z_1];
}
//把数组一复制到数组二
static void vCopy(const float *v1, float *v2)
{
	register int i;
	for (i = 0; i < 3; i++)
		v2[i] = v1[i];
}

static void vCross(const float *v1, const float *v2, float *cross)
{
	float temp[3];

	temp[X_1] = (v1[Y_1] * v2[Z_1]) - (v1[Z_1] * v2[Y_1]);
	temp[Y_1] = (v1[Z_1] * v2[X_1]) - (v1[X_1] * v2[Z_1]);
	temp[Z_1] = (v1[X_1] * v2[Y_1]) - (v1[Y_1] * v2[X_1]);
	vCopy(temp, cross);
}

//空间点到原点的长度
static float vLength(const float *v)
{
	return sqrt(v[X_1] * v[X_1] + v[Y_1] * v[Y_1] + v[Z_1] * v[Z_1]);
}

//将每个坐标乘以一个浮点数
static void vScale(float *v, float div)
{
	v[X_1] *= div;
	v[Y_1] *= div;
	v[Z_1] *= div;
}
//计算法向量
static void vNormal(float *v)
{
	vScale(v, 1.0f / vLength(v));
}
//标量积
static float vDot(const float *v1, const float *v2)
{
	return v1[X_1] * v2[X_1] + v1[Y_1] * v2[Y_1] + v1[Z_1] * v2[Z_1];
}

static void vAdd(const float *src1, const float *src2, float *dst)
{
	dst[X_1] = src1[X_1] + src2[X_1];
	dst[Y_1] = src1[Y_1] + src2[Y_1];
	dst[Z_1] = src1[Z_1] + src2[Z_1];
}


#pragma region 辅助函数

static GLfloat
_glmMax(GLfloat a, GLfloat b)
{
	if (a > b)
		return a;
	return b;
}

static GLfloat _glmAbs(GLfloat f)
{
	if (f < 0)
		return -f;
	return f;
}

/* _glmDot: compute the dot product of two vectors//计算两个向量的标量积
*
* u - array of 3 GLfloats (GLfloat u[3])
* v - array of 3 GLfloats (GLfloat v[3])
*/
static GLfloat _glmDot(GLfloat* u, GLfloat* v)
{
	assert(u);
	assert(v);

	/* compute the dot product */
	return u[X_1] * v[X_1] + u[Y_1] * v[Y_1] + u[Z_1] * v[Z_1];
}

/* _glmCross: compute the cross product of two vectors
* 计算向量交叉乘积
* u - array of 3 GLfloats (GLfloat u[3])
* v - array of 3 GLfloats (GLfloat v[3])
* n - array of 3 GLfloats (GLfloat n[3]) to return the cross product in
*/
static GLvoid _glmCross(GLfloat* u, GLfloat* v, GLfloat* n)
{
	assert(u);
	assert(v);
	assert(n);

	/* compute the cross product (u x v for right-handed [ccw]) */ //右手规则
	n[X_1] = u[Y_1] * v[Z_1] - u[Z_1] * v[Y_1];
	n[Y_1] = u[Z_1] * v[X_1] - u[X_1] * v[Z_1];
	n[Z_1] = u[X_1] * v[Y_1] - u[Y_1] * v[X_1];
}

/* _glmNormalize: normalize a vector //单位化矩阵
*
* n - array of 3 GLfloats (GLfloat n[3]) to be normalized
*/
static GLvoid _glmNormalize(GLfloat* n)
{
	GLfloat l;

	assert(n);

	/* normalize */
	l = (GLfloat)sqrt(n[X_1] * n[X_1] + n[Y_1] * n[Y_1] + n[Z_1] * n[Z_1]);
	n[0] /= l;
	n[1] /= l;
	n[2] /= l;
}

/* _glmEqual: compares two vectors and returns GL_TRUE if they are
* equal (within a certain threshold) or GL_FALSE if not. An epsilon 计算两个向量在阈值内是否相等
* that works fairly well is 0.000001.
*
* u - array of 3 GLfloats (GLfloat u[3])
* v - array of 3 GLfloats (GLfloat v[3])
*/
static bool _glmEqual(GLfloat* u, GLfloat* v, GLfloat epsilon)
{
	if (_glmAbs(u[0] - v[0]) < epsilon &&
		_glmAbs(u[1] - v[1]) < epsilon &&
		_glmAbs(u[2] - v[2]) < epsilon)
	{
		return GL_TRUE;
	}
	return GL_FALSE;
}
#pragma endregion

#endif