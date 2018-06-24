#pragma once 
#ifndef __GLSL_HEADER__
#define __GLSL_HEADER__
#define  GLEW_STATIC
#include "Rotation_Control.h"
#include <cmath>
#include <GL/freeglut.h>
#include <algorithm>
#include "Multiple_View.h"
#include <stdio.h>
#include <Windows.h>
#include "stdlib.h"
#include "math.h"
#include "LoadObj.h"
#include <GL/glaux.h>
#include "Octree_cgw.h" 

// #include "Cgw_Softbody.h" //  
#include <stdio.h>  
#undef random 
#include "NxPhysics.h" 
#include"common\SoftBody\MySoftBody.h"
#include"common\SoftBody\ObjMesh.h"
#pragma comment( lib, "PhysXLoader.lib" )
#pragma comment( lib, "NxCooking.lib" )

#pragma comment( lib, "glaux.lib"   )

#define BUFFER_OFFSET(i) ((void*)(i))

class GLSL_Loader 
{
public: 
	int size  ;
	bool gHardwareSoftBodySimulation  ;
	bool             gPause  ;
	NxPhysicsSDK*    gPhysicsSDK  ;
	NxScene*         gScene  ;
	NxVec3           gDefaultGravity; //  (0.0f, -9.81f, 0.0f);
	float            gRatio  ;
	// ************  Actor globals  ************ //  
	int ** Vertex_face_Linkage ; // 顶点到 //
	float *  updated_normals;


	char *fileName ;   //     
	char tetFileName[256], objFileName[256], s[256]    ;   // 系统思想的太严重 // 
    /*****************************************/
	TriangleMesh  mesh  ;  // 
	NxActor* groundPlane    ; // Focus actor
	NxActor* gSelectedActor  ; // Focus actor
	NxSoftBody* gSelectedSoftBody   ; // Array of SoftBody objects 
	int	  gSelectedSoftBodyVertex   ; // Array of SoftBody objects
	NxArray<MySoftBody*> gSoftBodies ;
	NxArray<ObjMesh *>   gObjMeshes  ;
	bool  shadows          ;  // 
	bool  gShowTetraMesh   ;  // 
	int   counter          ;  //
	NxActor* actor   ; //    
	int maxVertices  ; //
	// ******************************  // 
	float *received_vertexes; //  
    // ******************************  //  

	void GLSL_Loader::Normal_Update(); 
	void GLSL_Loader::InitNx( float sizex, float sizey, float sizez );  //  
	void GLSL_Loader::RenderCall_Back( NxMat34 stylus_pose ); //       
	void GLSL_Loader::RenderCall_Back(NxVec3 velocity, NxMat33 stylus_orient ); // 
	void GLSL_Loader::RenderCall_Back(NxVec3 velocity); //  
	// 
	void GLSL_Loader::InitialiseSystem(float sizex, float sizey, float sizez);
	void GLSL_Loader::SetupParticles(int np, int ns)   ; // 
	//     // 
	NxSoftBodyDesc softBodyDesc;  
public:    
	// 
	void  GLSL_Loader::GLSL_Vertex_Frag_Enable3(  int begin_triangle , int length  ) ; 
	void  GLSL_Loader::GLSL_Vertex_Frag_Disable3() ; 
	// 
	GLSL_Loader::GLSL_Loader();
	GLSL_Loader::~GLSL_Loader();
	GLSL_Loader::GLSL_Loader( string f_name ) ; //  

	string filename ; 

	int* Selected_Vertex; // 
	int Selected_vertex_count;  // 
	//
	float3 Probe_Hand_Origin;
	float3 Probe_Hand_direction;
	int temporary_count_; // cgw comment this // 

   // 
	GLuint    gVertexProgram           ;
	GLuint    gFragmentProgram         ;
	GLuint    gLinkedProgram           ;

	GLuint glParam_ot_Param ;
	GLuint gParam_IV_Params ; 
	GLuint gParam_VPMatrix  ;
	GLuint gParam_WMatrix   ;  
	// 

	MyVertex       * pvertex   ;  // 
	MyVertex       * pvertex_backup ;  
	int            * indexData   ; // 
	Multiple_View  * multi_cam2  ; // 


	// *********************************************  // 
	GLuint    gVertexProgram_normal           ;
	GLuint    gFragmentProgram_normal         ;
	GLuint    gLinkedProgram_normal           ;

	GLuint   glParam_ot_Param_normal ;
	GLuint   gParam_IV_Params_normal ; 
	GLuint   gParam_VPMatrix_normal  ;
	GLuint   gParam_WMatrix_normal   ;  
	// **************** IV 的 vertex profile **************** //
	// **************** IV 的 vertex profile **************** // 
	GLfloat                   Max_Size ;
	GLuint                    gVertexBuffer;
	GLuint					  gIndexBuffer;
	GLuint                    texture[2]  ;

	void        GLSL_Loader::GLERR()  ; 
	void        GLSL_Loader::Model_Load() ; // 
	void        GLSL_Loader::Model_Load_Soft_Simulation( float3 size ) ; // 
	// 
	void        GLSL_Loader::tet_deal( int *Select_Original_Vertex );
	void        GLSL_Loader::GLSL_INIT()  ;
	// 
	void        GLSL_Loader::LoadGLTextures() ;
	void        GLSL_Loader::LoadGLTextures( string f_name  )  ; // 
	void        GLSL_Loader::LoadGLTextures( string f_name , int m  )  ;  // 
	// 
	void        GLSL_Loader::GLSL_Vertex_Frag_Enable(float center_x , float center_y , float window_x , float window_y, float pitch, float pitchy ,float backfront_flag, GLdouble *clip_plane );
	void        GLSL_Loader::GLSL_Vertex_Frag_Disable(); 

	void        GLSL_Loader::GLSL_Vertex_Frag_Enable2(); 
	void        GLSL_Loader::GLSL_Vertex_Frag_Enable2(int soft); // 
	void        GLSL_Loader::GLSL_Vertex_Frag_Disable2(); 

	void        GLSL_Loader::XPPrint(const char *format, ...) ;
	GLenum      GLSL_Loader::CreateShader(const char *filename, GLenum type) ;
	void        GLSL_Loader::LoadOBJ(const char *filename) ;
	void        GLSL_Loader::glsl_validate_program () ;
	void        GLSL_Loader::GLSL_INIT_2()  ; 
	int         GLSL_Loader::Intersect_leap_probe( GLfloat * Transformation_of_Model  ) ; 
	//  
	float  *model_view ;
	//  
	float  *project_matrix ;
	//  
	bool Back_Flag ;
	int  Face_Num  ;
	int  Mesh_Size ;
	bool b_ina_Flag ;
	bool c_ina_Flag ;
	bool c_inb_Flag ;
	bool ab_near_Flag ;
	bool bc_near_Flag ;
	bool ca_near_Flag ;
	bool share_the_same_face1 ;
	bool share_the_same_face2 ;
	bool share_the_same_face3 ;
	bool share_the_same_face4 ;
	bool addition_1;
	bool addition_2;
	bool addition_3;
	bool addition_4;
protected:

private: 
	int  window_w , window_h ;
	bool Full_Screen_Flag ;
};
#endif



