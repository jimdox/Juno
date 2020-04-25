#version 450 core
in vec3 position;
in vec3 normal;

out vec3 pass_baseColor;
out vec3 surfaceNormal;
out vec3 toLightDir[4];
out vec3 cameraDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition[4];
uniform vec3 baseColor;

void main()
{
        vec4 globalPosition = model * vec4(position, 1.0);
        gl_Position = projection * view * globalPosition;
        pass_baseColor = baseColor;

        surfaceNormal = (model * vec4(normal, 0.0)).xyz;
        
        for(int i=0; i<4; i++)
        {
                toLightDir[i] = lightPosition[i] - globalPosition.xyz;
        }
        cameraDir = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - globalPosition.xyz;
        
}






