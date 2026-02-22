$input a_position, a_color0, a_texcoord0, a_normal
$output v_color0, v_texcoord0, v_normal

#include <bgfx_shader.sh>

void main()
{
    // Умножаем позицию на World-View-Projection матрицу
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    
    // Передаем цвет и UV дальше
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
    
    // Передаем нормаль в мировые координаты (вращаем вместе с объектом)
    v_normal = mul(u_model[0], vec4(a_normal, 0.0)).xyz;
}
