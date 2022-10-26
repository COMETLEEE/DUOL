#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class RenderTarget;

 /**

     @class   RenderPass
     @brief   파이프라인에 렌더타겟들을 바인딩하기위한 렌더타겟의 묶음
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT RenderPass : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderPass);

	};
}