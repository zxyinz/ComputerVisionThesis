varying vec3 normal;

varying vec3 direction;
varying vec3 half_vec;

varying vec4 ambient;
varying vec4 diffuse;
varying vec4 specular;
varying vec4 glo_ambient;
void main()
{
	//gl_ModelViewProjectionMatrix*gl_Vertex;
	normal=normalize(gl_NormalMatrix*gl_Normal);	
	direction=normalize(vec3(gl_LightSource[0].position));
	//vec4 View=gl_ModelViewMatrix*gl_Vertex;
	//F_Light.half_vec=normalize(vec3(gl_LightSource[0].position)-View.xyz);
	diffuse=gl_FrontMaterial.diffuse*gl_LightSource[0].diffuse;
	ambient=gl_FrontMaterial.ambient*gl_LightSource[0].ambient;
	glo_ambient=gl_FrontMaterial.ambient*gl_LightModel.ambient;
	gl_Position=ftransform();
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_TexCoord[1]=gl_MultiTexCoord0;
	gl_TexCoord[2]=gl_MultiTexCoord0;
	gl_FrontColor=gl_Color;
} 
