#include "gltrackball.h"
#include<cmath>
#include<gleasymath.h>
using namespace std;

#define TRACKBALLSIZE (0.8f)

static float tb_project_to_sphere(float, float, float);
static void normalize_Quat(float[4]);

//模仿一个追踪球，将点投影到球上面，然后指出旋转轴，是P1，P2和P1，O的交叉乘积；
//注意：这是一个变形的球，在中心的球；但是根据双曲线函数旋转变形远离球心。

void trackBall(float q[4], float x1, float y1, float x2, float y2)
{
	float a[3]; /* Axis of rotation */ //旋转轴
	float phi;  /* how much to rotate about axis */ //绕轴旋转的角度
	float p1[3], p2[3], d[3];
	float t;

	if (x1 == x2 && y1 == y2) {
		/* Zero rotation */
		vZero(q);
		q[3] = 1.0;
		return;
	}

	/*
	* First, figure out z-coordinates for projection of P1 and P2 to
	* deformed sphere  首先，指出首点和尾点的投影z坐标系统，得到两个三维向量
	*/
	vSet(p1, x1, y1, tb_project_to_sphere(TRACKBALLSIZE, x1, y1));
	vSet(p2, x2, y2, tb_project_to_sphere(TRACKBALLSIZE, x2, y2));

	/*
	*  Now, we want the cross product of P1 and P2
	//交叉矩阵
	*/
	vCross(p2, p1, a);

	/*
	*  Figure out how much to rotate around that axis.
		计算旋转多少度
	*/
	vSub(p1, p2, d);
	t = vLength(d) / (2.0f*TRACKBALLSIZE);//除以直径

	/*
	* Avoid problems with out-of-control values...
	*/
	if (t > 1.0f) t = 1.0f;
	if (t < -1.0f) t = -1.0f;
	phi = 2.0f * asin(t);//计算出旋转角度

	axis_To_Quat(a, phi, q);
}
/*
*  Given an axis and angle, compute quaternion.
*/
//根据轴和角度，计算四元数（参照搜索结果）
void axis_To_Quat(float a[3], float phi, float q[4])
{
	vNormal(a);//单位化
	vCopy(a, q);//复制到q
	vScale(q, sin(phi / 2.0f));//乘以一个sin值
	q[3] = cos(phi / 2.0f);//计算数组最后一个值
}

/*
* Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
// 投影x，y坐标对到半径r的球体上或双曲线上，如果离中心球体中心很远
* if we are away from the center of the sphere.
*/
static float
tb_project_to_sphere(float r, float x, float y)
{
	float d, t, z;

	d = sqrt(x*x + y*y);
	if (d < r * 0.70710678118654752440) {    /* Inside sphere 球体内部*/ //0.70710678118654752440=二分之根号二
		z = sqrt(r*r - d*d);
	}
	else {           /* On hyperbola */ //双曲线上 /球外部
		t = r / 1.41421356237309504880f;//还是乘以二分之根号二
		z = t*t / d; // 双曲线的焦点
	}
	return z;
}

//根据两个四元组计算出一个等价的四元组
// 按照习惯在单位化结果在指定的次数被调用了之后，以防止发生错误
// 这个习惯被记下来所以q1和q2可能和dest相等
#define RENORMCOUNT 97

void add_Quats(float q1[4], float q2[4], float dest[4])
{
	static int count = 0;
	float t1[4], t2[4], t3[4];
	float tf[4];

	vCopy(q1, t1);
	vScale(t1, q2[3]);

	vCopy(q2, t2);
	vScale(t2, q1[3]);

	vCross(q2, q1, t3);
	vAdd(t1, t2, tf);
	vAdd(t3, tf, tf);
	tf[3] = q1[3] * q2[3] - vDot(q1, q2);

	dest[0] = tf[0];
	dest[1] = tf[1];
	dest[2] = tf[2];
	dest[3] = tf[3];

	if (++count > RENORMCOUNT) {
		count = 0;
		normalize_Quat(dest);
	}
}

/*
* Quaternions always obey:  a^2 + b^2 + c^2 + d^2 = 1.0
* If they don't add up to 1.0, dividing by their magnitued will
* renormalize them.
*
* Note: See the following for more information on quaternions:
*
* - Shoemake, K., Animating rotation with quaternion curves, Computer
*   Graphics 19, No 3 (Proc. SIGGRAPH'85), 245-254, 1985.
* - Pletinckx, D., Quaternion calculus as a basic tool in computer
*   graphics, The Visual Computer 5, 2-13, 1989.
*/
static void normalize_Quat(float q[4])//单位化数组
{
	int i;
	float mag;

	mag = (q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	for (i = 0; i < 4; i++) q[i] /= mag;
}

/*
* Build a rotation matrix, given a quaternion rotation.
* 构建一个旋转矩阵，指定一个四元组旋转
*/
void
build_Rotmatrix(float m[4][4], float q[4])//[x,y,z,w] //四元数组计算出旋转矩阵
{
	//为什么这样计算，
	m[0][0] = 1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]); //1−2(y2 + z2)
	m[0][1] = 2.0f * (q[0] * q[1] - q[2] * q[3]);//2(yz−xw)
	m[0][2] = 2.0f * (q[2] * q[0] + q[1] * q[3]);//2(xa+yw)
	m[0][3] = 0.0f;

	m[1][0] = 2.0f * (q[0] * q[1] + q[2] * q[3]);//2(xy+zw)
	m[1][1] = 1.0f - 2.0f * (q[2] * q[2] + q[0] * q[0]);//1-2（z2+x2）
	m[1][2] = 2.0f * (q[1] * q[2] - q[0] * q[3]);//2(yz-xw)
	m[1][3] = 0.0f;

	m[2][0] = 2.0f * (q[2] * q[0] - q[1] * q[3]);//2(xz-yw)
	m[2][1] = 2.0f * (q[1] * q[2] + q[0] * q[3]); // 2(yz+xw)
	m[2][2] = 1.0f - 2.0f * (q[1] * q[1] + q[0] * q[0]);//1-2(y2+x2)
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;//矩阵的最后一行是[0,0,0,1]，因为列的前三个数代表了空间的轴向量，不需要第四个参数，最后一个代表平移的向量
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

/*
* trackball function that results a openGL matrix
*	追踪球函数返回一个gl矩阵
*/
void
trackBall_GL_Matrix(double m[16], float p1x, float p1y, float p2x, float p2y)
{
	float q[4];
	trackBall(q, p1x, p1y, p2x, p2y);

	float matrix[4][4];
	build_Rotmatrix(matrix, q);
	//按照行顺序添加到数组中
	m[0] = matrix[0][0]; m[4] = matrix[1][0]; m[8] = matrix[2][0]; m[12] = matrix[3][0];
	m[1] = matrix[0][1]; m[5] = matrix[1][1]; m[9] = matrix[2][1]; m[13] = matrix[3][1];
	m[2] = matrix[0][2]; m[6] = matrix[1][2]; m[10] = matrix[2][2]; m[14] = matrix[3][2];
	m[3] = matrix[0][3]; m[7] = matrix[1][3]; m[11] = matrix[2][3]; m[15] = matrix[3][3];
}