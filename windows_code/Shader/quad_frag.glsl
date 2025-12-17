#version 330 core

in vec3 fragPosition;
in vec4 fragColor;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform mat4 mvp;
uniform sampler2D texture0;
uniform vec3 cameraPosition;
uniform vec3 cameraDirection;
uniform int drawFlag;

out vec4 finalColor;

void main()
{
	vec4 textureColor = texture(texture0, fragTexCoord);
    
    
	finalColor = textureColor;
	finalColor*= fragColor;
    
    if(drawFlag == 1)
    {
        vec3 directionToCamera = fragPosition - cameraPosition;
        directionToCamera = normalize(directionToCamera);
        
        finalColor.xyz *= dot(directionToCamera , -fragNormal);
    }
    
}