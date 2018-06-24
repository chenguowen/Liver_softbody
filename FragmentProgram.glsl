// Automatically generated - do not edit - MXHLSL2GLSL v0.17

uniform sampler2D texture1;

void main() // Original entrypoint: PixelShaderMain
{
	gl_FragColor.w = 1.0;
	float Tmp00 = (dot(gl_TexCoord[1].xyz, vec3(0.0, 0.0, 1.0)));  	Tmp00 = Tmp00 * Tmp00;
	gl_FragColor.xyzw = vec4(Tmp00, Tmp00, Tmp00, Tmp00)  * texture2D(texture1, gl_TexCoord[0].xy);  // 
}
// EOF