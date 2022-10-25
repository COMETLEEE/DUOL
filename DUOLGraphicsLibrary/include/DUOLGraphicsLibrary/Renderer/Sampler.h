#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   Sampler
     @brief   파이프라인에 바인딩될 리소스 중 샘플러.
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Sampler : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderTarget);

	public:
		virtual ResourceType GetResourceType() override;

	};

}