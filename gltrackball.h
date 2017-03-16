#ifndef GL_TRACKBALL
#define GL_TRACKBALL
//根据起始点坐标和终点坐标构建四元组旋转数组
void trackBall(float q[4], float x1, float y1, float x2, float y2);

//返回一个opengl的矩阵
void trackBall_GL_Matrix(double m[16], float x1, float y1, float x2, float y2);

//给定两个四元组，得到另一个四元组
void add_Quats(float *q1, float *q2, float *dest);

//根据四元组得到一个旋转矩阵
void build_Rotmatrix(float m[4][4], float q[4]);

//根据轴和一个关于旋转的角度计算出一个四元组，角度是一个弧度，结果保存在第三个参数中。
void axis_To_Quat(float a[3], float phi, float q[4]);
#endif // !GL_TRACKBALL


