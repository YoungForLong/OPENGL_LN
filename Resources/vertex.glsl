#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 texCoord;
// out vec3 normalV;
// out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D texture_diffuse1;

void main()
{
	// fragPos = vec3(model * vec4(aPos, 1.0));
	texCoord = aTexCoord;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}