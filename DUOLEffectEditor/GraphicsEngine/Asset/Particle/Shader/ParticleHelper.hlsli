#include "Common.hlsli"
struct CommonInfo // 144
{
    float4x4 gTransformMatrix;
    float4x4 gDeltaMatrix;
    
    float2 gStartDelay; // �� �� �ڿ� ��ƼŬ�� ����� ��.					
    float2 gStartLifeTime; // �� ��ƼŬ�� ���� �ð�.						

    float2 gStartSpeed; // ��ƼŬ ������ ���� �ӵ�.
    float2 gStartRotation; // ��ƼŬ�� ���� ȸ��.							

    float4 gStartSize; // ��ƼŬ�� ���� ũ��.							

    float4 gStartColor[2]; // ��ƼŬ�� ���� ����									

    float2 gGravityModifier; // ��ƼŬ�� �������� �߷�.
    float gParticlePlayTime; // ��ƼŬ ��� �� �帥 �ð�.
    float gObjectID; // ��ƼŬ�� ������ �ִ� ������Ʈ ID					

    int gisLooping; // �ݺ�����.
    float gSimulationSpeed;
    float gDuration; // �� �� ���� ��ƼŬ ��ü�� ����� ��.					
    int gMaxParticles; // ��ƼŬ �ִ� ��� ������.							

};
struct Emission // 16
{
    int gEmissiveCount; // �ѹ��� ��� ���� ��ų��.

    float gEmissiveTime; // ���� ������� �ɸ��� �ð�.

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
    float3 gVelocity; // �ð��� ���� ��ƼŬ �ӷ�.
    float pad;
    float3 gOrbital; // Orbital �˵��� ������ �󸶳� ���� �� �ΰ�.
    float gConvertTime;
    float3 gOffset; // �˵� �߽��� Offset ��
    float pad2;
};
struct Force_over_LifeTime // 16
{
    float3 gForce;
    float pad;
};
struct Color_Over_LifeTime // 48
{
    float4 gAlpha_Ratio[8]; // ���� �� ����.	16
    
    float4 gColor_Ratio[8]; // �÷� ����.	32
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
    float2 pad;
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
struct StreamOutParticle
{
    float3 PosW : POSITION;
    float3 VelW : VELOCITY; // Start speed
    float4 SizeW_StartSize : SIZE_STARTSIZE; // Start size
    float4 Age_LifeTime_Rotation_Gravity : AGE_LIFETIME_ROTATION_GRAVITY;
    
    uint Type : TYPE; // ������ΰ�
    uint VertexID : VERTEXID;
    
    float4 StartColor : STARTCOLOR;
    float4 Color : COLOR; // Start Color

    float2 QuadTexC[4] : QUADTEX;
    float3 InitEmitterPos : EMITTERPOS;
    float3 PrevPos[15] : PREVPOS; // Trail�� �׸��� ���� ���� �Ÿ����� ��ġ�� ����Ѵ�. 
    
    float3 LatestPrevPos : LASTESTPREVPOS;
    
};
struct ParticleVertexOut
{
    float3 PosW : POSITIONT;
    float3 LatestPrevPos : LASTESTPREVPOS;
    
    float2 SizeW : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
    
    float3 Age_LifeTime_Rotation : AGE_LIFETIME_ROTATION;
    
    float2 QuadTexC[4] : QUADTEX;
    float3 PrevPos[15] : PREVPOS;
};


void SetShape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW);
void SetTextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4]);
void SetColorOverLifeTime(float ratio
, out float4 color);
void PushBackPrevPos(float3 prevPosIn[15], float3 posW, out float3 prevPosOut[15]);

void Orbital(StreamOutParticle particleIn, float deltaTime, out StreamOutParticle particelOut);
void SetBillBoard(
float3 cameraPos, ParticleVertexOut particle,
out float3 look, out float3 right, out float3 up);
void CollisionCheck(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut);
// -------------------------------------------------- ���� ���� -------------------------------

