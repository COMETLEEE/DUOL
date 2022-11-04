cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
    float4 gColor;
};

struct VertexIn
{
    float3 PosL  : POSITION;
};

struct VertexOut
{
    float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};
struct PSOut
{
    float4 Depth : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Albedo : SV_Target3;
    
};
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
    // 바깥쪽에서 월드-뷰-프로젝션 행렬을 받아서 동차절단좌표계 (아직 NDC아님)으로 변환해준다.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
	// Just pass vertex color into the pixel shader.
    // 헬퍼 오브젝트는 버텍스컬러에 의존해서 그리게 한다.
    vout.Color = gColor;
    
    return vout;
}

PSOut PS(VertexOut pin) : SV_Target
{
    PSOut _PSOut;
    _PSOut.Depth = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _PSOut.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _PSOut.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    _PSOut.Albedo = pin.Color;
    return _PSOut;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
