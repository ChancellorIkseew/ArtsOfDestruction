$input a_position, a_color0 // Заявляем входные атрибуты из VertexLayout
$output v_color0            // Заявляем выходной атрибут, который пойдет в varying

#include <bgfx_shader.sh>   // Стандартные определения BGFX (u_modelViewProj и т.п.)

void main() {
    // u_modelViewProj — стандартная матрица (модель*вид*проекция) от BGFX.
    // Если она не задана, BGFX по умолчанию использует identity, что оставляет объект в NDC (-1..1)
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_color0 = a_color0; // Передаем цвет "как есть" в фрагментный шейдер
}
