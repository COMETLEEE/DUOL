#pragma once
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11Renderer.h"

namespace DUOLGraphicsLibrary
{
	extern "C"
	{
		DUOLGRAPHICSLIBRARY_EXPORT DUOLGraphicsLibrary::Renderer* DUOLGraphics_Renderer_Alloc(const DUOLGraphicsLibrary::RendererDesc& renderDesc)
		{
			return new DUOLGraphicsLibrary::D3D11Renderer(renderDesc);
		}

		DUOLGRAPHICSLIBRARY_EXPORT void DUOLGraphics_Renderer_Free(DUOLGraphicsLibrary::Renderer* renderer)
		{
			delete renderer;
		}
	}
}

