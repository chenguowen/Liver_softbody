#pragma once
#ifndef  _LOADOBJ_H_
#define  _LOADOBJ_H_

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h> 
#include <omp.h>


#define _DEBUG_FLAG_ false 
using namespace std;
// 
// 
// #define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z*(v).z)  
#define norm2(v)     ((v).x * (v).x + (v).y * (v).y + (v).z*(v).z)        // norm2 = squared length of vector   // 
#define norm(v)       sqrt( ((v).x * (v).x + (v).y * (v).y + (v).z*(v).z)   )  // norm = length of vector       //  
#define norm_3(x,y,z)   sqrt( ( x * x + y * y + z*z)   )  // norm = length of vector            // 

#define distance_obj(u,v)     norm(u-v)       // distance = norm of difference      // 
// 
// 

// Obj loader
struct TriangleFace 
{
	int v[3];             // vertex indices 存储
	int Define_index[7] ;
	int Normal_index[3] ;
};


// 
class float3
{
public:	
	float x;
	float y;
	float z;
public:	
	float3(){x=0;y=0;z=0;}
	float3(float mx,float my,float mz){x=mx;y=my;z=mz;}
	~float3(){}

	float3 operator+(float3);
	float3 operator-(float3);
	float3 operator/(float);

	friend float3 operator*(float m,float3 f3)
	{
		return float3(f3.x*m,f3.y*m,f3.z*m);
	}
	friend float3 operator*(float3 f3,float m) //  
	{
		return float3(f3.x*m,f3.y*m,f3.z*m);
	}

	float3 operator = (float3);

	bool operator == (const float3&);

	float3& operator += (float3);   


	void float3::cross(const float3 &left, const float3 & right)	//prefered version, w/o temp object.
	{
		// temps needed in case left or right is this.
		x = (left.y * right.z) - (left.z * right.y);
		y = (left.z * right.x) - (left.x * right.z);
		z = (left.x * right.y) - (left.y * right.x);  
	} 
	void float3::normalize()	//prefered version, w/o temp object.
	{
		// temps needed in case left or right is this.
		float tmp = norm_3(x, y, z);
		x = x / tmp;
		y = y / tmp;
		z = z / tmp; 
	}
};

// 

// cgw 八叉树 
struct Octree   // void Octree::Octree() {} ;  // 
{   // int* index_vector;           //
	vector<int> index_vector ; //  

	int  son_count    ;  // 
	int  depth        ;  // 
	int  is_Leaf_flag ;  //  
	int  is_Root_flag ;  //  
	// 应该存储的三角形索引而非顶点索引 // 
	int  Min_index ;
	int  Max_index ;  
	// 
	float3 bounding_box [2] ; // 
	Octree** sons  ;    //  

	// 
};


// 
struct TriangleMesh
{
	Octree  *   obj_Octree     ; //    
	vector<float3> verts;       // 顶点坐标

	vector<TriangleFace> faces; // 三角形的三个顶点在vert数组中的位置 8052 × 2 

	vector<float3> Normals ;    // 顶点的法线 或者是 8052

	vector<float3> Texture_2D ; // 存储纹理的坐标

	//模型的包围盒
	float3 bounding_box[2];
	//模型的包围球参数
	float3 bounding_sphere_c;
	float bounding_sphere_r;
};

void loadObj(const std::string filename, TriangleMesh &mesh);

struct MyVertex
{
	float x, y, z;        //Vertex
	float nx, ny, nz;     //Normal
	float s0, t0;         //Texcoord0
};

// Meffy: 邻近表
struct LinkVertex
{
	int    index  ;        // 顶点索引 //   // 
	float  length ;        // 距离     // 总共的长度 有的时候（广度优先搜索中间）表示是否访问过 //  
};

// Meffy: 相邻面索引
struct NearFace
{
	int index;     // 索引
	float angle;   // 夹角
};


// Meffy: tet索引
struct Tet_List
{
	int index;
	float x,y,z;
};

// Meffy: TetData索引
struct TetData
{
	int index;
	int length;
};


//int **link = (int**) malloc(100*sizeof(int*)) ;  // 二维数组
//link[0][0]=10;
//link[0][1-10]


#endif