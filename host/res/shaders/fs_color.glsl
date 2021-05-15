#version 420

in vec3 vCol;

out vec4 fragColor;

void main()
{
    fragColor = vec4(vCol, 1.0);
}