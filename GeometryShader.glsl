#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

in VS_OUT {
	vec3 position;
	vec3 color;
	vec2 texture;
} _in[];

out GS_OUT {
	vec3 position;
	vec3 color;
	vec3 normal;
	vec2 texture;
} _out;

uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 offset;

vec3 inWorld(vec3 pos)
{
	 return vec3(world * vec4(pos, 1));
}

vec3 normal()
{
	return normalize(cross(
	 	inWorld(_in[1].position) - inWorld(_in[0].position),
		inWorld(_in[2].position) - inWorld(_in[0].position)
	));
}

void main()
{
	mat4 pvw = proj * view * world;

	_out.normal = normal();

	for(int i = 0; i < 3; i++)
	{
		gl_Position = pvw * vec4(_in[i].position + offset, 1);
		_out.position = inWorld(_in[i].position + offset);
		_out.color = _in[i].color.rgb;
		_out.texture = _in[i].texture.st;
		EmitVertex();
	}
	EndPrimitive();

	for(int i = 0; i < 3; i++)
	{
		gl_Position = pvw * vec4(_in[i].position - offset, 1);
		_out.position = vec3(world * vec4(_in[i].position - offset, 1));
		_out.color = _in[i].color.rgb;
		_out.texture = _in[i].texture.st;
		EmitVertex();
	}
	EndPrimitive();

}
