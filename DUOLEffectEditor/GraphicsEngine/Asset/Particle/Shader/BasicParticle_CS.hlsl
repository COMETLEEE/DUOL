#include "ConstantBuffer.hlsli"

Texture2D gRandomTex : register(t0); // ComputeShader�� ��°�� Texture1D�� ���ø��� �ȵǴ°�...?

Texture2D gNoiseTex : register(t1); // HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����

cbuffer CB_DynamicBuffer : register(b2) // �� �����Ӹ��� ���� ���ؼ�...
{
    float g_EmiitionTime;
    int g_dim;
    float pad1;
    float pad2;
};
struct Counter
{
    int g_particleCounter;
    int g_EmiiterCounter;
    int g_AllGroupSync;
    int pad4;
};

RWStructuredBuffer<ParticleStruct> ResultParticleBuffer : register(u0); // ��ƼŬ ����.

RWStructuredBuffer<Counter> CounterBuffer : register(u1); // ��ƼŬ ����.

void ExchangeParticle(int destIndex, int srcIndex)
{

}

float4 RandUnitVec4(float offset)
{
    float u = (gCommonInfo.gParticlePlayTime + offset);

	// hlsl �� �����Լ��� ����Ǿ� ���� �ʾ� ���� �ؽ��ĸ� ���� ���� ����.
    float4 v = gRandomTex.SampleLevel(samAnisotropic, u, 0);

    return normalize(v);
}

float4 RandVec4(float offset)
{
    float u = (gCommonInfo.gParticlePlayTime + offset);

	// hlsl �� �����Լ��� ����Ǿ� ���� �ʾ� ���� �ؽ��ĸ� ���� ���� ����.
    float4 v = gRandomTex.SampleLevel(samAnisotropic, u, 0);

    return v;
}


