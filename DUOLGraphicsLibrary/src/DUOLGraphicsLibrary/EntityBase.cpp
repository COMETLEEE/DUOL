#include "DUOLGraphicsLibrary/EntityBase.h"
#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{

	bool EntityBase::IsInstanceOf(int id) const
	{
		return (static_cast<EntityID>(id) == EntityID::EntityBase);
	}

	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Renderer, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Buffer, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(BufferArray, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Texture, EntityBase)


	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Sampler, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Resource, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(Shader, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(RenderTarget, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(RenderContext, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(CommandBuffer, EntityBase)
	DUOLGRAPHICSLIBRARY_IMPLEMENT_ENTITY(PipelineState, EntityBase)
}
