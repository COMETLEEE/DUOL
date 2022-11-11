
struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
    float3 tangent : TANGENT;
};
struct PSOut
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Albedo : SV_Target3;
    float4 Diffuse : SV_Target4;
    float4 Specular : SV_Target5;
    float4 Ambient : SV_Target6;
};
PSOut main(VertexOut pin) : SV_TARGET
{
	//// 보간 때문에 법선이 더 이상 단위벡터가 아닐 수 있으므로 다시 정규화한다.
 //   pin.NormalW = normalize(pin.NormalW);
 //   float3 toEyeW = normalize(gEyePosW - pin.PosW);
 //   float4 litColor = pin.Color;
 //   float3 normalMapSample = float3(1, 1, 1);
 //   float3 NormalW = float3(1.0f, 1.0f, 1.0f);
 //   if (gUseTexure)
 //   {
 //       litColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
 //   }
 //   if (gUseNormalMap)
 //   {
 //       normalMapSample = gNormalMap.Sample(samAnisotropic, pin.Tex).rgb;
 //       NormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.tangent);
 //   }
 //   else
 //   {
 //       NormalW = pin.NormalW;
 //   }
	///// 큐브 맵핑, 환경 맵. 적용 todo 나중에 다시 해보자~
	//{
 //       float3 incident = -toEyeW;
 //       float3 reflectionVector = reflect(incident, pin.NormalW);
 //       float4 reflectionColor = gCubeMap.Sample(samAnisotropic, reflectionVector);
 //       litColor += reflectionColor;
 //   }

    PSOut _psOut;
    _psOut.Position = float4(pin.PosW, 1.0f);
    _psOut.Depth = float4(pin.PosH.zzz, 1.0f);
    _psOut.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);
    _psOut.Normal = float4(pin.NormalW, 1.0f);
    _psOut.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f);
    _psOut.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f);
    _psOut.Ambient = float4(1.0f, 1.0f, 1.0f, 1.0f);

    return _psOut;
}
