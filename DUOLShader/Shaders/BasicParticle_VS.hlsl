#include "ConstantBuffer.hlsli"
#include "ParticleHelper.hlsli"

struct VertexIDIn
{
    uint index : SV_VertexID; // 방출기인가
};

VertexIDOut ComputeShaderDrawVS(VertexIDIn vin)
{
    VertexIDOut vout;
    
    vout.index = vin.index;
    
    return vout;
}
