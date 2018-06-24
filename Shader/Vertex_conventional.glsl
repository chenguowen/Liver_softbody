// Automatically generated - do not edit - MXHLSL2GLSL v0.17
// #version 100  ������ ���Զ��Ż�  �ҾͲ����� 
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
	// ����λ��
	vertex.textureCoord.xy = gl_MultiTexCoord0.xy; //   ����Ӧ  // 

	// gl_VertexΪģ������ϵ������  Vertex  ��������������ϵ�����Ϊֹ  //
	Pw = gl_Vertex.x * WMatrix[0];
	Pw += gl_Vertex.y * WMatrix[1];
	Pw += gl_Vertex.z * WMatrix[2];
	Pw += 1.0         * WMatrix[3];

	gl_ClipVertex = Pw; // gl_ClipVertexΪ�ü�����ϵ�ڲ���λ�á�
	//                            //
	//   PwΪ��������ϵ�ڵĶ���   //
	Ph = Pw.x * VPMatrix[0];
	Ph += Pw.y * VPMatrix[1];
	Ph += Pw.z * VPMatrix[2];
	Ph += Pw.w * VPMatrix[3];

	gl_Position = Ph;  // gl_PositionΪNDC����ϵ�ڲ���λ�� ����һ��Ϊ(-1,1)*(-1,1)*(-1,1)

	//  Normal ������   //
	Nw = gl_MultiTexCoord1.x * WMatrix[0].xyz;
	Nw += gl_MultiTexCoord1.y * WMatrix[1].xyz;
	Nw += gl_MultiTexCoord1.z * WMatrix[2].xyz;

	vertex.normal.xyz = normalize(Nw.xyz);  // ģ��ת����ķ�����������������ϵ ��
	vertex.position_ = gl_ClipVertex;  // �����λ��
	return;
}
// EOF