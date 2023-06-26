#pragma once
#include "DUOLGraphicsEngine/Util/ThreadPool/ThreadPool.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	class ResourceManager;

 /**

     @class   TableLoader
     @brief   테이블들을 로드하는 함수들
     @details ~
     @author  KyungMin Oh

 **/
	class TableLoader
	{
	public:
  /**
      @brief  텍스쳐와 렌더타겟을 동시에 생성합니다.
      @param  resourceManager - 
      @param  screenSize      - 
      @retval                 - 
  **/
		static bool LoadRenderTargetTable(ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize);

		static bool LoadShaderTable(ResourceManager* resourceManager);

		static bool LoadSampler(ResourceManager* resourceManager);

		static bool LoadPipelineStateTable(ResourceManager* resourceManager);

		static bool LoadRenderingPipelineTable(ResourceManager* resourceManager);

		static bool LoadTextureListWithMultiThread(ResourceManager* resourceManager,const std::vector<MaterialForDeferredLoad>&);

	};

	//todo:: threadPool의 적당한 위치가 어디일까.
	//todo:: 지금은 static 전역변수로 박아넣고 셰이더 컴파일할때만 멀티스레드 환경에서 돌려줍니다.
	static ThreadPool g_threadPool(8);
}
