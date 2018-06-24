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
	int v[3];             // vertex indices �洢
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

// cgw �˲��� 
struct Octree   // void Octree::Octree() {} ;  // 
{   // int* index_vector;           //
	vector<int> index_vector ; //  

	int  son_count    ;  // 
	int  depth        ;  // 
	int  is_Leaf_flag ;  //  
	int  is_Root_flag ;  //  
	// Ӧ�ô洢���������������Ƕ������� // 
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
	vector<float3> verts;       // ��������

	vector<TriangleFace> faces; // �����ε�����������vert�����е�λ�� 8052 �� 2 

	vector<float3> Normals ;    // ����ķ��� ������ 8052

	vector<float3> Texture_2D ; // �洢���������

	//ģ�͵İ�Χ��
	float3 bounding_box[2];
	//ģ�͵İ�Χ�����
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

// Meffy: �ڽ���
struct LinkVertex
{
	int    index  ;        // �������� //   // 
	float  length ;        // ����     // �ܹ��ĳ��� �е�ʱ�򣨹�����������м䣩��ʾ�Ƿ���ʹ� //  
};

// Meffy: ����������
struct NearFace
{
	int index;     // ����
	float angle;   // �н�
};


// Meffy: tet����
struct Tet_List
{
	int index;
	float x,y,z;
};

// Meffy: TetData����
struct TetData
{
	int index;
	int length;
};


//int **link = (int**) malloc(100*sizeof(int*)) ;  // ��ά����
//link[0][0]=10;
//link[0][1-10]


#endif