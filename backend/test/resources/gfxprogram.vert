#version 120

// using older version to make sure lxgfx filters 
// out potential gl_ named variables

uniform mat4 worldMatrix[2];  // regular, invTranspose

varying vec2 vTexcoord;
varying vec3 vNormal;

void main()
{
  vNormal   = mat3(worldMatrix[1]) * gl_Normal;
  vTexcoord =  (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;
  gl_Position = gl_ModelViewProjectionMatrix * worldMatrix[0] * gl_Vertex;
}