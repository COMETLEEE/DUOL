//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "PBRHelper.hlsli"

#define MAX_LIGHT_CNT 30

struct Light // 1 + 1 + 1 + 1 + 4 = 8 ... 8 * 30 = 240
{
    uint Type;
    float3 Direction;
    
    float3 Position;
    float Range;
    
    float3 Color;
    float Intensity;
  
    float Attenuation;
    float AttenuationRadius;
    float2 pad;

};

struct CascadeShadow
{
    matrix Shadow[4]; // 4 * 4 
    float4 CascadeOffset;
};

float4 ComputePBRDirectionalLight
	(in Light DL
    , in float3 specularColor
	, in float3 albedoColor
	, in float3 normal
    , in float3 eyeVec
    , in float roughness
    , in float metallic)
{
    
    float3 light = normalize(-DL.Direction);
    
    float3 halfVec = normalize(eyeVec + light);
    float NdotL = saturate(dot(normal, light));
    
    //test
    //float fNDotL = NdotL * 0.5f + 0.5f;
    //ceil
    {
    // float fBandNum = 3.0f;
    // float fBandedNDotL = ceil(NdotL * fBandNum) / fBandNum;
    
    // float NdotH = saturate(dot(normal, halfVec));
    // float powedNdotH = pow(NdotH, 300);
    
    // float smoothstepNdotH = smoothstep(0.005f, 0.01f, powedNdotH);
    
    // float NdotV = saturate(dot(normal, eyeVec));
    // float LdotH = saturate(dot(light, halfVec));
    
    // float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, fBandedNDotL, LdotH, smoothstepNdotH);
       
    // float3 radiance = CalcIluminance(DL.Intensity) * DL.Color * fBandedNDotL;

    // float4 litColor = float4(retColor * radiance, 1.f);
    // return litColor;
    }
    //normal
        
        {
        float NdotH = saturate(dot(normal, halfVec));
        float NdotV = saturate(dot(normal, eyeVec));
        float LdotH = saturate(dot(light, halfVec));
    
        float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
       
        float3 radiance = CalcIluminance(DL.Intensity) * DL.Color * NdotL;
  
        float4 litColor = float4(retColor * radiance, 1.f);
        return litColor;
    }

}


float4 ComputePBRSpotLight
	(in Light SL
    , in float3 specularColor
	, in float3 albedoColor
	, in float3 normal
    , in float3 eyeVec
    , in float roughness
    , in float metallic
    , in float3 position)
{
    float4 litColor = { 0.f, 0.f, 0.f, 0.f };
    
    float3 PtoL = SL.Position - position;
    float3 light = normalize(PtoL);
    float dist = length(PtoL);
    float lightDirection = normalize(SL.Direction);
    
    //for SL Iluminance
    float DdotL = dot(lightDirection, -light);
    
    if (DdotL < SL.Range)
        return litColor;
    
    float3 halfVec = normalize(eyeVec + light);
    float NdotL = saturate(dot(normal, light));

    //ceil    
    // {
    // float fBandNum = 3.0f;
    // float fBandedNDotL = ceil(NdotL * fBandNum) / fBandNum;
    
    // float NdotH = saturate(dot(normal, halfVec));
    // float powedNdotH = pow(NdotH, 300);
    
    // float smoothstepNdotH = smoothstep(0.005f, 0.01f, powedNdotH);
    
    // float NdotV = saturate(dot(normal, eyeVec));
    // float LdotH = saturate(dot(light, halfVec));
    
    // float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, fBandedNDotL, LdotH, smoothstepNdotH);
       
    // float3 radiance = CalcIluminance(SL.Intensity) * SL.Color * fBandedNDotL;
    // float powdist = pow(dist, 2);
    //   //distance Attenuation
    // float distanceAttenuation = 1.0f / (powdist * +1);
    // float radiusMask = saturate(1 - (powdist / pow(SL.AttenuationRadius, 2)));
    
    // litColor = float4(retColor * radiance, 1.f);
    // litColor *= DdotL * radiusMask * distanceAttenuation;
    
    // return litColor;
    // }

    {
    
        float NdotH = saturate(dot(normal, halfVec));
    
        float NdotV = saturate(dot(normal, eyeVec));
        float LdotH = saturate(dot(light, halfVec));
    
        float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
       
        float3 radiance = CalcIluminance(SL.Intensity) * SL.Color * NdotL;
        float powdist = pow(dist, 2);
      //distance Attenuation
        float distanceAttenuation = 1.0f / (powdist * +1);
        float radiusMask = saturate(1 - (powdist / pow(SL.AttenuationRadius, 2)));
    
        litColor = float4(retColor * radiance, 1.f);
        litColor *= DdotL * radiusMask * distanceAttenuation;
    
        return litColor;
    }

}

