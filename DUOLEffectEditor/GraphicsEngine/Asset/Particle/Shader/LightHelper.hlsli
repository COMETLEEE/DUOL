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

	//여기서 쓰일 라이트벡터는 점에서 나오는 방향벡터기떄문에 -를 붙인다.
    float3 lightVec = normalize(-DL.Direction);

	//메테리얼과 라이트의 앰비언트를 곱한다.
	//메테리얼의 ambient는 빛을 얼마나 흡수하는지.
	//라이트의 ambient는 빛이 얼마나 센지를 표현한다.

	//노멀벡터와 라이트벡터 간의 내적을 통해 Diffuse의 수치를 구한다.
	//0보다 크면 빛과 방향이 같은?(빛이 닿지않는 면!)
    float DiffuseFactor = dot(lightVec, normal);

    
    //DiffuseFactor = smoothstep(0, 0.3f, DiffuseFactor);
	
	// flatten -> if문의 나뉘는 분기점을 미리 계산하고
	// x의 값에 따라 선택한다.
	// 이게 왜 더 효율적인지는 모르겠다.
	[flatten]
    if (DiffuseFactor > 0.0f)
    {
		//반사벡터를 구한다. 
        float3 v = reflect(DL.Direction, normal);
		//반사벡터와 눈과의 내적을 통하여 눈에 반사벡터의 빛이 들어올 각도인지 판단한다.
		//그리고 스페큘라의 보정값?에 따라 제곱.
        float SpecFactor = pow(max(dot(v, eyeVec), 0.0f), 35);

		//Diffuse와 Specular도 값과 함께 메테리얼의 정보와 빛의 세기에따라..
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
 