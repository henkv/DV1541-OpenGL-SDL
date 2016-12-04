#version 430

in GS_OUT {
	vec3 position;
	vec3 color;
	vec3 normal;
	vec2 texture;
} _in;

out vec4 fragmentColor;

uniform sampler2D bthImage;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 ambientLight;

void main () {
	vec4 textureColor = texture(bthImage, _in.texture);
	if (textureColor.a < 0.33)
		discard;

	vec3 lightNormal = normalize(lightPosition - _in.position);
	float diffuse = max(dot(_in.normal, lightNormal), 0);

	fragmentColor =	vec4(_in.color * diffuse + _in.color * ambientLight, 1) * textureColor;
}
