#include "ConstantBuffer.hlsli"

#ifdef ssrVer1
    //--------------------------------------------------------------------------------------
    // Globals
    //--------------------------------------------------------------------------------------

struct ScreenSpaceReflection
{
    float maxDistance;
    float resolution;
    float thickness;
    int steps;
};

    // PerObject
cbuffer cbPerObject : register(b2)
{
    ScreenSpaceReflection g_screenSpaceReflection;
};

static const float ssrmaxDistance = 100;
static const float ssrresolution = 0.5f;
static const float ssrthickness = 0.5f;
static const int ssrsteps = 100;

    //--------------------------------------------------------------------------------------
    // Textures and Samplers
    //--------------------------------------------------------------------------------------

Texture2D g_Normal : register(t0);
Texture2D g_PosW : register(t1);
    // Texture2D g_Depth : register(t2);

SamplerState g_samTriPoint : register(s0);

    //--------------------------------------------------------------------------------------
    // Input / Output structures
    //--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};
    //--------------------------------------------------------------------------------------
    // Functions
    //--------------------------------------------------------------------------------------

float4 ViewToPixelSpace(float4 viewPos)
{
    float4 pixelPos;
    pixelPos = mul(viewPos, g_Camera.g_ProjectionMatix);
    pixelPos.xyz /= pixelPos.w;
    pixelPos.xy *= float2(0.5f, -0.5f);
    pixelPos.xy += float2(0.5f, 0.5f);

    pixelPos.xy *= g_ScreenXY;

    return pixelPos;
};

    //--------------------------------------------------------------------------------------
    // Pixel Shader
    //--------------------------------------------------------------------------------------

float4 PSMain(PS_IN input)
        : SV_TARGET
{

    float4 position = g_PosW.Sample(g_samTriPoint, input.Texcoord0);
    float3 normal = mul(g_Normal.Sample(g_samTriPoint, input.Texcoord0).xyz, (float3x3)g_Camera.g_ViewMatrix).xyz;
    
    normal = normalize(normal);

    float3 cameraToPos = mul(float4(position.xyz, 1.f), g_Camera.g_ViewMatrix).xyz;
    float3 unitCameraToPos = normalize(cameraToPos);

    if (position.w <= 0.0f
          //|| mask.b != 1.0
          //|| mask.r <= 0.0
          )
    {
        clip(-1);
    }

    float3 reflectV = normalize(reflect(unitCameraToPos, normal));

    float4 startView = float4(cameraToPos + reflectV * 0, 1.f);
    float4 endView = float4(cameraToPos + reflectV * ssrmaxDistance, 1.f);

	// UV좌표계상의 위치를 구한후 픽셀좌표계로 옮겨준다
    float4 startPixel = ViewToPixelSpace(startView);
    float4 endPixel = ViewToPixelSpace(endView);

    float deltaX = endPixel.x - startPixel.x;
    float deltaY = endPixel.y - startPixel.y;

    endPixel.xy = max(min(endPixel.xy, float2(0,0)), g_ScreenXY);

        // X의 길이가 큰지, Y의 길이가 큰지.
    float useX = abs(deltaX) >= abs(deltaY) ? 1 : 0;
        // 긴길이를 기준으로 이동값을 구함.
    float delta = lerp(abs(deltaY), abs(deltaX), useX) * clamp(ssrresolution, 0, 1);

    float2 increment = float2(deltaX, deltaY) / max(delta, 0.1f);

    float2 frag = startPixel.xy;
    float2 uv = 0;

    int hit0 = 0;
    int hit1 = 0;

    float search0 = 0;
    float search1 = 0;

    float viewDist = 0;
    float depth = 0;

    float3 incPos;

    deltaX = max(deltaX, 0.01f);
    deltaY = max(deltaY, 0.01f);

    int inc = 0;

    [unroll(100)]
    for (inc = 0; inc < int(delta); inc++)
    {
        frag += increment;
        uv.xy = frag / g_ScreenXY;
        
        incPos = mul(float4(g_PosW.Sample(g_samTriPoint, float2(uv.x, uv.y)).xyz, 1.f), g_Camera.g_ViewMatrix).xyz;

        search1 = lerp((frag.y - startPixel.y) / deltaY, (frag.x - startPixel.x) / deltaX, useX);
        search1 = clamp(search1, 0.0, 1.0);

            // https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
            // 비례한 거리는 원형투영 보간으로 viewDist.움직인 거리를 구한다.(pdf상의서의 비례거리 t)
        float viewDist = (startView.z * endView.z) / max(lerp(endView.z, startView.z, search1), 0.01f);
        depth = viewDist - incPos.z;

        if (depth > 0 && depth < ssrthickness)
        {
            hit0 = 1;
                // 히트시 포문중단!
            break;
        }
        else
        {
                // 이전프레임 미 히트시 저장한다
            search0 = search1;
        }
    }

    search1 = search0 + ((search1 - search0) / 2.0);

    float steps = ssrsteps * hit0;
    inc = 0;
    // 히트했을때 2차 검증포문

    for (inc = 0; inc < steps; ++inc)
    {
            // hit한 보간위치와 이전프레임의 위치의 사이로 구한다.
        frag = lerp(startPixel.xy, endPixel.xy, search1);
        uv.xy = frag / g_ScreenXY;

        incPos = mul(float4(g_PosW.Sample(g_samTriPoint, float2(uv.x, uv.y)).xyz, 1.f), g_Camera.g_ViewMatrix).xyz;
        float viewDist = (startView.z * endView.z) / lerp(endView.z, startView.z, search1);
        depth = viewDist - incPos.z;

        if (depth > 0 && depth < ssrthickness)
        {
                //히트시에는 가까운쪽으로.
            hit1 = 1;
            search1 = search0 + ((search1 - search0) / 2.f);
            break;
        }
        else
        {
                // 노히트시에는 충돌한 쪽으로 움직인다.
            float temp = search1;
            search1 = search1 + ((search1 - search0) / 2.f);
            search0 = temp;
        }
    }

    float visibility = hit1
            * /*incPos.w */1.0f // 알파값을
            * (1 - max(dot(-unitCameraToPos, reflectV), 0.f))
            * (1 - clamp(depth / ssrthickness, 0.f, 1.f))
            * (1 - clamp(length(incPos - cameraToPos) / ssrmaxDistance, 0.f, 1.f))
            * (uv.x < 0 || uv.x > 1 ? 0 : 1)
            * (uv.y < 0 || uv.y > 1 ? 0 : 1);

    visibility = clamp(visibility, 0, 1);

    return float4(float2(uv.x, uv.y), visibility, 1.f);
}
#else

