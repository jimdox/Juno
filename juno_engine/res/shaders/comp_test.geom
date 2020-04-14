#version 430
#extension GL_ARB_compute_shader: enable;
#extension GL_ARG_shader_storage_buffer_object: enable;

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float particleSize;

out vec2 texCoord;

void main()
{

    //base?

    gl_Position = gl_in[0].gl_Position;
    texCoord = vec2(0,0);

    EmitVertex();

    //right
    gl_Position = gl_in[0].gl_Position + vec4(vec3(1,0,0) * particleSize, 0.0);
    texCoord = vec2(1,0);

    EmitVertex();

    //up

    gl_Position = gl_in[0].gl_Position + vec4(vec3(0,1,0) * particleSize, 0.0);
    texCoord = vec2(0,1);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(vec3(1,1,0) * particleSize, 0.0);
    texCoord = vec2(1,1);
    EmitVertex();

    EndPrimitive();
}