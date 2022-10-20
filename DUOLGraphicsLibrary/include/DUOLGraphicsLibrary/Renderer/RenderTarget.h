#pragma once
#include <DUOLMath/DUOLMath.h>
#include "DUOLGraphicsLibrary/EntityBase.h"

namespace DUOLGraphicsLibrary
{
	class DUOLGRAPHICSLIBRARY_EXPORT RenderTarget : public EntityBase
	{
		DUOLGRAPHICSLIBRARY_DECLARE_ENTITY(EntityID::RenderTarget);
	
	public:
		virtual DUOLMath::Vector2 GetResolution() const abstract;
		
		virtual int GetNumberOfRenderTargets() abstract;

		virtual bool IsDepthStencil() const abstract;

		virtual bool IsColor() const abstract;

		virtual bool IsRenderContext() { return false; }
	};
}