struct ScreenSpaceReflection
{
    float maxDistance;
    float resolution;
    float thickness;
    int steps;
};

static const float g_maxDistance = 15;
static const float g_resolution = 0.3f;
static const float g_thickness = 0.5f;
static const int g_steps = 5;
//// PerObject
cbuffer cbperObject : register(b2)
{
    float cb_stride;
    // Step in horizontal or vertical pixels between samples. This is a float
    // because integer math is slow on GPUs, but should be set to an integer >= 1.
    float cb_maxSteps;    // Maximum number of iterations. Higher gives better images but may be slow.
    float cb_maxDistance; // Maximum camera-space distance to trace before returning a miss.
    float cb_strideZCutoff;
    // More distant pixels are smaller in screen space. This value tells at what point to
    // start relaxing the stride to give higher quality reflections for objects far from
    // the camera.

    float cb_numMips;       // the number of mip levels in the convolved color buffer
    float cb_fadeStart;     // determines where to start screen edge fading of effect
    float cb_fadeEnd;       // determines where to end screen edge fading of effect
    float cb_sslr_padding0; // padding for alignment

    float cb_zThickness; // thickness to ascribe to each pixel in the depth buffer
};
//--------------------------------------------------------------------------------------
// Textures and Samplers
//--------------------------------------------------------------------------------------

Texture2D g_Normal : register(t0);
Texture2D g_Depth : register(t1);
Texture2D g_PosW : register(t2);
Texture2D g_ObjectID : register(t3);

SamplerState g_samTriPoint : register(s0);

//--------------------------------------------------------------------------------------
// Input / Output structures
//--------------------------------------------------------------------------------------

