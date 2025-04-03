
Texture2D texDiffuse : register(t0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
    float3 WorldPos : WORLD_POSTITION;
    float3 WorldNormal : WORLD_NORMAL;
};

// cbuffers for lab 2
cbuffer PointLightBuffer : register(b0)
{
    float4 LightPos;
    float4 CameraPos;
};

cbuffer PhongComponentsBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------


float4 PS_main(PSIn input) : SV_Target
{
	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
    
    //return float4(input.Normal * 0.5 + 0.5, 1);
    
	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
	// return float4(input.TexCoord, 0, 1);
	
	// Phong Shader for lab 2
    
    float3 lightVector = LightPos.xyz - input.WorldPos;
    float3 reflectedLight = normalize(reflect(lightVector, input.WorldNormal));
    float3 cameraView = normalize(CameraPos.xyz - input.WorldPos);
    
    return float4(Ambient.xyz + max(Diffuse.xyz * (lightVector * input.Normal), 0) + max(pow(max((Specular.xyz * (reflectedLight * cameraView)), 0), Specular.w), 0), 1);
    
    
    //float diffuseIntensity = max(0, dot(input.WorldNormal, lightNormal));
    
    //float3 cameraNormal = normalize(CameraPos.xyz - input.WorldPos);
    //float specularIntensity = pow(max(0, dot(reflectedRay, cameraNormal)), Specular.w);

    //return float4(Ambient + Diffuse * diffuseIntensity + Specular * specularIntensity);
}