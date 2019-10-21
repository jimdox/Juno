#version 450 core
in vec2 pass_texCoordinates;
in vec3 surfaceNormal;
in vec3 lightDir;

out vec4 output_color; 

uniform sampler2D diffuse;
uniform vec3 lightColor;

void main()
{
        vec3 unit_normal = normalize(surfaceNormal);
        vec3 unit_lightDir = normalize(lightDir);

        float relativeDir = dot(unit_normal, unit_lightDir);
        float brightness = max(relativeDir, 0.0);
        vec3 diffuse_factor = brightness * lightColor;

        output_color = vec4(diffuse_factor, 1.0) * texture(diffuse, pass_texCoordinates);


}
