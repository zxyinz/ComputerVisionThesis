void main()
{
	vec3 Depth=gl_FragCoord.z;
	Depth=pow(Depth,128);
	gl_FragColor=vec4(Depth,1.0);
}