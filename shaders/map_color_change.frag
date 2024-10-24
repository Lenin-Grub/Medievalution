#version 150 compatibility

uniform sampler2D map_texture;
uniform float transparency;
uniform vec4 select_color;
uniform float height;
uniform float width;

vec4 map = texture2D(map_texture, gl_TexCoord[0].xy);
vec4 color = vec4(0.0f,0.0f,0.0f,1.0f);


float threshold(in float thr1, in float thr2 , in float val) 
{
 if (val < thr1) {return 0.0;}
 if (val > thr2) {return 1.0;}
 return val;
}


float avg_intensity(in vec4 pix) 
{
 return (pix.r + pix.g + pix.b)/3.;
}


vec4 get_pixel(in vec2 coords, in float dx, in float dy) 
{
 return texture2D(map_texture,coords + vec2(dx, dy));
}


float IsEdge(in vec2 coords)
{
//set thickness of border lines
  float dxtex = 0.5 / width;
  float dytex = 0.5 / height;
  float pix[9];
  int k = -1;
  float delta;

  
  for (int i=-1; i<2; i++) 
  {
   for(int j=-1; j<2; j++) 
   {
    k++;
    pix[k] = avg_intensity(get_pixel(coords, float(i)*dxtex, float(j)*dytex));
   }
  }

 
  delta = (abs(pix[1]-pix[7]) +
           abs(pix[5]-pix[3]) +
           abs(pix[0]-pix[8]) +
           abs(pix[2]-pix[6])
           )/4.;

  return threshold(0.001,0.009,clamp(1.8*delta,0.0,1.0));
}

void main()
{
  float isEdgeValue = IsEdge(vec2(gl_TexCoord[0].xy));
  vec3 borderColor = vec3(0.0f, 0.0f, 0.0f);
  vec3 selectionColor = vec3(1.0f, 1.0f, 1.0f);

  // draw black border line or colored selected area
  if (isEdgeValue > 0.0f)
    gl_FragColor = vec4(borderColor, 1);
  else if (select_color.rgb == map.rgb)
    gl_FragColor = vec4(selectionColor, 0.5);
  else
    gl_FragColor = vec4(map.rgb, transparency);
}