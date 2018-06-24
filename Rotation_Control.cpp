#include "Rotation_Control.h"
// 


Obj_control::Obj_control()
{
	spin_x               = 0.0 ;
	spin_y               = 0.0 ;
	Move_Z               = 0.0 ;
	Latest_State         = 1.0 ;
	Rotation_index       = 0.0 ;
	//  // 
	Now_Matrix_Mouse           = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 只管旋转状态， 不管平移状态 // 
	Now_Matrix_Mouse[12]       = Now_Matrix_Mouse[13]   = Now_Matrix_Mouse[14]    =   0  ;  	Now_Matrix_Mouse[15]       = 1 ;

	Now_Matrix_Leap           = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 只管旋转状态， 不管平移状态 // 
	Now_Matrix_Leap[12]       = Now_Matrix_Leap[13]   = Now_Matrix_Leap[14]    =   0  ;  	Now_Matrix_Leap[15]       = 1 ;
	//
	Transform_Matrix_Leap    = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0 
	Transform_Matrix_Mouse   = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0 
	Indentity                = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0 

	Backup_Translation    = ( GLfloat * ) calloc(  3 , sizeof ( GLfloat ) )  ;  // 初始化全是0 
	Backup_Translation[2] = 100  ; // 

	Init_Grasp_Inv_Mat    = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0  
	// 
	Transform_Matrix_Leap [0]   =  Transform_Matrix_Leap [1+4]  = Transform_Matrix_Leap [2+4*2 ] = Transform_Matrix_Leap  [3+4*3 ] =  1 ; // // 对角置 1    
	Transform_Matrix_Mouse[0]   =  Transform_Matrix_Mouse[1+4]  = Transform_Matrix_Mouse[2+4*2 ] = Transform_Matrix_Mouse [3+4*3 ] =  1 ; // // 对角置 1    
	Indentity[0] = Indentity [1+4]  = Indentity[2+4*2 ] = Indentity [3+4*3 ] =  1 ; // // 对角置 1    
	// Mag_Rotation_Mat  =  ; // 

	Backup_Rotation_Matrix    = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0
	Backup_Rotation_Matrix[0] =  Backup_Rotation_Matrix[1+4] = Backup_Rotation_Matrix[2+4*2 ]= Backup_Rotation_Matrix[3+4*3 ]=  1 ; // // 对角置1
	x  = y =  z  =  0  ;  

	Full_Screen_Flag  = false ; // 

	listener = new SampleListener();

	std::cout << "Press Enter to quit..." << std::endl;
}

// 
Obj_control::Obj_control(GLfloat a , GLfloat b, GLfloat c )
{
	x  = a ;  //
	y  = b ;  //
	z  = c ;  //
	spin_x               = 0.0 ; //
	spin_y               = 0.0 ; //
	Move_Z               = 0.0 ; //
	Latest_State         = 1.0 ; //
	Rotation_index       = 0.0 ; // 
	// 
	Backup_Translation    = ( GLfloat * ) calloc(  3 , sizeof ( GLfloat ) )  ;  // 初始化全是0 
	Backup_Translation[2] =  100     ;   //   
	// 
	Now_Matrix_Mouse           = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 只管旋转状态， 不管平移状态 // 
	Now_Matrix_Mouse[12]       = Now_Matrix_Mouse[13]   = Now_Matrix_Mouse[14]    =   0  ;  	Now_Matrix_Mouse[15]       = 1 ;

	Now_Matrix_Leap           = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 只管旋转状态， 不管平移状态 // 
	Now_Matrix_Leap[12]       = Now_Matrix_Leap[13]   = Now_Matrix_Leap[14]    =   0  ;  	Now_Matrix_Leap[15]       = 1 ;
	// 
	// Leap motion 和 手动操作完全分离 // 
	Transform_Matrix_Mouse     = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0
	Transform_Matrix_Leap      = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0
	// 
	Transform_Matrix_Leap [0]   =  Transform_Matrix_Leap [1+4]  = Transform_Matrix_Leap [2+4*2 ] = Transform_Matrix_Leap  [3+4*3 ] =  1 ; // // 对角置 1    
	Transform_Matrix_Mouse[0]   =  Transform_Matrix_Mouse[1+4]  = Transform_Matrix_Mouse[2+4*2 ] = Transform_Matrix_Mouse [3+4*3 ] =  1 ; // // 对角置 1    
	// Mag_Rotation_Mat  =  ; // 


	Backup_Rotation_Matrix    = ( GLfloat * ) calloc( 16 , sizeof ( GLfloat ) )  ;  // 初始化全是0
	
	Backup_Rotation_Matrix[0] =  Backup_Rotation_Matrix[1+4] = Backup_Rotation_Matrix[2+4*2 ]= Backup_Rotation_Matrix[3+4*3 ]=  1 ; // // 对角置1

	Full_Screen_Flag          = false; 

	// cgw comment this //  listener = new SampleListener();

	std::cout << "Press Enter to quit..." << std::endl;
	
}



