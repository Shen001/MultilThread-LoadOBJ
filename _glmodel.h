#ifndef _GL_MODEL
#define _GL_MODEL

#include<string>
#include<gl/glut.h>
#include<qstring.h>
#include<QList>

using namespace std;

#define MAX_TEXTURE 256
//static int TextureArray[MAX_TEXTURE] = { 0 };

//定义渲染模式
#define _GL_NONE (0) //0
#define _GL_FLAT (1<<0)//1
#define _GL_SMOOTH (1<<1)//2
#define _GL_TEXTURE (1<<2)//4
#define _GL_COLOR (1<<3)//8
#define _GL_METERIAL (1<<4)//16
#define _GL_SELECT (1<<5)//32 选择模式
#define _GL_RENDER (1<<6)//64 渲染模式
//材质
struct  Material
{
	size_t index_Material;//当前的材质的索引,代表在model中的集合中的第几个
	QString materialName;

	GLfloat _Ka[3];//ambient
	GLfloat _Kd[3];//diffuse
	GLfloat _Ks[3];//specular

	QString imageName;//图片的路径名称，此处应为完整路径
};

//节点
struct Point3
{
	GLfloat _X;
	GLfloat _Y;
	GLfloat _Z;
};
//节点法向量
struct VertNormals
{
	GLfloat _NX;
	GLfloat _NY;
	GLfloat _NZ;
};
//节点纹理坐标
struct TextCoords
{
	GLfloat U;
	GLfloat V;
};

//// 三角形 //********暂时所有的几何对象还是通过三角网进行表示*********//
//struct Triangle
//{
//	int V[3];//存储的是三个节点的索引
//	int N[3];
//	int T[3];
//
//	size_t index_Triangle;//平面法向量的索引
//};

struct FacetNormal
{
	GLfloat NX;
	GLfloat NY;
	GLfloat NZ;
};

struct Face
{
	QString materialName;//对应材质名称

	int index_Face;//平面法向量的索引
	int index_Text;//
	int index_Name;//pickname

	QList<int> list_index_Points;//面的点集合在model中的索引
	QList<int> list_index_TextCoords;//
	QList<int> list_index_VertNormals;

	bool isS;//设置是否被选中状态，被选中则渲染为红色
};

//模型类
class _GLModel
{
public:


	QString path;//obj文件路径
	QString mtllibName;//材质文件名称
	size_t num_Vertices;//节点个数
	size_t num_Normals;//节点向量的个数
	size_t num_Textcoords;//节点纹理坐标个数
	size_t num_Materials;//材质个数
	size_t num_Faces;//面的个数

	QList<Point3> list_Origin_Vertics;//原始坐标的点集
	QList<Point3> list_Vertices;//节点对象集合
	QList<VertNormals> list_Normals;//节点向量集合
	QList<TextCoords> list_Textcoords;//纹理坐标集合
	QList<Face> list_Faces;//面集合
	QList<Material> list_Materials;//材质集合
	QList<FacetNormal> list_FaceNormal;//面向量集合

	QList<QString> list_ImagePath;//贴图路径集合,全路径

	int textureArray[MAX_TEXTURE];//注册纹理数组
	int textureArray_Fake[MAX_TEXTURE];//取消注册纹理数组
	//std::vector<unsigned int> textVector;

	float center[3];
	size_t currentSelectedFace;//当前选中面的索引
};

//将图形显示在屏幕中间
GLfloat _glUnitize(_GLModel* model,float *center);
//计算模型维度（即，x，y，z方向上的最大值）
void _glDimensions(_GLModel* model, GLfloat* dimensions);
//放大缩小指定的因子
void _glScale(_GLModel* model, GLfloat scale);
//翻转所有的模型中的多边形曲线，默认是逆时针方向。也改变法向量的方向
void _glReverseWinding(_GLModel* model);
//构造面法向量，假定逆时针
void _glFacetNormals(_GLModel* model);
//构建节点法向量
void _glVertexNormals(_GLModel* model, GLfloat angle);
//产生纹理坐标按照纹理映射的线性投影
void _glLinearTexture(_GLModel* model);
//产生纹理坐标按照纹理映射的球形投影
void _glSpheremapTexture(_GLModel* model);
//清除当前模型
 void _glDelete(_GLModel* model);
//读取OBJ文件
 _GLModel* _glReadOBJ(QString filename);
//将模型写入到OBJ文件（导出）
void _glWriteOBJ(_GLModel *model, QString filename, size_t mode);
//根据指定的模式渲染模型
void _glDraw(_GLModel *model, size_t mode);
//为指定的模型产生一个展示列表
size_t _glList(_GLModel *model, size_t mode);
//消除（焊接）邻域的向量
size_t _glWeld(_GLModel* model, size_t mode);
//
void _glConstructIndexFromName(_GLModel* model);







#endif