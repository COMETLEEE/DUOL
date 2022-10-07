#pragma once
#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/ResourceFlags.h"

namespace DUOLGraphicsLibrary
{

	class Resource : public EntityBase
	{

		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Resource);

	public:

		virtual ResourceType GetResourceType() abstract;

	};

}

