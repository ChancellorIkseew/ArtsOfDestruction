$input v_color0, v_texcoord0, v_normal, v_textureIndex
#include <bgfx_shader.sh>
SAMPLER2D(s_texColor, 0);

void main() {
    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0); 
}
