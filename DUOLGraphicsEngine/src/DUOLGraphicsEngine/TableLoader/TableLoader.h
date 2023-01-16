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
		static bool LoadRenderTargetTable(ResourceManager* resourceManager, const DUOLMath::Vector2& screenSize);

		static bool LoadShaderTable(ResourceManager* resourceManager);

		static bool LoadSampler(ResourceManager* resourceManager);

		static bool LoadPipelineStateTable(ResourceManager* resourceManager);

		static bool LoadRenderingPipelineTable(ResourceManager* resourceManager);


	};
}