[numthreads(1024, 1, 1)]
void CS_Main(uint3 groupID : SV_GroupID, uint3 groupTreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex)
{
    int ID = groupID.x * 1024 + groupID.y * g_dim * 1024 + groupID.z * g_dim * g_dim * 1024 + groupTreadID.x;
   
    int ID_Temp = ID;
   
    if (ID >= gCommonInfo.gMaxParticles)
        return;
    
    ParticleStruct p = ResultParticleBuffer[ID];
    
    if (p.Type == PT_EMITTER)
    {
        if (g_EmiitionTime >= gEmission.gEmissiveTime)
        {
                //���� �ð�����//����
            int count = CounterBuffer[0].g_EmiiterCounter; //CounterBuffer[0].count;
                
            if (count > gEmission.gEmissiveCount)
                return;
            
            InterlockedAdd(CounterBuffer[0].g_EmiiterCounter, 1, count);
            
            if (count < gEmission.gEmissiveCount)
            {
                
                float4 vRandom1 = RandUnitVec4(ID * 0.003f);
                
                const float4 vRandom2 = RandUnitVec4(vRandom1.x);
                const float4 vRandom3 = RandUnitVec4(vRandom1.y);
                const float4 vRandom4 = RandUnitVec4(vRandom1.z);
                const float4 vRandom5 = RandUnitVec4(vRandom1.w);

                    // ���� �������� 0~1�� ���� �ʿ��ϴ�.
                float4 vunsignedRandom2 = (RandVec4(vRandom1.x) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;
                float4 vunsignedRandom3 = (RandVec4(vRandom1.y) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;
                float4 vunsignedRandom4 = (RandVec4(vRandom1.z) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;

                float4 vUnUnitRandom2 = RandVec4(vunsignedRandom2.x);
                float4 vUnUnitRandom3 = RandVec4(vunsignedRandom2.y);
                float4 vUnUnitRandom4 = RandVec4(vunsignedRandom2.z);
                
                p.Type = PT_FLARE;
                
                p.PosW = float3(0, 0, 0);
                p.VelW = float4(0, 0, 0, 1.0f);

                p.SizeW_StartSize = float4(1.0f, 1.0f, 1.0f, 1.0f);
                p.Age_LifeTime_Rotation_Gravity = float4(0, 10.0f, 0, 0);
                    
                p.StartColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
                p.Color = p.StartColor;

                p.QuadTexC[0] = float2(0.0f, 1.0f); // lefttop
                p.QuadTexC[1] = float2(0.0f, 0.0f); //leftbottom
                p.QuadTexC[2] = float2(1.0f, 1.0f); // righttop
                p.QuadTexC[3] = float2(1.0f, 0.0f); // ritghtbottom
    

                p.InitEmitterPos = gCommonInfo.gTransformMatrix[3];
                    
                    [unroll]
                for (int i = 0; i < 15; i++)
                {
                    p.PrevPos[i] = float4(p.PosW, 1.0f);
                }
                    
                p.LatestPrevPos = float4(p.PosW - p.VelW.xyz, 1.0f);
                
                ManualShape(vRandom1, vRandom2, vRandom5, vunsignedRandom2, vUnUnitRandom2, p.PosW.xyz, p.VelW.xyz);

                p.SizeW_StartSize.zw = lerp(gCommonInfo.gStartSize.xy, gCommonInfo.gStartSize.zw, vunsignedRandom2.y);

                p.SizeW_StartSize.xy = p.SizeW_StartSize.zw;
                    
                p.Age_LifeTime_Rotation_Gravity.x = 0.0f;
                    
                p.Age_LifeTime_Rotation_Gravity.y = lerp(gCommonInfo.gStartLifeTime[0], gCommonInfo.gStartLifeTime[1], vunsignedRandom2.z);

                p.Age_LifeTime_Rotation_Gravity.z = lerp(gCommonInfo.gStartRotation[0], gCommonInfo.gStartRotation[1], vunsignedRandom3.x);

                p.Age_LifeTime_Rotation_Gravity.w = lerp(gCommonInfo.gGravityModifier[0], gCommonInfo.gGravityModifier[1], vunsignedRandom3.z) /** gCommonInfo.gSimulationSpeed*/;
                    
                p.StartColor = lerp(gCommonInfo.gStartColor[0], gCommonInfo.gStartColor[1], vunsignedRandom3.y);
                    
                p.Color = p.StartColor;

                ManualColorOverLifeTime(0, gColorOverLifetime.gAlpha_Ratio, gColorOverLifetime.gColor_Ratio, p.Color);
                
                p.Type = PT_FLARE;
                    
                if (vunsignedRandom4.w <= gTrails.gRatio)
                    p.Type = PT_TRAIL;

                p.InitEmitterPos = gCommonInfo.gTransformMatrix[3];
                    
                    [unroll]
                for (i = 0; i < 15; i++)
                {
                    p.PrevPos[i] = float4(p.PosW, 1.0f);

                }
                    
                p.LatestPrevPos = float4(p.PosW - p.VelW.xyz, 1.0f);
                    
                ManualTextureSheetAnimation(vunsignedRandom4, p.QuadTexC);
                
                p.Age_LifeTime_Rotation_Gravity.x = 0;
            }
        }
            
    }
    else
    {
        float deltaTime = gTimeStep * gCommonInfo.gSimulationSpeed;
    
        p.Age_LifeTime_Rotation_Gravity.x += deltaTime;
        
        if (p.Age_LifeTime_Rotation_Gravity.x <= p.Age_LifeTime_Rotation_Gravity.y) // ��ƼŬ�� ��� �ִٸ� ������Ʈ�� ������...!
        {
            
            InterlockedAdd(CounterBuffer[0].g_particleCounter, 1, ID_Temp);
            // ��ƼŬ�� �����ð�
            p.LatestPrevPos = float4(p.PosW, 1.0f);

            float t = p.Age_LifeTime_Rotation_Gravity.x;

            float ratio = t / p.Age_LifeTime_Rotation_Gravity.y;

            if (!(gParticleFlag & Use_Commoninfo_WorldSpace))
            {
                p.PosW = mul(float4(p.PosW, 1.0f), gCommonInfo.gDeltaMatrix).xyz;

                p.InitEmitterPos = gCommonInfo.gTransformMatrix[3];
            }
            
            ManualTrail(p.PrevPos, p.PosW, p.PrevPos);
            
            ManualForceOverLifeTime(p.PosW, ratio, deltaTime, p.PosW);
            
            p.VelW.xyz += float3(0, -p.Age_LifeTime_Rotation_Gravity.w, 0) * deltaTime;
            
            p.PosW += p.VelW.xyz * deltaTime;
            
            ManualVelocityOverLifeTime(p, deltaTime, p); // Velocity�� ���� �������� �ϴ� ������ Orbital�� �ֱ� ����.
                
            ManualRoationOverLifeTime(p.Age_LifeTime_Rotation_Gravity.z, gRotationOverLifetime.gAngularVelocity, p.Age_LifeTime_Rotation_Gravity.y, deltaTime, p.Age_LifeTime_Rotation_Gravity.z);
            
            float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            
            ManualColorOverLifeTime(ratio, gColorOverLifetime.gAlpha_Ratio, gColorOverLifetime.gColor_Ratio, color);
            
            float2 size = float2(1, 1);
            
            ManualSizeOverLifeTime(ratio, size);
           
            p.Color = p.StartColor * color;

            p.SizeW_StartSize.xy = p.SizeW_StartSize.zw * size;
            
            ManualNoise(p.SizeW_StartSize.xy, p.VelW.xyz, p.Age_LifeTime_Rotation_Gravity.z,
                gNoiseTex, samAnisotropic, deltaTime, gGamePlayTime,
            p.SizeW_StartSize.xy, p.VelW.xyz, p.Age_LifeTime_Rotation_Gravity.z);

            ManualCollision(p.PosW, p.VelW.xyz, deltaTime, p.Age_LifeTime_Rotation_Gravity.x, p.Age_LifeTime_Rotation_Gravity.y
            , p.PosW, p.VelW.xyz, p.Age_LifeTime_Rotation_Gravity.x);
            
        }
        else
        {
            p.Type = PT_EMITTER;
            p.PosW = float3(0, 0, 0);
            p.VelW = float4(0, 0, 0, 1.0f);

            p.SizeW_StartSize = float4(1.0f, 1.0f, 1.0f, 1.0f);
            p.Age_LifeTime_Rotation_Gravity = float4(0, 10.0f, 0, 0);
                    
            p.StartColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
            p.Color = p.StartColor;

            p.QuadTexC[0] = float2(0.0f, 1.0f); // lefttop
            p.QuadTexC[1] = float2(0.0f, 0.0f); //leftbottom
            p.QuadTexC[2] = float2(1.0f, 1.0f); // righttop
            p.QuadTexC[3] = float2(1.0f, 0.0f); // ritghtbottom
    

            p.InitEmitterPos = gCommonInfo.gTransformMatrix[3];
                    
                    [unroll]
            for (int i = 0; i < 15; i++)
            {
                p.PrevPos[i] = float4(p.PosW, 1.0f);
            }
                    
            p.LatestPrevPos = float4(p.PosW - p.VelW.xyz, 1.0f);
        }
    }
    
    ResultParticleBuffer[ID] = p;
}


[numthreads(1024, 1, 1)]
void CS_ResetParticleBuffer(uint3 groupID : SV_GroupID, uint3 groupTreadID : SV_GroupThreadID) // CPU Acess Flag�� ����ϸ� ������ ����� �����. ������ GPU���� ó������.
{
    int ID = groupID.x * 1024 + groupID.y * g_dim * 1024 + groupID.z * g_dim * g_dim * 1024 + groupTreadID.x;

    ResultParticleBuffer[ID].Type = PT_EMITTER;
}

[numthreads(1, 1, 1)]
void CS_ClearCounter(uint3 groupID : SV_GroupID, uint3 groupTreadID : SV_GroupThreadID)
{
    CounterBuffer[0].g_EmiiterCounter = 0;
    CounterBuffer[0].g_particleCounter = 0;
    CounterBuffer[0].g_AllGroupSync = 0;
}