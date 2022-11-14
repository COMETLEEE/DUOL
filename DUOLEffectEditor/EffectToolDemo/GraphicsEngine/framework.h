#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일




/// 엔진것..
#include "..\Common\Define.h"
#include <d3d11.h>
#include <fstream>

#include "../Library/inc/SimpleMath.h"

// DirectXTex => MicroSoft 오픈 소스. 텍스쳐를 다루는 라이브러리
#include "..\\Library\\inc\\DirectXTex.h"
#include "..\\Library\\inc\\SpriteFont.h"
#include "..\\Library\\inc\\SpriteBatch.h"

#if defined(DEBUG) || defined(_DEBUG)
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x86_Debug.lib")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x86_Debug.lib")
#else
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x64_Debug.lib")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x64_Debug.lib")
#endif
#else
#if defined (_WIN32) && !defined(_WIN64)
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x86_Release")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x86_Release")
#else
#pragma comment(lib, "..\\Library\\lib\\DirectXTex_x64_Release")
#pragma comment(lib, "..\\Library\\lib\\DirectXTK_x64_Release")
#endif
#endif


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
#include "BlendState.h"

#include "Factory.h"
