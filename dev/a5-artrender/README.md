Assignment 5: Art Render

CSCI 4611

Created by Blake Sartor

This program implements both a Phong GLSL fragment shader and a toon shading shader.

Per-Pixel Phong Shading:

    This fragment shader correctly calculates per-pixel shading using the standard phong lighting model.
    This shader calculates ambient, diffuse, and specular light based on Lambert's Law.
    Illumination = ambient + diffuse + specular.
    Since this is a phong shader, the lighting is calculated for each pixel by interpolating the normals
    of each vertex.
    
Artsy Shader:

    This shader contains the same ligthing calculations as in the phong shader above,
    however instead we use the intesity of light to lookup into a texture the final color.
    For diffuse lighting n dot L ranges from -1 to 1, which will be the value we use to look up inside the texture.
    This is then multiplied by our normal diffuse light values.
    For specular lighting, we clamp h dot n to positive values.
    This artsy fragment shader calculates per-pixel toon shading using diffuseRamp, and specularRamp textures to control the lighting.
    
Outline Shader:

    This artsy shader also utilizes a outline vertex shader. This vertex shader determines wear to draw silhouette
    eges using the Card and Mitchell method.
    