void Obj_control::Process_Leap_Motion (  void ) 
{
	if( listener->One_hand_frame_counts >= 1 ) 
	{
		Now_Matrix_Leap[8]  = - listener->direction.x  ;
		Now_Matrix_Leap[9]  = - listener->direction.y  ;
		Now_Matrix_Leap[10] = - listener->direction.z  ;
		Now_Matrix_Leap[3]  =  0  ;
		Now_Matrix_Leap[4]  =   listener->normal.x ; 
		Now_Matrix_Leap[5]  =   listener->normal.y ; 
		Now_Matrix_Leap[6]  =   listener->normal.z ; 
		Cross_Matrix ( &Now_Matrix_Leap[0] ,  &Now_Matrix_Leap[4], &Now_Matrix_Leap[8]  ) ;  
		Now_Matrix_Leap[7]  =  0  ;
		Now_Matrix_Leap[11] =  0  ;
		Now_Matrix_Leap[12] =  0  ;
		Now_Matrix_Leap[13] =  0  ;
		Now_Matrix_Leap[14] =  0  ;
		Now_Matrix_Leap[15] =  1  ; 
		// 
		if(  listener->if_Begin_Grasp_Flag ) 
		{
			Matrix_Equal_Inv_4by4 ( Init_Grasp_Inv_Mat , Now_Matrix_Leap  ) ;  
			listener->Ori_dist_between_hand  =  listener->Now_dist_between_hand    ;
			listener->Ori_radius_of_hand     =  listener->radius_hand_right ;  
			listener->Ori_postion            =  listener->Right_position    ; 
			
			listener->if_Begin_Grasp_Flag  = 0 ; 
		}
		float Mag =  1 ;
		// 
		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity( );  //  // 
		glMultMatrixf( Now_Matrix_Leap    )      ; //  cgw  comment //  
		glMultMatrixf( Init_Grasp_Inv_Mat    )   ; //  要除掉初始抓取矩阵的影响 //  
		glMultMatrixf( Backup_Rotation_Matrix  ) ; //  cgw  comment // 
		//  //
		glGetFloatv  ( GL_MODELVIEW_MATRIX  ,  this->Transform_Matrix_Leap  ) ;  // 
		//  //  
		this->Transform_Matrix_Leap[12] =  Backup_Translation[0] +  ( listener->position.x - listener->Ori_postion.x )   ;  // 屏幕左右方向
		this->Transform_Matrix_Leap[13] =  Backup_Translation[1] +  ( listener->position.y - listener->Ori_postion.y )  - 100  ;  // 屏幕上下方向 给出的值只能是正的
		this->Transform_Matrix_Leap[14] =  Backup_Translation[2] +  ( listener->position.z - listener->Ori_postion.z )      ;  // 屏幕前后方向  
		// 
		// cout<<this->Transform_Matrix_Leap[12] << " " <<this->Transform_Matrix_Leap[13] << " " <<this->Transform_Matrix_Leap[14]  << endl ; 
		// cout<<this->Transform_Matrix_Leap[12] << " " <<this->Transform_Matrix_Leap[13] << " " <<this->Transform_Matrix_Leap[14]  << endl ; 
	} 
	// 
	if( listener->if_Loss_Flag > 0.5  )
	{
		Matrix_Equal_3by3 ( Backup_Rotation_Matrix  , this->Transform_Matrix_Leap  )   ; //  
		/*Backup_Translation[0]  = this->Transform_Matrix_Leap[12] ;
		Backup_Translation[1]  = this->Transform_Matrix_Leap[13] ;
		Backup_Translation[2]  = this->Transform_Matrix_Leap[14] ;*/
		Backup_Translation[0]  = 0   ; //
		Backup_Translation[1]  = 0   ; //
		Backup_Translation[2]  = 100 ; //
		listener->if_Loss_Flag = 0   ; //
	} //                               //  
	Transform_Matrix_Mouse[14] = 100 ; //  



}
// 
void Obj_control::Process_Leap_Motion_left  (  void ) 
{

}
void Obj_control::Process_Leap_Motion_right (  void ) 
{

}

// 
void Obj_control::Matrix_Equal_3by3( GLfloat*Matrix1 , GLfloat*Matrix2  )
{
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<3;j++)
		{
			Matrix1[i*4+j] = Matrix2[i*4+j] ;
		}
	}
}

