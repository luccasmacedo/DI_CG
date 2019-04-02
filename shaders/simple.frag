#version 130

uniform vec3 userColor;
out vec4 fragcolor;

void main()
{
	fragcolor = vec4(userColor,1.0);
}
