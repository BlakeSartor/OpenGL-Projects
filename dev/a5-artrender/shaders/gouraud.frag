#version 330

in vec4 color;
in vec2 textcoords;
out vec4 final_color;

uniform sampler2D surfaceTex;

void main() {
    //final_color = color;
    //vec4 tex_color = texture(surface_tex, textcoords);
    //final_color = color * tex_color;
    final_color = color;
}
