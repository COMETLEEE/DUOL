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
		//D3D11에선 MAX가 8임.
		std::vector<RenderTarget*> _renderTargetView;

		RenderTarget* _depthStencilView;
	};
}
