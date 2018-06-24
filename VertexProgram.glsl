// Automatically generated - do not edit - MXHLSL2GLSL v0.17

uniform vec4  VPMatrix[4];
uniform vec4  WMatrix[4];

uniform vec4  IV_Parameters;
uniform vec3  other_Parameters;

// uniform vec4 IV_Parameters ;

// uniform   means just c++ to glsl; and can't be modified in glsl
// varying   means from vertex to fragment 
// attribute means variables which can only be used in vertex shader  // ��������� // 

void main() // Original entrypoint: VertexShaderMain
{
	vec3 Nw;
	vec4 Pw, Ph;
	// ����λ��
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy; //   ����Ӧ  // 

	// gl_VertexΪģ������ϵ������  Vertex  ��������������ϵ�����Ϊֹ  //
	Pw = gl_Vertex.x * WMatrix[0];
	Pw += gl_Vertex.y * WMatrix[1];
	Pw += gl_Vertex.z * WMatrix[2];
	Pw += 1.0         * WMatrix[3];

	Pw.x = -3 * (IV_Parameters.x - Pw.x) / (other_Parameters.z * abs(Pw.z));
	Pw.y = -3 * (IV_Parameters.y - Pw.y) / (other_Parameters.z * abs(Pw.z));



	gl_ClipVertex = Pw; // gl_ClipVertexΪ�ü�����ϵ�ڲ���λ�á�
	// 

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
	gl_TexCoord[1].xyz = Nw.xyz;  // ģ��ת����ķ�����������������ϵ ��
	return;
}
// EOF