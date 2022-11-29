#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class DUOLGRAPHICSLIBRARY_EXPORT RenderTarget;

 /**

     @class   RenderPass
     @brief   파이프라인에 렌더타겟들을 바인딩하기위한 렌더타겟의 묶음
     @details ~
     @author  KyungMin Oh

 **/
	struct RenderPass
	{
		std::vector<RenderTarget*> _renderTargetViewRefs;

		RenderTarget* _depthStencilViewRef;
	};
}