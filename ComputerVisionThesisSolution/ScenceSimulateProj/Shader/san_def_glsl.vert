#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec3 tex_coord01;
layout (location = 4) in vec3 tex_coord02;
layout (location = 5) in vec3 tex_coord03;

layout (std140) uniform SanSystemVariable
{
	/*Render Window Infomation*/
	vec2 WindowSize;
	/*System Matrix*/
	mat4x4 ModelViewMatrix;
	mat4x4 ProjectionMatrix;
	/*Material*/
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Emissive;
	vec3 Specular;
	float Shineness;
	vec4 MaterialControl;
} RenderConfig;

out vec3 TextureCoord01;

void main()
{
	gl_Position=position*SanSystemVariable.ModelViewMatrix*SanSystemVariable.ProjectionMatrix;
	TextureCoord01=tex_coord01;
} 
