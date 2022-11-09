#include "LightHelper.fx"
#include "Shadow.fx"

cbuffer cbPerFrame
{
    float3 gEyePosW;
}

cbuffer cbPerObject
{
    float4x4 gWorld; // 월드 메트릭스?
    float4x4 gWorldlnvTranspose; // 역전치
    float4x4 gWorldViewProj; // 뷰 프로젝션
    Material gMaterial;
}



Texture2D gDiffuseMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

// 비등방 필터링
SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};



GeoIn VS(Vertexln vin)
{
    GeoIn vout;

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);

    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = vin.BoneWeight[0];
    weights[1] = vin.BoneWeight[1];
    weights[2] = vin.BoneWeight[2];
    weights[3] = 1.f - vin.BoneWeight[0] - vin.BoneWeight[1] - vin.BoneWeight[2];

    if (weights[0] > 0)
    {
        for (int i = 0; i < 4; i++)
        {
            if (weights[i] <= 0.0001f)
                break;
            posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndex[i]]).xyz;
            normalL += weights[i] * mul(vin.NormalL, (float3x3) gBoneTransforms[vin.BoneIndex[i]]).xyz;
        }
    }
    else
    {
        posL = vin.PosL;
        normalL = vin.NormalL;
    }

	//세계 공간으로 변환한다 .
 //   vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
 //   vout.NormalW = mul(normalL, (float3x3) gWorldlnvTranspose); //역전치
	//// 동차 절단 공간으로 변환한다.
 //   vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);
 //   vout.Color = vin.Color;
 //   vout.Tex = vin.Tex;
 //   vout.tangent = vin.tangent;

    
    vout.PosL = posL;
    vout.NormalL = normalL;
    vout.Color = vin.Color;
    vout.Tex = vin.Tex;
    vout.tangent = vin.tangent;
    
    return vout;
}
PSOut PS(VertexOut pin,
	uniform bool gUseTexure, uniform bool gUseNormalMap) : SV_Target
{
	// 보간 때문에 법선이 더 이상 단위벡터가 아닐 수 있으므로 다시 정규화한다.
    pin.NormalW = normalize(pin.NormalW);
    float3 toEyeW = normalize(gEyePosW - pin.PosW);
    float4 litColor = pin.Color;
    float3 normalMapSample = float3(1, 1, 1);
    float3 NormalW = float3(1.0f, 1.0f, 1.0f);
    if (gUseTexure)
    {
        litColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    }
    if (gUseNormalMap)
    {
        normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
        NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.tangent);
    }
    else
    {
        NormalW = pin.NormalW;
    }
	/// 큐브 맵핑, 환경 맵. 적용 todo 나중에 다시 해보자~
	{
        float3 incident = -toEyeW;
        float3 reflectionVector = reflect(incident, pin.NormalW);
        float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
        litColor += reflectionColor;
    }
    PSOut _psOut;
    _psOut.Position = float4(pin.PosW, 1.0f);
    _psOut.Depth = float4(pin.PosH.zzz, 1.0f);
    _psOut.Albedo = litColor;
    _psOut.Normal = float4(NormalW, 1.0f);
    _psOut.Diffuse = gMaterial.Diffuse;
    _psOut.Specular = gMaterial.Specular;
    _psOut.Ambient = gMaterial.Ambient;
    return _psOut;
}

[maxvertexcount(6)]
void GS(triangle GeoIn gin[3],
        inout TriangleStream<VertexOut> triStream)
{
    VertexOut gout[6];
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        gout[i].PosH = mul(float4(gin[i].PosL /*+ gin[i].NormalL * 10*/, 1.0f), gWorldViewProj);
        gout[i].PosW = mul(float4(gin[i].PosL /*+ gin[i].NormalL * 10*/, 1.0f), gWorld).xyz;
        gout[i].NormalW = mul(gin[i].NormalL, (float3x3) gWorldlnvTranspose); //역전치
        gout[i].Color = gin[i].Color;
        gout[i].Tex = gin[i].Tex;
        gout[i].tangent = gin[i].tangent;
		
    }
    for (int j = 0; j < 3; j++)
    {
        gout[j + 3].PosH = mul(float4(gin[j].PosL /*+ float3(1, 0, 0) * 100*/, 1.0f), gWorldViewProj);
        gout[j + 3].PosW = mul(float4(gin[j].PosL /*+ float3(1, 0, 0) * 100*/, 1.0f), gWorld).xyz;
        gout[j + 3].NormalW = mul(gin[j].NormalL, (float3x3) gWorldlnvTranspose); //역전치
        gout[j + 3].Color = gin[j].Color;
        gout[j + 3].Tex = gin[j].Tex;
        gout[j + 3].tangent = gin[j].tangent;
		
    }
    
    triStream.Append(gout[0]);
    triStream.Append(gout[1]);
    triStream.Append(gout[2]);
    
    triStream.RestartStrip();
    
    triStream.Append(gout[3]);
    triStream.Append(gout[4]);
    triStream.Append(gout[5]);
    
    triStream.RestartStrip();

}

// 텍스쳐와 라이트를 사용하는 테크
technique11 TextureLightTech
{
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS(true, false)));
    }
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, Shadow_VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, Shadow_PS()));
    }
   
};
// 라이트만 사용하는 테크.
technique11 LightTech
{
   
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS(false, false)));
    }
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, Shadow_VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, Shadow_PS()));
    }
   
};

// 노말맵 테크

technique11 NormalTech
{
    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(CompileShader(gs_5_0, GS()));
        SetPixelShader(CompileShader(ps_5_0, PS(true, true)));
    }
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, Shadow_VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, Shadow_PS()));
    }
   
};


