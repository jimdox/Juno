#version 450 core
in vec2 pass_texCoordinates;
in vec3 surfaceNormal;
in vec3 toLightDir[4];
in vec3 cameraDir;

out vec4 output_color; 

uniform sampler2D diffuse;
uniform vec3 lightColor[4];
uniform vec3 attenuations[4];
uniform float reflectivity;
uniform float shineDamper;


void main()
{
        vec3 unit_normal = normalize(surfaceNormal);
        vec3 unit_cameraDir = normalize(cameraDir);

        vec3 net_diffuse = vec3(0.0, 0.0, 0.0);
        vec3 net_specular = vec3(0.0, 0.0, 0.0);

        vec3 test_att = vec3(1.0, 0.0009, 0.0001);

        for(int i=0; i<4; i++)
        {
                float distToSrc = length(toLightDir[i]);
                //float att_factor = attenuations[i].x + (attenuations[i].y * distToSrc) + (attenuations[i].z * distToSrc * distToSrc);
                float att_factor = test_att.x + (test_att.y * distToSrc) + (test_att.z * distToSrc * distToSrc);
                vec3 unit_toLightDir = normalize(toLightDir[i]);
                float relativeDir = dot(unit_normal, unit_toLightDir);
                float brightness = max(relativeDir, 0.0);

                vec3 lightDir = -unit_toLightDir;
                vec3 lightReflectDir = reflect(lightDir, unit_normal);
                
                
                float specular_factor = dot(lightReflectDir, unit_cameraDir);
                specular_factor = max(specular_factor, 0.0);
                float damper_factor = pow(specular_factor, shineDamper);
                
                net_diffuse = net_diffuse + (brightness * lightColor[i]);
                net_specular = net_specular + (damper_factor * reflectivity * lightColor[i])/att_factor;
        }

        net_diffuse = max(net_diffuse, 0.05);


        vec4 texColor = texture(diffuse, pass_texCoordinates);
        if(texColor.a < 0.5)
        {
                discard;
        }
        output_color = vec4(net_diffuse, 1.0) * texColor + vec4(net_specular, 1.0);


}
