#include "txtread.h"

using namespace std;


txt_Draw::txt_Draw() 
{
	text_display = (char *) malloc(100*sizeof(char) )   ; // 
	ROW = 0.8 - 1  ;
	VOL = 0.8  ;

	incline=TRUE  ; 
	underline=TRUE;
	Fontsize=-28  ;
	fcolorr=1.0 ; 
	fcolorg=1.0 ;
	fcolorb=1.0 ;

	Weight=400;
	myfont = "Candara"  ;
}
txt_Draw::~txt_Draw() {}
 
// 
void txt_Draw::drawtext()
{
	//字体绘制
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);

	glColor3f( 1 , 1 , 1 );
	glRasterPos2f ( ROW , VOL ) ; 

	glPrint( text_display  )   ; 

	glPopAttrib(); 
	glPopMatrix();
}
//显示字体函数
GLvoid txt_Draw::glPrint( char *fmt,...)
{
	char text[256];
	va_list ap;
	if (fmt==NULL)
		return;
	char *e ;
	for (e = fmt; *e != '\0'; e++)
	{

		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , *e);
	}
}
//
void txt_Draw::Set_text(char * str , double x ,double y )
{
	// free(text_display) ; // 
	text_display = str ; 
	VOL          = y ;
	ROW          = x ;
	//memcpy(text_display,str,sizeof(str)/sizeof(char) );
	// memset(  )
}

// 
