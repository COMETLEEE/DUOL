#include "Common.hlsli"

#define PT_EMITTER 0
#define PT_FLARE 1
#define PT_TRAIL 2

struct CommonInfo // 144
{
    float4x4 gTransformMatrix;
    float4x4 gDeltaMatrix;
    
    float2 gStartDelay; // 몇 초 뒤에 파티클이 재생될 지.					
    float2 gStartLifeTime; // 한 파티클의 생존 시간.						

    float2 gStartSpeed; // 파티클 생성시 시작 속도.
    float2 gStartRotation; // 파티클의 시작 회전.							

    float4 gStartSize; // 파티클의 시작 크기.							

    float4 gStartColor[2]; // 파티클의 시작 색상									

    float2 gGravityModifier; // 파티클에 가해지는 중력.
    float gParticlePlayTime; // 파티클 재생 후 흐른 시간.
    float gObjectID; // 파티클이 가지고 있는 오브젝트 ID					

    int gisLooping; // 반복여부.
    float gSimulationSpeed;
    float gDuration; // 몇 초 동안 파티클 객체가 재생될 지.					
    int gMaxParticles; // 파티클 최대 출력 사이즈.							

};
struct Emission // 16
{
    int gEmissiveCount; // 한번에 몇개를 방출 시킬지.

    float gEmissiveTime; // 다음 방출까지 걸리는 시간.

    float2 pad;
};
struct Shape // 64
{
    float gAngle;
    float gRadius;
    float gDonutRadius;
    float gArc;

    float3 gPosition;
    float pad0;

    float3 gRotation;
    float pad1;

    float3 gScale;
    float pad2;
};
struct Velocity_Over_LifeTime // 16
{
    float3 gVelocity; // 시간에 따른 파티클 속력.
    float pad;
    float3 gOrbital; // Orbital 궤도의 영향을 얼마나 받을 것 인가.
    float gConvertTime;
    float3 gOffset; // 궤도 중심의 Offset 값
    float pad2;
};
struct Force_over_LifeTime // 16
{
    float3 gForce;
    float pad;
};
struct Color_Over_LifeTime // 48
{
    float4 gAlpha_Ratio[8]; // 알파 값 비율.	16
    
    float4 gColor_Ratio[8]; // 컬러 비율.	32
};
struct Size_Over_Lifetime // 16
{
    float gStartSize;
    float gEndSize;
    float gStartOffset;
    float gEndOffset;
};
struct Rotation_Over_Lifetime // 16
{
    float gAngularVelocity;
    float3 pad;
};
struct Noise
{
    float gStregth;
    float gScrollSpeed;
    float gPositionAmount;
    float gRotationAmount;

    float gSizeAmount;
    float3 pad;
};
struct Collision
{
    int gPlaneCount;
    float gBoundce;
    float gLifeTimeLoss;
    float pad;
    
    float4 gPlanePosition[8];
    float4 gPlanNormalVec[8];
};
struct Texture_Sheet_Animation // 16
{
    int2 gGrid_XY;
    int gTimeMode;
    float pad;
};

struct Trails // 16
{
    float gRatio;
    float gLifeTime;
    float gMinimumVertexDistance;
    float gWidthOverTrail;

    int gTrailsFlag;
    int gTrailVertexCount;
    int2 pad;

    float4 gAlpha_Ratio_Lifetime[8];
    float4 gColor_Ratio_Lifetime[8];
    float4 gAlpha_Ratio_Trail[8];
    float4 gColor_Ratio_Trail[8];
};

struct Particle_Renderer // 16
{
    float gSpeedScale;
    float gLengthScale;
    uint gBlendType;
    float pad;
};
cbuffer CB_PerObject_Particle : register(b1)
{
    CommonInfo gCommonInfo;

    Emission gEmission;

    Shape gShape;

    Velocity_Over_LifeTime gVelocityOverLifetime;

    Force_over_LifeTime gForceOverLifeTime;

    Color_Over_LifeTime gColorOverLifetime;

    Size_Over_Lifetime gSizeOverLifetime;

    Rotation_Over_Lifetime gRotationOverLifetime;

    Noise gNoise;

    Collision gCollision;
    
    Texture_Sheet_Animation gTextureSheetAnimation;

    Trails gTrails;
    
    Particle_Renderer gParticleRenderer;

    int gParticleFlag;

    float3 pad10;
};

struct ParticleStruct // 16 바이트 정렬 필 수.
{
    float Type;
    float3 PosW;
    
    float4 VelW; // Start speed
    
    float4 SizeW_StartSize; // Start size
    float4 Age_LifeTime_Rotation_Gravity;
    
    float4 StartColor;
    float4 Color; // Start Color

    float2 QuadTexC[4];
    float4 InitEmitterPos;
    float4 PrevPos[15]; // Trail을 그리기 위해 일정 거리마다 위치를 기록한다. 
    
    float4 LatestPrevPos;
    
};
struct StreamOutParticle
{
    uint Type : TYPE; // 방출기인가
    uint VertexID : VERTEXID;
    
    float3 PosW : POSITION;
    float3 VelW : VELOCITY; // Start speed
    
    float4 SizeW_StartSize : SIZE_STARTSIZE; // Start size
    float4 Age_LifeTime_Rotation_Gravity : AGE_LIFETIME_ROTATION_GRAVITY;
    
