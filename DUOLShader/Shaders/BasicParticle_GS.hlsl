#include "ConstantBuffer.hlsli"
#include "ParticleHelper.hlsli"

Texture2D gRandomTex : register(t0); // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

Texture2D gNoiseTex : register(t1); // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

SamplerState samAnisotropic : register(s0); // HLSL에는 랜덤함수가 내장되어 있지 않아서 랜덤 텍스처를 만들어 랜덤 구현

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

        SetBillBoard(g_Camera.g_CameraPosition, p, look, right, up);

        /// ---------------------------- 사각형으로 파티클 확장. ---------------------------
        float halfWidth = 0.5f * p.SizeW_StartSize.x;
        float halfHeight = 0.5f * p.SizeW_StartSize.y;

        float4 v[4];
        v[0] = float4(worldPos + halfWidth * right - halfHeight * up, 1.f);
        v[1] = float4(worldPos + halfWidth * right + halfHeight * up, 1.f);
        v[2] = float4(worldPos - halfWidth * right - halfHeight * up, 1.f);
        v[3] = float4(worldPos - halfWidth * right + halfHeight * up, 1.f);

        GeoOut gout;
        
        gout.Color = p.Color;

        gout.PosH = mul(v[0], g_Camera.g_ViewProjectionMatrix);
        gout.Tex = p.QuadTexC[0];
        triStream.Append(gout);

        gout.PosH = mul(v[1], g_Camera.g_ViewProjectionMatrix);
        gout.Tex = p.QuadTexC[1];
        triStream.Append(gout);

        gout.PosH = mul(v[2], g_Camera.g_ViewProjectionMatrix);
        gout.Tex = p.QuadTexC[2];
        triStream.Append(gout);

        gout.PosH = mul(v[3], g_Camera.g_ViewProjectionMatrix);
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
     
        [unroll(50)]
        for (int j = 0; j < vertexCount; j++)
        {
            float halfWidth = 0.5f * p.TrailWidth;
            if (gTrails.gTrailsFlag & Use_TrailFlag_SizeAffectsWidth)
                halfWidth = halfWidth * p.SizeW_StartSize.x;
            
            
            // ------------------------------------- Color ----------------------------------------------------
            float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
            float4 trailColor = float4(1.0f, 1.0f, 1.0f, 1.0f); // 길이에 따른 색상.
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
            
            float3 look = normalize(g_Camera.g_CameraPosition - posTemp);
            float3 up = normalize(direction) * (gParticleRenderer.gLengthScale + length(direction) * gParticleRenderer.gSpeedScale);
            float3 right = normalize(cross(look, up));
            
            // ------------------------------------- Position -------------------------------------------------
            // ------------------------------------- TexUV ----------------------------------------------------
            float y = 0;
            y = CalculateTrailUV(gout[j].Tex.y, totalMoveDistance, p.TrailScrollSpeed.y);
            // ------------------------------------- TexUV ----------------------------------------------------
            
            
            gout[j].Color = color * trailColor;
            
            gout[j].Tex = float2(1 + p.TrailScrollSpeed.x * gCommonInfo.gParticlePlayTime, y);
            gout[j].PosH = mul(float4(posTemp - halfWidth * right - look * 0.1f, 1.0f), g_Camera.g_ViewProjectionMatrix);
            triStream.Append(gout[j]);
            
            gout[j].Tex = float2(0 + p.TrailScrollSpeed.x * gCommonInfo.gParticlePlayTime, y);
            gout[j].PosH = mul(float4(posTemp + halfWidth * right - look * 0.1f, 1.0f), g_Camera.g_ViewProjectionMatrix);
            triStream.Append(gout[j]);
        }
        
    }

}

//[maxvertexcount(30)]
//    void DrawTrailGS
//    (point ParticleVertexOut gin[1],
//	inout TriangleStream<GeoOut> triStream)
//{
    

//}