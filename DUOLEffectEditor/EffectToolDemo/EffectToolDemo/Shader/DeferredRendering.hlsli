
#include "ConstantBuffer.hlsli"

#define NUM_MOTIONBLUR_SAMPLES     8



Texture2D gDiffuseMap : register(t0); // ����ϱ� ����

Texture2D gDepthBuffer : register(t1); // ���۵� ������ ���� ���۵�.
Texture2D gNormalBuffer : register(t2);
Texture2D gPositionBuffer : register(t3);
Texture2D gAlbedoBuffer : register(t4);
Texture2D gMatDiffuse : register(t5);
Texture2D gMatSpecular : register(t6);
Texture2D gMatAmbient : register(t7);

SamplerState samAnisotropic : register(s0);
//{
//    Filter = ANISOTROPIC;
//    MaxAnisotropy = 4;

//    AddressU = WRAP;
//    AddressV = WRAP;
//};

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

// ���ؽ� ���̴��� ������ �Լ�
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output;

    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
    return output;
}

// �ؽ�ó ����� �ϴ� ���̴�
float4 PS_TextureRender(VS_OUT input) : SV_Target
{
    return gDiffuseMap.Sample(samAnisotropic, input.uv);
}

// ����Ʈ ���μ���
float4 PS_PostProcess(VS_OUT input) : SV_Target
{
    //float4 _Pos = gPositionBuffer.Sample(samAnisotropic, input.uv);
    //float4 _Normal = gNormalBuffer.Sample(samAnisotropic, input.uv);
    //// ��Ǻ�!
    //float4 color = 0;

    //// �ؽ�ó��ǥ�� ������ǥ�� ����
    //float4 Proj_Curr = mul(float4(_Pos.xyz, 1), gCurrentViewProj);
    //// ���̰��� ������� ���� World��ǥ ���ϱ�

    //float4 Proj_Prev = mul(float4(_Pos.xyz, 1), gPrevViewProj);
    //Proj_Prev /= Proj_Prev.w;
    //Proj_Curr /= Proj_Curr.w;
    //
    //// ���� ������ǥ�� ���� ������ǥ�� ������� �ӵ����� ���ϱ�
    //float f = 1.0f / NUM_MOTIONBLUR_SAMPLES;
    //float2 velocity = float2(Proj_Prev.x - Proj_Curr.x, Proj_Curr.y - Proj_Prev.y) / 2 * f;
    //
    //// �� ���� Blur Time!
    //for (int j = 0; j < NUM_MOTIONBLUR_SAMPLES; j++)
    //{
    //    float4 currentColor = gDiffuseMap.Sample(samSEX, input.uv + velocity.xy * j);
    //    color += currentColor;
    //}
    //color = color / NUM_MOTIONBLUR_SAMPLES;

    float4 color = gDiffuseMap.Sample(samAnisotropic, input.uv);

    return color;
}

// ���۵� ����.
float4 PS_DeferredRender(VS_OUT input) : SV_Target
{
    
    Material _Mat;
    _Mat.Diffuse = gMatDiffuse.Sample(samAnisotropic, input.uv);
    _Mat.Specular = gMatSpecular.Sample(samAnisotropic, input.uv);
    _Mat.Ambient = gMatAmbient.Sample(samAnisotropic, input.uv);
    _Mat.Reflect = float4(1.0f, 1.0f, 1.0f, 1.0f);
    // ���е��� ���� 0 �� ���� �Ӽ���� �����Ѵ�.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 _Depth = gDepthBuffer.Sample(samAnisotropic, input.uv);
    float4 _Normal = gNormalBuffer.Sample(samAnisotropic, input.uv);
    float4 _Pos = gPositionBuffer.Sample(samAnisotropic, input.uv);
    float4 _Albedo = gAlbedoBuffer.Sample(samAnisotropic, input.uv);
    //float4 _ShadowDepth = gShadowMap.Sample(samAnisotropic, input.uv);
    
    if (_Normal.x == 0 && _Normal.y == 0 && _Normal.z == 0)
    {
        //ť����� ������ ���� ���Ѵ�~!
        return _Albedo;
    }
    else
    {
        // �ӽ÷� ����Ʈ �־����...!


        float3 toEyeW = normalize(gEyePosW - _Pos.xyz);
    //// �������� �⿩�� ���� ���Ѵ�.
        float4 A, D, S;

        ComputeDirectionalLight(_Mat, gDirLight, _Normal.xyz, toEyeW, A, D, S);
        ambient += A;
        diffuse += D;
        spec += S;

        for (int i = 0; i < gPointCount; i++)
        {
            ComputePointLight(_Mat, gPointLight[i],
	_Pos.xyz, _Normal.xyz, toEyeW, A, D, S);

            ambient += A;
            diffuse += D;
            spec += S;
        }

        ComputeSpotLight(_Mat, gSpotLight,
	_Pos.xyz, _Normal.xyz, toEyeW, A, D, S);

        ambient += A;
        diffuse += D;
        spec += S;

        _Albedo = _Albedo * (ambient + diffuse) + spec;



        //�׸���!!!! �ϴ� ��������!
    //    float4 lightViewProjPos = mul(_Pos, gLightViewProj);
    //
    //// NDC �������� �ű� �� Shadow Texture�� ������ ��ǥ (�� �������� ���� ������ �ȼ�) �� �ؽ��� ������ �����ؼ� �����ô�.
    //    float2 projTexCoord = lightViewProjPos.xy; // /= lightViewProjPos.w; // ������ȯ ����
    //
    //    projTexCoord.y = -projTexCoord.y;
    //
    //    projTexCoord = projTexCoord * 0.5f + 0.5f; // -1.0 ~ 1.0�� ���� 0 ~ 1�� ���� ��Ų��.
    //
    //    if ((saturate(projTexCoord.x) == projTexCoord.x) && (saturate(projTexCoord.y) == projTexCoord.y)) // ������ ���� ������ �������� Ȯ���Ѵ�. 
    //    {
    //        float shadowDepthValue = gShadowMap.Sample(samAnisotropic, float2(projTexCoord.x, projTexCoord.y)).x;

    //        float currentDepthValue = lightViewProjPos.z; // / lightViewProjPos.w; // ������ȯ ����
    //    
    //    // ���� �������� �� Depth���� ���� �ȼ��� Depth�� �� ũ�� => ������ �� �ڿ� �ִ�. => �׸��ڰ� �����Ѵ�.
    //        if (shadowDepthValue < currentDepthValue - 0.001f)
    //            _Albedo = _Albedo / 2;

    //    }
    
    }
    
    return _Albedo;
}

//technique11 TextureRenderTech
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
//        SetGeometryShader(NULL);
//        SetPixelShader(CompileShader(ps_5_0, PS_MAIN()));
//    }
//}

//technique11 DeferredRenderTech
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
//        SetGeometryShader(NULL);
//        SetPixelShader(CompileShader(ps_5_0, PS_DeferredMAIN()));
//    }
//}

//technique11 PostProcessingTech
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, VS_MAIN()));
//        SetGeometryShader(NULL);
//        SetPixelShader(CompileShader(ps_5_0, PS_PostProcess()));
//    }
//}