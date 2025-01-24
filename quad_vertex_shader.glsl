#version 420 core
layout(location = 0) in vec2 aPos;  // Position
layout(location = 1) in vec3 aColor; // Color

out vec3 ourColor;  // Output color to fragment shader

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    ourColor = aColor;  // Pass color to fragment shader
}