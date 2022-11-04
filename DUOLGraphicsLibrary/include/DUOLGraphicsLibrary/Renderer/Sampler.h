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

	protected:
		Sampler(const UINT64& guid) :
			Resource(guid)
		{

		}

	public:
		virtual ResourceType GetResourceType() override;

	};

}