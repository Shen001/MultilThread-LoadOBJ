#ifndef GL_PICK_H
#define GL_PICK_H

#include<gl/glut.h>
#include<qmatrix4x4.h>
#include<gleasymath.h>

//将数组转换为4x4矩阵
static void Transform16to4x4(double* mD, QMatrix4x4 matrix)
{
	QVector4D vector0(mD[0], mD[4], mD[8], mD[12]);
	QVector4D vector1(mD[1], mD[5], mD[9], mD[13]);
	QVector4D vector2(mD[2], mD[6], mD[10], mD[14]);
	QVector4D vector3(mD[3], mD[7], mD[11], mD[15]);

	matrix.setColumn(0, vector0);
	matrix.setColumn(1, vector1);
	matrix.setColumn(2, vector2);
	matrix.setColumn(3, vector3);
}

static QMatrix4x4 Transform16to4x4(double* mD)
{
	QMatrix4x4 matrix(mD[0], mD[1], mD[2], mD[3], mD[4], mD[5], mD[6], mD[7], mD[8], mD[9], mD[10], mD[11], mD[12], mD[13], mD[14], mD[15]);
	return matrix;
}

static QMatrix4x4 glGetMatrixAndViewport(float *viewportF)
{
	QMatrix4x4 qMatrix4X4, pModelMatrix, pProjectionMatrix;
	double m1[16], m2[16];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	for (int i = 0; i < 4; ++i) viewportF[i] = viewport[i];

	glGetDoublev(GL_MODELVIEW_MATRIX, m1);
	glGetDoublev(GL_PROJECTION_MATRIX, m2);

	pModelMatrix = Transform16to4x4(m1);
	pProjectionMatrix = Transform16to4x4(m2);

	qMatrix4X4 = pProjectionMatrix*pModelMatrix;
	return qMatrix4X4;
}

static Point3 Proj(QMatrix4x4 matrix, const float* viewport, const Point3 p)
{
	const float vx = viewport[0];
	const float vy = viewport[1];
	const float vw2 = viewport[2] / 2.0f;//二分之宽
	const float vh2 = viewport[3] / 2.0f;//二分之高

	QVector4D vp(p._X, p._Y, p._Z, 1.0);
	QVector4D vpp = matrix*vp;
	QVector4D ndc = vpp / vpp[3];

	Point3 sc = {
		vw2*ndc[0] + vx + vw2,
		vh2*ndc[1] + vy + vh2,
		ndc[2]
	};
	return sc;
}

static QList<Point3> FillProjectedVetor(_GLModel* model, QMatrix4x4 matrix, const float* viewportF)
{
	QList<Point3> vecs;
	for (size_t i = 0; i < model->num_Vertices; i++)
	{
		vecs.push_back(Proj(matrix, viewportF, model->list_Vertices[i]));
	}
	return vecs;
}

static BOOL PtInPolygon(float x, float y, Face ptPolygon, QList<Point3> list)
{
	int nCount = ptPolygon.list_index_Points.size();//点个数
	// 交点个数
	int nCross = 0;
	for (int i = 0; i < nCount; i++)
	{
		Point3 p1 = list[ptPolygon.list_index_Points[i]];
		Point3 p2 = list[ptPolygon.list_index_Points[(i + 1) % nCount]];// 最后一个点与第一个点连线

		if (p1._Y == p2._Y)
			continue;
		if (y < _glmMin(p1._Y, p2._Y))
			continue;
		if (y >= _glmMax(p1._Y, p2._Y))
			continue;

		// 求交点的x坐标
		double xR = (double)(y - p1._Y) * (double)(p2._X - p1._X) / (double)(p2._Y - p1._Y) + p1._X;

		// 只统计p1p2与p向右射线的交点
		if (xR > x)
		{
			nCross++;
		}
	}
	// 交点为偶数，点在多边形之外
	return (nCross % 2 == 1);
}



#endif