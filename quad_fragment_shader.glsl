#version 420 core

in vec3 ourColor;  // Color from the vertex shader
out vec4 FragColor; // Final color output

void main()
{
    FragColor = vec4(ourColor, 1.0f);  // Set the output color
}