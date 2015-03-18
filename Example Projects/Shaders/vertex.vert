#version 110

void main(void)
{
	// transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}