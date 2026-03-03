$input v_color0, v_texcoord0, v_normal, v_textureIndex

#include <bgfx_shader.sh>

SAMPLER2DARRAY(s_texColor, 0);

vec4 calculateLighting(vec4 texel, vec3 surfaceNormal, vec3 lightDirection) {
    if (texel.a < 0.1) {
        discard;
    }

    vec3 unitNormal = normalize(surfaceNormal);
    float diffuseStrength = max(0.0, dot(unitNormal, lightDirection));
    float minAmbient = 0.25;
    float totalLightIntensity = minAmbient + diffuseStrength;
    return vec4(texel.rgb * totalLightIntensity, 1.0);
}

void main() {
    vec3 lightDir = normalize(vec3(0.5, 1.0, -0.5));
    vec3 uvLayer = vec3(v_texcoord0, v_textureIndex);
    vec4 texelColor = texture2DArray(s_texColor, uvLayer);
    gl_FragColor = calculateLighting(texelColor, v_normal, lightDir) * v_color0;
}
