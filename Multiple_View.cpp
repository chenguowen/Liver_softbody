 #include "Multiple_View.h"
#include <omp.h>

void Pitch_Reset_fcn() {} 

// Construction 
Multiple_View::Multiple_View(   ) 
{
	double_of_pixel =  1 ; 
	IsFirst_Flag    =  1 ;
	IF_Square       =  1 ; // 正方形 
	WIDTH        = 3840/2*double_of_pixel ; //  
	HEIGHT       = 2400/2*double_of_pixel ; //
	
	if (IF_Square < 0 || IsFirst_Flag > 0.5 )
	{
		PITCH = 2.32   ; //  
		PITCH_Y = PITCH * sqrt(3.0) / 2.0;
		PPI   = 264 * double_of_pixel; // 
		GAP   = 3.0   ; // 
		dx    = 25.4 / double(PPI);
		// 
		Radius_X = (PITCH / dx) / 2.0;           // 目前为横向的最大可能半径 //
		Radius_Y = (PITCH_Y / dx) / 2.0;           // 目前为纵向的最大可能半径 //

		Perspective_Pixel_num_x = Radius_X * 2 + 1; // 单个透镜下面的像素数量
		Perspective_Pixel_num_y = Radius_Y * 2 + 1; // 单个透镜下面的像素数量
		LENS_NUM_X = WIDTH  * dx / PITCH; // 
		LENS_NUM_Y = HEIGHT * dx / PITCH_Y / 2; // 
		// 
		MLA_NUMBDER_HEXAGON = LENS_NUM_X * LENS_NUM_Y * 2 ; //
		cout << "MLA_NUMBDER_HEXAGON = " << MLA_NUMBDER_HEXAGON << endl;
	} // 
	cout << "MLA_NUMBDER_HEXAGON = " << MLA_NUMBDER_HEXAGON << endl;
	//      // 
	if ( IF_Square > 0 )   //  
	{ // 
		PITCH        = 8.0  ; //  
		PITCH_Y      = PITCH ;
		PPI          = 102 *double_of_pixel  ; // 
		GAP          = 12  ; //
		dx           = 25.4/double(PPI);  
		// 
		Radius_X     = (PITCH/dx)/2.0 ;       // 目前为横向的最大可能半径
		Radius_Y     = (PITCH_Y/dx)/2.0 ;       // 目前为纵向的最大可能半径
		// 
		Perspective_Pixel_num_x = Radius_X*2+1 ; // 单个透镜下面的像素数量
		Perspective_Pixel_num_y = Radius_Y*2+1 ; // 单个透镜下面的像素数量
		// //printf("TEXTURE_WIDTH_Set = %d \n",TEXTURE_WIDTH_Set);
		// 
		LENS_NUM_X = WIDTH  * dx / PITCH   ; // 
		LENS_NUM_Y = HEIGHT * dx / PITCH_Y ; // 
		// 
		MLA_NUMBDER_RECTANGLE  = LENS_NUM_X * LENS_NUM_Y  ; // 
	}

	Bound_box_Init[0].x = -80 ;  // 
	Bound_box_Init[0].y = -80 ;  // 
	Bound_box_Init[0].z = -80 ;  // 

	Bound_box_Init[1].x =  80 ;  // 
	Bound_box_Init[1].y =  80 ;  // 
	Bound_box_Init[1].z =  20 ;  // 

	Multiple_Viewport_Set() ;                         // 
	Multiple_Lens_Project_Matrix_Generation( ) ;      // 
	View_Render_Flag_Launch( Identity_Matrix , Bound_box_Init ) ;  // 
	// 
	IsFirst_Flag  =  0 ;  // 
}
// 
//  
void Multiple_View::View_Render_Flag_Update () // 如果模型位置改变后需要实时update 
{ // 
	glm::vec4 point_edge[8]  ; // 

	//   计算出的box的四个角空间位置   //  
	for ( int i=0; i<2; i++ ) // 
	{
		for ( int j=0; j<2; j++ ) // 
		{
			for ( int k=0; k<2; k++ ) // 
			{
				point_edge[i*2*2+j*2+k][0] = Bound_box_Init[i].x ; 
				point_edge[i*2*2+j*2+k][1] = Bound_box_Init[j].y ; 
				point_edge[i*2*2+j*2+k][2] = Bound_box_Init[k].z ; 
				point_edge[i*2*2+j*2+k][3] = 1; 
			}
		}
	}

	for ( int i=0; i<8 ; i++ ) // 
	{
		point_edge[i] =    Identity_Matrix * point_edge[i]  ; // 计算出新位置 // 
		// cout<<" points=  "<<i<< "  "<<point_edge[i][0]<<"  "<<point_edge[i][1]<<"  "<<point_edge[i][2]<<"  "<<point_edge[i][3]<<endl ;
	}
	// 
	double halfPitch_X = PITCH   / 2.0 /GAP ; 
	double halfPitch_Y = PITCH_Y / 2.0 /GAP ; 

	if ( IF_Square > 0)
	{
		#pragma omp parallel for
		for ( int i = 0; i< this->LENS_NUM_X  ;i++ )
		{
			#pragma omp parallel for 
			for ( int j = 0 ;j< this->LENS_NUM_Y  ; j++ ) 
			{
				//View_Render_Flag[i][j][0]       =  true ;  // false  ;   //  center in xy direction	// 前面		
				//View_Render_Flag[i][j][1]       =  true ;  // false  ;   //  center in xy direction    // 背面   
				//    View_Center_Back[i][j]   ///
				int num_less_back[]  = {0,0}   , num_greater_back[]  = {0,0}  ,
					num_less_front[] = {0,0}   , num_greater_front[] = {0,0}  ; 

				// int num_less_back  , num_greater_back ,  num_less_front ,num_greater_front  ;  
				for ( int kk = 0 ; kk < 8 ; kk++ ) // 
				{
					// 
					if( point_edge[kk][2] <  0   )
					{
						if( - (  View_Center_Back[i][j][4] - point_edge[kk][0] ) / point_edge[kk][2] > - halfPitch_X  )  
						{
							num_greater_back[0] ++ ; 
						}
						if( - ( View_Center_Back[i][j][4] - point_edge[kk][0] ) / point_edge[kk][2] <   halfPitch_X ) 
						{
							num_less_back[0] ++ ; 
						}

						if(- ( View_Center_Back[i][j][5] - point_edge[kk][1] ) / point_edge[kk][2] >  - halfPitch_Y  ) 
						{
							num_greater_back[1] ++ ; 
						}
						if(- ( View_Center_Back[i][j][5] - point_edge[kk][1] ) / point_edge[kk][2] <    halfPitch_Y  ) 
						{
							num_less_back[1] ++ ; 
						}  
					} 
					else
					{
						if(   (  View_Center_Back[i][j][4] - point_edge[kk][0] ) / point_edge[kk][2] > - halfPitch_X  )  
						{
							num_greater_front[0] ++ ; 
						}

						if(   ( View_Center_Back[i][j][4] - point_edge[kk][0] ) / point_edge[kk][2] <   halfPitch_X ) 
						{
							num_less_front[0] ++ ; 
						}

						if(  ( View_Center_Back[i][j][5] - point_edge[kk][1] ) / point_edge[kk][2] >  - halfPitch_Y  ) 
						{
							num_greater_front[1] ++ ; 
						}
						if(  ( View_Center_Back[i][j][5] - point_edge[kk][1] ) / point_edge[kk][2] <    halfPitch_Y  ) 
						{
							num_less_front[1] ++ ; 
						} 
					}   


					View_Render_Flag[i][j][0] =  ( num_less_front[1]> 0   )&& ( num_greater_front[1] > 0  ) &&  ( num_less_front[0] > 0  )&& ( num_greater_front[0] > 0  )  ; 
					View_Render_Flag[i][j][1] =  ( num_less_back[1] > 0   )&& ( num_greater_back [1] > 0  ) &&  ( num_less_back [0] > 0  )&& ( num_greater_back[0]  > 0  )  ;
					 
				}
			}
		}
	} // 
	else  //  六边形 
	{
		//
		#pragma omp parallel for
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			#pragma omp parallel for
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{
				View_Render_Flag[i][2*j+0][0] = false  ;
				View_Render_Flag[i][2*j+0][1] = false  ;
				// 
				View_Render_Flag[i][2*j+1][0] = false  ;
				View_Render_Flag[i][2*j+1][1] = false  ;

				//  View_Center_Back[i][j]
				for ( int i = 0; i<8 ; i++ ) // 
				{
					if( point_edge[i][2] > 0  )
					{ //  View_Center_Back[ 0-1-2-3 ] 存储的是我们的东西   //  
						View_Render_Flag[i][2*j+0][0] = View_Render_Flag[i][2*j+0][0] || (
							abs ( View_Center_Back[i][2*j][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 

						View_Render_Flag[i][2*j+1][0] = View_Render_Flag[i][2*j+1][0] || (
							abs ( View_Center_Back[i][2*j+1][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j+1][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 
					} 
					else
					{ // 
						View_Render_Flag[i][2*j+0][1] = View_Render_Flag[i][2*j+0][1] || (
							abs ( View_Center_Back[i][2*j][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 

						View_Render_Flag[i][2*j+1][1] = View_Render_Flag[i][2*j+1][1] || (
							abs ( View_Center_Back[i][2*j+1][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j+1][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 
					}// =  Model_View_Matrix * point_edge[i]  ; // 计算出新位置 // 
				}
			}
		}
	}
}
//  
void Multiple_View::View_Render_Flag_Launch ( glm::mat4 Model_View_Matrix , float3* Bound_box ) // 
{
	glm::vec4 point_edge[8]  ; // 

	//   计算出的box的四个角空间位置   //  
	for ( int i=0; i<2; i++ ) // 
	{
		for ( int j=0; j<2; j++ ) // 
		{
			for ( int k=0; k<2; k++ ) // 
			{
				point_edge[i*2*2+j*2+k][0] = Bound_box[i].x ; 
				point_edge[i*2*2+j*2+k][1] = Bound_box[j].y ; 
				point_edge[i*2*2+j*2+k][2] = Bound_box[k].z ; 
				point_edge[i*2*2+j*2+k][3] = 1; 
			}
		}
	}  
	//                      //   
	//                      // 
	for ( int i=0; i<8 ; i++ ) // 
	{
		point_edge[i] =  Model_View_Matrix * point_edge[i]  ; // 计算出新位置 // 
		cout<<" points=  " <<point_edge[i][0]<<"  "<<point_edge[i][1]<<"  "<<point_edge[i][2]<<"  "<<point_edge[i][3]<<endl ;
	}
	// 

	if ( IF_Square > 0)
	{
		View_Render_Flag = (bool  ***) malloc( sizeof(bool  **) * ( (this->LENS_NUM_X  ) ) )  ;
		#pragma omp parallel for
		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			View_Render_Flag[i]      = (bool  **) malloc( sizeof(bool  *) * ( this->LENS_NUM_Y  )  )  ; 
		}
		//
		#pragma omp parallel for
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			#pragma omp parallel for
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{    
				View_Render_Flag[i][j] = (bool  *) malloc( sizeof(bool  ) * 4  )  ;
			}
		}
		#pragma omp parallel for
		for ( int i = 0; i< this->LENS_NUM_X  ;i++ )
		{
			#pragma omp parallel for
			for ( int j = 0 ;j< this->LENS_NUM_Y  ; j++ ) 
			{
				View_Render_Flag[i][j][0]       =  false ;  // false  ;   //  center in xy direction	// 前面		
				View_Render_Flag[i][j][1]       =  false ;  // false  ;   //  center in xy direction    // 背面   
				//    View_Center_Back[i][j]   /// 
				#pragma omp parallel for
				for ( int i=0; i<8 ; i++ ) // 
				{
					if( point_edge[i][2] > 0   )
					{
						View_Render_Flag[i][j][0] = View_Render_Flag[i][j][0] || (
							abs ( View_Center_Back[i][j][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH   / GAP /2.0    &&  
							abs ( View_Center_Back[i][j][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 
					} 
					else
					{
						View_Render_Flag[i][j][1] = View_Render_Flag[i][j][1] || ( 
							abs ( View_Center_Back[i][j][4] - point_edge[i][0] ) / abs( - point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][j][5] - point_edge[i][1] ) / abs( - point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  //  
					}  // 
					// =  Model_View_Matrix * point_edge[i]  ; // 计算出新位置 // 
				}
			}
		}
	} // 
	else  //  六边形 
	{
		View_Render_Flag = (bool  ***) malloc( sizeof(bool  **) * ( (this->LENS_NUM_X  ) ) )  ;

		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			View_Render_Flag[i]       = (bool  **) malloc( sizeof(bool  *) * 2 * ( this->LENS_NUM_Y  )  )  ; 
		}
		// 
		for ( int i = 0; i<this->LENS_NUM_X ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y ;j++)
			{    
				View_Render_Flag[i][2*j+0]       = (bool  *) malloc( sizeof(bool  ) * 4  )  ;
				View_Render_Flag[i][2*j+1]       = (bool  *) malloc( sizeof(bool  ) * 4  )  ;
			}
		}
		//
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{
				View_Render_Flag[i][2*j+0][0] = false  ;
				View_Render_Flag[i][2*j+0][1] = false  ;
				// 
				View_Render_Flag[i][2*j+1][0] = false  ;
				View_Render_Flag[i][2*j+1][1] = false  ;

				//  View_Center_Back[i][j]
				for ( int i = 0; i<8 ; i++ ) // 
				{
					if( point_edge[i][2] > 0  )
					{ //  View_Center_Back[ 0-1-2-3 ] 存储的是我们的东西   //  
						View_Render_Flag[i][2*j+0][0] = View_Render_Flag[i][2*j+0][0] || (
							abs ( View_Center_Back[i][2*j][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 

						View_Render_Flag[i][2*j+1][0] = View_Render_Flag[i][2*j+1][0] || (
							abs ( View_Center_Back[i][2*j+1][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j+1][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 
					} 
					else
					{ // 
						View_Render_Flag[i][2*j+0][1] = View_Render_Flag[i][2*j+0][1] || (
							abs ( View_Center_Back[i][2*j][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 

						View_Render_Flag[i][2*j+1][1] = View_Render_Flag[i][2*j+1][1] || (
							abs ( View_Center_Back[i][2*j+1][4] - point_edge[i][0] ) / abs(  point_edge[i][2]  ) < PITCH / GAP /2.0    &&  
							abs ( View_Center_Back[i][2*j+1][5] - point_edge[i][1] ) / abs(  point_edge[i][2]  ) < PITCH_Y / GAP /2.0  )  ;  // 
					}// =  Model_View_Matrix * point_edge[i]  ; // 计算出新位置 // 
				}
			}
		}
	}
}
// 
void Multiple_View::View_Render_Flag_Free () // 
{ // 
	if ( IF_Square > 0) 
	{
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{
				free ( View_Render_Flag[i][j] ) ;  
			}
		}
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			free ( View_Render_Flag[i] ) ;   // 
		}
		free ( View_Render_Flag ) ; // 
	}
	else
	{
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{
				free ( View_Render_Flag[i][2*j] ) ;  
				free ( View_Render_Flag[i][2*j+1 ] ) ;  
			}
		}
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			free ( View_Render_Flag[i] ) ;   // 
		}
		free ( View_Render_Flag ) ; // 
	}
}
// 
void Multiple_View::Multiple_Viewport_Set()
{
	// 普通的透镜需要给出来那些量呢：透镜的初始像素坐标（xy） Viewportal的大小(xy) 
	// View的几何Center , Bottom, UP, Left, Right ; 
	if ( IF_Square > 0)
	{
		Multiple_Viewport = (int  ***) malloc( sizeof(int  **) * ( (this->LENS_NUM_X  ) ) )  ;
		View_Center_Back  = (float***) malloc( sizeof(float**) * ( (this->LENS_NUM_X  ) ) )  ;
		// View_Center_Front = (float***) malloc( sizeof(float**) * ( (this->LENS_NUM_X  ) ) )  ;

		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			Multiple_Viewport[i]      = (int  **) malloc( sizeof(int  *) * ( this->LENS_NUM_Y  )  )  ; 
			View_Center_Back[i]       = (float**) malloc( sizeof(float*) * ( this->LENS_NUM_Y  )  )  ; 
			// View_Center_Front[i]      = (float**) malloc( sizeof(float*) * ( this->LENS_NUM_Y  )  )  ; 
		}
		// 
		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{    
				Multiple_Viewport[i][j] = (int  *) malloc( sizeof(int  ) * 4  )  ;
				View_Center_Back [i][j] = (float*) malloc( sizeof(float) * 6  )  ;
				// View_Center_Front[i][j] = (float*) malloc( sizeof(float) * 4  )  ;
			}
		}
		for ( int i = 0; i< this->LENS_NUM_X  ;i++ )
		{
			for ( int j = 0 ;j< this->LENS_NUM_Y  ; j++ ) 
			{
				
				View_Center_Back[i][j][4]       =  i*PITCH    - WIDTH  *dx / 2  - 0*this->dx  ;   //  center in xy direction 
				View_Center_Back[i][j][5]       =  j*PITCH_Y  - HEIGHT *dx / 2  - 0*this->dx  ;   //  center in xy direction 
				
				Multiple_Viewport[i][j][0] = (View_Center_Back[i][j][4] + WIDTH   *dx / 2  )  / this->dx -  this->Radius_X   ; // 起始点的像素
				Multiple_Viewport[i][j][1] = (View_Center_Back[i][j][5] + HEIGHT  *dx / 2  )  / this->dx -  this->Radius_Y   ;   
				Multiple_Viewport[i][j][2] = this->Radius_X*2 + 1 ; // 窗口大一点好
				Multiple_Viewport[i][j][3] = this->Radius_Y*2 + 1 ; // 窗口大一点好
				// glViewport() 
				View_Center_Back[i][j][0]       =  Multiple_Viewport[i][j][0]*dx + 0.0*dx - i*this->PITCH    ;                                // 左侧的窗宽 
				View_Center_Back[i][j][1]       =  Multiple_Viewport[i][j][0]*dx + 0.0*dx + Multiple_Viewport[i][j][2]*dx - i*this->PITCH   ; // 右侧的窗宽 
 				View_Center_Back[i][j][2]       =  Multiple_Viewport[i][j][1]*dx + 0.0*dx - j*this->PITCH_Y  ; 
				View_Center_Back[i][j][3]       =  Multiple_Viewport[i][j][1]*dx + 0.0*dx + Multiple_Viewport[i][j][3]*dx - j*this->PITCH_Y ; //  
			}
		}
	}
	else
	{
		Multiple_Viewport = (int  ***) malloc( sizeof(int  **) * ( (this->LENS_NUM_X  ) ) )  ;
		View_Center_Back  = (float***) malloc( sizeof(float**) * ( (this->LENS_NUM_X  ) ) )  ;
		//View_Center_Front = (float***) malloc( sizeof(float**) * ( (this->LENS_NUM_X  ) ) )  ;

		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			Multiple_Viewport[i]       = (int  **) malloc( sizeof(int  *) * 2 * ( this->LENS_NUM_Y  )  )  ; 
			View_Center_Back[i]        = (float**) malloc( sizeof(float*) * 2 * ( this->LENS_NUM_Y  )  )  ; 
			// View_Center_Front[i]       = (float**) malloc( sizeof(float*) * 2 * ( this->LENS_NUM_Y  )  )  ;
		}
		// 
		for ( int i = 0; i<this->LENS_NUM_X ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y ;j++)
			{    
				Multiple_Viewport[i][2*j+0]       = (int  *) malloc( sizeof(int  ) * 4  )  ;
				Multiple_Viewport[i][2*j+1]       = (int  *) malloc( sizeof(int  ) * 4  )  ;

				View_Center_Back[i][2*j+0]        = (float*) malloc( sizeof(float) * 6  )  ;
				View_Center_Back[i][2*j+1]        = (float*) malloc( sizeof(float) * 6  )  ;

				//View_Center_Front[i][2*j+0]        = (float*) malloc( sizeof(float) * 4  )  ;
				//View_Center_Front[i][2*j+1]        = (float*) malloc( sizeof(float) * 4  )  ;
			}
		}

		for ( int i = 0; i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y  ;j++)
			{
				Multiple_Viewport[i][2*j+0][0] = (i+0.0) * this->PITCH  /this->dx -  this->Radius_X   ;
				Multiple_Viewport[i][2*j+0][1] = (2*j+0) * this->PITCH_Y/this->dx -  this->Radius_Y   ;
				Multiple_Viewport[i][2*j+0][2] = this->Radius_X*2 + 1 ;
				Multiple_Viewport[i][2*j+0][3] = this->Radius_Y*2 + 1 ;

				Multiple_Viewport[i][2*j+1][0] = (i+0.5) * this->PITCH/this->dx   -  this->Radius_X   ;
				Multiple_Viewport[i][2*j+1][1] = (2*j+1) * this->PITCH_Y/this->dx -  this->Radius_Y   ;
				// //printf("Multiple_Viewport[i][2*j+1][0] =  %d \n ",Multiple_Viewport[i][2*j+1][0] );
				Multiple_Viewport[i][2*j+1][2] = this->Radius_X*2 + 1;
				Multiple_Viewport[i][2*j+1][3] = this->Radius_Y*2 + 1;

				View_Center_Back[i][2*j+0][0]       =  Multiple_Viewport[i][2*j+0][0] *dx + 0.0*dx - i*this->PITCH ;    // 
				View_Center_Back[i][2*j+0][1]       =  Multiple_Viewport[i][2*j+0][0] *dx + 0.0*dx + Multiple_Viewport[i][2*j+0][2]*dx - i*this->PITCH ;    // 
				View_Center_Back[i][2*j+0][2]       =  Multiple_Viewport[i][2*j+0][1] *dx + 0.0*dx - (2*j+0) * this->PITCH_Y ; 
				View_Center_Back[i][2*j+0][3]       =  Multiple_Viewport[i][2*j+0][1] *dx + 0.0*dx + Multiple_Viewport[i][2*j+0][3]*dx - (2*j+0) * this->PITCH_Y ; 
				View_Center_Back[i][2*j+0][4]       =  (i + 0.0 )*this->PITCH    - WIDTH  *dx / 2  ;
				View_Center_Back[i][2*j+0][5]       =  (2*j + 0 )*this->PITCH_Y  - HEIGHT *dx / 2  ;

				View_Center_Back[i][2*j+1][0]       =  Multiple_Viewport[i][2*j+1][0] *dx + 0.0*dx - (i+0.5)*this->PITCH ;
				View_Center_Back[i][2*j+1][1]       =  Multiple_Viewport[i][2*j+1][0] *dx + 0.0*dx + Multiple_Viewport[i][2*j+1][2]*dx - (i+0.5)*this->PITCH ;
				View_Center_Back[i][2*j+1][2]       =  Multiple_Viewport[i][2*j+1][1] *dx + 0.0*dx - (2*j+1) * this->PITCH_Y ; 
				View_Center_Back[i][2*j+1][3]       =  Multiple_Viewport[i][2*j+1][1] *dx + 0.0*dx + Multiple_Viewport[i][2*j+1][3]*dx - (2*j+1) * this->PITCH_Y ; 
				View_Center_Back[i][2*j+1][4]       =  (i + 0.5 )*this->PITCH     - WIDTH  *dx / 2  ;
				View_Center_Back[i][2*j+1][5]       =  (2*j + 1 )*this->PITCH_Y   - HEIGHT *dx / 2  ;
			}
		}
	}
}

// ******** 所有的设置 ******** //

void Multiple_View::Multiple_View_Set ( double pitch , double gap , double ppi, double width, double height , int Flag  ) 
{
	Multiple_Viewport_Free() ;
	Multiple_Lens_Project_Matrix_Free() ; //  
	Multiple_View_Center_Free() ; // 
	View_Render_Flag_Free () ;

	IF_Square       =   Flag  ;

	if ( IF_Square > 0 )
	{
		PITCH        = 8.0  ; //  
		PITCH_Y      = PITCH ;
		PPI          = 102 *double_of_pixel  ; // 
		GAP          = 12  ; //
		dx           = 25.4/double(PPI);  
		// 
		Radius_X     = (PITCH/dx)/2.0 ;       // 目前为横向的最大可能半径
		Radius_Y     = (PITCH_Y/dx)/2.0 ;       // 目前为纵向的最大可能半径
		// 
		Perspective_Pixel_num_x = Radius_X*2+1 ; // 单个透镜下面的像素数量
		Perspective_Pixel_num_y = Radius_Y*2+1 ; // 单个透镜下面的像素数量
		// //printf("TEXTURE_WIDTH_Set = %d \n",TEXTURE_WIDTH_Set);
		// 
		LENS_NUM_X = WIDTH  * dx / PITCH   ; // 
		LENS_NUM_Y = HEIGHT * dx / PITCH_Y ; // 
		// 
		MLA_NUMBDER_RECTANGLE  = LENS_NUM_X * LENS_NUM_Y  ; // 

	}
	else
	{
		PITCH        = pitch ; //  
		PITCH_Y      = PITCH * sqrt(3.0)/2.0 ; // 
		PPI          = ppi   ; // 
		GAP          = gap   ; // 
		dx           = 25.4/double(PPI); 

		Radius_X     = (PITCH   / dx  )/2.0  + 0.5  ;           // 目前为横向的最大可能半径
		Radius_Y     = (PITCH_Y / dx  )/2.0  + 0.5  ;           // 目前为纵向的最大可能半径

		WIDTH        = width  ;
		HEIGHT       = height ;

		Perspective_Pixel_num_x = Radius_X*2+1 ; // 单个透镜下面的像素数量
		Perspective_Pixel_num_y = Radius_Y*2+1 ; // 单个透镜下面的像素数量
		// //printf("TEXTURE_WIDTH_Set = %d \n",TEXTURE_WIDTH_Set);

		LENS_NUM_X = WIDTH  * dx / PITCH          + 0.5  ; // 
		LENS_NUM_Y = HEIGHT * dx / PITCH_Y / 2.0  + 0.5  ; //  
	}
	// 
	//printf("pitch = %f" , PITCH ) ;
	//printf("pitch_y = %f" , PITCH_Y ) ;
	// 
	Multiple_Viewport_Set() ;   // 包含了Viewport设定 和 
	Multiple_Lens_Project_Matrix_Generation();  // 此处为
	View_Render_Flag_Launch( Identity_Matrix , Bound_box_Init  ) ;  // 
	// Multiple_View_Center_Free();
	IsFirst_Flag = 0 ;
}

 
// 单个透镜的投影函数的_Projection_矩阵生成 // 
void Multiple_View::Multiple_Lens_Project_Matrix_Generation()
{
	float eyex , eyey , centerx , centery , eyez ,centerz ;
	if ( IF_Square > 0 )
	{ 
		Multiple_Lens_Project_Matrix_Front = (float**) calloc( LENS_NUM_X*LENS_NUM_Y  , sizeof(float) ) ; 
		Multiple_Lens_Project_Matrix_Back  = (float**) calloc( LENS_NUM_X*LENS_NUM_Y  , sizeof(float) ) ; 
		{
			int Count = 0 ;
			for (int i = 0 ; i< LENS_NUM_X ;i++ )
			{
				for (int j= 0 ; j< LENS_NUM_Y ;j++ )
				{   
					{
						
						Count = j + i * LENS_NUM_Y ; 
						if(   (Multiple_Lens_Project_Matrix_Front[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL  ||   (Multiple_Lens_Project_Matrix_Back[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL)
						{
							//printf("Not enough space!\n");
							exit(0);
						}
						else
						{ 
							eyez = 0 ;
							centerz = -100;  
							glMatrixMode(GL_PROJECTION);   glPushMatrix() ; //
							glLoadIdentity(); 
							glFrustum  ( View_Center_Back[i][j][0]/1000.0 , View_Center_Back[i][j][1]/1000.0 ,  View_Center_Back[i][j][2]/1000.0  ,  View_Center_Back[i][j][3]/1000.0  , GAP/1000.0  , 700 ); 

							eyex = centerx =   View_Center_Back[i][j][4]  ; // 
							eyey = centery =   View_Center_Back[i][j][5]  ; //  
							gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Back[Count] ); 
							
							glMatrixMode(GL_PROJECTION);   glPopMatrix() ; //

							centerz = 100; 

							glMatrixMode(GL_PROJECTION);  glPushMatrix() ; //

							glLoadIdentity(); 
							glFrustum  ( -View_Center_Back[i][j][0]/1000.0 , -View_Center_Back[i][j][1]/1000.0 ,  -View_Center_Back[i][j][3]/1000.0  ,  -View_Center_Back[i][j][2]/1000.0  , GAP/1000.0 ,700 ); 
							eyex = centerx =   View_Center_Back[i][j][4]  ; // 
							eyey = centery =   View_Center_Back[i][j][5]; //  
							gluLookAt( eyex, eyey, eyez, centerx, centery,  centerz, 0, -1 , 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Front[Count] ); 

							glMatrixMode(GL_PROJECTION);   glPopMatrix() ; //
						}
						
					}
				
				}
			}
		}
	}

	else 
	{  // 
		Multiple_Lens_Project_Matrix_Front = (float**) calloc( LENS_NUM_X*LENS_NUM_Y*2  , sizeof(float) ) ; 
		Multiple_Lens_Project_Matrix_Back  = (float**) calloc( LENS_NUM_X*LENS_NUM_Y*2  , sizeof(float) ) ; 
		{
			int Count = 0 ;
			for (int i = 0 ; i< LENS_NUM_X ;i++ )
			{
				for (int j= 0 ; j< LENS_NUM_Y ;j++ )
				{   
					Count = 2 * j + 0 + i*2*LENS_NUM_Y ; 
					{
						eyez    =  0  ; // 
						if(   (Multiple_Lens_Project_Matrix_Front[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL  ||   (Multiple_Lens_Project_Matrix_Back[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL)
						{
							//printf("Not enough space!\n");
							exit(0);
						}
						else
						{ // Back part
							glMatrixMode(GL_PROJECTION);    glPushMatrix() ; //  
							glLoadIdentity();            //  
							glFrustum  ( View_Center_Back[i][2*j][0]/1000.0 , View_Center_Back[i][2*j][1]/1000.0 ,  View_Center_Back[i][2*j][2]/1000.0  ,  View_Center_Back[i][2*j][3]/1000.0  , GAP/1000.0  , 500 ); 

							centerz = -100; 

							eyex = centerx =   View_Center_Back[i][2*j][4]  ; // 
							eyey = centery =   View_Center_Back[i][2*j][5]; //  
							gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Back[Count] ); 
							glMatrixMode(GL_PROJECTION);    glPopMatrix() ; //  


							// Front part 
							centerz =  100;   
							glMatrixMode(GL_PROJECTION);    glPushMatrix() ; //  
							glLoadIdentity(); 
							glFrustum ( -View_Center_Back[i][2*j][0]/1000.0 , -View_Center_Back[i][2*j][1]/1000.0 ,  View_Center_Back[i][2*j][2]/1000.0  ,  View_Center_Back[i][2*j][3]/1000.0  , GAP/1000.0  , 500 ); 

							eyex = centerx =   View_Center_Back[i][2*j][4]  ;  // 
							eyey = centery =   View_Center_Back[i][2*j][5]  ;  //  
							gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, -1, 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Front[Count] );  
							glMatrixMode(GL_PROJECTION);    glPopMatrix() ; //
						}
					}

					
					Count = 2 * j + 1 + i * 2 * LENS_NUM_Y ; 
					{

						if(   (Multiple_Lens_Project_Matrix_Front[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL  ||   (Multiple_Lens_Project_Matrix_Back[Count] = (float*) calloc( 16, sizeof(float) ) ) ==NULL)
						{
							//printf("Not enough space!\n");
							exit(0);
						}
						else
						{
							centerz =  -100;   
							glMatrixMode(GL_PROJECTION);  glPushMatrix() ; //  
							glLoadIdentity();            //  
							//printf(" i = %d \n", i);
							//printf(" j = %d \n", j);
							//printf(" LENS_NUM_X = %d \n ", LENS_NUM_X );
							//printf(" LENS_NUM_Y = %d \n ", LENS_NUM_Y ); 
							glFrustum  ( View_Center_Back[i][2*j+1][0]/1000.0 , View_Center_Back[i][2*j+1][1]/1000.0 ,  View_Center_Back[i][2*j+1][2]/1000.0  ,  View_Center_Back[i][2*j+1][3]/1000.0  , GAP/1000.0  , 600 ); 

							eyex = centerx =   View_Center_Back[i][2*j+1][4]  ; // 
							eyey = centery =   View_Center_Back[i][2*j+1][5]  ; //  
							gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Back[Count] ); 

							glMatrixMode(GL_PROJECTION);  glPopMatrix() ; //  

							centerz =  100  ; // 
							glMatrixMode(GL_PROJECTION);    glPushMatrix() ; //  
							glLoadIdentity(); 
							glFrustum  (- View_Center_Back[i][2*j+1][0]/1000.0 , -View_Center_Back[i][2*j+1][1]/1000.0 ,   View_Center_Back[i][2*j+1][2]/1000.0  ,  View_Center_Back[i][2*j+1][3]/1000.0  , GAP/1000.0  , 600 ); 
							eyex = centerx =   View_Center_Back[i][2*j+1][4]  ; // 
							eyey = centery =   View_Center_Back[i][2*j+1][5]  ; //  
							gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, -1, 0);  
							glGetFloatv(GL_PROJECTION_MATRIX  ,  Multiple_Lens_Project_Matrix_Front[Count] );  
							glMatrixMode(GL_PROJECTION);  glPopMatrix() ; //  

							if (Count==121)
							{
								FILE *fp =  fopen("Projection_Matrix.txt","w");  
								fprintf(fp,"Multiple_Lens_Project_Matrix_Front = [... %% [%d]\n", Count );  
								for (int i=0;i<4;i++) {
									for (int j=0;j<4;j++)
									{
										fprintf(fp,"%f \t" , Multiple_Lens_Project_Matrix_Front[Count][i*4+j]); 
									}
									if (i==3)
									{
										fprintf(fp,"] ; \n");  
									}
									else
									{
										fprintf(fp,"\n");  
									}
									
								}

								fprintf(fp,"\n") ;
								fprintf(fp,"\n") ;

								fprintf(fp,"Multiple_Lens_Project_Matrix_Back= [... %% [%d]\n", Count );  
								for (int i=0;i<4;i++) {
									for (int j=0;j<4;j++)
									{
										fprintf(fp,"%f \t" , Multiple_Lens_Project_Matrix_Back [Count][i*4+j]); 
									}
									if (i==3)
									{
										fprintf(fp,"] ; \n");  
									}
									else
									{
										fprintf(fp,"\n");  
									}								
								}
								fclose(fp);
							}




						}
					}

				}
			}
		}
	}
}





//      //
void Multiple_View::Multiple_View_Center_Free()
{
	if (IF_Square > 0 )
	{
		for ( int i = 0 ;i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y ;j++)
			{
				free( View_Center_Back[i][j] );
				//free( View_Center_Front[i][j] );
			}
		}
		// 
		for ( int i = 0 ;i <this->LENS_NUM_X ;i++)
		{
			free( View_Center_Back[i]);
			//free( View_Center_Front[i]);
		}
		free(View_Center_Back) ;
		//free(View_Center_Front) ;
	}
	else
	{
		// 
		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			for ( int j = 0 ;j < this->LENS_NUM_Y ;j++)
			{   
				free( View_Center_Back[i][2*j+0] );
				free( View_Center_Back[i][2*j+1] );
				// 此处过去写错了 // 
				//free( View_Center_Front[i][2*j+0] );
				//free( View_Center_Front[i][2*j+1] );
			}
		}
		// 
		for ( int i = 0 ;i<this->LENS_NUM_X ; i++)
		{
			free( View_Center_Back[i]);
			// free( View_Center_Front[i]);
		}
		free( View_Center_Back) ;
		// free( View_Center_Front) ;
	}
}

//        0        //
//                //
void Multiple_View::Multiple_Viewport_Free()
{
	///               ///
	if (IF_Square > 0)
	{
		for ( int i = 0 ;i<this->LENS_NUM_X  ;i++)
		{
			for ( int j = 0 ;j<this->LENS_NUM_Y ;j++)
			{
				free( Multiple_Viewport[i][j] );
			}
		}
		// 
		for ( int i = 0 ;i <this->LENS_NUM_X ;i++)
		{
			free( Multiple_Viewport[i]);
		}
		free(Multiple_Viewport) ;
	}
	else
	{
		// 
		for ( int i = 0 ;i<this->LENS_NUM_X ;i++)
		{
			for ( int j = 0 ;j < this->LENS_NUM_Y ;j++)
			{   
				free( Multiple_Viewport[i][2*j+0] );
				free( Multiple_Viewport[i][2*j+1] );
			}
		}
		// 
		for ( int i = 0 ;i<this->LENS_NUM_X ; i++)
		{
			free( Multiple_Viewport[i]);
		}
		free(Multiple_Viewport) ;
	}
	// 
	// 
}

// 
void Multiple_View::Multiple_Lens_Project_Matrix_Free() 
{
	if (IF_Square > 0 )
	{
		for ( int i = 0 ;i<this->LENS_NUM_X*this->LENS_NUM_Y  ;i++)
		{
			free( Multiple_Lens_Project_Matrix_Back [i] );
			free( Multiple_Lens_Project_Matrix_Front[i] );
		}
		// 
		free(Multiple_Lens_Project_Matrix_Back) ;
		free(Multiple_Lens_Project_Matrix_Front) ;
	}
	else
	{
		for ( int i = 0 ;i<this->LENS_NUM_X*this->LENS_NUM_Y*2  ;i++)
		{
			free( Multiple_Lens_Project_Matrix_Back[i] );
			free( Multiple_Lens_Project_Matrix_Front[i] );
		}
		// 
		free(Multiple_Lens_Project_Matrix_Back) ;
		free(Multiple_Lens_Project_Matrix_Front) ;
	}
}
// Decomposition 
Multiple_View::~Multiple_View() {}
//
void Multiple_View::Cross_Matrix( float*Matrix ,float*Matrix1,float*Matrix2)
{
	Matrix[0] = 1 * ( Matrix1[1]*Matrix2[2]-Matrix1[2]*Matrix2[1] );
	Matrix[1] = 1 * ( Matrix1[0]*Matrix2[2]-Matrix1[2]*Matrix2[0] );
	Matrix[2] = 1 * ( Matrix1[1]*Matrix2[0]-Matrix1[0]*Matrix2[1] );
	double sum = sqrt(  Matrix[0] *Matrix[0]  +  Matrix[1] *Matrix[1] +  Matrix[2] *Matrix[2] );
	Matrix[1]  = Matrix[1] /sum;
	Matrix[0]  = Matrix[0] /sum;
	Matrix[2]  = Matrix[2] /sum;
}
