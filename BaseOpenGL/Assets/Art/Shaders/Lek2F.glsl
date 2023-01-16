#version 330 core

out vec4 FragColor;

in vec4 color;                           //color sent to rest of pipeline

void main()
{
    FragColor = color;
}
