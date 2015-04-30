varying vec3 normal;
void main()
{
	normal=normalize(gl_NormalMatrix*gl_Normal);	
	direction=normalize(vec3(gl_LightSource[0].position));
	gl_Position=ftransform();
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_FrontColor=vec4(1.0);
} 