struct PS_IN
{
    float4 PosH : SV_POSITION;
    float2 Texcoord0 : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------

float4 ViewToPixelSpace(float4 viewPos)
{
    float4 pixelPos;
    pixelPos = mul(viewPos, g_Camera.g_ProjectionMatix);
    pixelPos.xyz /= pixelPos.w;
    pixelPos = pixelPos * 0.5 + 0.5;
    pixelPos.y = 1.f - pixelPos.y;
    pixelPos.xy *= g_ScreenXY;

    return pixelPos;
};

float distanceSquared(float2 a, float2 b)
{
    a -= b;
    return dot(a, a);
}

//현재 픽셀의 Z값.
bool intersectsDepthBuffer(float z, float minZ, float maxZ)
{
    /*
     * Based on how far away from the camera the depth is,
     * adding a bit of extra thickness can help improve some
     * artifacts. Driving this value up too high can cause
     * artifacts of its own.
     */

    // float depthScale = min(1.0f, z * cb_strideZCutoff);
    // float delta = maxZ - minZ;

    // float value = min(delta, cb_zThickness);

    float depthScale = min(1.0f, z * cb_strideZCutoff);
    z += cb_zThickness + lerp(0.0f, 2.0f, depthScale);
    return (maxZ >= z) && (minZ - cb_zThickness <= z);

    // float maxscreenz = z + value;
    // float minscreenz = z - value;
    // return !(maxZ < minscreenz) &&! (minZ > maxscreenz);
}

bool intersectsDepthBuffer2(float z, float prevZ, float minZ, float maxZ)
{
    /*
     * Based on how far away from the camera the depth is,
     * adding a bit of extra thickness can help improve some
     * artifacts. Driving this value up too high can cause
     * artifacts of its own.
     */

    float maxScreenZ = prevZ;
    float minScreenZ = z;

    if(z >= prevZ)
    {
        maxScreenZ = z;
        minScreenZ = prevZ;
    }

    bool Max = (minZ - g_thickness > maxScreenZ) ? false : true;
    bool Min = (minScreenZ > maxZ) ? false : true;

    return (Max && Min);
}

float linearizeDepth(float depth, float near, float far)
{
    float4 ret = mul(float4(0, 0, depth, 1.f), g_Camera.g_InverseProjectionMatrix);
    return ret.z / ret.w;

    //return near * far / (far - depth * (far - near));
}

float2 pixelizePosH(float3 posH)
{
    float2 pixel = posH.xy * g_ScreenXY;

    return floor(pixel);
}

float linearDepthTexelFetch(int2 hitPixel)
{
    // Load returns 0 for any value accessed out of bounds
    // /return linearizeDepth(depthBuffer.Load(int3(hitPixel, 0)).r);
}

void swap(inout float a, inout float b)
{
    float t = a;
    a = b;
    b = t;
}

float4 ViewToTexture(float4 viewPos)
{

    float4 pixelPos;
    pixelPos = mul(viewPos, g_Camera.g_ProjectionMatix);
    pixelPos.xy *= float2(0.5f, -0.5f);
    pixelPos.xy += float2(0.5f * pixelPos.w , 0.5f * pixelPos.w);
    
    pixelPos.xy *= g_ScreenXY;

    return pixelPos;
};

// Returns true if the ray hit something
bool traceScreenSpaceRay(
    // Camera-space ray origin, which must be within the view volume
    float3 csOrig,
    // Unit length camera-space ray direction
    float3 csDir,
    // Number between 0 and 1 for how far to bump the ray in stride units
    // to conceal banding artifacts. Not needed if stride == 1.
    float jitter,
    // Pixel coordinates of the first intersection with the scene
    out float2 hitPixel,
    // Camera space location of the ray hit
    out float3 hitPoint)
{
    // Clip to the near plane
    float rayLength = ((csOrig.z + csDir.z * cb_maxDistance) < g_Camera.g_CameraNear) ? (g_Camera.g_CameraNear - csOrig.z) / csDir.z : cb_maxDistance;
    float3 csEndPoint = csOrig + csDir * rayLength;

    // Project into homogeneous clip space
    // float4 H0 = mul(float4(csOrig, 1.0f), g_Camera.g_ProjectionMatix);
    // H0.xy *= g_ScreenXY;
    // float4 H1 = mul(float4(csEndPoint, 1.0f), g_Camera.g_ProjectionMatix);
    // H1.xy *= g_ScreenXY;

    float4 H0 = ViewToTexture(float4(csOrig, 1.0f));
    float4 H1 = ViewToTexture(float4(csEndPoint, 1.0f));;

    // invw. 동차좌표
    float k0 = 1.0f / H0.w;
    float k1 = 1.0f / H1.w;

    //
    // The interpolated homogeneous version of the camera-space points
    //사실상 z가 1인 공간으로 투영시키는것이나 다름없다.
    float3 Q0 = csOrig * k0;
    float3 Q1 = csEndPoint * k1;

    // Screen-space endpoints
    float2 P0 = H0.xy * k0;
    float2 P1 = H1.xy * k1;

    //스크린사이즈 바깥으로 나가는 픽셀들에 대해 관리해준다.
    //P1 = min(max(P1, float2(0, 0)), g_ScreenXY);

        // If the line is degenerate, make it cover at least one pixel
    // to avoid handling zero-pixel extent as a special case later
    P1 += (distanceSquared(P0, P1) < 0.0001f) ? float2(0.01f, 0.01f) : 0.0f;


    float2 delta = P1 - P0;

    // x와 y의 크기차이를 비교하여.. 구한다
    // Permute so that the primary iteration is in x to collapse
    // all quadrant-specific DDA cases later
    bool permute = false;
    if (abs(delta.x) < abs(delta.y))
    {
        // This is a more-vertical line
        permute = true;
        delta = delta.yx;
        P0 = P0.yx;
        P1 = P1.yx;
    }

    // sign -> 음수면 return -1, 양수면 return 1
    float stepDir = sign(delta.x);
    // invdx를 무조건 양수로 만들기 위해 stepDir를 곱해준다.
    float invdx = stepDir / delta.x;

    // k0 - start 지점의 inv 동차좌표(w)
    // Q0 camera space / k0
    // P0 screen space / k0

    // Track the derivatives of Q and k
    //카메라스페이스 delta Q 차이를 이동거리만큼 나눠준다.
    //동차좌표계간 사이의 delta값 inv 나누기
    float3 dQ = (Q1 - Q0) * invdx;
    float dk = (k1 - k0) * invdx;

    //픽셀자체의 이동거리이다.
    float2 dP = float2(stepDir, delta.y * invdx);

    // Scale derivatives by the desired pixel stride and then
    // offset the starting values by the jitter fraction
    // 가까울수록 픽셀이 차지하는 크기는 많다. scale을 높여버리네..
    float strideScale = 1.0f - min(1.0f, csOrig.z * cb_strideZCutoff);
    float stride = 1.0f + strideScale * cb_stride;

    dP *= cb_stride;
    dQ *= cb_stride;
    dk *= cb_stride;

    P0 += dP * jitter;
    Q0 += dQ * jitter;
    k0 += dk * jitter;

    // Slide P from P0 to P1, (now-homogeneous) Q from Q0 to Q1, k from k0 to k1
    float4 PQk = float4(P0, Q0.z, k0);
    float4 dPQk = float4(dP, dQ.z, dk);
    float3 Q = Q0;

    // Adjust end condition for iteration direction
    float end = P1.x * stepDir;

    float stepCount = 0.0f;
    float prevZMaxEstimate = csOrig.z;
    float rayZMin = prevZMaxEstimate;
    float rayZMax = prevZMaxEstimate;
    float sceneZMax = rayZMax + 100.0f;
    //float prevScreenZMax = sceneZMax;

    for (;
         ((PQk.x * stepDir) <= end) && (stepCount < cb_maxSteps) && !intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax) && (sceneZMax != 0.0f);
         ++stepCount)
    {
        rayZMin = prevZMaxEstimate;
        rayZMax = (dPQk.z * 0.5f + PQk.z) / (dPQk.w * 0.5f + PQk.w);

        prevZMaxEstimate = rayZMax;
        if (rayZMin > rayZMax)
        {
            swap(rayZMin, rayZMax);
        }

        hitPixel = permute ? PQk.yx : PQk.xy;
        // You may need hitPixel.y = depthBufferSize.y - hitPixel.y; here if your vertical axis
        // is different than ours in screen space
        // scene z = 픽셀에서 추출한 z값을 비교할 버퍼.
        //prevScreenZMax = sceneZMax;
        sceneZMax = linearizeDepth(g_Depth.Load(int3(hitPixel, 0)).r, g_Camera.g_CameraNear, g_Camera.g_CameraFar);

        PQk += dPQk;
    }

