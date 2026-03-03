$input a_position, a_color0, a_texcoord0, a_normal
$output v_color0, v_texcoord0, v_normal, v_textureIndex

#include <bgfx_shader.sh>

uniform vec4 u_texData;

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
    v_textureIndex = u_texData.x;
    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
}
