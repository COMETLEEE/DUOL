#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class DUOLGRAPHICSLIBRARY_EXPORT RenderTarget;

 /**

     @class   RenderPass
     @brief   ���������ο� ����Ÿ�ٵ��� ���ε��ϱ����� ����Ÿ���� ����
     @details ~
     @author  KyungMin Oh

 **/
	struct RenderPass
	{
		std::vector<RenderTarget*> _renderTargetViewRefs;

		RenderTarget* _depthStencilViewRef;
	};
}