#version 330 core
in vec2 gl_TexCoord[1];
out vec4 gl_FragColor;

// Важно! Количество тайлов в тайловой карте
const int tile_max     = 64;

uniform sampler2D atlas;
uniform sampler2D index_map;
uniform vec2 tile_size;
uniform vec2 atlas_size;
uniform vec2 tileIndices[tile_max];

const float tile_scale = 20.0;

void main() 
{
    vec2 tc      = gl_TexCoord[0].xy;
    float rValue = texture(index_map, tc).r;
    int index    = int(rValue * 8.0 + 0.5);
    index        = clamp(index, 0, 8);

    vec2 tile_id = tileIndices[index];

    vec2 tiles_in_atlas = atlas_size / tile_size;
    vec2 local_uv       = fract(tc * tile_scale * tiles_in_atlas);
    vec2 uv             = (tile_id + local_uv) / tiles_in_atlas;

    vec4 final_color = texture(atlas, uv);
    gl_FragColor     = final_color;
}