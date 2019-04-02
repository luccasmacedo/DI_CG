varying float LightIntensity; 
varying vec3  MCposition; 
uniform sampler3D Noise; 
uniform float NoiseScale; 
void main() 
{ 
    vec4  noisevec  = texture3D(Noise, NoiseScale * MCposition); 
    float intensity = min(1.0, noisevec[3] * 18.0); 
    vec3  color     = vec3(intensity * LightIntensity); 
    gl_FragColor    = vec4(color, 1.0); 
}
