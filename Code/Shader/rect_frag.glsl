#version 330 core

in vec3 fragPosition;
in vec4 fragColor;
in vec2 fragTexCoord;

flat in vec4 fragRotation;
flat in vec3 centrePosition;
flat in vec2 rectSize;
flat in vec4 rectRadius;

out vec4 finalColor;

uniform sampler2D texture0;

vec3 quat_transform( vec4 q, vec3 v )
{
    return v + 2.*cross( q.xyz, cross( q.xyz, v ) + q.w*v ); 
}

vec4 QuaternionInvert(vec4 q)
{
    vec4 result = q;

    float lengthSq = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;

    if (lengthSq != 0.0f)
    {
        float invLength = 1.0f/lengthSq;

        result.x *= -invLength;
        result.y *= -invLength;
        result.z *= -invLength;
        result.w *= invLength;
    }

    return result;
}


float sdRoundedBox(in vec2 p, in vec2 b, in vec4 r )
{	
	r.xy = (p.x > 0.0) ? r.xy : r.zw;
	r.x = (p.y > 0.0) ? r.x : r.y;
	
    vec2 q = abs(p) - b + r.x;

	return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r.x;
}
	
void main()
{

	vec3 PointPosition = (fragPosition.xyz - centrePosition.xyz);
	PointPosition = quat_transform(QuaternionInvert(fragRotation ), PointPosition);

	//why is rectangle size got to be half???
	float SDF_Distance = sdRoundedBox(PointPosition.xy, rectSize / 2, rectRadius );

	vec4 TextureColor = texture(texture0, fragTexCoord);

	finalColor= fragColor;

	finalColor*= TextureColor;

	float WhiteBorder = 0;
	SDF_Distance += WhiteBorder;
	
	//move the blur to vertex buffer
	float blur = 0.003f;
	float alpha = smoothstep(blur , -blur, SDF_Distance);

	finalColor.a *= alpha;
	//finalColor.a = 1;
	//finalColor = vec4(1/distance(rectSize/2 , PointPosition),0,0,1.0);

}
