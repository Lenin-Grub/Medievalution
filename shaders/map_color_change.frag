#version 150 compatibility

uniform sampler2D map_texture;
uniform float transperency;

float threshold(in float thr1, in float thr2 , in float val) {
 if (val < thr1) {return 0.0;}
 if (val > thr2) {return 1.0;}
 return val;
}

// averaged pixel intensity from 3 color channels
float avg_intensity(in vec4 pix) {
 return (pix.r + pix.g + pix.b)/3.;
}

vec4 get_pixel(in vec2 coords, in float dx, in float dy) {
 return texture2D(map_texture,coords + vec2(dx, dy));
}

// returns pixel color
float IsEdge(in vec2 coords){
  float dxtex = 0.5 / 3072.0 /*image width*/;
  float dytex = 0.5 / 2048.0 /*image height*/;
  float pix[9];
  int k = -1;
  float delta;

  // read neighboring pixel intensities
  for (int i=-1; i<2; i++) {
   for(int j=-1; j<2; j++) {
    k++;
    pix[k] = avg_intensity(get_pixel(coords,float(i)*dxtex,
                                          float(j)*dytex));
   }
  }

  // average color differences around neighboring pixels
  delta = (abs(pix[1]-pix[7])+
          abs(pix[5]-pix[3]) +
          abs(pix[0]-pix[8])+
          abs(pix[2]-pix[6])
           )/4.;

  return threshold(0.001,0.009,clamp(1.8*delta,0.0,1.0));
}
  
  vec4 map = texture2D(map_texture, gl_TexCoord[0].xy);

void main()
{
  vec4 color = vec4(0.0f,0.0f,0.0f,1.0f);
  color.rgba = vec4(IsEdge(vec2( gl_TexCoord[0].xy)), 1.0f, 1.0f, 1.0f );
  if(color.r > 0.0f)
  {
	vec3 black = vec3(0.0f,0.0f,0.0f);
	color.rgba = vec4(black,1.0f);
  }
  else
  {
  	color.rgba = vec4(map.rgb,transperency);
  }
  gl_FragColor = color;
}