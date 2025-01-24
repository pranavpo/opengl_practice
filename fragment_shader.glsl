#version 330 core

// Input from vertex shader
in vec3 fragColor;

// Output to the screen
out vec4 FragColor;

void main()
{
    // Set the fragment color, including alpha (set to 1.0 for fully opaque)
    FragColor = vec4(fragColor, 1.0);
}