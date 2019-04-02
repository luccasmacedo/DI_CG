// simple toon fragment shader
// www.lighthouse3d.com

varying vec3 normal, lightDir;

void main()
{
	float intensity;
	vec3 n;
	vec4 color;

	n = normalize(normal);
	intensity = max(dot(lightDir,n),0.0);

	// TOON FRAGMENT
	if (intensity > 0.97)
		color = vec4(1.0,1.0,1.0,1.0);
	else if (intensity > 0.66)
		color = vec4(0.8,0.8,0.2,1.0);
	else if (intensity > 0.33)
		color = vec4(0.2,0.2,0.2,1.0);
	else
		color = vec4(0.1,0.1,0.1,1.0);

/*
	// METAL FRAGMENT
	float i = intensity;
	if(intensity > 0.95)
		color = vec4(0.9,1.0,0.9,1.0);
	else if(intensity < 0.1)
		color = vec4(0.0,0.0,0.0,1.0);
	else
		color = vec4(i,i,0.0,1.0);
*/
	gl_FragColor = color;
}
