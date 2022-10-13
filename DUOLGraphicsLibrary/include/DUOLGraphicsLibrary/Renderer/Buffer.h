#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/BufferFlags.h"

namespace DUOLGraphicsLibrary
{
	class DUOLGRAPHICSLIBRARY_EXPORT Buffer : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Buffer);

	protected:

		BufferDesc _bufferDesc;

	public:

		virtual ResourceType GetResourceType() override;

		virtual BufferDesc GetBufferDesc() { return _bufferDesc; }

	};
}
