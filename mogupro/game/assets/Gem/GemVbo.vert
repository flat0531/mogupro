#version 150 core

uniform mat4	ciModelViewProjection;
uniform float sysTime;
in vec4			ciPosition;
in vec4        ciColor;
out vec4         vColor;		

void main( void ) {

	gl_Position	= ciModelViewProjection * ciPosition;
	vec4 col = ciColor;
	col.a = sin( col.a + sysTime ) * 0.5 + 1.0;
	vColor = col;
}