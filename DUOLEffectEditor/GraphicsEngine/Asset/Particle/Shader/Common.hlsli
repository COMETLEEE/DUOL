#define NONE  0
#define Use_ParticleSystemCommonInfo  1 << 0 // ParticleSystemCommonInfo Module
#define Use_Emission 1 << 1 // Emission Module
#define Use_Shape 1 << 2 // Shape Module
#define Use_Velocity_Over_Lifetime 1 << 3 // Velocity_Over_Lifetime Module
#define Use_Force_over_Lifetime 1 << 4 // Force_over_Lifetime Module
#define Use_Color_over_Lifetime 1 << 5 // Color_over_Lifetime Module
#define Use_Size_over_Lifetime 1 << 6 // Size_over_Lifetime Module
#define Use_Rotation_over_Lifetime 1 << 7 // Rotation_over_Lifetime Module
#define Use_Noise 1 << 8 // Noise Module
#define Use_Collision 1 << 9 // Collision Module
#define Use_Texture_Sheet_Animation  1 << 10 // Texture_Sheet_Animation Module
#define Use_Trails 1 << 11 // Trails Module  
#define Use_Renderer 1 << 12 // Renderer Module

#define Use_Shape_Sphere 1 << 13
#define Use_Shape_Hemisphere 1 << 14
#define Use_Shape_Cone 1 << 15
#define Use_Shape_Donut 1 << 16
#define	Use_Shape_Box 1 << 17
#define Use_Shape_Circle 1 << 18
#define Use_Shape_Rectangle 1 << 19

#define Use_Renderer_BillBoard 1 << 20
#define Use_Renderer_StretchedBillBoard 1 << 21
#define Use_Renderer_HorizontalBillBoard 1 << 22
#define Use_Renderer_VerticalBillBoard 1 << 23
#define Use_Renderer_Mesh 1 << 24

#define Use_Commoninfo_WorldSpace 1 << 25

#define Use_TrailFlag_WorldSpace 1 << 0
#define Use_TrailFlag_DieWithParticle 1 << 1
#define Use_TrailFlag_SizeAffectsWidth 1 << 2
#define Use_TrailFlag_SizeAffectsLifetime 1 << 3
#define Use_TrailFlag_InheritParticleColor 1 << 4
#define Use_TrailFlag_GenerateLightingData 1 << 5
#define Use_TrailFlag_TextureMode_Stretch 1 << 6
#define Use_TrailFlag_TextureMode_Tile 1 << 7
#define Use_TrailFlag_TextureMode_DistributePerSegment 1 << 8
#define Use_TrailFlag_TextureMode_RepeatPerSegment 1 << 9

#define PI 3.141592f


#ifndef __QUATERNION_INCLUDED__
#define __QUATERNION_INCLUDED__

#define QUATERNION_IDENTITY float4(0, 0, 0, 1)

#ifndef PI
#define PI 3.14159265359f
#endif 

// Quaternion multiplication
// http://mathworld.wolfram.com/Quaternion.html
float4 qmul(float4 q1, float4 q2)
{
    return float4(
        q2.xyz * q1.w + q1.xyz * q2.w + cross(q1.xyz, q2.xyz),
        q1.w * q2.w - dot(q1.xyz, q2.xyz)
    );
}

// Vector rotation with a quaternion
// http://mathworld.wolfram.com/Quaternion.html
float3 rotate_vector(float3 v, float4 r)
{
    float4 r_c = r * float4(-1, -1, -1, 1);
    return qmul(r, qmul(float4(v, 0), r_c)).xyz;
}

// A given angle of rotation about a given axis
float4 rotate_angle_axis(float angle, float3 axis)
{
    float sn = sin(angle * 0.5);
    float cs = cos(angle * 0.5);
    return float4(axis * sn, cs);
}

// https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
float4 from_to_rotation(float3 v1, float3 v2)
{
    float4 q;
    float d = dot(v1, v2);
    if (d < -0.999999)
    {
        float3 right = float3(1, 0, 0);
        float3 up = float3(0, 1, 0);
        float3 tmp = cross(right, v1);
        if (length(tmp) < 0.000001)
        {
            tmp = cross(up, v1);
        }
        tmp = normalize(tmp);
        q = rotate_angle_axis(PI, tmp);
    }
    else if (d > 0.999999)
    {
        q = QUATERNION_IDENTITY;
    }
    else
    {
        q.xyz = cross(v1, v2);
        q.w = 1 + d;
        q = normalize(q);
    }
    return q;
}

float4 q_conj(float4 q)
{
    return float4(-q.x, -q.y, -q.z, q.w);
}

