$input a_position, a_color0, a_texcoord0, a_normal, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_color0, v_texcoord0, v_normal, v_textureIndex

#include <bgfx_shader.sh>

void main() {
    mat4 modelInst = mtxFromRows(i_data0, i_data1, i_data2, i_data3);
    gl_Position = mul(u_viewProj, mul(modelInst, vec4(a_position, 1.0)));
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
    v_textureIndex = i_data4.x;
    v_normal = mul(modelInst, vec4(a_normal, 0.0)).xyz;
}
