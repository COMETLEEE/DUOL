#include "ConstantBuffer.hlsli"

Texture2D gRandomTex : register(t0); // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

Texture2D gNoiseTex : register(t1); // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

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

	// hlsl 에 랜덤함수가 내장되어 있지 않아 랜덤 텍스쳐를 통해 랜덤 구현.
    float4 v = gRandomTex.SampleLevel(samAnisotropic, u, 0);

    return normalize(v);
}

float4 RandVec4(float offset)
{
    float u = (gCommonInfo.gParticlePlayTime + offset);

	// hlsl 에 랜덤함수가 내장되어 있지 않아 랜덤 텍스쳐를 통해 랜덤 구현.
    float4 v = gRandomTex.SampleLevel(samAnisotropic, u, 0);

    return v;
}

[maxvertexcount(2)]
void StreamOutGS(point StreamOutParticle gin[1],
	inout PointStream<StreamOutParticle> ptStream)
{
    float deltaTime = gTimeStep * gCommonInfo.gSimulationSpeed;
    gin[0].Age_LifeTime_Rotation_Gravity.x += deltaTime;
    if (gin[0].Type == PT_EMITTER) // 0이라면 방출기.
    {
        if (gin[0].VertexID < gCommonInfo.gMaxParticles)
        {
            if (gCommonInfo.gDuration >= gCommonInfo.gParticlePlayTime || gCommonInfo.gisLooping)
            {
		// 일정 시간마다 방출
                if (gin[0].Age_LifeTime_Rotation_Gravity.x > gEmission.gEmissiveTime)
                {
                    const float4 vRandom1 = RandUnitVec4(gin[0].VertexID * 0.003f);
                    const float4 vRandom2 = RandUnitVec4(vRandom1.x);
                    const float4 vRandom3 = RandUnitVec4(vRandom1.y);
                    const float4 vRandom4 = RandUnitVec4(vRandom1.z);
                    const float4 vRandom5 = RandUnitVec4(vRandom1.w);

                    // 선형 보간에는 0~1의 값이 필요하다.
                    float4 vunsignedRandom2 = (RandVec4(vRandom1.x) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;
                    float4 vunsignedRandom3 = (RandVec4(vRandom1.y) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;
                    float4 vunsignedRandom4 = (RandVec4(vRandom1.z) + float4(1.0f, 1.0f, 1.0f, 1.0f)) / 2.0f;

                    float4 vUnUnitRandom2 = RandVec4(vunsignedRandom2.x);
                    float4 vUnUnitRandom3 = RandVec4(vunsignedRandom2.y);
                    float4 vUnUnitRandom4 = RandVec4(vunsignedRandom2.z);

                    // 랜덤 값.
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
    	// 항상 방출기는 유지시킨다.
        ptStream.Append(gin[0]);
        // 방출기가 한개면 유지가 되지만 여러개가 되면 밀려서 없어지는 버그가 있다... 이것을 어떻게 해결 할 것인가...?
    }
    else
    {
		// 파티클의 생존시간
        if (gin[0].Age_LifeTime_Rotation_Gravity.x <= gin[0].Age_LifeTime_Rotation_Gravity.y) // 파티클이 살아 있다면 업데이트를 해주자...!
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
            
            g_velocityOverLifeTimeInstance.VelocityOverLifeTime(gin[0], deltaTime, gin[0]); // Velocity를 가장 마지막에 하는 이유는 Orbital이 있기 때문.
                
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
// 하나의 점을 4개로 확장해서 텍스처를 입혀서 출력한다..!
[maxvertexcount(4)]
    void DrawGS
    (point VertexIDOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
    ParticleStruct p = particleBuffer[gin[0].index];
    
	// 방출기는 넘어가자
    if (p.Type != PT_EMITTER)
    {
        float3 worldPos = p.PosW.xyz;

        float3 look;
        float3 right = float3(1, 0, 0);
        float3 up = float3(0, 1, 0);

        SetBillBoard(gCameraPosW, p, look, right, up);

        /// ---------------------------- 사각형으로 파티클 확장. ---------------------------
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




// Trail을 나타내기 위한 Main, MaxvertexCount는 1024바이트를 넘을 수 없다.
#define MaxVertexCount 100

[maxvertexcount(MaxVertexCount)]
    void DrawTrailGS
    (point VertexIDOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
    
    ParticleStruct p = particleBuffer[gin[0].index];
    
    if (p.Type == PT_TRAIL)
    {
        GeoOut gout[50] =
        {
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
            { float4(0, 0, 0, 0), float4(0, 0, 0, 0), float2(0, 0) },
        };
        
        float totalMoveDistance = 0;
        
        // 첫번째 버텍스는 별다른 조건 없이 출력.
        float3 worldPos = p.PosW;
        
        gout[0].PosH.xyz = worldPos;
        
        // 첫번째 버텍스는 별다른 조건 없이 출력.
        
        int vertexCount = 1;
        
        for (int i = 0; i < TrailCount; i++)
        {
            if (vertexCount >= MaxVertexCount / 2)
                break;

            worldPos = p.PrevPos[i].xyz;
            
            float len = distance(worldPos, p.PrevPos[i + 1].xyz);
            
            float maxDistance = gTrails.gMinimumVertexDistance;
            
            if (i + 2 < TrailCount) // 마지막 두개 어떻게 처리할까!!
            {
                if (maxDistance < len)
                {
                    float3 prev = gout[vertexCount - 1].PosH.xyz;
                    
                    float splineCount = 1;
                    
                    while (maxDistance < len)
                    {
                        if (vertexCount >= MaxVertexCount / 2)
                            break;
                        
                        float3 splinePos = float3(0, 0, 0);
                        
                        int mulCount = 0;
                        
                        while (true)
                        {
                            splinePos = CatmullRomSplineCalculate(prev, worldPos, p.PrevPos[i + 1].xyz, p.PrevPos[i + 2].xyz, 1 - 0.01f * mulCount++);
                            
                            len = distance(splinePos, prev);
                            
                            if (maxDistance * splineCount > len || mulCount == 100)
                                break;
                        }
                        
                        splineCount++;
                        
                        len = distance(splinePos, p.PrevPos[i + 1].xyz);
                        
                        totalMoveDistance += distance(splinePos, gout[vertexCount - 1].PosH.xyz);
                        
                        gout[vertexCount].Tex.y = totalMoveDistance; //임시 저장용 변수. 현재까지 이동거리를 저장한다.
                        gout[vertexCount++].PosH.xyz = splinePos;
                    }
                    

                }
                else
                {
                    totalMoveDistance += distance(worldPos, gout[vertexCount - 1].PosH.xyz);
                    
                    gout[vertexCount].Tex.y = totalMoveDistance; //임시 저장용 변수. 현재까지 이동거리를 저장한다.
                    gout[vertexCount++].PosH.xyz = worldPos;
                }
            }
            else
            {
                if (maxDistance < len)
                {
                    if (i + 1 < TrailCount)
                    {
                        totalMoveDistance += distance(worldPos, gout[vertexCount - 1].PosH.xyz);
                    
                        gout[vertexCount].Tex.y = totalMoveDistance; //임시 저장용 변수. 현재까지 이동거리를 저장한다.
                        gout[vertexCount++].PosH.xyz = worldPos;
                    }
                    else if (i + 2 < TrailCount)
                    {
                        while (maxDistance < len)
                        {
                            if (vertexCount >= MaxVertexCount / 2)
                                break;
                        
                            float3 splinePos = float3(0, 0, 0);
                    
                            int mulCount = 0;
                        
                            while (maxDistance < len)
                            {
                                splinePos = lerp(worldPos, p.PrevPos[i + 1].xyz, pow(2, mulCount++));
                            
                                len = distance(splinePos, worldPos);
                            }
                        
                            len = distance(splinePos, p.PrevPos[i + 1].xyz);
                            
                            worldPos = splinePos;
                        
                            totalMoveDistance += distance(splinePos, gout[vertexCount - 1].PosH.xyz);
                    
                            gout[vertexCount].Tex.y = totalMoveDistance; //임시 저장용 변수. 현재까지 이동거리를 저장한다.
                            gout[vertexCount++].PosH.xyz = splinePos;
                        }
                    }
                }
                else
                {
                    totalMoveDistance += distance(worldPos, gout[vertexCount - 1].PosH.xyz);
                
                    gout[vertexCount].Tex.y = totalMoveDistance; //임시 저장용 변수. 현재까지 이동거리를 저장한다.
                    gout[vertexCount++].PosH.xyz = worldPos;
                }

            }
                
        }

        // ------------------------------------------------- geoOut 배열에 담아둔 포지션 정보를 버텍스로 변환.
        
        float t = p.Age_LifeTime_Rotation_Gravity.x;

        float ratio = t / p.Age_LifeTime_Rotation_Gravity.y;
        
        float texCoordStep = 1.0f / (float) (vertexCount - 1);
        
        float3 lastDirection = gout[vertexCount - 2].PosH.xyz - gout[vertexCount - 1].PosH.xyz;
        
        for (int j = 0; j < vertexCount; j++)
        {
            float halfWidth = 0.5f * p.TrailWidth;
            if (gTrails.gTrailsFlag & Use_TrailFlag_SizeAffectsWidth)
                halfWidth = halfWidth * p.SizeW_StartSize.x;
            
            
            // ------------------------------------- Color ----------------------------------------------------
            float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            float4 trailColor = float4(1.0f, 1.0f, 1.0f, 1.0f);; // 길이에 따른 색상.
            float trailRatio = (float) j / (float) (vertexCount - 1); // 길이에 따른 색상 변화를 주기 위한 비율.
            
            SetColorOverLifeTime(trailRatio, gTrails.gAlpha_Ratio_Trail, gTrails.gColor_Ratio_Trail, trailColor);
            SetColorOverLifeTime(ratio, gTrails.gAlpha_Ratio_Lifetime, gTrails.gColor_Ratio_Lifetime, color);
        
            if (gTrails.gTrailsFlag & Use_TrailFlag_InheritParticleColor)
            {
                if (gParticleFlag & Use_Color_over_Lifetime)
                    color *= p.Color;
            }
            // ------------------------------------- Color ----------------------------------------------------
            // ------------------------------------- Position -------------------------------------------------
            float3 direction = float3(0, 0, 0);
                
            float3 posTemp = gout[j].PosH.xyz;
            
            if (j + 1 < vertexCount)
                direction = gout[j].PosH.xyz - gout[j + 1].PosH.xyz;
            else
                direction = lastDirection;
            
            float3 look = normalize(gCameraPosW - posTemp);
            float3 up = normalize(direction) * (gParticleRenderer.gLengthScale + length(direction) * gParticleRenderer.gSpeedScale);
            float3 right = normalize(cross(gCameraPosW - posTemp, up));
            
            // ------------------------------------- Position -------------------------------------------------
            // ------------------------------------- TexUV ----------------------------------------------------
            float y = 0;
            y = CalculateTrailUV(gout[j].Tex.y, totalMoveDistance, p.TrailScrollSpeed.y);
            // ------------------------------------- TexUV ----------------------------------------------------
            
            gout[j].Color = color * trailColor;
            
            gout[j].Tex = float2(1 + p.TrailScrollSpeed.x * gCommonInfo.gParticlePlayTime, y);
            gout[j].PosH = mul(float4(posTemp - halfWidth * right - look * 0.1f, 1.0f), gViewProj);
            triStream.Append(gout[j]);
            
            gout[j].Tex = float2(0 + p.TrailScrollSpeed.x * gCommonInfo.gParticlePlayTime, y);
            gout[j].PosH = mul(float4(posTemp + halfWidth * right - look * 0.1f, 1.0f), gViewProj);
            triStream.Append(gout[j]);
        }
        
    }
}

