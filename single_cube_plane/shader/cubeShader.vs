// ----------------------------------------------------------------------------
//
//
// ----------------------------------------------------------------------------
#version 130

// ...
in vec3 aPos;
in vec2 aTexCoord;

// ...
out vec2 TexCoord;

// ...
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// ...
void main()
{
  gl_Position = projection * view * model * vec4(aPos, 1.0f);
  TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}