void Obj_control::Matrix_Equal_4by4 ( GLfloat*Matrix1 , GLfloat*Matrix2  )
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			Matrix1[i*4+j] = Matrix2[i*4+j] ;
		}
	}
}
// 
void Obj_control::Matrix_Equal_Inv_4by4 ( GLfloat*Matrix1 , GLfloat*Matrix2  )
{
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			Matrix1[i*4+j] = Matrix2[ i+j*4 ] ;
		}
	}
}
void Obj_control::Matrix_Vector_Reset() // 保存图片时候会用到的命令 
{
	memcpy ( Transform_Matrix_Mouse ,Indentity , 16 * sizeof(GLfloat ) ) ; 
	Transform_Matrix_Mouse[14] = 100 ; // 
	memcpy ( Transform_Matrix_Leap ,Indentity , 16 * sizeof(GLfloat ) ) ; 
	Transform_Matrix_Leap [14] = 100 ; //  
	memcpy ( Backup_Rotation_Matrix ,Indentity , 16 * sizeof(GLfloat ) ) ; 
 
	Backup_Translation[0] = 000 ;
	Backup_Translation[1] = 000 ;
	Backup_Translation[2] = 100 ;
}

void Obj_control::Refresh_4f_Transform_Matrix (GLfloat* Transform_Matrix, GLfloat* RotationMatrix) 
{
	GLfloat Result[9];
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<3;j++)
		{
			Result[i*3+j] = Transform_Matrix[i*4+j];
		}
	}
	for (int i=0;i<3;i++)
	{
		for (int j=0;j<3;j++)
		{
			Transform_Matrix[i*4+j] = 0 ;
			for (int k=0;k<3;k++){
				Transform_Matrix[i*4+j] =  Transform_Matrix[i*4+j] + Result[i*3+k] * RotationMatrix[k*3+j];
			}
		}
	}
}

// 
void  Obj_control::Cross_Matrix( GLfloat*Matrix ,GLfloat*Matrix1,GLfloat*Matrix2)
{
	double sum = sqrt(  Matrix1[0] *Matrix1[0]  +  Matrix1[1] *Matrix1[1] +  Matrix1[2] *Matrix1[2] ); 
	Matrix1[0]  = Matrix1[0] /sum;
	Matrix1[1]  = Matrix1[1] /sum;
	Matrix1[2]  = Matrix1[2] /sum;
	
	sum = sqrt(  Matrix2[0] *Matrix2[0]  +  Matrix2[1] *Matrix2[1] +  Matrix2[2] *Matrix2[2] ); 
	Matrix2[0]  = Matrix2[0] /sum;
	Matrix2[1]  = Matrix2[1] /sum;
	Matrix2[2]  = Matrix2[2] /sum;

	Matrix[0] =  1 * ( Matrix1[1]*Matrix2[2]-Matrix1[2]*Matrix2[1] );
	Matrix[1] = -1 * ( Matrix1[0]*Matrix2[2]-Matrix1[2]*Matrix2[0] );
	Matrix[2] =  1 * ( Matrix1[0]*Matrix2[1]-Matrix1[1]*Matrix2[0] );
}

void  Obj_control::Matrix_Vector_mul(  GLfloat*Vector2 ,GLfloat*Matrix1,GLfloat*Vector1 )
{
	GLfloat Tmp[3] ; 
	Tmp[0]  = Matrix1[0+4*0] * Vector1[0]  + Matrix1[1+4*0] * Vector1[1]  + Matrix1[2+4*0] * Vector1[2]  ;
	Tmp[1]  = Matrix1[0+4*1] * Vector1[0]  + Matrix1[1+4*1] * Vector1[1]  + Matrix1[2+4*1] * Vector1[2]  ;
	Tmp[2]  = Matrix1[0+4*2] * Vector1[0]  + Matrix1[1+4*2] * Vector1[1]  + Matrix1[2+4*2] * Vector1[2]  ;

	Vector2[0] = Tmp[0] ;
	Vector2[1] = Tmp[1] ;
	Vector2[2] = Tmp[2] ;
}

