# ifndef GL_MODEL
#define GL_MODEL

#include<gl/glut.h>
#include <vector>
using namespace std;

#define MAX_TEXTURES 256
static GLuint textureArray[MAX_TEXTURES] = { 0 };

/* defines */
#define GLM_NONE     (0)    //0 /* render with only vertices */只渲染节点
#define GLM_FLAT     (1 << 0) //1   /* render with facet normals */ 使用面法向量渲染
#define GLM_SMOOTH   (1 << 1) //2   /* render with vertex normals */ 节点法向量
#define GLM_TEXTURE  (1 << 2) //4  /* render with texture coords */ 纹理坐标
#define GLM_COLOR    (1 << 3) //8   /* render with colors */ 颜色
#define GLM_MATERIAL (1 << 4) //16   /* render with materials */ 材质

//材质，即贴图资源
typedef struct _GLMmaterial
{
	char* name;        /* name of material */ //名称，如 1.mtl
	GLfloat diffuse[4];      /* diffuse component */ //固有色
	GLfloat ambient[4];      /* ambient component */ //材质的阴影色
	GLfloat specular[4];      /* specular component */ // 高光色
	//GLfloat emissive[4];      /* emmissive component */ // 放射
	GLfloat shininess;      /* specular exponent */ // 发光
	char* map_file;      /* filename of the texture map */ //纹理文件全称
} GLMmaterial;

//组（因为~~）
typedef struct _GLMgroup {
	char*				name;    /* name of this group */
	GLuint            numtriangles;  /* number of triangles in this group */
	GLuint*           triangles;    /* array of triangle indices */
	GLuint            material;           /* index to material for group */
	struct _GLMgroup* next;    /* pointer to next group in model *///指向下一个组的指针
} GLMgroup;

//三角形结构体
typedef struct {
	GLuint vindices[3];      /* array of triangle vertex indices */
	GLuint nindices[3];      /* array of triangle normal indices */
	GLuint tindices[3];      /* array of triangle texcoord indices*/
	GLuint findex;      /* index of triangle facet normal */ //该平面的向量索引
} GLMtriangle;
//六面体
typedef struct{
	

} GLMHexahedron;
/* GLMmodel: Structure that defines a model.
*/
typedef struct {
	char*    pathname;      /* path to this model */ //路径
	char*    mtllibname;      /* name of the material library */ //mtl路径

	GLuint   numvertices;      /* number of vertices in model */ //节点个数
	GLfloat* vertices;      /* array of vertices  */ //节点数组

	GLuint   numnormals;      /* number of normals in model */ //法向量个数
	GLfloat* normals;      /* array of normals */	//法向量数组

	GLuint   numtexcoords;    /* number of texcoords in model */ //纹理坐标个数
	GLfloat* texcoords;      /* array of texture coordinates */	//纹理坐标数组

	GLuint   numfacetnorms;    /* number of facetnorms in model */ //面法向量个数
	GLfloat* facetnorms;      /* array of facetnorms */	//面法向量数组

	GLuint       numtriangles;    /* number of triangles in model */ //三角形个数
	GLMtriangle* triangles;    /* array of triangles */	//三角形数组

	GLuint       nummaterials;    /* number of materials in model 
								  */ //材质个数

	std::vector <char*> textures;//图片的路径

	GLMmaterial* materials;    /* array of materials */ //材质数组

	GLuint			numgroups;    /* number of groups in model */ //组个数
	GLMgroup*    groups;      /* linked list of groups *///组索引

	GLfloat position[3];      /* position of the model */ //模型位置？

} GLMmodel;

//将图形显示在屏幕中间
GLfloat glmUnitize(GLMmodel* model, GLfloat center[3]);
//计算模型维度（即，x，y，z方向上的最大值）
GLvoid glmDimensions(GLMmodel* model, GLfloat* dimensions);
//放大缩小指定的因子
GLvoid glmScale(GLMmodel* model, GLfloat scale);
//翻转所有的模型中的多边形曲线，默认是逆时针方向。也改变法向量的方向
GLvoid glmReverseWinding(GLMmodel* model);
//构造面法向量，假定逆时针
GLvoid glmFacetNormals(GLMmodel* model);
//构建节点法向量
GLvoid glmVertexNormals(GLMmodel* model, GLfloat angle);
//产生纹理坐标按照纹理映射的线性投影
GLvoid glmLinearTexture(GLMmodel* model);
//产生纹理坐标按照纹理映射的球形投影
GLvoid glmSpheremapTexture(GLMmodel* model);
//清除当前模型
GLvoid glmDelete(GLMmodel* model);
//读取OBJ文件
GLMmodel* glmReadOBJ(char* filename);
//将模型写入到OBJ文件（导出）
GLvoid glmWriteOBJ(GLMmodel *model, char* filename, GLuint mode);
//根据指定的模式渲染模型
GLvoid glmDraw(GLMmodel *model, GLuint mode);
//为指定的模型产生一个展示列表
GLuint glmList(GLMmodel *model, GLuint mode);
//消除（焊接）邻域的向量
GLuint glmWeld(GLMmodel* model, GLuint mode);
#endif