    // Advance Q based on the number of steps
    Q.xy += dQ.xy * stepCount;
    hitPoint = Q * (1.0f / PQk.w);
    return intersectsDepthBuffer(sceneZMax, rayZMin, rayZMax);
}

// Ver 3
float4 PSMain(PS_IN input)
    : SV_TARGET
{
    int3 loadIndice = int3(input.PosH.x, input.PosH.y, 0);

    float3 normalWS = normalize(g_Normal.Load(loadIndice).xyz);
    float2 objectFlag = g_ObjectID.Load(loadIndice).zw;

    if (!any(normalWS) || !(asuint(objectFlag.y) & 1))
    {
        clip(-1);
    }

    float3 normalVS = normalize(mul(normalWS, (float3x3)g_Camera.g_ViewMatrix).xyz);
    float depth = g_Depth.Load(loadIndice).r;

    float4 worldPos = float4(g_PosW.Load(loadIndice).xyz, 1.f);

    // float4 rayNDC = float4(input.PosH.xy / g_ScreenXY, depth, 1.f);
    // rayNDC.xy -= float2(0.5, 0.5);
    // rayNDC.xy *= float2(2.f, -2.f);
    // float4 viewRay = mul(rayNDC, g_Camera.g_InverseProjectionMatrix);
    // viewRay.xyz /= viewRay.w;

    float4 viewRay = mul(worldPos, g_Camera.g_ViewMatrix);

    //float3 viewRay3 = worldPos.xyz - g_Camera.g_CameraPosition;

    //float4 viewRayPrj = mul(viewRay, g_Camera.g_ProjectionMatix);
    //viewRayPrj.xyz /= viewRayPrj.w;

    //float4 depthPos = mul(float4(0.f, 0.f, depth, 1.f), g_Camera.g_InverseProjectionMatrix);
    //float viewDepth = depthPos.z / depthPos.w; 
    
    //float viewDepth2 = linearizeDepth(depth, g_Camera.g_CameraNear, g_Camera.g_CameraFar);
    //return float4(viewDepth, viewDepth2, 0.f, 0.f);

    float3 rayOriginVS = viewRay.xyz;

    /*
     * Since position is reconstructed in view space, just normalize it to get the
     * vector from the eye to the position and then reflect that around the normal to
     * get the ray direction to trace.
     */
    float3 toPositionVS = normalize(rayOriginVS);
    float3 rayDirectionVS = normalize(reflect(toPositionVS, normalVS));

    // output rDotV to the alpha channel for use in determining how much to fade the ray
    float rDotV = dot(rayDirectionVS, toPositionVS);

    // out parameters
    float2 hitPixel = float2(0.0f, 0.0f);
    float3 hitPoint = float3(0.0f, 0.0f, 0.0f);

    // float jitter = cb_stride > 1.0f ? float(int(input.Texcoord0.x * g_ScreenXY.x + input.Texcoord0.y * g_ScreenXY.y) & 1) * 0.5f : 0.0f;
    float jitter = cb_stride > 1.0f ? float(int(toPositionVS.x + toPositionVS.y) & 1) * 0.5f : 0.0f;

    // perform ray tracing - true if hit found, false otherwise
    bool intersection = traceScreenSpaceRay(rayOriginVS, rayDirectionVS, jitter, hitPixel, hitPoint);

    depth = g_Depth.Load(int3(hitPixel, 0)).r;

    // move hit pixel from pixel position to UVs
    hitPixel *= (1.f / g_ScreenXY);
    if(hitPixel.x > 1.0f || hitPixel.x < 0.0f || hitPixel.y > 1.0f || hitPixel.y < 0.0f)
    {
        intersection = false;
    }

    return float4(hitPixel, depth, rDotV) * (intersection ? 1.0f : 0.0f);
    //return float4(hitPixel, depth, 1.f) * (intersection ? 1.0f : 0.0f);
}
#endif

//ver4