#include "ConstantBuffer.hlsli"

Texture2D gRandomTex : register(t0); // HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����

Texture2D gNoiseTex : register(t1); // HLSL���� �����Լ��� ����Ǿ� ���� �ʾƼ� ���� �ؽ�ó�� ����� ���� ����

StructuredBuffer<ParticleStruct> particleBuffer : register(t2);

struct GeoOut
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD1;
};
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

[maxvertexcount(2)]
void StreamOutGS(point StreamOutParticle gin[1],
	inout PointStream<StreamOutParticle> ptStream)
{
    float deltaTime = gTimeStep * gCommonInfo.gSimulationSpeed;
    gin[0].Age_LifeTime_Rotation_Gravity.x += deltaTime;
    if (gin[0].Type == PT_EMITTER) // 0�̶�� �����.
    {
        if (gin[0].VertexID < gCommonInfo.gMaxParticles)
        {
            if (gCommonInfo.gDuration >= gCommonInfo.gParticlePlayTime || gCommonInfo.gisLooping)
            {
		// ���� �ð����� ����
                if (gin[0].Age_LifeTime_Rotation_Gravity.x > gEmission.gEmissiveTime)
                {
                    const float4 vRandom1 = RandUnitVec4(gin[0].VertexID * 0.003f);
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

                    // ���� ��.
                    StreamOutParticle p;

                    g_shapeInstance.Shape(vRandom1, vRandom2, vRandom5, vunsignedRandom2, vUnUnitRandom2, p.PosW, p.VelW);

                    p.SizeW_StartSize.zw = lerp(gCommonInfo.gStartSize.xy, gCommonInfo.gStartSize.zw, vunsignedRandom2.y);

                    p.SizeW_StartSize.xy = p.SizeW_StartSize.zw;
                    
                    p.Age_LifeTime_Rotation_Gravity.x = 0.0f;
                    
                    p.Age_LifeTime_Rotation_Gravity.y = lerp(gCommonInfo.gStartLifeTime[0], gCommonInfo.gStartLifeTime[1], vunsignedRandom2.z);

                    p.Age_LifeTime_Rotation_Gravity.z = lerp(gCommonInfo.gStartRotation[0], gCommonInfo.gStartRotation[1], vunsignedRandom3.x);

                    p.Age_LifeTime_Rotation_Gravity.w = lerp(gCommonInfo.gGravityModifier[0], gCommonInfo.gGravityModifier[1], vunsignedRandom3.z) /** gCommonInfo.gSimulationSpeed*/;
                    
                    p.StartColor = lerp(gCommonInfo.gStartColor[0], gCommonInfo.gStartColor[1], vunsignedRandom3.y);
                    
                    p.Color = p.StartColor;

                    p.Type = PT_FLARE;
                    
                    if (vunsignedRandom4.w <= gTrails.gRatio)
                        p.Type = PT_TRAIL;

                    p.VertexID = 0;

                    p.InitEmitterPos = gCommonInfo.gTransformMatrix[3].xyz;
                    
                    [unroll]
                    for (int i = 0; i < 15; i++)
                    {
                        p.PrevPos[i] = p.PosW;
                    }
                    
                    p.LatestPrevPos = p.PosW - p.VelW;
                    
                    g_textureSheetAnimationInstance.TextureSheetAnimation(vunsignedRandom4, p.QuadTexC);

                    ptStream.Append(p);

                    gin[0].Age_LifeTime_Rotation_Gravity.x = 0;
                }
            }
        }
    	// �׻� ������ ������Ų��.
        ptStream.Append(gin[0]);
        // ����Ⱑ �Ѱ��� ������ ������ �������� �Ǹ� �з��� �������� ���װ� �ִ�... �̰��� ��� �ذ� �� ���ΰ�...?
    }
    else
    {
		// ��ƼŬ�� �����ð�
        if (gin[0].Age_LifeTime_Rotation_Gravity.x <= gin[0].Age_LifeTime_Rotation_Gravity.y) // ��ƼŬ�� ��� �ִٸ� ������Ʈ�� ������...!
        {
            gin[0].LatestPrevPos = gin[0].PosW;

            float t = gin[0].Age_LifeTime_Rotation_Gravity.x;

            float ratio = t / gin[0].Age_LifeTime_Rotation_Gravity.y;
            
    

            if (!(gParticleFlag & Use_Commoninfo_WorldSpace))
            {
                gin[0].PosW = mul(float4(gin[0].PosW, 1.0f), gCommonInfo.gDeltaMatrix).xyz;

                gin[0].InitEmitterPos = gCommonInfo.gTransformMatrix[3].xyz;
            }
            
            g_trails.Trails(gin[0].PrevPos, gin[0].PosW, gin[0].PrevPos);
            
            g_forceOverLifeTimeInstance.ForceOverLifeTime(gin[0].PosW, ratio, deltaTime, gin[0].PosW);
            
            gin[0].VelW += float3(0, -gin[0].Age_LifeTime_Rotation_Gravity.w, 0) * deltaTime;
            
            gin[0].PosW += gin[0].VelW * deltaTime;
            
            g_velocityOverLifeTimeInstance.VelocityOverLifeTime(gin[0], deltaTime, gin[0]); // Velocity�� ���� �������� �ϴ� ������ Orbital�� �ֱ� ����.
                
            g_roationOverLifeTimeInstance.RoationOverLifeTime(gin[0].Age_LifeTime_Rotation_Gravity.z, gRotationOverLifetime.gAngularVelocity, gin[0].Age_LifeTime_Rotation_Gravity.y, deltaTime, gin[0].Age_LifeTime_Rotation_Gravity.z);
            
            float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            
            g_colorOverLifeTimeInstance.ColorOverLifeTime(ratio, gColorOverLifetime.gAlpha_Ratio, gColorOverLifetime.gColor_Ratio, color);
            
            float2 size = float2(1, 1);
            
            g_sizeOverLifeTimeInstance.SizeOverLifeTime(ratio, size);
           
            gin[0].Color = gin[0].StartColor * color;

            gin[0].SizeW_StartSize.xy = gin[0].SizeW_StartSize.zw * size;
            
            g_noiseInstance.Noise(gin[0].SizeW_StartSize.xy, gin[0].VelW, gin[0].Age_LifeTime_Rotation_Gravity.z,
                gNoiseTex, samAnisotropic, deltaTime, gGamePlayTime,
            gin[0].SizeW_StartSize.xy, gin[0].VelW, gin[0].Age_LifeTime_Rotation_Gravity.z);

            g_collisionInstance.Collision(gin[0].PosW, gin[0].VelW, deltaTime, gin[0].Age_LifeTime_Rotation_Gravity.x, gin[0].Age_LifeTime_Rotation_Gravity.y
            , gin[0].PosW, gin[0].VelW, gin[0].Age_LifeTime_Rotation_Gravity.x);
            
            ptStream.Append(gin[0]);
        }
    }
}
// �ϳ��� ���� 4���� Ȯ���ؼ� �ؽ�ó�� ������ ����Ѵ�..!
[maxvertexcount(4)]
    void DrawGS
    (point VertexIDOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
    ParticleStruct p = particleBuffer[gin[0].index];
    
	// ������ �Ѿ��
    if (p.Type != PT_EMITTER)
    {
        float3 worldPos = p.PosW.xyz;

        float3 look;
        float3 right = float3(1, 0, 0);
        float3 up = float3(0, 1, 0);

        SetBillBoard(gCameraPosW, p, look, right, up);

        /// ---------------------------- �簢������ ��ƼŬ Ȯ��. ---------------------------
        float halfWidth = 0.5f * p.SizeW_StartSize.x;
        float halfHeight = 0.5f * p.SizeW_StartSize.y;

        float4 v[4];
        v[0] = float4(worldPos + halfWidth * right - halfHeight * up, 1.0f);
        v[1] = float4(worldPos + halfWidth * right + halfHeight * up, 1.0f);
        v[2] = float4(worldPos - halfWidth * right - halfHeight * up, 1.0f);
        v[3] = float4(worldPos - halfWidth * right + halfHeight * up, 1.0f);

        GeoOut gout;
        
        gout.Color = p.Color;

        gout.PosH = mul(v[0], gViewProj);
        gout.Tex = p.QuadTexC[0];
        triStream.Append(gout);

        gout.PosH = mul(v[1], gViewProj);
        gout.Tex = p.QuadTexC[1];
        triStream.Append(gout);

        gout.PosH = mul(v[2], gViewProj);
        gout.Tex = p.QuadTexC[2];
        triStream.Append(gout);

        gout.PosH = mul(v[3], gViewProj);
        gout.Tex = p.QuadTexC[3];
        triStream.Append(gout);
    }
    
}


