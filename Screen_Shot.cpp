#include "Screen_Shot.h"

SCREEN_SHOT::SCREEN_SHOT()
{
	If_SaveImage  = 0 ;
	filename_BMP  = (char*) malloc(30*sizeof(char)); 
	First_Flag    = 1  ; // 必须为int 类型 
	num_samples   = 8 ;
	Msaa_Flag     = false ; // 
}
SCREEN_SHOT::~SCREEN_SHOT(){} ;
// 

bool SCREEN_SHOT::screenshot( char* filename)
{
	GLenum lastBuffer;
	GLbyte* pBits = 0; // 图像数据 
	unsigned long lImageSize;
	GLint iViewport[4]; // 视图大小

	glGetIntegerv(GL_VIEWPORT, iViewport);
	lImageSize = iViewport[2] * iViewport[3] * 3 ;
	pBits =  (GLbyte*)new unsigned char[lImageSize];
	if (!pBits)
		return false;

	// 从color buffer中读取数据
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

	//
	glGetIntegerv(GL_READ_BUFFER, (GLint*)&lastBuffer); // 
	glReadBuffer( GL_FRONT ); // 
	glReadPixels(0, 0, iViewport[2], iViewport[3], GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
	glReadBuffer(lastBuffer);

	if (writeBMP_from_Data(filename, (unsigned char*)pBits, iViewport[2], iViewport[3]))
	{
		delete[] pBits  ;
		return true;
	}
	else
	{
		delete[] pBits  ;
		return false;
	}
}



//
void SCREEN_SHOT::InitFBO( int WIDTH , int HEIGHT ) 
{ 
	cout<<First_Flag<<endl; 
	if ( First_Flag!=1) // 如果不是第一次 
	{
		First_Flag = First_Flag + 1 ;  
		if ( Texture_width!=WIDTH && Texture_height!=WIDTH )
		{
			glDeleteBuffers(1, &window_fbo  ) ;
			glDeleteBuffers(1, &window_image) ;
			glDeleteBuffers(1, &window_oid  ) ; 
			// 
			glDeleteBuffers(1, &MSAA_fbo  ) ;
			glDeleteBuffers(1, &m_renderBufferColorMS  ) ;
			glDeleteBuffers(1, &m_renderBufferDepthMS  ) ; 
			// 
			cout<<"OK"<<endl; 
			cout<<"OK"<<endl; 
			// 
			glGenFramebuffers( 1, &MSAA_fbo );
			glBindFramebuffer( GL_FRAMEBUFFER, MSAA_fbo );	
			//  
			glGenRenderbuffers(1,&m_renderBufferColorMS);  
			glBindRenderbuffer(GL_RENDERBUFFER,m_renderBufferColorMS);  
			glRenderbufferStorageMultisample(GL_RENDERBUFFER , num_samples , GL_RGB ,WIDTH, HEIGHT);  
			glBindRenderbuffer(GL_RENDERBUFFER,0);  
			glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,  GL_RENDERBUFFER , m_renderBufferColorMS);   // 
			//
			glGenRenderbuffers(1,&m_renderBufferDepthMS);
			glBindRenderbuffer(GL_RENDERBUFFER ,m_renderBufferDepthMS); 
			glRenderbufferStorageMultisample (GL_RENDERBUFFER , num_samples , GL_DEPTH_COMPONENT24 ,WIDTH, HEIGHT  ); // GL_RENDERBUFFER ,GL_DEPTH_COMPONENT24 ,, width, height
			glBindRenderbuffer(GL_RENDERBUFFER , 0 );    // 
			glFramebufferRenderbuffer        (GL_FRAMEBUFFER ,GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , m_renderBufferDepthMS); 

			//   *********************  window buffer *************************** // 
			glGenFramebuffersEXT(1,&window_fbo);                  // 
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,window_fbo);  // 
			// 
			glGenTextures  (1, &window_image)    ;                     // 生成纹理对象 存储最终iv图像
			glBindTexture  (GL_TEXTURE_2D, window_image    );          // 绑定纹理对象为二维纹理
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // 对该纹理对象的缩小滤波器进行规定，如MIN
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  // 　GL_TEXTURE_2D: 操作2D纹理. GL_TEXTURE_WRAP_S: S方向上的贴图模式. GL_CLAMP: 将纹理坐标限制在0.0,1.0的范围之内.如果超出了会如何呢.不会错误,只是会边缘拉伸填充.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);  // 　GL_TEXTURE_2D: 操作2D纹理. GL_TEXTURE_WRAP_T: T方向上的贴图模式. GL_CLAMP: 将纹理坐标限制在0.0,1.0的范围之内.如果超出了会如何呢.不会错误,只是会边缘拉伸填充.

			glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); // 为他分配空间大小，是一个ＲＧＢＡ的空间
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D , window_image    ,0); // ban 
			// //分配zbuffer给FBO 使用  深度缓冲器  // 
			GLuint status  = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) ;
			// //分配zbuffer给FBO 使用  深度缓冲器  // 
			glGenRenderbuffersEXT(1,&window_oid); 
			glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,window_oid);   
			glRenderbufferStorageEXT    (GL_RENDERBUFFER_EXT ,GL_DEPTH_COMPONENT24,WIDTH,HEIGHT);
			glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT  ,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT , window_oid ); 

			switch( status ){
			case GL_FRAMEBUFFER_COMPLETE_EXT:
				//    MessageBox(NULL,"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_OK|MB_ICONEXCLAMATION);
				break;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
				MessageBox(NULL,L"GL_FRAMEBUFFER_UNSUPPORTED_EXT!",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
				exit(0);
				break;
			}
		}
		else
		{
			// 不进行改变
		}

	}
	else
	{   // 
		First_Flag = First_Flag + 1 ; 
		// msaa 
		cout<<"OK"<<endl;  
		glGenFramebuffers( 1, &MSAA_fbo );
		glBindFramebuffer( GL_FRAMEBUFFER, MSAA_fbo );	
		//  
		glGenRenderbuffers(1,&m_renderBufferColorMS);  
		glBindRenderbuffer(GL_RENDERBUFFER,m_renderBufferColorMS);  
		glRenderbufferStorageMultisample(GL_RENDERBUFFER , num_samples , GL_RGB ,WIDTH, HEIGHT);  
		glBindRenderbuffer(GL_RENDERBUFFER,0);  
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,  GL_RENDERBUFFER , m_renderBufferColorMS);   // 
		//
		glGenRenderbuffers(1,&m_renderBufferDepthMS);
		glBindRenderbuffer(GL_RENDERBUFFER ,m_renderBufferDepthMS); 
		glRenderbufferStorageMultisample (GL_RENDERBUFFER , num_samples , GL_DEPTH_COMPONENT24 ,WIDTH, HEIGHT  ); // GL_RENDERBUFFER ,GL_DEPTH_COMPONENT24 ,, width, height
		glBindRenderbuffer(GL_RENDERBUFFER , 0 );    // 
		glFramebufferRenderbuffer        (GL_FRAMEBUFFER ,GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , m_renderBufferDepthMS); 

		//   *********************  window buffer *************************** // 
		glGenFramebuffersEXT(1,&window_fbo);                  // 
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,window_fbo);  // 

		glGenTextures (1, &window_image)    ;                     // 生成纹理对象 存储最终iv图像
		glBindTexture (GL_TEXTURE_2D, window_image    );          // 绑定纹理对象为二维纹理

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);     // 对该纹理对象的缩小滤波器进行规定，

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  // 
		glTexImage2D   (GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0); // 为他分配空间大小，是一个ＲＧＢＡ的空间

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D , window_image    ,0); // ban 
		// //分配zbuffer给FBO 使用  深度缓冲器  // 
		GLuint status  = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) ;
		// //分配zbuffer给FBO 使用  深度缓冲器  // 
		glGenRenderbuffersEXT(1,&window_oid); 
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,window_oid);   
		glRenderbufferStorageEXT    (GL_RENDERBUFFER_EXT ,GL_DEPTH_COMPONENT24,WIDTH,HEIGHT);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT  ,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT , window_oid ); 

		switch( status ){
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			//    MessageBox(NULL,"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_OK|MB_ICONEXCLAMATION);
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			MessageBox(NULL,L"GL_FRAMEBUFFER_UNSUPPORTED_EXT!",L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			exit(0);
			break;
		}
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT ,  0    );  // windows 默认的分辨率 0 

	glBindTexture (GL_TEXTURE_2D,  0     );          // 绑定纹理对象为二维纹理
	
	Texture_width  = WIDTH  ;
	Texture_height = HEIGHT ;
} 




