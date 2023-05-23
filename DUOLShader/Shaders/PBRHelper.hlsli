#define PBR_PI 3.14159265359f
#define PBR_INV_PI 1.f / 3.14159265359f
#define e 2.71828182846f
#define Epsilon 0.0000001f
static const float3 Fdielectric = 0.04f;

float3 CalcIluminance(float lumen)
{
    return lumen / PBR_PI;
}

float3 Diffuse_Lambert(float3 DiffuseColor)
{
    float3 ret = DiffuseColor * (1 / PBR_PI);
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

    float denom = (cosThetaNH * cosThetaNH * (alphaSq - 1.0)) + 1.0f;
    
    return alphaSq / max(Epsilon, (PBR_PI * denom * denom));
}

float3 CookTorrance_GGX(
    in float roughness
	, in float metallic
	, in float3 specColor
	, in float3 albedoColor
	, in float NdotV
	, in float NdotL
	, in float VdotH
	, in float NdotH
    )
{
    float alpha = roughness * roughness;
    
    float3 F = FresnelSchlick(specColor, VdotH);
    float D = GGX_Distribution(NdotH, alpha);
    float G = G_Shlick_Smith_Hable(alpha, VdotH);

    float3 specK = F * G * D;
    
    float3 diffK = albedoColor ;
   
    return  specK + diffK;
}

///////////////////////////////////unreal//////////////////////////////////////////////////

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0f - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0f;
	float k = (r * r) / 8.0f; // EPBR_PIc suggests using this roughness remapPBR_PIng for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0f) + 1.0f;

	return alphaSq / max(PBR_PI * denom * denom, Epsilon);
}


float3 Unreal_PBR(
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
    float3 F = fresnelSchlick(specColor, max(0.0f, LdotH));
    float D = ndfGGX(NdotH, roughness);
    float G = gaSchlickGGX(NdotL, NdotV, roughness);

    //Epsilon을 넣어도 제대로 max값이 들어오지 않는것같다.. why..
    float3 specK = (F * D * G) / max(0.0000001f, 4.0 * NdotL * NdotV);
    //float3 specK = (F) / max(Epsilon, 4.0 * NdotL * NdotV);
    
    float3 diffK = albedoColor / PBR_PI;
   
    return diffK + specK;
}