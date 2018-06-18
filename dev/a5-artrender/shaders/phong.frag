#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using the Phong shading
// model.


in vec3 Position;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPosition;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;


void main() {
    // normalize fragment normal
    vec3 n = normalize(Normal);
    
    // unit vector from position to the light
    vec3 l = normalize(lightPosition - Position);
    
    // unit vector from position to the eye
    vec3 e = normalize(vec3(0,0,0) - Position);
    
    // halfway vector
    vec3 h = normalize(l + e);

    // color = ka * Ia + kd * Id * (n dot l ) + ks * Is * (h dot n )^s
    vec4 ambient = ka * Ia;
    vec4 diffuse = kd * Id * max(dot(n,l),0);
    vec4 specular = ks * Is * pow(dot(h,n),s);
    
    color = ambient + diffuse + specular;
   // color = vec4(0.4,1,0.4,1);
}
