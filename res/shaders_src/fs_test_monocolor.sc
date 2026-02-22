$input v_color0, v_texcoord0, v_normal
#include <bgfx_shader.sh>
SAMPLER2D(s_texColor, 0);

void main() {
    // Временно игнорируем всё и красим в ЖЕЛТЫЙ
    gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0); 
}
