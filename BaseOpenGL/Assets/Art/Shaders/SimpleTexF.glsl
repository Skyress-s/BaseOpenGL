#version 330 core

out vec4 FragColor;

in vec2 texCoords;
in vec4 color;                           //color sent to rest of pipeline

uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    // FragColor = color;
    FragColor = texture(texture1, texCoords);
}
