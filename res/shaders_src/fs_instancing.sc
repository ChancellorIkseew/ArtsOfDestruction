$input v_color0, v_texcoord0, v_normal

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

void main()
{
    vec4 texel = texture2D(s_texColor, v_texcoord0);
    
    // ВАЖНО для кустов: если пиксель прозрачный — не рисуем его вообще.
    // Это предотвратит проблемы с сортировкой прозрачности.
    if (texel.a < 0.1) {
        discard;
    }
    
    // Направление света
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    vec3 normal = normalize(v_normal);
    
    // Расчет диффузного освещения
    float diff = max(0.0, dot(normal, lightDir));
    
    float ambient = 0.2;
    float lightIntensity = ambient + diff;
    
    gl_FragColor = texel * v_color0 * lightIntensity;
}
