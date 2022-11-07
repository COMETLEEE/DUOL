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

void ComputeDirectionalLight(Material mat, DirectionalLight L, // ���Ɽ �Լ� // �Ÿ��� ������� �����ϰ� �־����� ��. �¾�
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// �ӽ÷� �ʱ�ȭ ���ѳ���..!
    mat.Specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
    mat.Ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
    mat.Diffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);


    L.Specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
    L.Ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
    L.Diffuse = float4(0.6f, 0.6f, 0.6f, 1.0f);
    L.Direction = float3(-30.f, -20.f, -40.f);

    L.Direction = normalize(L.Direction);


	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f); // �ֺ���
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f); // �л걤
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f); // �ݾȱ�


	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;

	//  ������(*) �� ���к��� ���ϴ� ��. ������ ������ �ƴ�. ���� DirectionalLight�� Ambient ��.
    ambient = L.Ambient * mat.Ambient;

	// (N dot L) ������ ��ĥ��� ���� ������.
    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f) // ���� ����� 0���� ũ�� ������ �Ѵ�.
    {
        float3 v = reflect(-lightVec, normal); // �ݻ纤�͸� ��ȯ�ϴ� �Լ�. Light�� ������ ������ �ݻ縦 ��ȯ��.
        float specFactor = pow(max(dot(v, toEye), 0.0f), 4.0f); // �ݻ� ���Ϳ� �þ߸� �����ϰ� ������ specular.w�� ��ŭ ���� specular.w���� Ŀ������ ������ ũ�Ⱑ �پ��. ���� ��¦��.

        diffuse = diffuseFactor * L.Diffuse * mat.Diffuse; //��ǻ� ��� 
        spec = specFactor * L.Specular * mat.Specular; //����ŧ���� ��� 
    }

}

//��(Point)���� ����
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// ������ �ʱ�ȭ
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);


	// ������ ��ü�� ���� ����.
    float3 lightVec = L.Position - pos;

	// �Ÿ�
    float d = length(lightVec);

	// �Ÿ��� �ָ� ����
    if (d > L.Range)
        return;

	// light���� ��ֶ�����.
    lightVec /= d;

	// �ֺ��� ���. (��𼭳� ���� ���̶� ����Ʈ�� ���� x)
    ambient = mat.Ambient * L.Ambient;

	// (N dot L) ������ ��ĥ��� ���� ������.
    float diffuseFactor = dot(lightVec, normal);
	// ���ǹ�
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal); // �ݻ� ���� Directionallight�� ����.
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;

		// Attenuate �Ÿ��� �ָ� ���� ���������� �ϱ�����.
        float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
        diffuse *= att;
        spec *= att;
    }
}

// SpotLight ������. ������.
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



	// ������ ũ�⸦ ����.
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
 