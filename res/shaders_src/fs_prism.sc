$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

// Объявляем сэмплер (текстурный слот)
SAMPLER2D(s_texColor, 0);

void main()
{
    // Читаем цвет из текстуры
    vec4 texel = texture2D(s_texColor, v_texcoord0);
    
    // Итоговый цвет = текстура * цвет вершины
    gl_FragColor = texel * v_color0;
    
    // Если не работает
    //gl_FragColor = vec4(v_texcoord0.x, v_texcoord0.y, 0.0, 1.0);
}
