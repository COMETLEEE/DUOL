//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Direction;
    float Spot;

    float3 Att;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

void ComputeDirectionalLight(Material mat, DirectionalLight L, // 지향광 함수 // 거리에 상관없이 일정하게 주어지는 빛. 태양
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// 임시로 초기화 시켜놓자..!
    mat.Specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
    mat.Ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
    mat.Diffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);


    L.Specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
    L.Ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
    L.Diffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);
    L.Direction = float3(-30.f, -20.f, -40.f);

    L.Direction = normalize(L.Direction);


	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f); // 주변광
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f); // 분산광
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f); // 반안광


	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;

	//  연산자(*) 은 성분별로 곱하는 것. 벡터의 곱셈이 아님. 대상과 DirectionalLight의 Ambient 곱.
    ambient = L.Ambient * mat.Ambient;

	// (N dot L) 재질의 거칠기는 빼고 연산중.
    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f) // 내적 결과가 0보다 크면 연산을 한다.
    {
        float3 v = reflect(-lightVec, normal); // 반사벡터를 반환하는 함수. Light가 들어오는 방향의 반사를 반환함.
        float specFactor = pow(max(dot(v, toEye), 0.0f), 4.0f); // 반사 벡터와 시야를 내적하고 재질의 specular.w값 만큼 제곱 specular.w값이 커질수록 원뿔의 크기가 줄어듬. 더욱 반짝임.

        diffuse = diffuseFactor * L.Diffuse * mat.Diffuse; //디퓨즈값 계산 
        spec = specFactor * L.Specular * mat.Specular; //스펙큘러값 계산 
    }

}

//점(Point)광을 구현
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// 변수들 초기화
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// 점광과 물체의 벡터 뺄셈.
    float3 lightVec = L.Position - pos;

	// 거리
    float d = length(lightVec);

	// 거리가 멀면 리턴
    if (d > L.Range)
        return;

	// light벡터 노멀라이즈.
    lightVec /= d;

	// 주변광 계산. (어디서나 같은 값이라서 라이트는 관여 x)
    ambient = mat.Ambient * L.Ambient;

	// (N dot L) 재질의 거칠기는 빼고 연산중.
    float diffuseFactor = dot(lightVec, normal);
	// 조건문
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal); // 반사 벡터 Directionallight와 동일.
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;

		// Attenuate 거리가 멀면 빛이 약해지도록 하기위함.
        float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
        diffuse *= att;
        spec *= att;
    }
}

// SpotLight 손전등. 점적광.
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// The vector from the surface to the light.
    float3 lightVec = L.Position - pos;

	// The distance from surface to light.
    float d = length(lightVec);

	// Range test.
    if (d > L.Range)
        return;

	// Normalize the light vector.
    lightVec /= d;

	// Ambient term.
    ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }



	// 원뿔의 크기를 지정.
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
    float att = spot / dot(L.Att, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;

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
 