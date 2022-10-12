#include "DUOLGraphicsLibrary/EntityBase.h"

#include "DUOLGraphicsLibrary/Renderer/Renderer.h"
//#include "DUOLGraphicsLibrary/Renderer/RenderContext.h"
//#include "DUOLGraphicsLibrary/Renderer/Buffer.h"
//#include "DUOLGraphicsLibrary/Renderer/BufferArray.h"
//#include "DUOLGraphicsLibrary/Renderer/Texture.h"
//#include "DUOLGraphicsLibrary/Renderer/Sampler.h"
//#include "DUOLGraphicsLibrary/Renderer/Shader.h"
//#include "DUOLGraphicsLibrary/Renderer/RenderTarget.h"

namespace DUOLGraphicsLibrary
{

	bool EntityBase::IsInstanceOf(int id) const
	{
		return (static_cast<EntityID>(id) == EntityID::EntityBase);
	}


	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Renderer, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(RenderContext, EntityBase)

	RenderContext::RenderContext(const ScreenDesc& screenDesc, const FrameRateDesc& frameRateDesc) :
		_screenDesc(screenDesc)
		, _frameRateDesc(frameRateDesc)
	{

	}

	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Buffer, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(BufferArray, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Texture, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Sampler, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Resource, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Shader, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(RenderTarget, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(CommandBuffer, EntityBase)

}