#pragma once
#include "Export.h"
#include "DUOLGraphicsLibrary/RendererFlags.h"

struct DUOLGRAPHICSENGINE_EXPORT DUOLGraphicsLibrary::RendererDesc;

namespace DUOLGraphicsEngine
{

	struct GraphicsEngineDesc
	{
		DUOLGraphicsLibrary::RendererDesc _rendererDesc;
		DUOLGraphicsLibrary::RenderContextDesc _contextDesc;
	};

}