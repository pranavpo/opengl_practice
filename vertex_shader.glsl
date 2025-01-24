#version 330 core

// Input attributes
layout (location = 0) in vec2 aPos; // Vertex position
layout (location = 1) in vec3 aCol; // Vertex color

// Output to fragment shader
out vec3 fragColor;

// Uniforms
uniform mat4 model;

void main()
{
    // Apply the transformation matrix and output the position
    gl_Position = model * vec4(aPos, 0.0, 1.0);
    
    // Pass the color to the fragment shader
    fragColor = aCol;
}