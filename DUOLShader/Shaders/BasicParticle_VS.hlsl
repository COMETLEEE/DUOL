#include "ConstantBuffer.hlsli"
#include "ParticleHelper.hlsli"

struct VertexIDIn
{
    uint index : SV_VertexID; // ������ΰ�
};

VertexIDOut ComputeShaderDrawVS(VertexIDIn vin)
{
    VertexIDOut vout;
    
    vout.index = vin.index;
    
    return vout;
}
