#version 420

uniform sampler2D tex;

in vec2 v_texcoord;

out vec4 color;

void main(void) {
    color = texture(tex, v_texcoord);
}
