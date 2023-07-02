#version 330 
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(0.0,0.0,0.0, 0.0); // see how we directly give a vec3 to vec4's constructor
}