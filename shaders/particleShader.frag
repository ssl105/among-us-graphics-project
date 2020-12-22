#version 330 core
// This is a sample fragment shader.

uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(color, 1.0f);
}