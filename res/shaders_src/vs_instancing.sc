$input a_position, a_color0, a_texcoord0, a_normal, i_data0, i_data1, i_data2, i_data3
$output v_color0, v_texcoord0, v_normal

#include <bgfx_shader.sh>

void main()
{
    // Собираем матрицу. mtxFromRows — это макрос из bgfx_shader.sh
    // который правильно разложит i_data0..3 в mat4
    mat4 modelInst = mtxFromRows(i_data0, i_data1, i_data2, i_data3);
    
    // Вычисляем финальную позицию
    gl_Position = mul(u_viewProj, mul(modelInst, vec4(a_position, 1.0)));
    
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
    
    // Трансформируем нормаль (только вращение и масштаб)
    v_normal = mul(modelInst, vec4(a_normal, 0.0)).xyz;
}
