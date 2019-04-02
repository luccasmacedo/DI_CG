#version 130

// The varying qualifier are deprecated from GLSL 1.40 and above
// Attribute and varying were changed to 'in' and 'out' qualifier
in vec3 N;
in vec3 v;

out vec4 fragColor;

void main (void)
{
   vec3 L = normalize(gl_LightSource[0].position.xyz - v);
   vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
   vec3 R = normalize(-reflect(L,N));

   //calculate Ambient Term:
   vec4 Iamb = gl_FrontLightProduct[0].ambient;

   //calculate Diffuse Term:
   vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);

   // calculate Specular Term:
   vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);

   // write Total Color
   fragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
}
