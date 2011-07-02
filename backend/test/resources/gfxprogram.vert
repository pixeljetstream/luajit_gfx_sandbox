#version 120

uniform mat4 worldMatrix[2];  // regular, invTranspose

varying vec2 texcoord;
varying vec3 normal;

void main()
{
  normal   = mat3(worldMatrix[1]) * gl_Normal;
  texcoord =  (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
  gl_Position = gl_ModelViewProjectionMatrix * worldMatrix[0] * gl_Vertex;
}