#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일




/// 엔진것..
#include "..\Common\Define.h"
#include <d3d11.h>
#include <fstream>

#include "../Library/inc/SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;


// 각종 typedef
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

#include "d3dUtil.h"
#include "GeometryGenerator.h"
#include "IGraphicsEngine.h"
#include "PassBase.h"
#include "Renderer.h"

#include "WirePass.h"
#include "BasicPass.h"
#include "BasicParticlePass.h"

#include "Device.h"
#include "Depth.h"
#include "DepthStencil.h"
#include "RenderTexture.h"
#include "RenderTarget.h"
#include "GizmoMesh.h"
#include "VBIBMesh.h"
#include "ParticleMesh.h"

#include "ResourceManager.h"
#include "DXEngine.h"
#include "Effects.h"
#include "RasterizerState.h"
#include "SamplerState.h"

#include "Factory.h"
