#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texture;

out VS_OUT {
	vec3 position;
	vec3 color;
	vec2 texture;
} _out;

void main()
{
	_out.position = position;
	_out.color    = color;
	_out.texture  = texture;
}
