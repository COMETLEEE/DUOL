struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Reflect;
};
struct Camera
{
    matrix g_ViewMatrix;
    matrix g_ProjectionMatix;
    matrix g_ViewProjectionMatrix;
    matrix g_ViewProjectionInverseTransposeMatrix;
};


struct Transfrom
{
    matrix g_World;
    matrix g_WorldInvTranspose;
};

struct DirectionalLight //5
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
	//패딩을 하는 이유. hlsl은 4차원 벡터에 채워넣는 구조를 띈다.
	//채워 넣되, 두개의 4차원 벡터 사이에 걸쳐 나누어지면 안된다.
    float pad;
	
    matrix ViewProjectionMatrix;
};
struct PointLight //5
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
	
    float3 Position;
    float Range;

	//거리에 따른 감쇠를 위한
    float3 Att;
    float pad;
	
    matrix ViewProjectionMatrix;
};
struct SpotLight //6
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

	//스팟라이트의 점적광 원뿔을 제어하는데 쓰이는 지수.
    float3 Direction;
    float Spot;

	//거리에 따른 감쇠를 위한
    float3 Att;
    float pad;
	
    matrix ViewProjectionMatrix;
};

struct Light
{
    uint DirectionalLightCnt;
    uint PointLightCnt;
    uint SpotLightCnt;
    uint pad;
    
    DirectionalLight g_DirLight[3];
    PointLight g_PointLight[10];
    SpotLight g_SpotLight[10];
};


//Directional
void ComputeDirectionalLight
	(Material mat, DirectionalLight DL,
	float3 normal, float3 eyeVec,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular)
{
    Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//여기서 쓰일 라이트벡터는 점에서 나오는 방향벡터기떄문에 -를 붙인다.
    float3 lightVec = -DL.Direction;

	//메테리얼과 라이트의 앰비언트를 곱한다.
	//메테리얼의 ambient는 빛을 얼마나 흡수하는지.
	//라이트의 ambient는 빛이 얼마나 센지를 표현한다.
    Ambient = mat.Ambient * DL.Ambient;

	//노멀벡터와 라이트벡터 간의 내적을 통해 Diffuse의 수치를 구한다.
	//0보다 크면 빛과 방향이 같은?(빛이 닿지않는 면!)
    float DiffuseFactor = dot(lightVec, normal);

    DiffuseFactor = smoothstep(0, 0.3f, DiffuseFactor);
	
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
        float SpecFactor = pow(max(dot(v, eyeVec), 0.0f), 0.1);

		//Diffuse와 Specular도 값과 함께 메테리얼의 정보와 빛의 세기에따라..
        Diffuse = DiffuseFactor * mat.Diffuse * DL.Diffuse;
        Specular = SpecFactor * mat.Specular * DL.Specular;
    }
}

//Directional
void ComputePointLight
	(Material mat, PointLight PL,
	float3 pos, float3 normal, float3 eyeVec,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular)
{
    Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//point light는 사방으로 퍼지는 빛.
	//물체(점)의 위치와 빛의 위치에 따라 벡터가 다르다!
    float3 lightVec = PL.Position - pos;

	//점과 광원사이의 거리에 따라 적용이 달라진다
    float d = length(lightVec);

	//d가 빛의 범위를 벗어났다면 적용을 시켜주지 않아용.
    if (d > PL.Range)
    {
        return;
    }
	
    lightVec /= d;
	
	//주변광
    Ambient = mat.Ambient * PL.Ambient;

	//빛이 막히지않고 도달한다는 가정하에 diffuse를 dot

    float DiffuseFactor = dot(lightVec, normal);

	[flatten]
    if (DiffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, eyeVec), 0.0f), mat.Specular.w);

        Diffuse = DiffuseFactor * mat.Diffuse * PL.Diffuse;
        Specular = specFactor * mat.Specular * PL.Specular;
    }

	//감쇠식 왜이렇게 구하는걸까
    float att = 1.0f / dot(PL.Att, float3(1.0f, d, d * d));

	//아무튼 거리에 따른 감쇠를 해준다.
    Diffuse *= att;
    Specular *= att;
}

void ComputeSpotLight
	(Material mat, SpotLight SL,
	float3 pos, float3 normal, float3 eyeVec,
	out float4 Ambient,
	out float4 Diffuse,
	out float4 Specular)
{
 //   Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
 //   Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
 //   Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
 //   float3 lightVec = SL.Position - pos;
	
 //   float d = length(lightVec);
	
	////d가 SL의 범위보다 클때는 적용하지 않는다.
	//if(d > SL.Range)
 //       return;
	
 //   lightVec /= d;
	
 //   Ambient = mat.Ambient * SL.Ambient;
	
 //   float diffuseFactor = dot(lightVec, normal);
	
	//[flatten] if (diffuseFactor > 0.0f)
	//{
 //       float3 v = reflect(-lightVec, normal);
 //       float specFactor = pow(max(dot(v, eyeVec), 0), mat.Specular.w);
		
 //       Diffuse = diffuseFactor * mat.Diffuse * SL.Diffuse;
 //       Specular = specFactor * mat.Specular * SL.Specular;
 //   }
	
	////빛의 감쇠
 //   float spot = pow(max(dot(-lightVec, SL.Direction), 0.0f), SL.Spot);
	
 //   float att = spot / dot(SL.Att, float3(1.0f, d, d * d));
	
 //   Ambient *= spot;
 //   Diffuse *= att;
 //   Specular *= att;
	
// Initialize outputs.
    Ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    Specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = SL.Position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if (d > SL.Range)
        return;
		
	// Normalize the light vector.
    lightVec /= d;
	
	// Ambient term.
    Ambient = mat.Ambient * SL.Ambient;

	// Add diffuse and speculaSSSS term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, eyeVec), 0.0f), mat.Specular.w);
					
        Diffuse = diffuseFactor * mat.Diffuse * SL.Diffuse;
        Specular = specFactor * mat.Specular * SL.Specular;
    }
	
	// Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, SL.Direction), 0.0f), SL.Spot);

	// Scale by spotlight factor and attenuate.
    float att = spot / dot(SL.Att, float3(1.0f, d, d * d));

    Ambient *= spot;
    Diffuse *= att;
    Specular *= att;
}

float CalcShadowFactor(uint mapSize, SamplerComparisonState samShadow, Texture2D shadowMap, float4 shadowH)
{
    shadowH.xyz /= shadowH.w;
    float depth = shadowH.z;
	
    const float dx = 1 / mapSize;
	
    float shadowFactor = 0.0f;
	
    const float2 offsets[9] =
    {
        float2(-dx, -dx), float2(0, -dx), float2(+dx, -dx),
        float2(-dx, 0), float2(0, 0), float2(+dx, 0),
        float2(-dx, +dx), float2(0, -dx), float2(+dx, +dx)
    };
    
    [unroll]
    for (int i = 0; i < 9; i++)
    {
        shadowFactor += shadowMap.SampleCmpLevelZero(samShadow, shadowH.xy * offsets[i], depth).r;
    }
	
    return shadowFactor /= 9.0f;
}