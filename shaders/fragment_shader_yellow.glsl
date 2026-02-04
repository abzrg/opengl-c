#version 410 core

// `out` declares an output variable.
// Fragment shader only requires one output variable: the final color output
// The color is represented as RGBA (A: alpha, representing opacity) format, and is a floating point
// number ranged between 0.0 and 1.0.
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.7f, 0.0f, 1.0f);
}