void SetShape(float4 vRandom1, float4 vRandom2, float4 vRandom5, float4 vunsignedRandom2, float4 vUnUnitRandom2
, out float3 InitialPosW, out float3 InitialVelW)
{
    float speed = lerp(gCommonInfo.gStartSpeed[0], gCommonInfo.gStartSpeed[1], vunsignedRandom2.x);

    InitialPosW = float3(gCommonInfo.gTransformMatrix[3][0], gCommonInfo.gTransformMatrix[3][1], gCommonInfo.gTransformMatrix[3][2]); /*gCommonInfo.gEmitPosW.xyz;*/
    InitialVelW = speed * float3(gCommonInfo.gTransformMatrix[1][0], gCommonInfo.gTransformMatrix[1][1], gCommonInfo.gTransformMatrix[1][2]); /* gCommonInfo.gEmitDirW;*/

    if (gParticleFlag & Use_Shape)
    {
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

            float3 topPosition = mul(float4(((float3(0, speed, 0) + (float3(vRandom1.x, 0, vRandom1.z) * TopRadius))).xyz, 1), gCommonInfo.gTransformMatrix);
            InitialPosW = mul(float4((float3(vRandom1.x, 0, vRandom1.z) * gShape.gRadius).xyz, 1), gCommonInfo.gTransformMatrix);
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
}

void SetTextureSheetAnimation(float4 vunsignedRandom4, out float2 QuadTexC[4])
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

void SetColorOverLifeTime(float ratio, float4 alpha_Ratio[8], float4 color_Ratio[8],
 out float4 color)
{
    float4 startColor = (1.0f, 1.0f, 1.0f, 1.0f);
    float4 endColor = (1.0f, 1.0f, 1.0f, 1.0f);
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
    for (int i = 0; i < 8; i++)
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

void PushFrontPrevPos(float3 prevPosIn[15], float3 posW, float4x4 deltaTM, out float3 prevPosOut[15])
{
    [unroll]
    for (int i = 13; i >= 0; i--)
    {
        if (!(gTrails.gTrailsFlag & Use_TrailFlag_WorldSpace) && !(gParticleFlag & Use_Commoninfo_WorldSpace))
            prevPosIn[i] = mul(float4(prevPosIn[i], 1.0f), deltaTM);
        prevPosIn[i + 1] = prevPosIn[i];
    }
    prevPosIn[0] = posW;

    prevPosOut = prevPosIn;

}

void Orbital(StreamOutParticle particleIn, float deltaTime, out StreamOutParticle particelOut)
{
    float3 axis = gVelocityOverLifetime.gOrbital;
    
    if (length(axis) == 0 || !(gParticleFlag & Use_Velocity_Over_Lifetime))
    {
        particelOut = particleIn;
        return;
    }
    
    float3 n_axis = normalize(axis);
    
    float3 orbitalCenterPos = dot(particleIn.PosW, n_axis) * n_axis + particleIn.InitEmitterPos; // ��ƼŬ�� ���ϴ� ����!

    float3 offset = particleIn.PosW - particleIn.InitEmitterPos;

    float power = length(axis);
    
    float angle = deltaTime * power;
    
    float4 rotateQuat = rotate_angle_axis(angle, n_axis);
    
    float3 rotated = rotate_vector(offset, rotateQuat);
    
    particleIn.VelW = rotate_vector(particleIn.VelW, rotateQuat);
    
    particleIn.PosW = rotated + particleIn.InitEmitterPos;
    
    // angle == t;
    float3 graivtDir = orbitalCenterPos - particleIn.PosW;
    
    float3 n_graivtDir = normalize(graivtDir) * power;
   
    particleIn.VelW += n_graivtDir * deltaTime;
    
    particelOut = particleIn;
}

void SetBillBoard(
float3 cameraPos, ParticleVertexOut particle,
out float3 look, out float3 right, out float3 up)
{
    if (gParticleFlag & Use_Renderer_BillBoard)
    {
		// ī�޶� �������� �迧
        look = normalize(cameraPos - particle.PosW);
        right = normalize(cross(float3(0, 1, 0), look));
        up = normalize(cross(look, right));
    }
    else if (gParticleFlag & Use_Renderer_StretchedBillBoard)
    {
        float3 direction = particle.PosW - particle.LatestPrevPos;
            
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
            // ��� �����ϴ°ɱ�...
    }
    else
    {
        // ī�޶� �������� �迧
        look = normalize(cameraPos - particle.PosW);
        right = normalize(cross(float3(0, 1, 0), look));
        up = normalize(cross(look, right));
    }
    
    const float costheta = cos(particle.Age_LifeTime_Rotation.z);
    const float sintheta = sin(particle.Age_LifeTime_Rotation.z);
    const float OneMinusCos = 1.0f - costheta;
        
    const float X2 = pow(look.x, 2);
    const float Y2 = pow(look.y, 2);
    const float Z2 = pow(look.z, 2);

    float4x4 RotationTM;
        
    RotationTM[0] = float4(X2 * OneMinusCos + costheta, look.x * look.y * OneMinusCos + look.z * sintheta, look.x * look.z * OneMinusCos - look.y * sintheta, 0);
    RotationTM[1] = float4(look.x * look.y * OneMinusCos - look.z * sintheta, Y2 * OneMinusCos + costheta, look.y * look.z * OneMinusCos + look.x * sintheta, 0);
    RotationTM[2] = float4(look.x * look.z * OneMinusCos + look.y * sintheta, look.y * look.z * OneMinusCos - look.x * sintheta, Z2 * OneMinusCos + costheta, 0);
    RotationTM[3] = float4(0, 0, 0, 1);

    right = mul(float4(right, 1.0f), RotationTM);
    up = mul(float4(up, 1.0f), RotationTM);
}

float3 TextureSample(Texture2D tex, SamplerState sam, float2 uv)
{
    return tex.SampleLevel(sam, uv, 0);
}


void NoiseSetSRT(float2 sizeW, float3 posW, float rotW,
Texture2D noiseMap, SamplerState sam, float deltaTime, float gamePlayTime,
out float2 size,
out float3 pos,
out float rot)
{
    float eps = 0.001f;
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

void CollisionCheck(float3 posW, float3 velW, float deltaTime, float age, float lifeTime, out float3 posWout, out float3 velWout, out float ageOut)
{
    [unroll]
    for (int i = 0; i < gCollision.gPlaneCount; i++)
    {
        float3 planeToPos = posW - gCollision.gPlanePosition[i].xyz;
        
        float t = dot(planeToPos, gCollision.gPlanNormalVec[i].xyz);

        if (t <= 0) // �浹!
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