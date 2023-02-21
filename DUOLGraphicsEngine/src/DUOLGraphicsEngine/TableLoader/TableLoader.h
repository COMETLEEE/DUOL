#pragma once
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


	};
}
