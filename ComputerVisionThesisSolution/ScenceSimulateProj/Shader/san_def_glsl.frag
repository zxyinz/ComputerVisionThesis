#version 430 core
layout (binding = 0) uniform sampler2D diff_tex;
layout (binding = 2) uniform sampler2D normal_tex;
layout (binding = 4) uniform sampler2D spec_tex;
layout (binding = 5) uniform sampler2D height_tex;

in vec3 TextureCoord;
out vec4 FragColor;

void main()
{
	FragColor=vec4(1.0,0.0,0.0,1.0);
}