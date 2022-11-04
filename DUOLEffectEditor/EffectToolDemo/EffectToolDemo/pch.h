#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>

#include "../Library/inc/SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;


using tchar = TCHAR;

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif

#include "MuscleEngine.h"
#include "SceneManager.h"
#include "ObjectManager.h"
//#include "DebugManager.h"
#include "KeyBoard.h"
#include "CTime.h"



#include "ComponentsHead.h"

#include "GameProcess.h"
