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
        vec3 frag_to_camera = fragPosition - cameraPosition;
        float distance_to_camera = length(frag_to_camera);
        
        frag_to_camera = normalize(frag_to_camera);
        
        distance_to_camera = (1.0 / distance_to_camera) * 10;
        distance_to_camera = min(1.2 , distance_to_camera);
        
        finalColor.xyz *= dot(frag_to_camera , -fragNormal) * distance_to_camera;
    }
}