float4 ComputePBRPointLight
	(in Light PL
    , in float3 specularColor
	, in float3 albedoColor
	, in float3 normal
    , in float3 eyeVec
    , in float roughness
    , in float metallic
    , in float3 position)
{
    float3 lightVector = PL.Position - position;
    
    float3 light = normalize(lightVector);
    float dist = length(lightVector);
    
    float3 halfVec = normalize(eyeVec + light);
    float NdotL = saturate(dot(normal, light));
    

    //Normal
        {
        float NdotH = saturate(dot(normal, halfVec));
    
        float NdotV = saturate(dot(normal, eyeVec));
        float LdotH = saturate(dot(light, halfVec));
    
        float3 retColor = CookTorrance_GGX(roughness, metallic, specularColor, albedoColor, NdotV, NdotL, LdotH, NdotH);
    
    //why divide 4 : light radiance iluminance divide PI
        float3 radiance = CalcIluminance(PL.Intensity) / 4.f * PL.Color * NdotL;
  
        float powdist = pow(dist, 2);
    
    //distance Attenuation
        float distanceAttenuation = 1.0f / (powdist * +1);
        float radiusMask = saturate(1 - (powdist / pow(PL.AttenuationRadius, 2)));
    
        float4 litColor = float4(retColor * radiance, 1.f);
        litColor *= radiusMask * distanceAttenuation;
    
        return litColor;
    }
}

float4 ComputeLegacyDirectionalLight
	(in Light DL
	, in float3 albedoColor
	, in float3 normal
    , in float3 eyeVec)
{
    float4 litColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    float4 Diffuse = float4(0.4f, 0.4f, 0.4f, 1.0f);
    float4 Specular = float4(0.4f, 0.4f, 0.4f, 1.0f);

	//���⼭ ���� ����Ʈ���ʹ� ������ ������ ���⺤�ͱ⋚���� -�� ���δ�.
    float3 lightVec = normalize(-DL.Direction);

	//���׸���� ����Ʈ�� �ں��Ʈ�� ���Ѵ�.
	//���׸����� ambient�� ���� �󸶳� ����ϴ���.
	//����Ʈ�� ambient�� ���� �󸶳� ������ ǥ���Ѵ�.

	//��ֺ��Ϳ� ����Ʈ���� ���� ������ ���� Diffuse�� ��ġ�� ���Ѵ�.
	//0���� ũ�� ���� ������ ����?(���� �����ʴ� ��!)
    float DiffuseFactor = dot(lightVec, normal);

    
    //DiffuseFactor = smoothstep(0, 0.3f, DiffuseFactor);
	
	// flatten -> if���� ������ �б����� �̸� ����ϰ�
	// x�� ���� ���� �����Ѵ�.
	// �̰� �� �� ȿ���������� �𸣰ڴ�.
	[flatten]
    if (DiffuseFactor > 0.0f)
    {
		//�ݻ纤�͸� ���Ѵ�. 
        float3 v = reflect(DL.Direction, normal);
		//�ݻ纤�Ϳ� ������ ������ ���Ͽ� ���� �ݻ纤���� ���� ���� �������� �Ǵ��Ѵ�.
		//�׸��� ����ŧ���� ������?�� ���� ����.
        float SpecFactor = pow(max(dot(v, eyeVec), 0.0f), 35);

		//Diffuse�� Specular�� ���� �Բ� ���׸����� ������ ���� ���⿡����..
        litColor += float4(DiffuseFactor * albedoColor + SpecFactor * Specular.xyz, 1.f);
    }
    
    return litColor;
}

float2 ViewPosToUV(float2 posXY)
{
    float2 uv = posXY + 1.f;

    uv /= 2.0f;
    uv.y = 1.f - uv.y;

    return uv;
}

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);
    
    return bumpedNormalW;

}
 