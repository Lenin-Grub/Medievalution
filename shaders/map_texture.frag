#version 330 core

in vec2 gl_TexCoord[1];
out vec4 gl_FragColor;


const int tile_max = 64;

uniform sampler2D atlas;
uniform sampler2D index_map;
uniform vec2 tile_size;
uniform vec2 atlas_size;
uniform vec2 tileIndices[tile_max];

const float tile_scale = 20.0;

uniform float u_time;

vec2 random2(vec2 st) 
{
    st = vec2(dot(st, vec2(127.1, 311.7)),
              dot(st, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

float perlinNoise(vec2 st) 
{
    vec2 i = floor(st);
    vec2 f = fract(st);
    vec2 u = f * f * (3.0 - 2.0 * f);

    float a = random2(i + vec2(0.0, 0.0)).x;
    float b = random2(i + vec2(1.0, 0.0)).x;
    float c = random2(i + vec2(0.0, 1.0)).x;
    float d = random2(i + vec2(1.0, 1.0)).x;

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

float fbm(vec2 st) 
{
    float value = 0.0;
    float amplitude = 0.55;
    float frequency = 1.0;
    for (int i = 0; i < 4; i++) 
    {
        value += amplitude * perlinNoise(st * frequency);
        st *= 1.7;
        frequency *= 1.7;
        amplitude *= 0.5;
    }
    return value;
}

void main() 
{
    vec2 tc = gl_TexCoord[0].xy;

    float rValue = texture(index_map, tc).r;
    int index = int(rValue * 8.0 + 0.5);
    index = clamp(index, 0, 8);

    vec2 tile_id = tileIndices[index];

    vec2 tiles_in_atlas = atlas_size / tile_size;
    vec2 local_uv = fract(tc * tile_scale * tiles_in_atlas);
    vec2 atlas_uv = (tile_id + local_uv) / tiles_in_atlas;

    vec4 base_color = texture(atlas, atlas_uv);

    if (index == 0) 
    {

        vec2 uv = tc * 6.0;
        float t = u_time * 0.3;
        float waveStrength = 0.3;
        float waveX = sin(uv.y * 3.0 + t) * waveStrength;
        float waveY = sin(uv.x * 3.0 + t + 1.57) * waveStrength * 0.6;

        vec2 distorted = uv;
        distorted.x += waveX;
        distorted.y += waveY;

        float noise = fbm(distorted + vec2(t * 0.5, t * 0.7));
        noise = 0.5 + 0.5 * noise;  // [0,1]

        distorted.x += noise * 0.1;

        float finalNoise = fbm(distorted * 1.2);
        finalNoise = 0.5 + 0.5 * finalNoise;

        vec3 waterColor = mix(
            vec3(0.0, 0.1, 0.3),
            vec3(0.1, 0.3, 0.5),
            finalNoise
        );
        float highlight = smoothstep(0.8, 1.0, finalNoise);
        waterColor = mix(waterColor, vec3(1.0), highlight * 0.3);

        gl_FragColor = vec4(waterColor, 1.0);
    }
    else 
    {
        gl_FragColor = base_color;
    }
}