// https://jp.mathworks.com/help/aeroblks/quaternioninverse.html
float4 q_inverse(float4 q)
{
    float4 conj = q_conj(q);
    return conj / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

float4 q_diff(float4 q1, float4 q2)
{
    return q2 * q_inverse(q1);
}

float4 q_look_at(float3 forward, float3 up)
{
    float3 right = normalize(cross(forward, up));
    up = normalize(cross(forward, right));

    float m00 = right.x;
    float m01 = right.y;
    float m02 = right.z;
    float m10 = up.x;
    float m11 = up.y;
    float m12 = up.z;
    float m20 = forward.x;
    float m21 = forward.y;
    float m22 = forward.z;

    float num8 = (m00 + m11) + m22;
    float4 q = QUATERNION_IDENTITY;
    if (num8 > 0.0)
    {
        float num = sqrt(num8 + 1.0);
        q.w = num * 0.5;
        num = 0.5 / num;
        q.x = (m12 - m21) * num;
        q.y = (m20 - m02) * num;
        q.z = (m01 - m10) * num;
        return q;
    }

    if ((m00 >= m11) && (m00 >= m22))
    {
        float num7 = sqrt(((1.0 + m00) - m11) - m22);
        float num4 = 0.5 / num7;
        q.x = 0.5 * num7;
        q.y = (m01 + m10) * num4;
        q.z = (m02 + m20) * num4;
        q.w = (m12 - m21) * num4;
        return q;
    }

    if (m11 > m22)
    {
        float num6 = sqrt(((1.0 + m11) - m00) - m22);
        float num3 = 0.5 / num6;
        q.x = (m10 + m01) * num3;
        q.y = 0.5 * num6;
        q.z = (m21 + m12) * num3;
        q.w = (m20 - m02) * num3;
        return q;
    }

    float num5 = sqrt(((1.0 + m22) - m00) - m11);
    float num2 = 0.5 / num5;
    q.x = (m20 + m02) * num2;
    q.y = (m21 + m12) * num2;
    q.z = 0.5 * num5;
    q.w = (m01 - m10) * num2;
    return q;
}

float4 q_slerp(in float4 a, in float4 b, float t)
{
    // if either input is zero, return the other.
    if (length(a) == 0.0)
    {
        if (length(b) == 0.0)
        {
            return QUATERNION_IDENTITY;
        }
        return b;
    }
    else if (length(b) == 0.0)
    {
        return a;
    }

    float cosHalfAngle = a.w * b.w + dot(a.xyz, b.xyz);

    if (cosHalfAngle >= 1.0 || cosHalfAngle <= -1.0)
    {
        return a;
    }
    else if (cosHalfAngle < 0.0)
    {
        b.xyz = -b.xyz;
        b.w = -b.w;
        cosHalfAngle = -cosHalfAngle;
    }

    float blendA;
    float blendB;
    if (cosHalfAngle < 0.99)
    {
        // do proper slerp for big angles
        float halfAngle = acos(cosHalfAngle);
        float sinHalfAngle = sin(halfAngle);
        float oneOverSinHalfAngle = 1.0 / sinHalfAngle;
        blendA = sin(halfAngle * (1.0 - t)) * oneOverSinHalfAngle;
        blendB = sin(halfAngle * t) * oneOverSinHalfAngle;
    }
    else
    {
        // do lerp if angle is really small.
        blendA = 1.0 - t;
        blendB = t;
    }

    float4 result = float4(blendA * a.xyz + blendB * b.xyz, blendA * a.w + blendB * b.w);
    if (length(result) > 0.0)
    {
        return normalize(result);
    }
    return QUATERNION_IDENTITY;
}

float4x4 quaternion_to_matrix(float4 quat)
{
    float4x4 m = float4x4(float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0), float4(0, 0, 0, 0));

    float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
    float x2 = x + x, y2 = y + y, z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;

    m[0][0] = 1.0 - (yy + zz);
    m[0][1] = xy - wz;
    m[0][2] = xz + wy;

    m[1][0] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[1][2] = yz - wx;

    m[2][0] = xz - wy;
    m[2][1] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);

    m[3][3] = 1.0;

    return m;
}

#endif // __QUATERNION_INCLUDED__



//float4x4 CreateRotateTM(float3 Rot)
//{
//    float4x4 RotateTM;

//    float2 cos_sin = normalize(float2(Rot.z, Rot.y));
//    //float _cos = cos(3.14f / 4.0f);
//    //float _sin = sin(3.14f / 4.0f);
//    float _cos = cos_sin.x;
//    float _sin = cos_sin.y;
//    float4x4 xRotate = //x รเ
//    {
//        1, 0, 0, 0,
//            0, _cos, -_sin, 0,
//            0, _sin, _cos, 0,
//            0, 0, 0, 1
//    };
//    float rad = atan2(_sin, _cos);
   
//    cos_sin = normalize(float2(Rot.x, Rot.z));
//    _cos = cos_sin.x; //cos_sin.x; // x
//    _sin = cos_sin.y; //cos_sin.y; // y
//    if (abs(rad) >= PI / 2)
//    {
//        rad = atan2(_sin, _cos);
//        rad = 3.141592f + rad % 3.141592f;
//        _cos = cos(rad);
//        _sin = sin(rad);

//    }
//    float4x4 yRotate =
//    {
//        _cos, 0, _sin, 0,
//            0, 1, 0, 0,
//            -_sin, 0, _cos, 0,
//            0, 0, 0, 1
//    };
//    cos_sin = normalize(float2(Rot.y, Rot.x)); //  zรเ
//    _cos = cos_sin.x; //cos_sin.x; // x
//    _sin = cos_sin.y; //cos_sin.y; // y
//    float4x4 zRotate =
//    {
//        _cos, -_sin, 0, 0,
//            _sin, _cos, 0, 0,
//            0, 0, 1, 0,
//            0, 0, 0, 1
//    };


//    //RotateTM = mul(mul(zRotate, xRotate), yRotate);
//    RotateTM = mul(xRotate, yRotate);

//    return RotateTM;

//}