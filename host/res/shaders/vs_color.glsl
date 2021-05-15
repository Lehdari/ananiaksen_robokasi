#version 420

layout(location = 0) in vec3 pos;
layout(location = 3) in vec3 col;

out vec3 vCol;

uniform mat4 objectToWorld;
uniform mat4 worldToClip;

void main()
{
    vCol = col;
    gl_Position = worldToClip * objectToWorld * vec4(pos, 1.0);
}