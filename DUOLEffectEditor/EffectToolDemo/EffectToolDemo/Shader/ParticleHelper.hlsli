
struct CommonInfo
{
    float3 gEmitDirW;
    float gDuration; // �� �� ���� ��ƼŬ ��ü�� ����� ��.					

    float3 gEmitPosW;
    int gMaxParticles; // ��ƼŬ �ִ� ��� ������.							

    float2 gStartDelay; // �� �� �ڿ� ��ƼŬ�� ����� ��.					
    float2 gStartLifeTime; // �� ��ƼŬ�� ���� �ð�.						

    float2 gStartSpeed; // ��ƼŬ ������ ���� �ӵ�.
    float2 gStartRotation; // ��ƼŬ�� ���� ȸ��.							

    float4 gStartSize; // ��ƼŬ�� ���� ũ��.							

    float4 gStartColor[2]; // ��ƼŬ�� ���� ����									

    float2 gGravityModifier; // ��ƼŬ�� �������� �߷�.
    float gParticlePlayTime; // ��ƼŬ ��� �� �帥 �ð�.
    float pad4; // ��ƼŬ�� �������� �߷�.						

    int gisLooping; // �ݺ�����.
    int3 pad3; // �����ΰ��� ..?						
};
struct Emission
{
    float gEmissiveCount; // �ѹ��� ��� ���� ��ų��.

    float gEmissiveTime; // ���� ������� �ɸ��� �ð�.

    float2 pad;
};
struct Color_Over_LifeTime
{
    float4 gStartColor; // ��ƼŬ�� ���� ũ��.	
    
    float4 gEndColor; // ��ƼŬ�� ���� ũ��.	
};
struct Velocity_Over_LifeTime
{
    float3 gVelocity; // �ð��� ���� ��ƼŬ �ӷ�.
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