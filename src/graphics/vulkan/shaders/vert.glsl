#version 460

layout(binding = 0) uniform UniformBufferObject
{
    mat4 transform;
} ubo;

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 colorIn;

layout(location = 0) out vec3 colorOut;

void main()
{ 
    gl_Position = ubo.transform * vec4(positionIn, 1.0);

    colorOut = colorIn;

    return;
}
