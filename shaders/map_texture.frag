#version 150 compatibility

uniform sampler2D atlas;     // Текстура-атлас
uniform sampler2D colormap;  // Цветовая карта
uniform vec2 tile_size;       // Размер тайла
uniform vec2 atlas_size;      // Размер атласа
float tile_scale = 15.0f;     // Масштаб тайлов

void main() 
{
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 color = texture2D(colormap, texCoord);
    vec2 tileIndex = vec2(0.0, 0.0);

if (distance(color.rgb, vec3(1.0, 1.0, 1.0)) < 0.01) 
    {// трава
        tileIndex = vec2(3.0, 3.0);
    } 
else if (distance(color.rgb, vec3(1.0, 0.0, 1.0)) < 0.01) 
    {// вода
        tileIndex = vec2(1.0, 0.0);
    } 
 else if (distance(color.rgb, vec3(0.922, 0.702, 0.914)) < 0.01) 
    {// холмы
        tileIndex = vec2(4.0, 0.0);
    } 
else if (distance(color.rgb, vec3(0.835, 0.565, 0.780)) < 0.01) 
    {
        tileIndex = vec2(1.0, 4.0);
    } 
else if (distance(color.rgb, vec3(0.498, 0.094, 0.235)) < 0.01) 
    {
        tileIndex = vec2(2.0, 4.0);
    } 
else if (distance(color.rgb, vec3(0.337, 0.486, 0.106)) < 0.01) 
    {
        tileIndex = vec2(5.0, 5.0);
    } 
else if (distance(color.rgb, vec3(0.596, 0.827, 0.514)) < 0.01) 
    {
        tileIndex = vec2(0.0, 6.0);
    }

    vec2 scaledTexCoord = texCoord * tile_scale;
    vec2 tileUV         = fract(scaledTexCoord * atlas_size / tile_size);
    tileUV              = clamp(tileUV, 0.001, 0.999);
    vec2 uv             = (tileIndex * tile_size + tileUV * tile_size) / atlas_size;
    vec4 finalColor     = texture2D(atlas, uv);
    gl_FragColor        = finalColor;
}