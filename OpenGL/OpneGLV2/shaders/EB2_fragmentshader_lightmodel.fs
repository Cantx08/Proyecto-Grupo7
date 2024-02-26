#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool isStar;

void main()
{    
    if(isStar){
        FragColor = texture(texture_diffuse1, TexCoords);
    }else{
        FragColor = vec4(1.0); // set alle 4 vector values to 1.0
    }
}