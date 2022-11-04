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



#include "GeometryGenerator.h"
#include "IGraphicsEngine.h"
#include "Device.h"
#include "Depth.h"
#include "DepthStencil.h"
#include "RenderTexture.h"
#include "Display.h"
#include "RenderTarget.h"
#include "Camera.h"
#include "InputLayout.h"
#include "GizmoMesh.h"
#include "VBIBMesh.h"

#include "ResourceManager.h"
#include "DXEngine.h"
#include "Effects.h"
#include "RasterizerState.h"

#include "Factory.h"
