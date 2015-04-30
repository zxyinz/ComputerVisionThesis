uniform sampler2D texture0;
uniform vec2 TexCoord;
varying vec3 normal;
varying vec3 direction;
varying vec3 half_vec;
varying vec4 ambient;
varying vec4 diffuse;
varying vec4 specular;
varying vec4 glo_ambient;
vec4 PhoneSepcular(vec3 normal, vec3 light_dir, vec3 half_vec, float spec_pow)
{
	vec4 Specular = vec4(0.0, 0.0, 0.0, 0.0);
	half_vec = normalize(half_vec = vec3(0.0, 0.0, 1.0));
	float dot_NormalLight = max(dot(normal, light_dir), 0.0);
	float dot_NormalHalf = max(dot(normal, half_vec), 0.0);
	if (dot_NormalLight>0.0)
	{
		Specular = vec4(pow(dot_NormalHalf, spec_pow));
	}
	return Specular;
}
vec3 NormalMap(vec3 normal, mat3 normal_mat, sampler2D normal_map, vec2 coord)
{
	float theta = acos(normal.y);
	float phi = acos(normal.x);
	normal = texture2D(normal_map, coord).xyz;
	normal = normalize(normal);
	theta = theta + normal.y;
	phi = phi - normal.x;
	normal = vec3(cos(phi)*sin(theta), cos(theta), sin(phi)*sin(theta));
	return normal;
}
void main()
{
	vec3 color = texture2D(texture0, gl_TexCoord[0].st).xyz;
	float dot_NLight = max(dot(normal, direction), 0.0);
	diffuse = dot_NLight*diffuse*vec4(color, 1.0);
	ambient = ambient*vec4(color, 1.0);
	glo_ambient = glo_ambient*vec4(color, 1.0);
	vec4 Specular = PhoneSepcular(normal, direction, half_vec, 128.0);

	float Depth = gl_FragCoord.z;
	Depth = pow(Depth, 128);

	gl_FragColor = diffuse + ambient + glo_ambient + Specular;

	gl_FragColor = vec4(1.0 - Depth);
}