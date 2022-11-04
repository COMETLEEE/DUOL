#pragma once
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Renderer/D3D11Renderer.h"

namespace DUOLGraphicsLibrary
{
	static std::unique_ptr<D3D11Renderer> g_renderer;

	extern "C"
	{
		DUOLGRAPHICSLIBRARY_EXPORT DUOLGraphicsLibrary::Renderer* DUOLGraphics_Renderer_Alloc(const DUOLGraphicsLibrary::RendererDesc& renderDesc)
		{
			g_renderer = std::make_unique<D3D11Renderer>(renderDesc);
			return g_renderer.get();
		}

		DUOLGRAPHICSLIBRARY_EXPORT void DUOLGraphics_Renderer_Free(DUOLGraphicsLibrary::Renderer* renderer)
		{
			if (g_renderer.get() == renderer)
			{
				g_renderer.reset();
			}
		}
	}
}

