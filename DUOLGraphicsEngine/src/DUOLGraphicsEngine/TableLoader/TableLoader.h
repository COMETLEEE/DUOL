#pragma once
#include "DUOLGraphicsEngine/Util/ThreadPool/ThreadPool.h"
#include "DUOLGraphicsEngine/ResourceManager/ResourceManager.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGraphicsEngine
{
	class ResourceManager;

 /**

     @class   TableLoader
     @brief   ���̺���� �ε��ϴ� �Լ���
     @details ~
     @author  KyungMin Oh

 **/
	class TableLoader
	{
	public:
  /**
      @brief  �ؽ��Ŀ� ����Ÿ���� ���ÿ� �����մϴ�.
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

	//todo:: threadPool�� ������ ��ġ�� ����ϱ�.
	//todo:: ������ static ���������� �ھƳְ� ���̴� �������Ҷ��� ��Ƽ������ ȯ�濡�� �����ݴϴ�.
	static ThreadPool g_threadPool(8);
}