    float4 StartColor : STARTCOLOR;
    float4 Color : COLOR; // Start Color

    float2 QuadTexC[4] : QUADTEX;
    float3 InitEmitterPos : EMITTERPOS;
    float3 PrevPos[15] : PREVPOS; // Trail을 그리기 위해 일정 거리마다 위치를 기록한다. 
    
    float3 LatestPrevPos : LASTESTPREVPOS;
};
struct ParticleVertexOut
{
    float3 PosW : POSITIONT;
    uint Type : TYPE;
    
    float3 LatestPrevPos : LASTESTPREVPOS;
    
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    
    float3 Age_LifeTime_Rotation : AGE_LIFETIME_ROTATION;
    
    float2 QuadTexC[4] : QUADTEX;
    float3 PrevPos[15] : PREVPOS;
};

struct VertexIDOut
{
    uint index : VERTEXID; // 방출기인가
};

void SetTextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4]);
void SetColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8],
 out float4 color);
void PushBackPrevPos(float3 prevPosIn[15], float3 posW, out float3 prevPosOut[15]);

void Orbital(float3 posW, float3 InitEmitterPos, float3 velW, float deltaTime, out float3 velW_Out, out float3 posW_Out);
void SetBillBoard(
float3 cameraPos, ParticleStruct particle,
out float3 look, out float3 right, out float3 up);
void CollisionCheck(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut);
// -------------------------------------------------- 전방 선언 -------------------------------

float3 TextureSample(Texture2D tex, SamplerState sam, float2 uv)
{
    return tex.SampleLevel(sam, uv, 0).xyz;
}

interface IParticleInterFace_Shape
{
    void Shape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW);
};
interface IParticleInterFace_VelocityOverLifeTime
{
    void VelocityOverLifeTime(StreamOutParticle particleIn, float deltaTime, out StreamOutParticle particelOut);
};
interface IParticleInterFace_ForceOverLifeTime
{
    void ForceOverLifeTime(float3 posW, float ratio, float deltaTime, out float3 posWOut);
};
interface IParticleInterFace_ColorOverLifeTime
{
    void ColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8], out float4 color);
};
interface IParticleInterFace_SizeOverLifeTime
{
    void SizeOverLifeTime(float3 ratio, out float2 size);
};
interface IParticleInterFace_RoationOverLifeTime
{
    void RoationOverLifeTime(float originRotate, float angularVelocity, float lifetime, float deltaTime, out float rotateOut);
};
interface IParticleInterFace_Noise
{
    void Noise(float2 sizeW, float3 posW, float rotW,
Texture2D noiseMap, SamplerState sam, float deltaTime, float gamePlayTime,
out float2 size, out float3 pos, out float rot);
};
interface IParticleInterFace_Collision
{
    void Collision(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut);
};
interface IParticleInterFace_TextureSheetAnimation
{
    void TextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4]);
};
interface IParticleInterFace_Trails
{
    void Trails(float3 prevPosIn[15], float3 posW, out float3 prevPosOut[15]);
};

