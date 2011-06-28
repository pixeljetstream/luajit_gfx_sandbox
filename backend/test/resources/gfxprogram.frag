#version 120
#extension GL_ARB_shader_subroutine : enable
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_NV_shader_buffer_load : enable

varying vec2 inTexcoord;
varying vec3 inNormal;

layout(std140) uniform block {
  vec4  ycolor;
};

subroutine vec3 shuffle_fn(vec3 v); 

subroutine (shuffle_fn)
vec3 shuffle_xyz(vec3 v)
{
  return v;
}

subroutine (shuffle_fn)
vec3 shuffle_zyx(vec3 v)
{
  return v.zyx;
}

uniform sampler2D ztex;
uniform vec4*     xcolor;

subroutine uniform shuffle_fn shuffleFunc;

void main()
{
  vec3 normal = abs(normalize(inNormal));
  vec4 color = (vec4)0;
  
  normal = shuffleFunc(normal);
  
  color += normal.x * xcolor[0];
  color += normal.y * ycolor;
  color += normal.z * texture2D(ztex,inTexcoord);
  
  
  gl_FragColor = color;
}