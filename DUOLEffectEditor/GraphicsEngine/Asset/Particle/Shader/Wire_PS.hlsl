struct VertexOut
{
	float4 PosH  : SV_POSITION;
};

struct PSOut
{
    float4 Albedo : SV_Target0;
    float4 Depth : SV_Target1;
    float4 Normal : SV_Target2;
    float4 Position : SV_Target3;
};


PSOut main(VertexOut vin) : SV_TARGET
{
	PSOut _psOut;

	_psOut.Depth = float4(0.0f, 0.0f, 0.0f, 0.0f);
	_psOut.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	_psOut.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
	_psOut.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return _psOut;
}