class CShape : IParticleInterFace_Shape
{
    void Shape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW)
    {
        float speed = lerp(gCommonInfo.gStartSpeed[0], gCommonInfo.gStartSpeed[1], vunsignedRandom2.x);

        InitialPosW = float3(gCommonInfo.gTransformMatrix[3][0], gCommonInfo.gTransformMatrix[3][1], gCommonInfo.gTransformMatrix[3][2]); /*gCommonInfo.gEmitPosW.xyz;*/
        InitialVelW = speed * float3(gCommonInfo.gTransformMatrix[1][0], gCommonInfo.gTransformMatrix[1][1], gCommonInfo.gTransformMatrix[1][2]); /* gCommonInfo.gEmitDirW;*/
        
        if (gParticleFlag & Use_Shape_Sphere)
        {
            float4x4 temp = gCommonInfo.gTransformMatrix;
                            
            temp[3].xyz = float3(0, 0, 0);

            float3 vel = mul(float4((vRandom1.xyz).xyz, 1), temp).xyz;
            InitialVelW = speed * vel;

            float3 pos = mul(float4((vRandom1.xyz).xyz * gShape.gRadius, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = pos.xyz;
        }
        else if (gParticleFlag & Use_Shape_Hemisphere)
        {
            float4x4 temp = gCommonInfo.gTransformMatrix;

            temp[3].xyz = float3(0, 0, 0);

            float3 vel = mul(float4(vRandom1.x, abs(vRandom1.y), vRandom1.z, 1), temp).xyz;
            InitialVelW = speed * vel;

            float3 pos = mul(float4(vRandom1.x, abs(vRandom1.y), vRandom1.z, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = pos.xyz * gShape.gRadius;
        }
        else if (gParticleFlag & Use_Shape_Cone)
        {
            float a = cos(gShape.gAngle) / speed;
            float y = abs(sin(gShape.gAngle) / a);

            float TopRadius = (y + gShape.gRadius);

            float3 topPosition = mul(float4(((float3(0, speed, 0) + (float3(vRandom1.x, 0, vRandom1.z) * TopRadius))).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = mul(float4((float3(vRandom1.x, 0, vRandom1.z) * gShape.gRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialVelW = (topPosition - InitialPosW) /** gCommonInfo.gSimulationSpeed*/;

        }
        else if (gParticleFlag & Use_Shape_Donut)
        {
            float2 radiusVec = vRandom1.xy;
            radiusVec = normalize(radiusVec);
                           
            InitialPosW = mul(float4((float3(radiusVec.x, 0, radiusVec.y) * gShape.gRadius + vRandom2.xyz * gShape.gDonutRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialVelW = speed * vRandom5.xyz;
        }
        else if (gParticleFlag & Use_Shape_Box)
        {
            InitialPosW = mul(float4((vUnUnitRandom2.xyz).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
        }
        else if (gParticleFlag & Use_Shape_Circle)
        {
            InitialPosW = mul(float4((float3(vRandom1.x, 0, vRandom1.z) * gShape.gRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            
            float4x4 temp = gCommonInfo.gTransformMatrix;
            temp[3] = float4(0, 0, 0, 1.0f);
            InitialVelW = mul(float4(vRandom1.x, 0, vRandom1.z, 1.0f), temp).xyz * speed;
        }
        else if (gParticleFlag & Use_Shape_Rectangle)
        {
            InitialPosW = mul(float4((float3(vUnUnitRandom2.x, 0, vUnUnitRandom2.z)).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
        }

    }
};
class CNullShape : IParticleInterFace_Shape
{
    void Shape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW)
    {
        float speed = lerp(gCommonInfo.gStartSpeed[0], gCommonInfo.gStartSpeed[1], vunsignedRandom2.x);

        InitialPosW = float3(gCommonInfo.gTransformMatrix[3][0], gCommonInfo.gTransformMatrix[3][1], gCommonInfo.gTransformMatrix[3][2]); /*gCommonInfo.gEmitPosW.xyz;*/
        InitialVelW = speed * float3(gCommonInfo.gTransformMatrix[1][0], gCommonInfo.gTransformMatrix[1][1], gCommonInfo.gTransformMatrix[1][2]); /* gCommonInfo.gEmitDirW;*/
    }
};

class CVelocityOverLifeTime : IParticleInterFace_VelocityOverLifeTime
{
    void VelocityOverLifeTime(StreamOutParticle particleIn, float deltaTime, out StreamOutParticle particelOut)
    {
        float ratio = particleIn.Age_LifeTime_Rotation_Gravity.x / particleIn.Age_LifeTime_Rotation_Gravity.y;
        
        float3 velocity = float3(0, 0, 0);
        
        velocity = lerp(0, gVelocityOverLifetime.gVelocity, ratio);
            
        particleIn.PosW += velocity * deltaTime;
            
        Orbital(particleIn.PosW, particleIn.InitEmitterPos, particleIn.VelW, deltaTime, particleIn.VelW, particleIn.PosW);
        
        particelOut = particleIn;

    }
};
class CNullVelocityOverLifeTime : IParticleInterFace_VelocityOverLifeTime
{
    void VelocityOverLifeTime(StreamOutParticle particleIn, float deltaTime, out StreamOutParticle particelOut)
    {
        particelOut = particleIn;
    }
};

class CForceOverLifeTime : IParticleInterFace_ForceOverLifeTime
{
    void ForceOverLifeTime(float3 posW, float ratio, float deltaTime, out float3 posWOut)
    {
        float3 force = float3(0, 0, 0);
        
        force = lerp(0, gForceOverLifeTime.gForce, ratio);
        
        posWOut = posW + force * deltaTime;
    }
};
class CNullForceOverLifeTime : IParticleInterFace_ForceOverLifeTime
{
    void ForceOverLifeTime(float3 posW, float ratio, float deltaTime, out float3 posWOut)
    {
        posWOut = posW;
    }
};

class CColorOverLifeTime : IParticleInterFace_ColorOverLifeTime
{
    void ColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8], out float4 color)
    {
        SetColorOverLifeTime(ratio, gColorOverLifetime.gAlpha_Ratio, gColorOverLifetime.gColor_Ratio, color);
    }
};
class CNullColorOverLifeTime : IParticleInterFace_ColorOverLifeTime
{
    void ColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8], out float4 color)
    {
        color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
};

class CSizeOverLifeTime : IParticleInterFace_SizeOverLifeTime
{
    void SizeOverLifeTime(float3 ratio, out float2 size)
    {
        size = lerp(gSizeOverLifetime.gStartSize - gSizeOverLifetime.gStartOffset, gSizeOverLifetime.gEndSize + gSizeOverLifetime.gEndOffset, ratio).xy;
        size = clamp(size, 0, 1);
    }
};
class CNullSizeOverLifeTime : IParticleInterFace_SizeOverLifeTime
{
    void SizeOverLifeTime(float3 ratio, out float2 size)
    {
        size = float2(1.0f, 1.0f);
    }
};

class CRoationOverLifeTime : IParticleInterFace_RoationOverLifeTime
{
    void RoationOverLifeTime(float originRotate, float angularVelocity, float lifetime, float deltaTime, out float rotateOut)
    {
        rotateOut = originRotate + angularVelocity / lifetime * deltaTime;
    }
};
class CNullRoationOverLifeTime : IParticleInterFace_RoationOverLifeTime
{
    void RoationOverLifeTime(float originRotate, float angularVelocity, float lifetime, float deltaTime, out float rotateOut)
    {
        rotateOut = originRotate;
    }
};

class CNoise : IParticleInterFace_Noise
{
    void Noise(float2 sizeW, float3 posW, float rotW,
Texture2D noiseMap, SamplerState sam, float deltaTime, float gamePlayTime,
out float2 size, out float3 pos, out float rot)
    {
        float eps = 0.01f;
        float3 curl0;
        float3 curl1;
        float3 curl2;

        float noise;

        float scrollOffset = gNoise.gScrollSpeed * gamePlayTime;

    {
            float2 uv = float2(posW.x, posW.y);

            noise = TextureSample(noiseMap, sam, float2(uv.x, uv.y)).x;

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).x;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).x;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).x;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).x;

            float3 b = (n1 - n2) / (2 * eps);


            curl0 = float3(b.x, -a.x, 0);
        }
	{
            float2 uv = float2(posW.y, posW.z);

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).y;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).y;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).y;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).y;

            float3 b = (n1 - n2) / (2 * eps);


            curl1 = float3(0, b.x, -a.x);
        }
	{
            float2 uv = float2(posW.x, posW.z) / 100.0f;

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).z;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).z;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).z;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).z;

            float3 b = (n1 - n2) / (2 * eps);


            curl2 = float3(b.x, 0, -a.x);
        }


        float3 curl = (curl0 + curl1 + curl2) / 2.0f;

        pos = posW + curl * deltaTime * gNoise.gStregth * gNoise.gPositionAmount;

        size = sizeW + ((noise * 2) - 1) * deltaTime * gNoise.gSizeAmount * gNoise.gStregth * gNoise.gSizeAmount;

        rot = rotW + ((noise * 2) - 1) * deltaTime * gNoise.gRotationAmount * gNoise.gStregth * gNoise.gRotationAmount;
    }
};
class CNullNoise : IParticleInterFace_Noise
{
    void Noise(float2 sizeW, float3 posW, float rotW,
Texture2D noiseMap, SamplerState sam, float deltaTime, float gamePlayTime,
out float2 size, out float3 pos, out float rot)
    {
        size = sizeW;
        pos = posW;
        rot = rotW;
    }
};

class CCollision : IParticleInterFace_Collision
{
    void Collision(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut)
    {
        ageOut = age;
        [unroll]
        for (int i = 0; i < gCollision.gPlaneCount; i++)
        {
            float3 planeToPos = posW - gCollision.gPlanePosition[i].xyz;
        
            float t = dot(planeToPos, gCollision.gPlanNormalVec[i].xyz);

            if (t <= 0) // 충돌!
            {

                velW = gCollision.gBoundce * 2 * dot(-velW, gCollision.gPlanNormalVec[i].xyz) * gCollision.gPlanNormalVec[i].xyz + velW;
                //velW = reflect(velW, (gCollision.gPlanNormalVec[i].xyz * gCollision.gBoundce));
                posW += velW * deltaTime;
            
                planeToPos = posW - gCollision.gPlanePosition[i].xyz;
        
                t = dot(planeToPos, gCollision.gPlanNormalVec[i].xyz);
            
                if (t <= 0)
                {
                    posW += dot(-planeToPos, gCollision.gPlanNormalVec[i].xyz) * gCollision.gPlanNormalVec[i].xyz;
                }
            
                ageOut = age + lifeTime * gCollision.gLifeTimeLoss;

            }
        }
        
        velWout = velW;
        posWout = posW;
    }
};
class CNullCollision : IParticleInterFace_Collision
{
    void Collision(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut)
    {
        velWout = velW;
        posWout = posW;
        ageOut = age;
    }
};

class CTextureSheetAnimation : IParticleInterFace_TextureSheetAnimation
{
    void TextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4])
    {
        int2 texIndex;
        texIndex.x = lerp(1, gTextureSheetAnimation.gGrid_XY[0] + 1, vunsignedRandom4.x);
        texIndex.y = lerp(1, gTextureSheetAnimation.gGrid_XY[1] + 1, vunsignedRandom4.y);

        float xStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.x;
        float yStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.y;
        float xMax = xStep * texIndex.x;
        float yMax = yStep * texIndex.y;
        float xMin = xStep * (texIndex.x - 1);
        float yMin = yStep * (texIndex.y - 1);

        QuadTexC[0] = float2(xMin, yMax); // lefttop
        QuadTexC[1] = float2(xMin, yMin); //leftbottom
        QuadTexC[2] = float2(xMax, yMax); // righttop
        QuadTexC[3] = float2(xMax, yMin); // ritghtbottom

    }
};
class CNullTextureSheetAnimation : IParticleInterFace_TextureSheetAnimation
{
    void TextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4])
    {
        QuadTexC[0] = float2(0.0f, 1.0f); // lefttop
        QuadTexC[1] = float2(0.0f, 0.0f); //leftbottom
        QuadTexC[2] = float2(1.0f, 1.0f); // righttop
        QuadTexC[3] = float2(1.0f, 0.0f); // ritghtbottom
    }
};

