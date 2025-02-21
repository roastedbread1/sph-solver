#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout(std430, binding = 1) buffer InstanceMatrices
{
    mat4 matrices[];
};

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    mat4 instanceMatrix = matrices[gl_InstanceID];
    vec4 worldPos = instanceMatrix * model * vec4(aPos, 1.0);
    FragPos =  vec3(worldPos);

    
    mat3 normalMatrix = mat3(instanceMatrix * model);
    Normal = normalMatrix * aNormal;

    TexCoords = aTexCoords;

    gl_Position = projection * view * worldPos;
}
