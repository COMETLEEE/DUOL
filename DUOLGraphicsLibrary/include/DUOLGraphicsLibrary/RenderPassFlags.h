#pragma once
#include <vector>

namespace DUOLGraphicsLibrary
{
	class RenderTarget;

	struct RenderPassComponentDesc
	{
		RenderTarget* renderTarget;
	};

	struct RenderPassDesc
	{
		//D3D11���� MAX�� 8��.
		std::vector<RenderTarget*> _renderTargetView;

		RenderTarget* _depthStencilView;
	};
}