class CTrails : IParticleInterFace_Trails
{
    void Trails(float3 prevPosIn[15], float3 posW, out float3 prevPosOut[15])
    {
        
           [unroll]
        for (int i = 0; i < 15; i++)
        {
            if (!(gTrails.gTrailsFlag & Use_TrailFlag_WorldSpace) && !(gParticleFlag & Use_Commoninfo_WorldSpace))
                prevPosIn[i] = mul(float4(prevPosIn[i], 1.0f), gCommonInfo.gDeltaMatrix).xyz;
        }
        
        if (distance(prevPosIn[0], posW) >= gTrails.gMinimumVertexDistance)
        {
                [unroll]
            for (int i = 13; i >= 0; i--)
            {
                prevPosIn[i + 1] = prevPosIn[i];
            }
            prevPosIn[0] = posW;

        }
        prevPosOut = prevPosIn;
    }
};
class CNullTrails : IParticleInterFace_Trails
{
    void Trails(float3 prevPosIn[15], float3 posW, out float3 prevPosOut[15])
    {
        prevPosIn = prevPosOut;
    }
};

void ManualShape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW)
{
    if (gParticleFlag & Use_Shape)
    {
        float speed = lerp(gCommonInfo.gStartSpeed[0], gCommonInfo.gStartSpeed[1], vunsignedRandom2.x);

        InitialPosW = float3(gCommonInfo.gTransformMatrix[3][0], gCommonInfo.gTransformMatrix[3][1], gCommonInfo.gTransformMatrix[3][2]); /*gCommonInfo.gEmitPosW.xyz;*/
        InitialVelW = speed * float3(gCommonInfo.gTransformMatrix[1][0], gCommonInfo.gTransformMatrix[1][1], gCommonInfo.gTransformMatrix[1][2]); /* gCommonInfo.gEmitDirW;*/
        
        if (gParticleFlag & Use_Shape_Sphere)
        {
            float4x4 temp = gCommonInfo.gTransformMatrix;
                            
            temp[3].xyz = float3(0, 0, 0);

            float3 vel = mul(float4((vRandom1.xyz).xyz, 1), temp).xyz;
            InitialVelW = speed * vel;

            float3 pos = mul(float4((vRandom1.xyz).xyz * gShape.gRadius, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = pos.xyz;
        }
        else if (gParticleFlag & Use_Shape_Hemisphere)
        {
            float4x4 temp = gCommonInfo.gTransformMatrix;

            temp[3].xyz = float3(0, 0, 0);

            float3 vel = mul(float4(vRandom1.x, abs(vRandom1.y), vRandom1.z, 1), temp).xyz;
            InitialVelW = speed * vel;

            float3 pos = mul(float4(vRandom1.x, abs(vRandom1.y), vRandom1.z, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = pos.xyz * gShape.gRadius;
        }
        else if (gParticleFlag & Use_Shape_Cone)
        {
            float a = cos(gShape.gAngle) / speed;
            float y = abs(sin(gShape.gAngle) / a);

            float TopRadius = (y + gShape.gRadius);

            float3 topPosition = mul(float4(((float3(0, speed, 0) + (float3(vRandom1.x, 0, vRandom1.z) * TopRadius))).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialPosW = mul(float4((float3(vRandom1.x, 0, vRandom1.z) * gShape.gRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialVelW = (topPosition - InitialPosW) /** gCommonInfo.gSimulationSpeed*/;

        }
        else if (gParticleFlag & Use_Shape_Donut)
        {
            float2 radiusVec = vRandom1.xy;
            radiusVec = normalize(radiusVec);
                           
            InitialPosW = mul(float4((float3(radiusVec.x, 0, radiusVec.y) * gShape.gRadius + vRandom2.xyz * gShape.gDonutRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            InitialVelW = speed * vRandom5.xyz;
        }
        else if (gParticleFlag & Use_Shape_Box)
        {
            InitialPosW = mul(float4((vUnUnitRandom2.xyz).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
        }
        else if (gParticleFlag & Use_Shape_Circle)
        {
            InitialPosW = mul(float4((float3(vRandom1.x, 0, vRandom1.z) * gShape.gRadius).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
            
            float4x4 temp = gCommonInfo.gTransformMatrix;
            temp[3] = float4(0, 0, 0, 1.0f);
            InitialVelW = mul(float4(vRandom1.x, 0, vRandom1.z, 1.0f), temp).xyz * speed;
        }
        else if (gParticleFlag & Use_Shape_Rectangle)
        {
            InitialPosW = mul(float4((float3(vUnUnitRandom2.x, 0, vUnUnitRandom2.z)).xyz, 1), gCommonInfo.gTransformMatrix).xyz;
        }
    }
    else
    {
        float speed = lerp(gCommonInfo.gStartSpeed[0], gCommonInfo.gStartSpeed[1], vunsignedRandom2.x);

        InitialPosW = float3(gCommonInfo.gTransformMatrix[3][0], gCommonInfo.gTransformMatrix[3][1], gCommonInfo.gTransformMatrix[3][2]); /*gCommonInfo.gEmitPosW.xyz;*/
        InitialVelW = speed * float3(gCommonInfo.gTransformMatrix[1][0], gCommonInfo.gTransformMatrix[1][1], gCommonInfo.gTransformMatrix[1][2]); /* gCommonInfo.gEmitDirW;*/
    }

}
void ManualVelocityOverLifeTime(ParticleStruct particleIn, float deltaTime, out ParticleStruct particelOut)
{
    if (gParticleFlag & Use_Velocity_Over_Lifetime)
    {
        float ratio = particleIn.Age_LifeTime_Rotation_Gravity.x / particleIn.Age_LifeTime_Rotation_Gravity.y;
        
        float3 velocity = float3(0, 0, 0);
        
        velocity = lerp(0, gVelocityOverLifetime.gVelocity, ratio);
            
        particleIn.PosW += velocity * deltaTime;
            
        Orbital(particleIn.PosW, particleIn.InitEmitterPos.xyz, particleIn.VelW.xyz, deltaTime, particleIn.VelW.xyz, particleIn.PosW);
    }

    particelOut = particleIn;
}
void ManualForceOverLifeTime(float3 posW, float ratio, float deltaTime, out float3 posWOut)
{
    if (gParticleFlag & Use_Force_over_Lifetime)
    {
        float3 force = float3(0, 0, 0);
        
        force = lerp(0, gForceOverLifeTime.gForce, ratio);
        
        posWOut = posW + force * deltaTime;
    }
    else
    {
        posWOut = posW;
    }

}
void ManualColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8], out float4 color)
{
    if (gParticleFlag & Use_Color_over_Lifetime)
    {
        SetColorOverLifeTime(ratio, gColorOverLifetime.gAlpha_Ratio, gColorOverLifetime.gColor_Ratio, color);
    }
    else
    {
        color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }

}
void ManualSizeOverLifeTime(float3 ratio, out float2 size)
{
    if (gParticleFlag & Use_Size_over_Lifetime)
    {
        size = lerp(gSizeOverLifetime.gStartSize - gSizeOverLifetime.gStartOffset, gSizeOverLifetime.gEndSize + gSizeOverLifetime.gEndOffset, ratio).xy;
        size = clamp(size, 0, 1);
    }
    else
    {
        size = float2(1.0f, 1.0f);
    }

}
void ManualRoationOverLifeTime(float originRotate, float angularVelocity, float lifetime, float deltaTime, out float rotateOut)
{
    if (gParticleFlag & Use_Rotation_over_Lifetime)
    {
        rotateOut = originRotate + angularVelocity / lifetime * deltaTime;
    }
    else
    {
        rotateOut = originRotate;
    }
}
void ManualNoise(float2 sizeW, float3 posW, float rotW,
Texture2D noiseMap, SamplerState sam, float deltaTime, float gamePlayTime,
out float2 size, out float3 pos, out float rot)
{
    if (gParticleFlag & Use_Noise)
    {
        float eps = 0.01f;
        float3 curl0;
        float3 curl1;
        float3 curl2;

        float noise;

        float scrollOffset = gNoise.gScrollSpeed * gamePlayTime;

    {
            float2 uv = float2(posW.x, posW.y);

            noise = TextureSample(noiseMap, sam, float2(uv.x, uv.y)).x;

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).x;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).x;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).x;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).x;

            float3 b = (n1 - n2) / (2 * eps);


            curl0 = float3(b.x, -a.x, 0);
        }
	{
            float2 uv = float2(posW.y, posW.z);

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).y;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).y;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).y;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).y;

            float3 b = (n1 - n2) / (2 * eps);


            curl1 = float3(0, b.x, -a.x);
        }
	{
            float2 uv = float2(posW.x, posW.z) / 100.0f;

            uv.y += scrollOffset;

            float3 n1 = TextureSample(noiseMap, sam, float2(uv.x + eps, uv.y)).z;
            float3 n2 = TextureSample(noiseMap, sam, float2(uv.x - eps, uv.y)).z;

            float3 a = (n1 - n2) / (2 * eps);

            n1 = TextureSample(noiseMap, sam, float2(uv.x, uv.y + eps)).z;
            n2 = TextureSample(noiseMap, sam, float2(uv.x, uv.y - eps)).z;

            float3 b = (n1 - n2) / (2 * eps);


            curl2 = float3(b.x, 0, -a.x);
        }


        float3 curl = (curl0 + curl1 + curl2) / 2.0f;

        pos = posW + curl * deltaTime * gNoise.gStregth * gNoise.gPositionAmount;

        size = sizeW + ((noise * 2) - 1) * deltaTime * gNoise.gSizeAmount * gNoise.gStregth * gNoise.gSizeAmount;

        rot = rotW + ((noise * 2) - 1) * deltaTime * gNoise.gRotationAmount * gNoise.gStregth * gNoise.gRotationAmount;
        
    }
    else
    {
        size = sizeW;
        pos = posW;
        rot = rotW;
    }
}
void ManualCollision(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut)
{
    if (gParticleFlag & Use_Collision)
    {
        ageOut = age;
        [unroll]
        for (int i = 0; i < gCollision.gPlaneCount; i++)
        {
            float3 planeToPos = posW - gCollision.gPlanePosition[i].xyz;
        
            float t = dot(planeToPos, gCollision.gPlanNormalVec[i].xyz);

            if (t <= 0) // 충돌!
            {

                velW = gCollision.gBoundce * 2 * dot(-velW, gCollision.gPlanNormalVec[i].xyz) * gCollision.gPlanNormalVec[i].xyz + velW;
                //velW = reflect(velW, (gCollision.gPlanNormalVec[i].xyz * gCollision.gBoundce));
                posW += velW * deltaTime;
            
                planeToPos = posW - gCollision.gPlanePosition[i].xyz;
        
                t = dot(planeToPos, gCollision.gPlanNormalVec[i].xyz);
            
                if (t <= 0)
                {
                    posW += dot(-planeToPos, gCollision.gPlanNormalVec[i].xyz) * gCollision.gPlanNormalVec[i].xyz;
                }
            
                ageOut = age + lifeTime * gCollision.gLifeTimeLoss;

            }
        }
        
        velWout = velW;
        posWout = posW;
    }
    else
    {
        velWout = velW;
        posWout = posW;
        ageOut = age;
    }

}
void ManualTextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4])
{
    if (gParticleFlag & Use_Texture_Sheet_Animation)
    {
        int2 texIndex;
        texIndex.x = lerp(1, gTextureSheetAnimation.gGrid_XY[0] + 1, vunsignedRandom4.x);
        texIndex.y = lerp(1, gTextureSheetAnimation.gGrid_XY[1] + 1, vunsignedRandom4.y);

        float xStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.x;
        float yStep = 1.0f / (float) gTextureSheetAnimation.gGrid_XY.y;
        float xMax = xStep * texIndex.x;
        float yMax = yStep * texIndex.y;
        float xMin = xStep * (texIndex.x - 1);
        float yMin = yStep * (texIndex.y - 1);

        QuadTexC[0] = float2(xMin, yMax); // lefttop
        QuadTexC[1] = float2(xMin, yMin); //leftbottom
        QuadTexC[2] = float2(xMax, yMax); // righttop
        QuadTexC[3] = float2(xMax, yMin); // ritghtbottom
    }
    else
    {
        QuadTexC[0] = float2(0.0f, 1.0f); // lefttop
        QuadTexC[1] = float2(0.0f, 0.0f); //leftbottom
        QuadTexC[2] = float2(1.0f, 1.0f); // righttop
        QuadTexC[3] = float2(1.0f, 0.0f); // ritghtbottom
    }

}
void ManualTrail(float4 prevPosIn[15], float3 posW, out float4 prevPosOut[15])
{
    if (gParticleFlag & Use_Trails)
    {
        
        if (!(gTrails.gTrailsFlag & Use_TrailFlag_WorldSpace) && !(gParticleFlag & Use_Commoninfo_WorldSpace))
        {
                [unroll]
            for (int i = 0; i < 15; i++)
            {
                prevPosIn[i].w = 1.0f;
                prevPosIn[i] = mul(prevPosIn[i], gCommonInfo.gDeltaMatrix);
            }
        }
        
        
        if (distance(prevPosIn[0].xyz, posW) >= gTrails.gMinimumVertexDistance)
        {
                [unroll]
            for (int i = 13; i >= 0; i--)
            {
                prevPosIn[i + 1] = prevPosIn[i];
            }
            prevPosIn[0] = float4(posW, 1.0f);
        }
        prevPosOut = prevPosIn;
    }
    else
    {
        prevPosIn = prevPosOut;
    }
}




IParticleInterFace_Shape g_shapeInstance; // 0
IParticleInterFace_VelocityOverLifeTime g_velocityOverLifeTimeInstance; // 1
IParticleInterFace_ForceOverLifeTime g_forceOverLifeTimeInstance; // 2
IParticleInterFace_ColorOverLifeTime g_colorOverLifeTimeInstance; // 3
IParticleInterFace_SizeOverLifeTime g_sizeOverLifeTimeInstance; // 4
IParticleInterFace_RoationOverLifeTime g_roationOverLifeTimeInstance; // 5
IParticleInterFace_Noise g_noiseInstance; // 6
IParticleInterFace_Collision g_collisionInstance; // 7
IParticleInterFace_TextureSheetAnimation g_textureSheetAnimationInstance; // 8
IParticleInterFace_Trails g_trails; // 9

void SetColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8],
 out float4 color)
{
    float4 startColor = float4(color_Ratio[0].xyz, alpha_Ratio[0].x);
    float4 endColor = float4(color_Ratio[0].xyz, alpha_Ratio[0].x);
    float startTime = 0.0f;
    float endTime = 1.0f;
    
    color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
		[unroll]
    for (int i = 0; i < 8; i++)
    {
        if (ratio < alpha_Ratio[i].w)
        {
            endColor.a = alpha_Ratio[i].x;
            endTime = alpha_Ratio[i].w;

            if (i != 0)
            {
                startColor.a = alpha_Ratio[i - 1].x;
                startTime = alpha_Ratio[i - 1].w;
            }
            else
            {
                startColor.a = alpha_Ratio[i].x;
                startTime = 0.0f;
            }
            break;
        }
        else if (1.0f <= alpha_Ratio[i].w)
        {
            startColor.a = alpha_Ratio[i - 1].x;
            startTime = alpha_Ratio[i - 1].w;
            endColor.a = alpha_Ratio[i].x;
            endTime = 1.0f + 0.00001f;
            break;
        }
        else if (i == 7)
        {
            startColor.a = alpha_Ratio[i].x;
            startTime = alpha_Ratio[i].w;
            endColor.a = alpha_Ratio[i].x;
            endTime = 1.0f + 0.00001f;
        }
    }
    float factor = 1.0f / (endTime - startTime);
    factor = (ratio - startTime) * factor;
    color.a = lerp(startColor.a, endColor.a, factor);

    	[unroll]
    for (i = 0; i < 8; i++)
    {
        if (ratio < color_Ratio[i].w)
        {
            endColor.xyz = color_Ratio[i].xyz;
            endTime = color_Ratio[i].w;

            if (i != 0)
            {
                startColor.xyz = color_Ratio[i - 1].xyz;
                startTime = color_Ratio[i - 1].w;
            }
            else
            {
                startColor.xyz = color_Ratio[i].xyz;
                startTime = 0.0f;
            }
            break;
        }
        else if (1.0f <= color_Ratio[i].w)
        {
            startColor.xyz = color_Ratio[i - 1].xyz;
            startTime = color_Ratio[i - 1].w;
            endColor.xyz = color_Ratio[i].xyz;
            endTime = 1.0f + 0.00001f;
            break;
        }
        else if (i == 7)
        {
            startColor.xyz = color_Ratio[i].xyz;
            startTime = color_Ratio[i].w;
            endColor.xyz = color_Ratio[i].xyz;
            endTime = 1.0f + 0.00001f;
        }
    }
    factor = 1.0f / (endTime - startTime);
    factor = (ratio - startTime) * factor;
    color.xyz = lerp(startColor.xyz, endColor.xyz, factor);
}


void Orbital(float3 posW, float3 InitEmitterPos, float3 velW, float deltaTime, out float3 velW_Out, out float3 posW_Out)
{
    float3 axis = gVelocityOverLifetime.gOrbital;
    
    if (length(axis) == 0 || !(gParticleFlag & Use_Velocity_Over_Lifetime))
    {
    }
    else
    {
        float3 n_axis = normalize(axis);
    
        float3 orbitalCenterPos = dot(posW, n_axis) * n_axis + InitEmitterPos; // 파티클이 향하는 방향!

        float3 offset = posW - InitEmitterPos;

        float power = length(axis);
    
        float angle = deltaTime * power;
    
        float4 rotateQuat = rotate_angle_axis(angle, n_axis);
    
        float3 rotated = rotate_vector(offset, rotateQuat);
    
        velW = rotate_vector(velW, rotateQuat);
    
        posW = rotated + InitEmitterPos;
    
    // angle == t;
        float3 graivtDir = orbitalCenterPos - posW;
    
        float3 n_graivtDir = normalize(graivtDir) * power;
   
        velW += n_graivtDir * deltaTime;
    }
    velW_Out = velW;
    posW_Out = posW;

}

void SetBillBoard(
float3 cameraPos, ParticleStruct particle,
out float3 look, out float3 right, out float3 up)
{
    if (gParticleFlag & Use_Renderer_BillBoard)
    {
		// 카메라 방향으로 룩엣
        look = normalize(cameraPos - particle.PosW);
        right = normalize(cross(float3(0, 1, 0), look));
        up = normalize(cross(look, right));
    }
    else if (gParticleFlag & Use_Renderer_StretchedBillBoard)
    {
        float3 direction = particle.PosW - particle.LatestPrevPos.xyz;
            
        look = normalize(cameraPos - particle.PosW);
        up = normalize(direction) * (gParticleRenderer.gLengthScale + length(direction) * gParticleRenderer.gSpeedScale);
        right = normalize(cross(up, look));
    }
    else if (gParticleFlag & Use_Renderer_HorizontalBillBoard)
    {
        look = float3(0, 1, 0);
        right = float3(-1, 0, 0);
        up = float3(0, 0, 1);
    }
    else if (gParticleFlag & Use_Renderer_VerticalBillBoard)
    {
        look = cameraPos - particle.PosW;
        look.y = 0;
        look = normalize(look);

        right = normalize(cross(float3(0, 1, 0), look));
        up = float3(0, 1, 0);
    }
    else if (gParticleFlag & Use_Renderer_Mesh)
    {
            // 어떻게 구현하는걸까...
    }
    else
    {
        // 카메라 방향으로 룩엣
        look = normalize(cameraPos - particle.PosW);
        right = normalize(cross(float3(0, 1, 0), look));
        up = normalize(cross(look, right));
    }
    
    const float costheta = cos(particle.Age_LifeTime_Rotation_Gravity.z);
    const float sintheta = sin(particle.Age_LifeTime_Rotation_Gravity.z);
    const float OneMinusCos = 1.0f - costheta;
        
    const float X2 = pow(look.x, 2);
    const float Y2 = pow(look.y, 2);
    const float Z2 = pow(look.z, 2);

    float4x4 RotationTM;
        
    RotationTM[0] = float4(X2 * OneMinusCos + costheta, look.x * look.y * OneMinusCos + look.z * sintheta, look.x * look.z * OneMinusCos - look.y * sintheta, 0);
    RotationTM[1] = float4(look.x * look.y * OneMinusCos - look.z * sintheta, Y2 * OneMinusCos + costheta, look.y * look.z * OneMinusCos + look.x * sintheta, 0);
    RotationTM[2] = float4(look.x * look.z * OneMinusCos + look.y * sintheta, look.y * look.z * OneMinusCos - look.x * sintheta, Z2 * OneMinusCos + costheta, 0);
    RotationTM[3] = float4(0, 0, 0, 1);

    right = mul(float4(right, 1.0f), RotationTM).xyz;
    up = mul(float4(up, 1.0f), RotationTM).xyz;
}