void SCREEN_SHOT::Process_Key()   
{
	cin.getline(filename_BMP ,30);
	If_SaveImage = 1 ; 
}

void SCREEN_SHOT::Disable_Save()   
{
	If_SaveImage = 0 ;
}

void SCREEN_SHOT::GET_Texture( GLuint texture_id , int Size_X , int Size_Y)  
{
	if (If_SaveImage==1)
	{
		if ( ( data = (GLbyte*) malloc( sizeof(unsigned char)*Size_X*Size_Y*3) )   )
		{
			glEnable(GL_TEXTURE_2D);  
			glBindTexture(GL_TEXTURE_2D , texture_id );  
			glGetTexImage(GL_TEXTURE_2D,0,GL_BGR,GL_UNSIGNED_BYTE,data);  
			glDisable(GL_TEXTURE_2D);  
			writeBMP(filename_BMP ,  Size_X  , Size_Y  );
			free ( data ) ; 
		}
		else
		{
			cout<<"Not enough space "<<endl ;
		}
	}
}
// 
bool SCREEN_SHOT::writeSingle_BMP_from_Data( const char filename[], unsigned char* data, unsigned int w, unsigned int h)
{
	std::ofstream out_file;

	/** 检查data */
	if(!data) 
	{
		std::cerr << "data corrupted! " << std::endl;
		out_file.close();
		return false;
	}

	/** 创建位图文件信息和位图文件头结构 */
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bitmapInfoHeader;

	//unsigned char textureColors = 0;/**< 用于将图像颜色从BGR变换到RGB */

	/** 打开文件,并检查错误 */
	out_file.open(filename, std::ios::out | std::ios::binary);
	if (!out_file)
	{
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}

	/** 填充BITMAPFILEHEADER */
	header.bfType = BITMAP_ID;
	header.bfSize = w*h*3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	/** 写入位图文件头信息 */
	out_file.write((char*)&header, sizeof(BITMAPFILEHEADER));

	/** 填充BITMAPINFOHEADER */
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth    = w;
	bitmapInfoHeader.biHeight   = h;
	bitmapInfoHeader.biPlanes   = 1;
	bitmapInfoHeader.biBitCount = 8;
	bitmapInfoHeader.biCompression = BI_RLE8; // BI_RLE4 BI_RLE8
	bitmapInfoHeader.biSizeImage = w * h  ; // 当压缩类型为BI_RGB是也可以设置为0
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	/** 写入位图文件信息 */
	out_file.write((char*)&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	/** 将指针移到数据开始位置 */
	out_file.seekp(header.bfOffBits, std::ios::beg);

	/** 写入图像数据 */
	out_file.write((char*)data, bitmapInfoHeader.biSizeImage);

	out_file.close();
	return true;
}
// 


// 
bool SCREEN_SHOT::writeBMP_from_Data(const char filename[], unsigned char* data, unsigned int w, unsigned int h)
{
	std::ofstream out_file;

	/** 检查data */
	if(!data) 
	{
		std::cerr << "data corrupted! " << std::endl;
		out_file.close();
		return false;
	}

	/** 创建位图文件信息和位图文件头结构 */
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bitmapInfoHeader;

	//unsigned char textureColors = 0;/**< 用于将图像颜色从BGR变换到RGB */

	/** 打开文件,并检查错误 */
	out_file.open(filename, std::ios::out | std::ios::binary);
	if (!out_file)
	{
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}

	/** 填充BITMAPFILEHEADER */
	header.bfType = BITMAP_ID;
	header.bfSize = w*h*3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	/** 写入位图文件头信息 */
	out_file.write((char*)&header, sizeof(BITMAPFILEHEADER));

	/** 填充BITMAPINFOHEADER */
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth    = w;
	bitmapInfoHeader.biHeight   = h;
	bitmapInfoHeader.biPlanes   = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB; // BI_RLE4 BI_RLE8
	bitmapInfoHeader.biSizeImage = w * h * 3; // 当压缩类型为BI_RGB是也可以设置为0
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	/** 写入位图文件信息 */
	out_file.write((char*)&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	/** 将指针移到数据开始位置 */
	out_file.seekp(header.bfOffBits, std::ios::beg);

	/** 写入图像数据 */
	out_file.write((char*)data, bitmapInfoHeader.biSizeImage);

	out_file.close();
	return true;
}
// 
//
bool SCREEN_SHOT::writeBMP(const char filename[] , unsigned int w, unsigned int h)
{
	std::ofstream out_file;
	/** 检查data */
	if(!data) 
	{
		std::cerr << "data corrupted! " << std::endl;
		out_file.close();
		return false;
	}

	/** 创建位图文件信息和位图文件头结构 */
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER bitmapInfoHeader;

	//unsigned char textureColors = 0;/**< 用于将图像颜色从BGR变换到RGB */

	/** 打开文件,并检查错误 */
	out_file.open(filename, std::ios::out | std::ios::binary);
	if (!out_file)
	{
		std::cerr << "Unable to open file " << filename << std::endl;
		return false;
	}

	/** 填充BITMAPFILEHEADER */
	header.bfType = BITMAP_ID;
	header.bfSize = w*h*3 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	/** 写入位图文件头信息 */
	out_file.write((char*)&header, sizeof(BITMAPFILEHEADER));

	/** 填充BITMAPINFOHEADER */
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = w;
	bitmapInfoHeader.biHeight = h;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB; // BI_RLE4 BI_RLE8
	bitmapInfoHeader.biSizeImage = w * h * 3; // 当压缩类型为BI_RGB是也可以设置为0
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	/** 写入位图文件信息 */
	out_file.write((char*)&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));

	/** 将指针移到数据开始位置 */
	out_file.seekp(header.bfOffBits, std::ios::beg);

	/** 写入图像数据 */
	out_file.write((char*)data, bitmapInfoHeader.biSizeImage);

	out_file.close();
	return true;
}
