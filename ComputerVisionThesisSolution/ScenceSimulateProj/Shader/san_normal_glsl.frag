uniform sampler2D texture0;
varying vec3 normal;
vec3 NormalMap(vec3 normal,mat3 normal_mat,sampler2D normal_map,vec2 coord)
{
	float theta=acos(normal.y);
	float phi=acos(normal.x);
	normal=texture2D(normal_map,coord).xyz;
	normal=normalize(normal);
	theta=theta+normal.y;
	phi=phi-normal.x;
	normal=vec3(cos(phi)*sin(theta),cos(theta),sin(phi)*sin(theta));
	return normal;
}
void main()
{
	normal=NormalMap(normal,gl_NormalMatrix,texture1,gl_TexCoord[0].st);
	gl_FragColor=vec4(normal,1.0);
}