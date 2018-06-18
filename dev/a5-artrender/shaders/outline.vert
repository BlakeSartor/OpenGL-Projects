#version 330

// CSci-4611 Assignment 5:  Art Render

// TODO: You need to modify this vertex shader to move the edge vertex along
// the normal away from the mesh surface IF you determine that the vertex
// belongs to a silhouette edge.


uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform mat4 projectionMatrix;
uniform float thickness;

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 leftNormal;
layout(location = 3) in vec3 rightNormal;

void main() {
    // vertext position in eye space
    vec3 v = (modelViewMatrix * vec4(vertex,1)).xyz;
    
    // unit vector from the vertex to eye point
    vec3 to_eye = normalize(vec3(0,0,0) - v);
    // normal transformed into eye space
    vec3 vert_norm = (normalMatrix * vec4(normal,0)).xyz;
    // left normal transformed into eye space
    vec3 left_norm = (normalMatrix * vec4(leftNormal,0)).xyz;
    // right normal transformed into eye space
    vec3 right_norm = (normalMatrix * vec4(rightNormal,0)).xyz;
    
    // if on silhouette edge
    if (dot(to_eye,left_norm) * dot(to_eye, right_norm) < 0.0) {
        v = v + thickness * vert_norm;
    }
    
    gl_Position = projectionMatrix * vec4(v,1);
    //gl_Position = projectionMatrix * modelViewMatrix * vec4(vector,1);

}