void  Obj_control::Roderigues(GLfloat* RotationMatrix,GLfloat *NowRotation_Matrix , double* rotxyz)
{
	double TMP[3] ;

	TMP[0] = rotxyz[0];
	TMP[1] = rotxyz[1];
	TMP[2] = rotxyz[2];
	if(TMP[0]*TMP[0] + TMP[1]*TMP[1] + TMP[2]*TMP[2] > 0 ) 
	{
		GLfloat theta= sqrt( rotxyz[0]*rotxyz[0] + rotxyz[1]*rotxyz[1] + rotxyz[2]*rotxyz[2] );
		rotxyz[0] = rotxyz[0] / theta ;
		rotxyz[1] = rotxyz[1] / theta ;
		rotxyz[2] = rotxyz[2] / theta ;
		// Matlab Version  : rotation=cos(theta)*eye(3)+(1-cos(theta))*r*r'+[0 -r(3) r(2); r(3) 0  -r(1); -r(2) r(1) 0]*sin(theta);
		RotationMatrix[0] = cos(theta)+ (1-cos(theta))*rotxyz[0]*rotxyz[0] + 0        *sin(theta);
		RotationMatrix[1] = 0         + (1-cos(theta))*rotxyz[0]*rotxyz[1] - rotxyz[2]*sin(theta);
		RotationMatrix[2] = 0         + (1-cos(theta))*rotxyz[0]*rotxyz[2] + rotxyz[1]*sin(theta);

		RotationMatrix[3] = 0         + (1-cos(theta))*rotxyz[1]*rotxyz[0] + rotxyz[2]*sin(theta);
		RotationMatrix[4] = cos(theta)+ (1-cos(theta))*rotxyz[1]*rotxyz[1] + 0        *sin(theta);
		RotationMatrix[5] = 0         + (1-cos(theta))*rotxyz[1]*rotxyz[2] - rotxyz[0]*sin(theta);

		RotationMatrix[6] = 0         + (1-cos(theta))*rotxyz[2]*rotxyz[0] - rotxyz[1]*sin(theta);
		RotationMatrix[7] = 0         + (1-cos(theta))*rotxyz[2]*rotxyz[1] + rotxyz[0]*sin(theta);
		RotationMatrix[8] = cos(theta)+ (1-cos(theta))*rotxyz[2]*rotxyz[2] + 0        *sin(theta);
	}
	else 
	{
		RotationMatrix[0] = 1.0;
		RotationMatrix[1] = 0;
		RotationMatrix[2] = 0;

		RotationMatrix[3] = 0;
		RotationMatrix[4] = 1.0;
		RotationMatrix[5] = 0;

		RotationMatrix[6] = 0;
		RotationMatrix[7] = 0;
		RotationMatrix[8] = 1.0;
	}
}


void  Obj_control::spinDisplay(void)
{
	spin_x    =  -( Now_xy[0]- Last_xy[0] )/400.0f*4; 
	spin_y    =  -( Now_xy[1]- Last_xy[1] )/400.0f*4;
	rotxyz[0] =   spin_y; 
	rotxyz[1] =   spin_x; 
	rotxyz[2] =   0     ; 

	Last_xy[0] = Now_xy[0];   //cout<<"spin_x =　"<<spin_x << "   spin_y ="<<spin_y<<endl;
	Last_xy[1] = Now_xy[1];   //cout<<"spin_x =　"<<spin_x << "   spin_y ="<<spin_y<<endl;
	Roderigues ( Now_Matrix_Mouse , Transform_Matrix_Mouse , rotxyz);          // Transform_Matrix包含前一个时刻的旋转矩阵，Now_Matrix是增长部分
	Refresh_4f_Transform_Matrix  ( Transform_Matrix_Mouse  , Now_Matrix_Mouse );  // glutPostRedisplay(); // 5.31 chenguowen 

}

//      //
void Obj_control::mouse(int botton,int state,int x,int y)
{
	switch(botton)
	{
	case GLUT_LEFT_BUTTON:   //当单击鼠标左键时开始旋转
		if(state==GLUT_DOWN )
		{ 
			Latest_State =1; 
			Last_xy[0] = x;
			Last_xy[1] = y;
		}
		if(state==GLUT_UP && Latest_State==1 )
		{
			Now_xy[0]= x; 
			Now_xy[1]= y;
			Latest_State = 0;
			spinDisplay();
		}
		break;

	default:
		break;

	}
}

void Obj_control::Mouse_motion(int x,int y )
{
	if(Latest_State==1 )
	{ 
		Now_xy[0]= x; 
		Now_xy[1]= y;
		spinDisplay();
	}
}

//  
void Obj_control::Idle()  
{  
	glutPostRedisplay();
}  

//  
void Obj_control::keyboard(unsigned char key, int x, int y)  
{  
	switch (key) {  
	case 'w':  
		Move_Z = Move_Z+0.3;
		glutPostRedisplay(); 
		break;  
	case 's':  
		Move_Z = Move_Z-0.3;
		glutPostRedisplay(); 
		break;  
	case'd':
		Rotation_index = Rotation_index - 3;
		glutPostRedisplay(); 
		break;  
	case'a':
		Rotation_index = Rotation_index + 3;
		glutPostRedisplay(); 
		break;  
	case'l': // 如果按l就输出leap motion拍摄的人手的位置
		cout<< "[x,y,z]=["<<listener->position.x<<","<<listener->position.y<<","<<listener->position.z<<"]"<<endl ; 
		system("pause") ; 
		break;
	}  
}  
Obj_control::~Obj_control() { } 