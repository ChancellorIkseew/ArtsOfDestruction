$input v_color0             // Получаем цвет из varying

#include <bgfx_shader.sh>

void main() {
    gl_FragColor = v_color0; // Устанавливаем итоговый цвет пикселя
}
