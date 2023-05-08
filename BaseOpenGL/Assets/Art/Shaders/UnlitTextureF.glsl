#version 330 core

out vec4 FragColor;

in vec4 color;                           //color sent to rest of pipeline
in vec2 texCoords;

uniform sampler2D texture1;

void main()
{
    FragColor = vec4(texture(texture1, texCoords).rgb, 1.0);
}
