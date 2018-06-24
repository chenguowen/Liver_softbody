//   适应纹理合成 // 
#include "LoadObj.h"
#include "math.h"

#define min(x,y) x<y? x:y
#define max(x,y) x>y? x:y

float3 float3::operator +(float3 m)
{
	float3 result    ; //
	result.x = x+m.x ; //
	result.y = y+m.y ; //
	result.z = z+m.z ; //
	return result; // 
}

float3 float3::operator - (float3 m)
{
	float3  result   ;
	result.x = x-m.x ;
	result.y = y-m.y ;
	result.z = z-m.z ;
	return   result  ;
}

float3 float3::operator /(float m)
{
	if(m==0){
		printf("error /");
		return float3(x,y,z);
	}
	else
		return float3(x/m,y/m,z/m);
}

float3 float3::operator =(float3 f3)
{
	x = f3.x;
	y = f3.y;
	z = f3.z;
	return float3(x,y,z);
}


bool float3::operator == (const float3 &f3)  // Meffy : operator == 重载
{
	float xd,yd,zd;
	xd = abs(this-> x - f3.x);
	yd = abs(this-> y - f3.y);
	zd = abs(this-> z - f3.z);
	if((xd < 0.001 )&&(yd < 0.001)&&(zd < 0.001))
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 
void loadObj( const std::string filename, TriangleMesh &mesh )
{
	// 
 	mesh.obj_Octree = new Octree() ; /// 
	// Scene bounding box 
	float3 scene_aabbox_min;
	float3 scene_aabbox_max;

	int total_number_of_triangles = 0;

	std::ifstream in(filename.c_str());

	if(!in.good())
	{
		std::cout  << "ERROR: loading obj:(" << filename << ") file is not good" << "\n"<<std::endl ;
		exit(0);
	}
	char buffer[256], str[255];
	float f1,f2,f3;
	int Nothing[6] ; 
	// 
	// 0 号文理 ex_atrium  左上角 
	// 1 号文理 int_left   左下角 
	// 2 号文理 int_right  右下角 
	// float offset_x[] = { 0.0 , 0.0 , 0.5 , 0.5 } , offset_y[] = { 0.5 , 0.0 , 0.0 ,  0.5 }  ; // 

	// float offset_x[] = { 0.0 , 0.0 , 0.5 , 0.5 } , offset_y[] = { 0.5 , 0.0 , 0.0 ,  0.5 }  ;  //  
	// float offset_x[] = { 0.5 , 0.0 , 0.5 , 0.5 } , offset_y[] = { 0.0 , 0.0 , 0.0 ,  0.5 }  ;  //  
	float offset_x[] = { 0.0 , 0.0 , 0.5 , 0.5 } , offset_y[] = { 0.5  , 0.0 , 0.0 ,  0.5 }  ;  //  

	int   indicator = 3  ; 

	while(!in.getline(buffer,255).eof())
	{

		buffer[255]='\0';
		sscanf_s(buffer,"%s",str,255); 
		
		if( buffer[0]=='u'  && 
			buffer[1]=='s'  && 
			buffer[2]=='e'  && 
			buffer[3]=='m'  && 
			buffer[4]=='t'  && 
			buffer[5]=='l' )
		{
			 sscanf( buffer,"usemtl material_%d", &indicator ) ; //   
			 std::cout<<"indicator = "<< indicator <<endl ;
		}  // 
		// reading a vertex
		else if (buffer[0]=='v' && (buffer[1]==' '  || buffer[1]==32) )
		{
			if ( sscanf(buffer,"v %f %f %f",&f1,&f2,&f3)==3)
			{
				mesh.verts.push_back(float3(f1,f2,f3));
			}
			else
			{
				std::cout << "ERROR: vertex not in wanted format in OBJLoader" << "\n"<<std::endl ; 
				exit(-1);
			}
		}
		// reading FaceMtls 

		else if (buffer[0]=='f' && (buffer[1]==' ' || buffer[1]==32) )
		{
			TriangleFace f;
			// int nt = sscanf(buffer,"f %d/%d/%d %d/%d/%d %d/%d/%d",&f.v[0],&f.Define_index[0],&Nothing[1],&f.v[1],&f.Define_index[1],&Nothing[4],&f.v[2],&f.Define_index[2],&Nothing[5]);
			int nt = sscanf(buffer,"f %d/%d/%d %d/%d/%d %d/%d/%d",    &f.v[0]  ,  &f.Define_index[0]  ,  &f.Normal_index[0] , 
															          &f.v[1]  ,  &f.Define_index[1]  ,  &f.Normal_index[1] , 
															          &f.v[2]  ,  &f.Define_index[2]  ,  &f.Normal_index[2] ) ;  
			// cout<<"nt = "<< nt << endl ; // 
			if ( nt == 9 ) // 
			{
				nt = sscanf(buffer,"f %d/%d/%d %d/%d/%d %d/%d/%d",    &f.v[0]  ,  &f.Define_index[0]  ,  &f.Normal_index[0]  , 
															          &f.v[1]  ,  &f.Define_index[1]  ,  &f.Normal_index[1]  , 
															          &f.v[2]  ,  &f.Define_index[2]  ,  &f.Normal_index[2] ) ;  
			}
			else if( nt == 6 )  // 
			{
				nt = sscanf(buffer,"f %d//%d %d//%d %d//%d",    &f.v[0]  ,   &f.Normal_index[0] ,   //
															    &f.v[1]  ,   &f.Normal_index[1] ,   // 
															    &f.v[2]  ,   &f.Normal_index[2] ) ; // 
				f.Define_index[0] = f.Define_index[1] = f.Define_index[2] = 1 ; //    
				float3 tmp ; tmp.x =  tmp.y =  tmp.z = 0.1 ; //
				mesh.Texture_2D.push_back( tmp ) ; //   
			}
			else if( nt == 3 )  // 
			{
				nt = sscanf(buffer,"f %d %d %d",    &f.v[0] , &f.v[1] , &f.v[2] ) ;  //   
				f.Define_index[0] = f.Define_index[1] = f.Define_index[2] = 1 ; //    
				f.Normal_index[0] = f.Normal_index[1] = f.Normal_index[2] = 1 ; //     
				float3 tmp ; tmp.x =  tmp.y =  tmp.z = 0.5 ; //
				mesh.Texture_2D.push_back( tmp ) ; // 
				mesh.Normals.push_back(tmp);  // 
			}
			// system("pause") ; // 
			f.Define_index[6] = indicator ;  ////   
			mesh.faces.push_back(f) ; // 
		}
		else if(buffer[0]=='v' && (buffer[1]=='n'))   // vn
		{
			float3 tmp;
			int nt = sscanf(buffer,"vn %f %f %f",&tmp.x,&tmp.y,&tmp.z); // 
			mesh.Normals.push_back(tmp);
		}
		else if(buffer[0]=='v' && (buffer[1]=='t'))  //vt
		{
			float3 tmp;
			int nt = sscanf(buffer,"vt %f %f",&tmp.x,&tmp.y); //    
			tmp.x = tmp.x ; // * 0.5 + offset_x[indicator] ; // 
			tmp.y = tmp.y ; //  * 0.5 + offset_y[indicator] ; // 
			mesh.Texture_2D.push_back(tmp);
		}
	}

	float xmin,ymin,zmin,xmax,ymax,zmax;
	int Pxmin,Pxmax,Pymin,Pymax,Pzmin,Pzmax;

	//calculate the bounding sphere
	xmin = xmax = mesh.verts[0].x;
	ymin = ymax = mesh.verts[0].y;
	zmin = zmax = mesh.verts[0].z;
	Pxmin = Pxmax = Pymin = Pymax = Pzmin = Pzmax = 0;

	//calculate the bounding box
	mesh.bounding_box[0] = float3(mesh.verts[0].x,mesh.verts[0].y,mesh.verts[0].z);
	mesh.bounding_box[1] = float3(mesh.verts[0].x,mesh.verts[0].y,mesh.verts[0].z);

	for(unsigned int i = 0 ; i < mesh.verts.size(); i++)
	{
		//update min value
		mesh.bounding_box[0].x = min(mesh.verts[i].x,mesh.bounding_box[0].x);
		mesh.bounding_box[0].y = min(mesh.verts[i].y,mesh.bounding_box[0].y);
		mesh.bounding_box[0].z = min(mesh.verts[i].z,mesh.bounding_box[0].z);

		//update max value
		mesh.bounding_box[1].x = max(mesh.verts[i].x,mesh.bounding_box[1].x);
		mesh.bounding_box[1].y = max(mesh.verts[i].y,mesh.bounding_box[1].y);
		mesh.bounding_box[1].z = max(mesh.verts[i].z,mesh.bounding_box[1].z);

		//update the  x min and max
		if (mesh.verts[i].x < xmin){
			xmin = mesh.verts[i].x;
			Pxmin = i;
		}
		else if(mesh.verts[i].x > xmax){
			xmax = mesh.verts[i].x;
			Pxmax = i;
		}
		//update the y min and max
		if (mesh.verts[i].y < ymin){
			ymin = mesh.verts[i].y;
			Pymin = i;
		}
		else if(mesh.verts[i].y > ymax){
			ymax = mesh.verts[i].y;
			Pymax = i;
		}
		//update the z min and max
		if(mesh.verts[i].z < zmin){
			zmin = mesh.verts[i].z;
			Pzmin = i;
		}
		else if(mesh.verts[i].z > zmax){
			zmax = mesh.verts[i].z;
			Pzmax = i;
		}
	}

	// 
	float tmp = max ( xmax-xmin , ymax-ymin )  ; 
	tmp       = max ( tmp       , zmax-zmin )  ; 
	// 
	for(unsigned int i = 0; i < mesh.verts.size(); i++)
	{
		mesh.verts[i].x = ( mesh.verts[i].x  -(xmax+xmin)/2.0 ) /tmp        ;
		mesh.verts[i].y = ( mesh.verts[i].y  -(ymax+ymin)/2.0 ) /tmp        ;
		mesh.verts[i].z = ( mesh.verts[i].z  -(zmax+zmin)/2.0 ) /tmp        ;
	}
	// 
	mesh.bounding_box[1] =  ( mesh.bounding_box[1]   - float3( (xmax+xmin)/2.0 , (ymax+ymin)/2.0 ,  (zmax+zmin)/2.0  )  ) / tmp          ; 
	mesh.bounding_box[0] =  ( mesh.bounding_box[0]   - float3( (xmax+xmin)/2.0 , (ymax+ymin)/2.0 ,  (zmax+zmin)/2.0  )  ) / tmp          ; 
	if( _DEBUG_FLAG_ ) 
	{
		TriangleMesh *octree_tmp = &mesh ; // 
		std::cout<<" octree_tmp->bound_box_xyz =  "<<octree_tmp->bounding_box[0].x<<"   "<<octree_tmp->bounding_box [0].y<<"   "<<octree_tmp->bounding_box [0].z<<std::endl ; 
		std::cout<<" octree_tmp->bound_box_xyz =  "<<octree_tmp->bounding_box[1].x<<"   "<<octree_tmp->bounding_box [1].y<<"   "<<octree_tmp->bounding_box [1].z<<std::endl ; 
		system( "pause" ) ; // 
	}

	//calculate the bounding sphere
	float3 dVx = mesh.verts[Pxmax] - mesh.verts[Pxmin];
	float3 dVy = mesh.verts[Pymax] - mesh.verts[Pymin];
	float3 dVz = mesh.verts[Pzmax] - mesh.verts[Pzmin];
	float dx2 = norm2(dVx);
	float dy2 = norm2(dVy);
	float dz2 = norm2(dVz);

	float3 center;
	float  radius2;
	float  radius;

	if (dx2 >= dy2 && dx2>=dz2) {					// x direction is largest extent  
		center = mesh.verts[Pxmin] + (dVx / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(mesh.verts[Pxmax] - center);// radius squared  
	}  
	else if (dy2 >= dx2  && dy2>=dz2){				// y direction is largest extent  
		center = mesh.verts[Pymin] + (dVy / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(mesh.verts[Pymax] - center);// radius squared  
	} 
	else{
		center = mesh.verts[Pzmin] + (dVz / 2.0);	// Center = midpoint of extremes  
		radius2 = norm2(mesh.verts[Pzmax] - center);// radius squared  	
	}

	radius = sqrt(radius2);  

	// now check that all points V[i] are in the ball  
	// and if not, expand the ball just enough to include them  
	float3 dV;
	float dist2,dist;
	for (unsigned int i=0;i<mesh.verts.size();i++)
	{
		dV = mesh.verts[i] - center;
		dist2 = norm2(dV);
		if (dist2 <= radius2) // V[i] is inside the ball already  
			continue;

		// V[i] not in ball, so expand ball to include it  
		dist = sqrt(dist2);  
		radius = (radius + dist) / 2.0;         // enlarge radius just enough  
		radius2 = radius * radius;  
		center = center + ((dist-radius)/dist) * dV;   // shift Center toward V[i]  

	}

	mesh.bounding_sphere_c = center;
	mesh.bounding_sphere_r = radius;

	std::cout<< "----------obj file loaded-------------"<<std::endl;
	std::cout<<"number of faces:" << mesh.faces.size() << " number of vertices:" << mesh.verts.size() << std::endl;
	std::cout<< "obj bounding box: min:(" 
		<< mesh.bounding_box[0].x << "," << mesh.bounding_box[0].y << "," << mesh.bounding_box[0].z <<") max:(" 
		<< mesh.bounding_box[1].x << "," << mesh.bounding_box[1].y << "," << mesh.bounding_box[1].z <<")" << std::endl
		<<"obj bounding sphere center:"<<mesh.bounding_sphere_c.x<<","<<mesh.bounding_sphere_c.y<<","<<mesh.bounding_sphere_c.z<<std::endl
		<<"obj bounding sphere radius:"<<mesh.bounding_sphere_r<<std::endl;

}
