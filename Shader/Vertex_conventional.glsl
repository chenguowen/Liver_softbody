// Automatically generated - do not edit - MXHLSL2GLSL v0.17
// #version 100  编译器 会自动优化  我就不管了 
// #version 330
uniform vec4  VPMatrix[4];
uniform vec4  WMatrix[4];// world model 

uniform vec4  IV_Parameters;
uniform vec3  other_Parameters;


out Vertex
{
	vec3 normal;
	vec4 position_;
	vec2 textureCoord; // in normalized units
} vertex;


void main() // Original entrypoint: VertexShaderMain
{
	vec3 Nw;
	vec4 Pw, Ph;
	// 纹理位置
	vertex.textureCoord.xy = gl_MultiTexCoord0.xy; //   不对应  // 

	// gl_Vertex为模型坐标系的坐标  Vertex  顶点在世界坐标系里面的为止  //
	Pw = gl_Vertex.x * WMatrix[0];
	Pw += gl_Vertex.y * WMatrix[1];
	Pw += gl_Vertex.z * WMatrix[2];
	Pw += 1.0         * WMatrix[3];

	gl_ClipVertex = Pw; // gl_ClipVertex为裁剪坐标系内部的位置。
	//                            //
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

	vertex.normal.xyz = normalize(Nw.xyz);  // 模型转动后的法向量方向（世界坐标系 ）
	vertex.position_ = gl_ClipVertex;  // 物体的位置
	return;
}
// EOF