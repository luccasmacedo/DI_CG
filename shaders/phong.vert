#version 130

// The varying qualifier are deprecated from GLSL 1.40 and above
// Attribute and varying were changed to 'in' and 'out' qualifier
out vec3 N;
out vec3 v;

void main(void)
{
   v = vec3(gl_ModelViewMatrix * gl_Vertex);
   N = normalize(gl_NormalMatrix * gl_Normal);

   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
