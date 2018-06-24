#pragma once

#ifndef __Cgw_Softbody_HEADER__
#define __Cgw_Softbody_HEADER__

#include <stdio.h>  
#undef random 
#include "NxPhysics.h" 
#include"common\SoftBody\MySoftBody.h"
#include"common\SoftBody\ObjMesh.h"
#pragma comment( lib, "PhysXLoader.lib" )
#pragma comment( lib, "NxCooking.lib" )
#include "LoadObj.h" 


class My_softbody_class
{
public: 
	int size = 4; 
	bool gHardwareSoftBodySimulation = true;
	bool             gPause = false;
	NxPhysicsSDK*    gPhysicsSDK = NULL;
	NxScene*         gScene = NULL;
	NxVec3           gDefaultGravity; //  (0.0f, -9.81f, 0.0f);
	float            gRatio = 1.0f;
	// ************  Actor globals  ************ //  
	char *fileName   = "Data//Liver_resized_to_2_Physx_export";//    
	//  
	char tetFileName[256]   ,   objFileName[256]   ,   s[256]     ;  // 系统思想的太严重 // 
	char *fileName_2 = "Data//Liver_resized_to_2_Physx_export_Tet.obj"  ;  // 系统思想的太严重 // 

	/*****************************************/

	TriangleMesh  mesh;  // 
	NxActor* groundPlane    = NULL; // Focus actor
	NxActor* gSelectedActor = NULL; // Focus actor
	NxSoftBody* gSelectedSoftBody       = NULL ;
	int		    gSelectedSoftBodyVertex = -1   ;   // Array of SoftBody objects
	NxArray<MySoftBody*> gSoftBodies;
	NxArray<ObjMesh *>   gObjMeshes ;
	bool shadows = true;
	bool gShowTetraMesh = true;
	int counter = 0; 
	// 
	     My_softbody_class::My_softbody_class();  
	void My_softbody_class::InitNx( )          ;  //  
	void My_softbody_class::RenderCall_Back( ) ; // 
	// 
}; // 


#endif // 
