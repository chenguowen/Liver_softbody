// 
#pragma once
#ifndef __OCTREE_CGW_H__
#define __OCTREE_CGW_H__

#include "LoadObj.h"
#include "math.h" 
 
bool  if_in_Boundbox( float3 point3d , float3* boundbox ) ; // 
// 
//void Octree_construtct( TriangleMesh * mesh , int threshold  , int depth  ) ;
//void Octree_construtct( TriangleMesh * mesh , int threshold  , int depth  ) ; 
//
void Octree_construtct( TriangleMesh * local_mesh , Octree *octree_tmp  , int threshold , int depth ) ; 
// 
void Octree_update( TriangleMesh * mesh ) ;
// 
int  Octree_Intersection( float *octree_tmp , int length , float3 point , float threshold );

void Octree_Show ( Octree *octree_tmp ) ; 

#endif
/*
  Leap motion 快速定位手指的位置   // 
  利用八叉树 快速返回碰触的是哪个面片  //  // 

*/
