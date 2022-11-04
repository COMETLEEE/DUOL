#pragma once
#include <vector>
#include "DUOLGraphicsLibrary/Renderer/RenderPass.h"
#include "DUOLGraphicsLibrary/RenderPassFlags.h"
#include "DUOLGraphicsLibrary_Direct3D11/Direct3D11.h"


namespace DUOLGraphicsLibrary
{
	class D3D11RenderPass : public RenderPass
	{
	public:
		D3D11RenderPass(const RenderPassDesc& renderPassDesc);


	private:
		std::vector<ID3D11RenderTargetView*> _renderTargetViewRefs;

		ID3D11DepthStencilView* _depthStencilViewRef;

		UINT _numOfRenderTargetViewRefs;

	public:
		void BindRenderPass(ID3D11DeviceContext* context);

	private:
		void LoadRenderTargetViewRefs(const RenderPassDesc& renderPassDesc);

		void LoadDepthStencilViewRefs(const RenderPassDesc& renderPassDesc);

	};

}
