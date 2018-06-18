#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to calculate per-fragment shading here using a toon shading model

in vec3 Position;
in vec3 Normal;

out vec4 color;

uniform vec3 lightPosition;
uniform vec4 Ia, Id, Is;

uniform vec4 ka, kd, ks;
uniform float s;

uniform sampler2D diffuseRamp;
uniform sampler2D specularRamp;



void main() {
    // normalize fragment normal
    vec3 n = normalize(Normal);
    
    // unit vector from position to the light
    vec3 l = normalize(lightPosition - Position);
    
    // unit vector from position to the eye
    vec3 e = normalize(vec3(0,0,0) - Position);
    
    // halfway vector
    vec3 h = normalize(l + e);
    
    vec4 ambient = ka * Ia;
    
    // diffuse intensity for looking up in texture
    float diff_intensity = (dot(n,l) + 1) / 2;
    // texture lookup
    vec4 diffuse = kd * Id * texture(diffuseRamp, vec2(diff_intensity,0));
    
    // speculate intesntiy for looking up in the texture
    float spec_intensity = dot(h,n);
    // texture lookup
    vec4 specular = ks * Is * texture(specularRamp, vec2(pow(max(spec_intensity,0),s),0));
    
    color = ambient + diffuse + specular;
    //color = vec4(0,0,0,1);
}
