#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texPos;

uniform sampler2D tex0;
uniform float time; // Pass time as a uniform for animation

void main()
{
    // Create ripple effect using sine waves
    float wave = sin(texPos.x * 10.0 + time * 2.0) * 0.02;
    wave += sin(texPos.y * 15.0 + time * 1.5) * 0.02;

    // Distort the texture coordinates
    vec2 distortedTexPos = texPos + vec2(wave, wave);

    // Sample the texture
    vec4 texColor = texture(tex0, distortedTexPos);

    // Add a pulsating brightness based on time
    float brightness = 1 + (0.5 + 0.5 * sin(time * 3.0));

    // Combine the texture with input color and brightness
    FragColor = texColor * vec4(color, 1.0) * brightness;
}

