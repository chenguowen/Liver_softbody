#include "Octree_cgw.h" 
//
bool if_in_Boundbox( float3 point3d , float3* boundbox )
{
	if( point3d.x >= boundbox[0].x && point3d.x <  boundbox[1].x && 
		point3d.y >= boundbox[0].y && point3d.y <  boundbox[1].y && 
		point3d.z >= boundbox[0].z && point3d.z <  boundbox[1].z )
	{ 	return true ;  	}
	else  // 
	{ 	return false ;  } // 
}
//                  //
void Octree_construtct( TriangleMesh * local_mesh , Octree *octree_tmp  , int threshold , int depth )
{ // 
	int count  ; //  
	cout << "***** depth = " << depth << " *********** " << endl; 
	octree_tmp->sons = ( Octree ** ) calloc ( 8 , sizeof (Octree *) ) ; //
	if ( depth==0 ) // 
	{   
		// 根节点 // 
		count = local_mesh->verts.size()   ; // 
		// 
		octree_tmp->depth         = depth  ; // 
		octree_tmp->is_Root_flag  = true   ; //  
		//  
		for ( int  i = 0 ; i<count ; i++ )
		{
			octree_tmp->index_vector.push_back( i ) ; // 
		}
		octree_tmp->bounding_box[0] = local_mesh->bounding_box[0] - float3 ( 1 ,1 ,1 ) ;  //   
		octree_tmp->bounding_box[1] = local_mesh->bounding_box[1] + float3 ( 1 ,1 ,1 )  ;  //   
		if (count<threshold)
		{
			octree_tmp->is_Leaf_flag = true ; // 
		}
		else
		{
			octree_tmp->is_Leaf_flag = false ; //  
		}   // 
	}       // 
	else
	{
		octree_tmp->depth         = depth ; // 
	} 
	// 
	Octree *octree_sons = (Octree *) calloc( 8 , sizeof ( Octree ) ) ;  // 
	for ( int i = 0 ; i<2 ;i++ ) // 
	{
		for ( int j = 0 ; j<2 ;j++ ) // 
		{
			for ( int k = 0 ; k<2 ;k++ ) 
			{   //
				if( _DEBUG_FLAG_ ) 
				{
					cout<<"i="<<i<<endl ;  // 
					cout<<"octree_tmp->bounding_box [0].x = "<<octree_tmp->bounding_box [0].x<<endl; 
				} // 
				octree_sons[i+j*2+k*4].bounding_box [0].x = octree_tmp->bounding_box [0].x + i * ( octree_tmp->bounding_box [1].x - octree_tmp->bounding_box [0].x  )  /2.0       ;     // 
				octree_sons[i+j*2+k*4].bounding_box [1].x = octree_sons[i+j*2+k*4].bounding_box [0].x + ( octree_tmp->bounding_box [1].x - octree_tmp->bounding_box [0].x  ) /2.0 ;     // 
				// 
				octree_sons[i+j*2+k*4].bounding_box [0].y = octree_tmp->bounding_box [0].y + j * ( octree_tmp->bounding_box [1].y - octree_tmp->bounding_box [0].y  )  /2.0       ;     // 
				octree_sons[i+j*2+k*4].bounding_box [1].y = octree_sons[i+j*2+k*4].bounding_box [0].y + ( octree_tmp->bounding_box [1].y - octree_tmp->bounding_box [0].y  ) /2.0 ;     // 
				// 
				octree_sons[i+j*2+k*4].bounding_box [0].z = octree_tmp->bounding_box [0].z + k * ( octree_tmp->bounding_box [1].z - octree_tmp->bounding_box [0].z  )  /2.0       ;     // 
				octree_sons[i+j*2+k*4].bounding_box [1].z = octree_sons[i+j*2+k*4].bounding_box [0].z + ( octree_tmp->bounding_box [1].z - octree_tmp->bounding_box [0].z  ) /2.0 ;     // 
				// 
				octree_sons[i+j*2+k*4].depth =  depth + 1   ; // 
				if( _DEBUG_FLAG_ ) 
				{
					cout<<"octree_tmp->bounding_box [1].x = "<<octree_tmp->bounding_box [1].x<<endl; 
				}
				int temp = 0 ; 
				for ( int kk= 0 ;kk<octree_tmp->index_vector.size() ;kk++ ) 
				{
					if( _DEBUG_FLAG_ ) 
					{
						temp++ ; //  
						cout<< "temp = "<< temp <<endl ;  // 
					}
					if ( if_in_Boundbox ( local_mesh->verts[ octree_tmp->index_vector[kk] ] , octree_sons[i+j*2+k*4].bounding_box   )  )
					{
						if( _DEBUG_FLAG_ ) 
						{
							cout<<"j="<<j<<endl ;  
						}
						octree_sons[i+j*2+k*4].index_vector.push_back(  octree_tmp->index_vector[kk]  ) ;  // ;    
						if( _DEBUG_FLAG_ ) 
						{
							cout<<"k="<<k<<endl ;  
						}
						// octree_tmp->index_vector.erase(kk) ;  // 
					}
				}
				if( _DEBUG_FLAG_ ) 
				{
					cout<<"doned"<<endl ; 
					cout<<"3"<<endl ; 
				}
			} // 
		}
	} // 
	if( _DEBUG_FLAG_ ) 
	{
		cout<<"4"<<endl ;  // 
	}
	octree_tmp->son_count = 0 ; 
	// 
	for ( int i = 0 ; i<2 ;i++ ) 
	{
		for ( int j = 0 ; j<2 ;j++ ) 
		{
			for ( int k = 0 ; k<2 ;k++ ) 
			{   //
				if( octree_sons[i+j*2+k*4].index_vector.size() > 0  ) // 如果子树非空 
				{   // 
					// cout<<"5"<<endl ; 
					if( _DEBUG_FLAG_ ) 
					{
						cout<<"depth = "<<depth<<endl ;  
					}
					octree_tmp->sons[ octree_tmp->son_count ] =   &octree_sons[i+j*2+k*4 ]; // ;
					// octree_tmp->sons.push_back ( octree_sons[i+j*2+k*4] ) ;  // 压入堆栈中间  
					// octree_tmp->sons[ octree_tmp->son_count ] ;
					if( _DEBUG_FLAG_ ) 
					{
						cout<<"5.5"<<endl ; // 
					}
					if ( octree_tmp->sons[ octree_tmp->son_count ]->index_vector.size() > threshold ) 
					{
						if( _DEBUG_FLAG_ ) 
						{
							cout<<"5.7"<<endl ;  
						}
						Octree_construtct( local_mesh , octree_tmp->sons[ octree_tmp->son_count ]    ,  threshold , depth + 1 ) ; //   
						if( _DEBUG_FLAG_ ) 
						{
							cout<<"5.8"<<endl ; 
						}
						octree_tmp->sons[ octree_tmp->son_count ]->is_Root_flag = false ; // 
						octree_tmp->sons[ octree_tmp->son_count ]->is_Leaf_flag = false ; // 
					}
					else
					{
						octree_tmp->sons[ octree_tmp->son_count ]->is_Root_flag = false ; // 
						octree_tmp->sons[ octree_tmp->son_count ]->is_Leaf_flag = true ; // 
						// free( &octree_sons[i+j*2+k*4 ]) ; 
					}

					octree_tmp->son_count++ ; 
					if( _DEBUG_FLAG_ ) 
					{
						cout<<"6"<<endl ; 
					}
				}
				// octree_tmp.
			}
		}
	}	//  
	return ; //  
}
// 
// 
void Octree_Show ( Octree *octree_tmp )
{
	if( octree_tmp->is_Leaf_flag ==false )
	{
		for (int i=0;i<octree_tmp->son_count; i++ )
		{
			Octree_Show( octree_tmp->sons[i] ) ;  // 
		}
	}
	else
	{ 
		for (int i = 0; i < octree_tmp->index_vector.size(); i++)
		{
			cout << octree_tmp->index_vector[i] << endl;
		}
	}
} 
//    cgw2018年不用八叉树了 直接遍历  找到八叉树的交点    //   
int Octree_Intersection( float * octree_tmp , int length, float3 point, float threshold )
{   // 
	int  return_vertex_index = -1 ;  //   
	// cout << "length" << length << endl;
	cout << " point.x  " << point.x << " " << point.y << " " << point.z << endl;
	for (int i = 0; i < length ; i++) // 
	{	float3 d = point-float3(octree_tmp[3*i+0],octree_tmp[3*i+1],octree_tmp[3*i+2]) ; // x.dist 
	    // cout << octree_tmp[3 * i + 0] << endl; 
		
		if ( norm(d) < threshold) { return_vertex_index = i ; break; } // 
	}   // 
	cout << return_vertex_index << endl;
	return return_vertex_index ; //
} //  