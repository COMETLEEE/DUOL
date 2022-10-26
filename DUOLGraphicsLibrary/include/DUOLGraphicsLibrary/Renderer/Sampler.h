#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"

namespace DUOLGraphicsLibrary
{
 /**

     @class   Sampler
     @brief   ���������ο� ���ε��� ���ҽ� �� ���÷�.
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