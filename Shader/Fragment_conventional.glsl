// Automatically generated - do not edit - MXHLSL2GLSL v0.17
#version 330
#extension GL_ARB_texture_query_lod : enable 

uniform sampler2D texture1;  // flat
in Vertex
{
	vec3 normal       ; // 
	vec4 position_    ; // 
	vec2 textureCoord ; // in normalized units 
} vertex  ;  // varing  //

float mip_map_level(in vec2 texture_coordinate)
{
	vec2  dx_vtc = dFdx(texture_coordinate);
	vec2  dy_vtc = dFdy(texture_coordinate);
	float delta_max_sqr = max(dot(dx_vtc, dx_vtc), dot(dy_vtc, dy_vtc));
	return  0.5 * log2(delta_max_sqr);
}
//                            //  
out vec4 fragColor;

void main() // Original entrypoint: PixelShaderMain
{
	float ambient   = 0.3  ;  // 在这里不能做计算 如 1 - diffuse //
	float diffusive = 0.50 ;  // 在这里不能做计算 如 1 - diffuse //
	float specular  = 1.0  ;  // 在这里不能做计算 如 1 - diffuse //
	//       //
	vec3 specular_pos2 = vec3(-400, -400,  1900 ) ; // gl_TexCoord[2].xyz   
	vec3 specular_pos  = vec3( 400, -1000, 1900 ) ; // gl_TexCoord[2].xyz     
	vec3 Viewer_pos    = vec3( -0,   0 ,  15000 ) ; // gl_TexCoord[2].xyz  
	//       //
	vec3    UTU[3];
    vec3 normal_cgw = normalize( vertex.normal ) ; //   记得多加一些这种 ; 
	UTU[0] = vec3 (normal_cgw.x*normal_cgw.x, normal_cgw.x*normal_cgw.y, normal_cgw.x*normal_cgw.z) ;  // 
	UTU[1] = vec3 (normal_cgw.y*normal_cgw.x, normal_cgw.y*normal_cgw.y, normal_cgw.y*normal_cgw.z) ;  // 
	UTU[2] = vec3 (normal_cgw.z*normal_cgw.x, normal_cgw.z*normal_cgw.y, normal_cgw.z*normal_cgw.z) ;  //  
	//  	//  入射光线  //       // 
	vec3  Specular_direction = vertex.position_.xyz - specular_pos ;  // 
	      Specular_direction = normalize(Specular_direction); //  
	//  	//  出射光线  //       // 
	vec3  Light_out = vec3(Specular_direction.x - 2 * dot(UTU[0], Specular_direction),
		                   Specular_direction.y - 2 * dot(UTU[1], Specular_direction),
	                       Specular_direction.z - 2 * dot(UTU[2], Specular_direction) ); 
	//  	// 观察光线   //       // 
	Viewer_pos = normalize (  Viewer_pos - vertex.position_.xyz ) ;  //   
	Light_out  = normalize (  Light_out  )  ; // 
	// 
	float myspecular = dot ( Viewer_pos, Light_out ) ; // 
	if (  myspecular < 0 ) { myspecular = 0 ;      }  // 
	// else{ myspecular =  exp ( - (acos(myspecular)/0.2)*(acos(myspecular)/0.2) ) ; } //  Gaussian distribution
	// else{ myspecular =  exp ( (pow(myspecular,2)-1)/pow(myspecular,2)/0.04 ) /0.04/pow(myspecular,4)  ; } //  Beckmann distribution  Heidrich–Seidel anisotropic distribution
	else{myspecular =  pow ( myspecular ,20 ) ;    } // Phong distribution or  Blinn–Phong shading model

	float mipmapLevel  = 4  ; // default in case extension is unavailable...
	mipmapLevel        = textureQueryLOD(texture1, vertex.textureCoord).x; // NOTE CAPITALIZATION 
 	float my_diffusive = diffusive * abs( dot(normal_cgw.xyz, vec3(0.0, 0.0, 1.0))) + ambient;
	vec4 my_color      = textureLod(texture1, vertex.textureCoord, mipmapLevel) ; // 
	fragColor.xyzw     = vec4( my_diffusive )*my_color  + myspecular * vec4(1, 1, 1, 1);  // 
}