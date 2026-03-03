$input v_color0, v_texcoord0, v_normal, v_textureIndex

#include <bgfx_shader.sh>

SAMPLER2DARRAY(s_texColor, 0);

void main() {
    vec3 uvLayer = vec3(v_texcoord0, v_textureIndex);
    vec4 texel = texture2DArray(s_texColor, uvLayer);
    if (texel.a < 0.1) {
        discard;
    }
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    vec3 normal = normalize(v_normal);
    float diff = max(0.0, dot(normal, lightDir));
    float ambient = 0.25;
    float lightIntensity = ambient + diff;
    gl_FragColor = texel * v_color0 * lightIntensity;
}