//// �ϳ��� ���� 4���� Ȯ���ؼ� �ؽ�ó�� ������ ����Ѵ�..!
//[maxvertexcount(4)]
//    void DrawGS
//    (point ParticleVertexOut gin[1],
//	inout TriangleStream<GeoOut> triStream)
//{
//	// ������ �Ѿ��
//    if (gin[0].Type != PT_EMITTER)
//    {
//        float3 worldPos = gin[0].PosW;

//        float3 look;
//        float3 right = float3(1, 0, 0);
//        float3 up = float3(0, 1, 0);

//        SetBillBoard(gCameraPosW, gin[0], look, right, up);

//        /// ---------------------------- �簢������ ��ƼŬ Ȯ��. ---------------------------
//        float halfWidth = 0.5f * gin[0].SizeW.x;
//        float halfHeight = 0.5f * gin[0].SizeW.y;

//        float4 v[4];
//        v[0] = float4(worldPos + halfWidth * right - halfHeight * up, 1.0f);
//        v[1] = float4(worldPos + halfWidth * right + halfHeight * up, 1.0f);
//        v[2] = float4(worldPos - halfWidth * right - halfHeight * up, 1.0f);
//        v[3] = float4(worldPos - halfWidth * right + halfHeight * up, 1.0f);

//        GeoOut gout;
        
