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
	//�е��� �ϴ� ����. hlsl�� 4���� ���Ϳ� ä���ִ� ������ ���.
	//ä�� �ֵ�, �ΰ��� 4���� ���� ���̿� ���� ���������� �ȵȴ�.
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

	//�Ÿ��� ���� ���踦 ����
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

	//���̶���Ʈ�� ������ ������ �����ϴµ� ���̴� ����.
    float3 Direction;
    float Spot;

	//�Ÿ��� ���� ���踦 ����
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

	//���⼭ ���� ����Ʈ���ʹ� ������ ������ ���⺤�ͱ⋚���� -�� ���δ�.
    float3 lightVec = -DL.Direction;

	//���׸���� ����Ʈ�� �ں��Ʈ�� ���Ѵ�.
	//���׸����� ambient�� ���� �󸶳� ����ϴ���.
	//����Ʈ�� ambient�� ���� �󸶳� ������ ǥ���Ѵ�.
    Ambient = mat.Ambient * DL.Ambient;

	//��ֺ��Ϳ� ����Ʈ���� ���� ������ ���� Diffuse�� ��ġ�� ���Ѵ�.
	//0���� ũ�� ���� ������ ����?(���� �����ʴ� ��!)
    float DiffuseFactor = dot(lightVec, normal);

    DiffuseFactor = smoothstep(0, 0.3f, DiffuseFactor);
	
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
        float SpecFactor = pow(max(dot(v, eyeVec), 0.0f), 0.1);

		//Diffuse�� Specular�� ���� �Բ� ���׸����� ������ ���� ���⿡����..
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

	//point light�� ������� ������ ��.
	//��ü(��)�� ��ġ�� ���� ��ġ�� ���� ���Ͱ� �ٸ���!
    float3 lightVec = PL.Position - pos;

	//���� ���������� �Ÿ��� ���� ������ �޶�����
    float d = length(lightVec);

	//d�� ���� ������ ����ٸ� ������ �������� �ʾƿ�.
    if (d > PL.Range)
    {
        return;
    }
	
    lightVec /= d;
	
	//�ֺ���
    Ambient = mat.Ambient * PL.Ambient;

	//���� �������ʰ� �����Ѵٴ� �����Ͽ� diffuse�� dot

    float DiffuseFactor = dot(lightVec, normal);

	[flatten]
    if (DiffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, eyeVec), 0.0f), mat.Specular.w);

        Diffuse = DiffuseFactor * mat.Diffuse * PL.Diffuse;
        Specular = specFactor * mat.Specular * PL.Specular;
    }

	//����� ���̷��� ���ϴ°ɱ�
    float att = 1.0f / dot(PL.Att, float3(1.0f, d, d * d));

	//�ƹ�ư �Ÿ��� ���� ���踦 ���ش�.
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
	
	////d�� SL�� �������� Ŭ���� �������� �ʴ´�.
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
	
	////���� ����
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