#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform bool useTexture;
uniform sampler2D texture1;

uniform sampler2D shadowMap;
uniform mat4 u_LightSpaceMatrix;

// === Verbesserte Shadow-Berechnung ===
float ShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDirNorm) {
    // Projektion in [0, 1]
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    // Außerhalb des Shadow Maps → kein Schatten
    if (projCoords.z > 1.0)
        return 0.0;

    // Adaptive Bias gegen Peter Panning
    float bias = max(0.05 * (1.0 - dot(norm, lightDirNorm)), 0.005);

    // PCF (weiches Shadow-Filtering)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    int kernelRadius = 2; // 5x5 Kernel

    for (int x = -kernelRadius; x <= kernelRadius; ++x) {
        for (int y = -kernelRadius; y <= kernelRadius; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float closestDepth = texture(shadowMap, projCoords.xy + offset).r;
            if (projCoords.z - bias > closestDepth)
                shadow += 1.0;
        }
    }

    float totalSamples = pow((2 * kernelRadius + 1), 2);
    shadow /= totalSamples;

    return shadow;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDirNorm = normalize(-lightDir);

    // Transformiere Fragmentposition in Licht-Sicht-Raum
    vec4 fragPosLightSpace = u_LightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = ShadowCalculation(fragPosLightSpace, norm, lightDirNorm);

    // Beleuchtung (diffus)
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = diff * lightColor * (1.0 - shadow);

    // Farbe
    vec3 baseColor = useTexture ? texture(texture1, TexCoord).rgb : objectColor;
    vec3 result = baseColor * diffuse;

    FragColor = vec4(result, 1.0);
}
