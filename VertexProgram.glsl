// Automatically generated - do not edit - MXHLSL2GLSL v0.17

uniform vec4  VPMatrix[4];
uniform vec4  WMatrix[4];

uniform vec4  IV_Parameters;
uniform vec3  other_Parameters;

// uniform vec4 IV_Parameters ;

// uniform   means just c++ to glsl; and can't be modified in glsl
// varying   means from vertex to fragment 
// attribute means variables which can only be used in vertex shader  // 顶点的特性 // 

void main() // Original entrypoint: VertexShaderMain
{
	vec3 Nw;
	vec4 Pw, Ph;
	// 纹理位置
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy; //   不对应  // 

	// gl_Vertex为模型坐标系的坐标  Vertex  顶点在世界坐标系里面的为止  //
	Pw = gl_Vertex.x * WMatrix[0];
	Pw += gl_Vertex.y * WMatrix[1];
	Pw += gl_Vertex.z * WMatrix[2];
	Pw += 1.0         * WMatrix[3];

	Pw.x = -3 * (IV_Parameters.x - Pw.x) / (other_Parameters.z * abs(Pw.z));
	Pw.y = -3 * (IV_Parameters.y - Pw.y) / (other_Parameters.z * abs(Pw.z));



	gl_ClipVertex = Pw; // gl_ClipVertex为裁剪坐标系内部的位置。
	// 

	//   Pw为世界坐标系内的顶点   //
	Ph = Pw.x * VPMatrix[0];
	Ph += Pw.y * VPMatrix[1];
	Ph += Pw.z * VPMatrix[2];
	Ph += Pw.w * VPMatrix[3];

	gl_Position = Ph;  // gl_Position为NDC坐标系内部的位置 即归一化为(-1,1)*(-1,1)*(-1,1)

	//  Normal 法向量   //
	Nw = gl_MultiTexCoord1.x * WMatrix[0].xyz;
	Nw += gl_MultiTexCoord1.y * WMatrix[1].xyz;
	Nw += gl_MultiTexCoord1.z * WMatrix[2].xyz;
	gl_TexCoord[1].xyz = Nw.xyz;  // 模型转动后的法向量方向（世界坐标系 ）
	return;
}
// EOF