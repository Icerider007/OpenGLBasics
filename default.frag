#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texPos;

uniform sampler2D tex0;
uniform float time; // Pass time as a uniform for animation


void main()
{
    FragColor = 0.5 * texture(tex0, texPos);
	FragColor += vec4(color, 1.0f);
}
