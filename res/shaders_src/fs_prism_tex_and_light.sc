$input v_color0, v_texcoord0, v_normal

#include <bgfx_shader.sh>

// Объявляем сэмплер (текстурный слот)
SAMPLER2D(s_texColor, 0);

void main()
{
    // Читаем цвет из текстуры
    vec4 texel = texture2D(s_texColor, v_texcoord0);
    
    // Направление света (как будто солнце светит сверху-сбоку)
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    vec3 normal = normalize(v_normal);
    
    // Расчет диффузного освещения (Dot Product)
    float diff = max(0.0, dot(normal, lightDir));
    
    // Ambient: минимальный свет в тенях (0.2 - 0.3)
    float ambient = 0.2;
    float lightIntensity = ambient + diff;
    
    // Итоговый цвет = текстура * цвет вершины * уровень света
    gl_FragColor = texel * v_color0 * lightIntensity;
    
    // Если не работает
    //gl_FragColor = vec4(v_texcoord0.x, v_texcoord0.y, 0.0, 1.0); //1)
    //gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);                     //2)
    
    //vec3 normal = normalize(v_normal);
    // Если не работает. Превращаем диапазон нормали [-1, 1] в [0, 1] для визуализации цвета
    //gl_FragColor = vec4(normal * 0.5 + 0.5, 1.0);
}
