// simple toon fragment shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;
uniform vec3 BaseColor;

void main()
{
	float intensity;
	vec3 n;
	vec4 color;

	n = normalize(normal);
	intensity = max(dot(lightDir,n),0.0);

	// METAL FRAGMENT
	float i = intensity;
	if(intensity > 0.99)
		color = vec4(1.0,1.0,1.0,1.0);
	else if(intensity < 0.1)
		color = vec4(0.1,0.1,0.1,1.0);
	else
		color = vec4(i*BaseColor.r, i*BaseColor.g, i*BaseColor.b,1.0);

	gl_FragColor = color;
}
