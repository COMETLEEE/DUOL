cbuffer cbSkinned
{
    float4x4 gBoneTransforms[96];
};

float4x4 gLightWorldViewProj; // ±¤¿ø ±âÁØ ºäÇÁ·ÎÁ§¼Ç
struct Vertexln
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
    float3 NormalL : NORMAL;
    float4 Color : COLOR;
    float3 BoneWeight : WEIGHT;
    uint4 BoneIndex : BONEINDEX;
    float3 tangent : TANGENT;
};
struct GeoIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
    float3 tangent : TANGENT;
};
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


VertexOut Shadow_VS(Vertexln vin)
{
    VertexOut _Out;
    _Out.Color = float4(0, 0, 0, 0);
    _Out.NormalW = float3( 0, 0, 0);
    _Out.PosW = float3(0, 0, 0);
    _Out.tangent = float3( 0, 0, 0);
    _Out.Tex = float2(0, 0);
    
    
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = vin.BoneWeight[0];
    weights[1] = vin.BoneWeight[1];
    weights[2] = vin.BoneWeight[2];
    weights[3] = 1.f - vin.BoneWeight[0] - vin.BoneWeight[1] - vin.BoneWeight[2];
   
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
    
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
    
    _Out.PosH = mul(float4(posL, 1.0f), gLightWorldViewProj);
    
    return _Out;

}


float4 Shadow_PS(VertexOut pin) : SV_Target7
{
    return float4(pin.PosH.zzz,1.0f);
    //return float4(1.0f,0,0,1.0f);
}