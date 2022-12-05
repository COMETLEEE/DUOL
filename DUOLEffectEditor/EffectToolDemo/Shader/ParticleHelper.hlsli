
struct CommonInfo
{
    float3 gEmitDirW;
    float gDuration; // 몇 초 동안 파티클 객체가 재생될 지.					

    float3 gEmitPosW;
    int gMaxParticles; // 파티클 최대 출력 사이즈.							

    float2 gStartDelay; // 몇 초 뒤에 파티클이 재생될 지.					
    float2 gStartLifeTime; // 한 파티클의 생존 시간.						

    float2 gStartSpeed; // 파티클 생성시 시작 속도.
    float2 gStartRotation; // 파티클의 시작 회전.							

    float4 gStartSize; // 파티클의 시작 크기.							

    float4 gStartColor[2]; // 파티클의 시작 색상									

    float2 gGravityModifier; // 파티클에 가해지는 중력.
    float gParticlePlayTime; // 파티클 재생 후 흐른 시간.
    float pad4; // 파티클에 가해지는 중력.						

    int gisLooping; // 반복여부.
    int3 pad3; // 시작인가요 ..?						
};
struct Emission
{
    float gEmissiveCount; // 한번에 몇개를 방출 시킬지.

    float gEmissiveTime; // 다음 방출까지 걸리는 시간.

    float2 pad;
};
struct Color_Over_LifeTime
{
    float4 gStartColor; // 파티클의 시작 크기.	
    
    float4 gEndColor; // 파티클의 시작 크기.	
};
struct Velocity_Over_LifeTime
{
    float3 gVelocity; // 시간에 따른 파티클 속력.
    float pad;
};
struct Rotation_Over_Lifetime
{
    float gAngularVelocity;
    float3 pad;
};
struct Size_Over_Lifetime
{
    float gStartSize;
    float gEndSize;
    float gStartOffset;
    float gEndOffset;
};
struct Texture_Sheet_Animation
{
    int2 gGrid_XY;
    int gTimeMode;
    float pad;
};