//        gout.Color = gin[0].Color;

//        gout.PosH = mul(v[0], gViewProj);
//        gout.Tex = gin[0].QuadTexC[0];
//        triStream.Append(gout);

//        gout.PosH = mul(v[1], gViewProj);
//        gout.Tex = gin[0].QuadTexC[1];
//        triStream.Append(gout);

//        gout.PosH = mul(v[2], gViewProj);
//        gout.Tex = gin[0].QuadTexC[2];
//        triStream.Append(gout);

//        gout.PosH = mul(v[3], gViewProj);
//        gout.Tex = gin[0].QuadTexC[3];
//        triStream.Append(gout);
//    }
//}



// Trail�� ��Ÿ���� ���� Main, MaxvertexCount�� 1024����Ʈ�� ���� �� ����.
// ���� ���ؽ� 1���� 8����Ʈ 100�� �ؼ� 800 ����Ʈ. ���� �ƽ���� �� �� ���� ��.
[maxvertexcount(60)]
    void DrawTrailGS
    (point VertexIDOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
    
    ParticleStruct p = particleBuffer[gin[0].index];
    
    if (p.Type == PT_TRAIL)
    {
        float t = p.Age_LifeTime_Rotation_Gravity.x;

        float ratio = t / p.Age_LifeTime_Rotation_Gravity.y;
        
        float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
        
        SetColorOverLifeTime(ratio, gTrails.gAlpha_Ratio_Lifetime, gTrails.gColor_Ratio_Lifetime, color);
        
        if (gTrails.gTrailsFlag & Use_TrailFlag_InheritParticleColor)
        {
            if (gParticleFlag & Use_Color_over_Lifetime)
                color *= p.Color;
        }
        
        float halfWidth = 0.5f * p.TrailWidth;
        
        if (gTrails.gTrailsFlag & Use_TrailFlag_SizeAffectsWidth)
            halfWidth = halfWidth * p.SizeW_StartSize.x;
        
        float texCoordStep = 1.0f / (float) (gTrails.gTrailVertexCount - 1);

        GeoOut gout;

        float yOffset = 0;

        float3 worldPos = p.PosW;

        float3 prevPosW = p.PrevPos[0].xyz;

        float3 direction = prevPosW - worldPos;

        float offsetLen = length(direction);

		[unroll(30)]
        for (int i = 1; i <= gTrails.gTrailVertexCount; ++i)
        {
            float trailRatio = (float) i / (float) gTrails.gTrailVertexCount; // ���̿� ���� ���� ��ȭ�� �ֱ� ���� ����.
            float4 trailColor; // ���̿� ���� ����.
            SetColorOverLifeTime(trailRatio, gTrails.gAlpha_Ratio_Trail, gTrails.gColor_Ratio_Trail, trailColor);
            gout.Color = color * trailColor;
            
            float y = 0;
            
            if (gTrails.gTrailsFlag & Use_TrailFlag_TextureMode_Stretch)
                y = texCoordStep * (i - 1);
            else if (gTrails.gTrailsFlag & Use_TrailFlag_TextureMode_Tile)
                y = yOffset;
            else if (gTrails.gTrailsFlag & Use_TrailFlag_TextureMode_DistributePerSegment)
                y = texCoordStep * (i - 1);
            else if (gTrails.gTrailsFlag & Use_TrailFlag_TextureMode_RepeatPerSegment)
                y = texCoordStep * (i - 1);

            if (i != gTrails.gTrailVertexCount)
            {
                float3 offsetDir = p.PrevPos[i - 1] - p.PrevPos[i];

                prevPosW = p.PrevPos[i].xyz + normalize(offsetDir) * offsetLen;
            
                direction = prevPosW - worldPos;
            }

            float len = length(direction);

            float3 look = normalize(gCameraPosW - worldPos);
            
            float3 up = normalize(direction) * (gParticleRenderer.gLengthScale + length(direction) * gParticleRenderer.gSpeedScale);
           
            float3 right = normalize(cross(up, look));
            
            float4 v[2];
            
            v[0] = float4(worldPos - halfWidth * right - look * 0.1f, 1.0f);
            
            v[1] = float4(worldPos + halfWidth * right - look * 0.1f, 1.0f);
            
            gout.PosH = mul(v[0], gViewProj);
            
            gout.Tex = float2(1, y);
            
            triStream.Append(gout);
            
            gout.PosH = mul(v[1], gViewProj);
            
            gout.Tex = float2(0, y);
            
            triStream.Append(gout);
            
            yOffset += len;

            worldPos = prevPosW;

        }
        
    }

}

//[maxvertexcount(30)]
//    void DrawTrailGS
//    (point ParticleVertexOut gin[1],
//	inout TriangleStream<GeoOut> triStream)
//{
    

//}