#version 120

uniform mat4 worldMatrix;
uniform mat4 viewProjMatrix;

varying vec2 texcoord;
varying vec3 normal;

void main()
{
  normal   = mat3(worldMatrix) * gl_Normal;
  texcoord =  (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
  gl_Position = viewProjMatrix * worldMatrix * gl_Vertex;
}