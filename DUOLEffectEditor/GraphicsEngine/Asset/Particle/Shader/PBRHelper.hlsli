#define PI 3.14159265359f
#define e 2.71828182846f
#define Epsilon 0.00001f
static const float3 Fdielectric = 0.04f;

float3 CalcIluminance(float lumen)
{
    return lumen / PI;
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
    float3 ret = DiffuseColor * (1 / PI);
    return ret;
}

float3 fresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    float3 r =(float3)1.f - roughness;
    return F0 + (max(r, F0) - F0) * pow(clamp(1.f - cosTheta, 0.f, 1.f), 5.f);
}

float3 FresnelSchlick(float3 f0, float cosThetaVH)
{
    float3 ret = f0 + (1.0 - f0) * pow(1.0 - cosThetaVH, 5.0f);

    return ret;
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

float GeometrySchlickGGX(float roughness, float NdotV)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float G_Shlick_Smith_Hable(float alpha, float LdotH)
{
    return rcp(lerp(LdotH * LdotH, 1, alpha * alpha * 0.25f));
}


float GGX_Distribution(float cosThetaNH, float alpha)
{
    float alphaSq = alpha * alpha;

    float denom = (cosThetaNH * cosThetaNH) * (alphaSq - 1.0) + 1.0;
    
    return alphaSq / max(Epsilon, (PI * denom * denom));
}

float3 CookTorrance_GGX(
    in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 albedoColor
	, in float NdotV
	, in float NdotL
	, in float LdotH
	, in float NdotH
    )
{
    float alpha = roughness * roughness;
    
    float3 F = FresnelSchlick(specColor, LdotH);
    float D = GGX_Distribution(NdotH, alpha);
    float G = G_Shlick_Smith_Hable(alpha, LdotH);
    //float G = GeometrySchlickGGX(roughness, NdotV);

    float3 specK = F * G * D;
    
    float3 diffK = albedoColor * NdotL;
   
    return specK + diffK;
}