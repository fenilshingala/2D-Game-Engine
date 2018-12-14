#Shader vertex
#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex;

uniform mat4 orthographic;
uniform mat4 transform;
uniform vec3 color;

out vec3 fragmentColor;
out vec2 TexCoord;

uniform float sheetsizeX;
uniform float sheetsizeY;
uniform float offsetX;
uniform float offsetY;

void main()
{
	gl_Position = orthographic * transform * vec4(aPos, 1.0);
	fragmentColor = color;
	TexCoord = vec2(tex.x*sheetsizeX+offsetX, tex.y*sheetsizeY+offsetY);
}



#Shader fragment
#version 430 core
out vec4 FragColor;

layout (location = 99) uniform vec4 opacity;

in vec3 fragmentColor;
in vec2 TexCoord;

uniform bool isTexture;
uniform sampler2D texture1;

void main()
{
	FragColor = vec4(fragmentColor, 1.0f);
	if (isTexture) {
		FragColor = texture(texture1, TexCoord) * opacity;
	}
}