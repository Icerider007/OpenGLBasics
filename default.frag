#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texPos;

uniform sampler2D tex0;

void main()
{
   FragColor = vec4(texPos, 0, 1);
   #FragColor = texture(tex0, texPos);
};


