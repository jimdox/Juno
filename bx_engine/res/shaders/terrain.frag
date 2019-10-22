#version 450 core
in vec2 pass_texCoordinates;
in vec3 surfaceNormal;
in vec3 toLightDir;
in vec3 cameraDir;

out vec4 output_color; 

uniform sampler2D diffuse;
uniform vec3 lightColor;
uniform float reflectivity;
uniform float shineDamper;


void main()
{
        vec3 unit_normal = normalize(surfaceNormal);
        vec3 unit_toLightDir = normalize(toLightDir);

        float relativeDir = dot(unit_normal, unit_toLightDir);
        float brightness = max(relativeDir, 0.005);
        vec3 diffuse_factor = brightness * lightColor;

        vec3 unit_cameraDir = normalize(cameraDir);
        vec3 lightDir = -unit_toLightDir;

        vec3 lightReflectDir = reflect(lightDir, unit_normal);
        float specular_factor = dot(lightReflectDir, unit_cameraDir);
        specular_factor = max(specular_factor, 0.0);

        float damper_factor = pow(specular_factor, shineDamper);
        vec3 result_specular = damper_factor * reflectivity * lightColor;

        vec4 texColor = texture(diffuse, pass_texCoordinates);
        if(texColor.a < 0.5)
        {
                discard;
        }
        output_color = vec4(diffuse_factor, 1.0) * texColor + vec4(result_specular, 1.0);


}
