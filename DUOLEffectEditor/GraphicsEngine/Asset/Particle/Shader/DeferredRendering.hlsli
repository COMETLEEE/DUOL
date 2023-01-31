
#include "ConstantBuffer.hlsli"

#define NUM_MOTIONBLUR_SAMPLES     8



Texture2D gDiffuseMap : register(t0); // 출력하기 위함

Texture2D gDepthBuffer : register(t1); // 디퍼드 조립을 위한 버퍼들.
Texture2D gNormalBuffer : register(t2);
Texture2D gPositionBuffer : register(t3);
Texture2D gAlbedoBuffer : register(t4);
Texture2D gMatDiffuse : register(t5);
Texture2D gMatSpecular : register(t6);
Texture2D gMatAmbient : register(t7);


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

// 버텍스 셰이더의 본질인 함수
VS_OUT VS_MAIN(VS_IN input)
{
    VS_OUT output;

    output.pos = float4(input.pos, 1.0f);
    output.uv = input.uv;
    return output;
}

// 텍스처 출력을 하는 쉐이더
float4 PS_TextureRender(VS_OUT input) : SV_Target
{
    return gDiffuseMap.Sample(samAnisotropic, input.uv);
}

// 포스트 프로세싱
float4 PS_PostProcess(VS_OUT input) : SV_Target
{
    //float4 _Pos = gPositionBuffer.Sample(samAnisotropic, input.uv);
    //float4 _Normal = gNormalBuffer.Sample(samAnisotropic, input.uv);
    //// 모션블러!
    //float4 color = 0;

    //// 텍스처좌표를 투영좌표로 변형
    //float4 Proj_Curr = mul(float4(_Pos.xyz, 1), gCurrentViewProj);
    //// 깊이값을 기반으로 현재 World좌표 구하기

    //float4 Proj_Prev = mul(float4(_Pos.xyz, 1), gPrevViewProj);
    //Proj_Prev /= Proj_Prev.w;
    //Proj_Curr /= Proj_Curr.w;
    //
    //// 직전 투영좌표와 현재 투영좌표를 기반으로 속도벡터 구하기
    //float f = 1.0f / NUM_MOTIONBLUR_SAMPLES;
    //float2 velocity = float2(Proj_Prev.x - Proj_Curr.x, Proj_Curr.y - Proj_Prev.y) / 2 * f;
    //
    //// 자 이제 Blur Time!
    //for (int j = 0; j < NUM_MOTIONBLUR_SAMPLES; j++)
    //{
    //    float4 currentColor = gDiffuseMap.Sample(samSEX, input.uv + velocity.xy * j);
    //    color += currentColor;
    //}
    //color = color / NUM_MOTIONBLUR_SAMPLES;

    float4 color = gDiffuseMap.Sample(samAnisotropic, input.uv);

    return color;
}

// 디퍼드 조립.
float4 PS_DeferredRender(VS_OUT input) : SV_Target
{
    
    Material _Mat;
    _Mat.Diffuse = gMatDiffuse.Sample(samAnisotropic, input.uv);
    _Mat.Specular = gMatSpecular.Sample(samAnisotropic, input.uv);
    _Mat.Ambient = gMatAmbient.Sample(samAnisotropic, input.uv);
    _Mat.Reflect = float4(1.0f, 1.0f, 1.0f, 1.0f);
    // 성분들의 합이 0 인 재질 속성들로 시작한다.
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 _Depth = gDepthBuffer.Sample(samAnisotropic, input.uv);
    float4 _Normal = gNormalBuffer.Sample(samAnisotropic, input.uv);
    float4 _Pos = gPositionBuffer.Sample(samAnisotropic, input.uv);
    float4 _Albedo = gAlbedoBuffer.Sample(samAnisotropic, input.uv);
    //float4 _ShadowDepth = gShadowMap.Sample(samAnisotropic, input.uv);
    
    // 라이팅을 안쓰고 있으니, 나중에 다시 추가하자.
 //   if (_Normal.x == 0 && _Normal.y == 0 && _Normal.z == 0)
 //   {
 //       //큐브맵은 라이팅 연산 안한당~!
 //       return _Albedo;
 //   }
 //   else
 //   {
 //       // 임시로 라이트 넣어두자...!


 //       float3 toEyeW = normalize(gEyePosW - _Pos.xyz);
 //   //// 각광원이 기여한 빛을 합한다.
 //       float4 A, D, S;

 //       ComputeDirectionalLight(_Mat, gDirLight, _Normal.xyz, toEyeW, A, D, S);
 //       ambient += A;
 //       diffuse += D;
 //       spec += S;

 //       for (int i = 0; i < gPointCount; i++)
 //       {
 //           ComputePointLight(_Mat, gPointLight[i],
	//_Pos.xyz, _Normal.xyz, toEyeW, A, D, S);

 //           ambient += A;
 //           diffuse += D;
 //           spec += S;
 //       }

 //       ComputeSpotLight(_Mat, gSpotLight,
	//_Pos.xyz, _Normal.xyz, toEyeW, A, D, S);

 //       ambient += A;
 //       diffuse += D;
 //       spec += S;

 //       _Albedo = _Albedo * (ambient + diffuse) + spec;



        //그림자!!!! 일단 지워놓자!
    //    float4 lightViewProjPos = mul(_Pos, gLightViewProj);
    //
    //// NDC 공간으로 옮긴 후 Shadow Texture를 참조할 좌표 (빛 관점에서 같은 방향의 픽셀) 를 텍스쳐 공간에 투영해서 얻어냅시다.
    //    float2 projTexCoord = lightViewProjPos.xy; // /= lightViewProjPos.w; // 공간변환 공부
    //
    //    projTexCoord.y = -projTexCoord.y;
    //
    //    projTexCoord = projTexCoord * 0.5f + 0.5f; // -1.0 ~ 1.0의 값을 0 ~ 1로 변경 시킨다.
    //
    //    if ((saturate(projTexCoord.x) == projTexCoord.x) && (saturate(projTexCoord.y) == projTexCoord.y)) // 쉐도우 맵의 범위에 들어오는지 확인한다. 
    //    {
    //        float shadowDepthValue = gShadowMap.Sample(samAnisotropic, float2(projTexCoord.x, projTexCoord.y)).x;

    //        float currentDepthValue = lightViewProjPos.z; // / lightViewProjPos.w; // 공간변환 공부
    //    
    //    // 빛의 시점에서 본 Depth보다 현재 픽셀의 Depth가 더 크다 => 빛보다 더 뒤에 있다. => 그림자가 져야한다.
    //        if (shadowDepthValue < currentDepthValue - 0.001f)
    //            _Albedo = _Albedo / 2;

    //    }
    
    //}

    return _Albedo;
}
