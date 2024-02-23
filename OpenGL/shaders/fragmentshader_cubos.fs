#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 FragColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() 
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);  // Dirección de la luz
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 lighting = light.ambient + light.diffuse * diff + light.specular * spec;
    FragColor = texture(texture2, TexCoord);
    //FragColor = vec4(lighting, 1.0) * texture(ourTexture, TexCoords);
}