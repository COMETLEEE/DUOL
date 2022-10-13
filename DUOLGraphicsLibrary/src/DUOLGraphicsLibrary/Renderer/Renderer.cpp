#include "DUOLGraphicsLibrary/Renderer/Renderer.h"

namespace DUOLGraphicsLibrary
{

	std::unique_ptr<Renderer> Renderer::CreateRenderer(const RendererDesc& renderDesc)
	{
		return nullptr;
	}

	void Renderer::DeleteRenderer(std::unique_ptr<Renderer>&& renderer)
	{
	}

}

