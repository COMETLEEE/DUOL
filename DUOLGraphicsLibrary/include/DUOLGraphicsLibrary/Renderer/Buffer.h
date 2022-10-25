#pragma once
#include "DUOLGraphicsLibrary/Renderer/Resource.h"
#include "DUOLGraphicsLibrary/BufferFlags.h"

namespace DUOLGraphicsLibrary
{

 /**

     @class   Buffer
     @brief   파이프라인에 바인딩 될 버퍼들. Vertex, Index, Constant.... 등
     @details ~
     @author  KyungMin Oh

 **/
	class DUOLGRAPHICSLIBRARY_EXPORT Buffer : public Resource
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::Buffer);
	protected:
		Buffer(const BufferDesc& bufferDesc) :
			_bufferDesc(bufferDesc)
		{

		}

	protected:
		BufferDesc _bufferDesc;

	public:

		virtual ResourceType GetResourceType() override;

		virtual BufferDesc GetBufferDesc() { return _bufferDesc; }

	};
}
