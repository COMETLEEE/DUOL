#include "D3D11RenderPass.h"
#include "DUOLGraphicsLibrary_Direct3D11/Util/DXHelper.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/RenderTarget/D3D11RenderTarget.h"
#include "DUOLGraphicsLibrary/Core/TypeCast.h"


DUOLGraphicsLibrary::D3D11RenderPass::D3D11RenderPass(const RenderPassDesc& renderPassDesc)
{
	LoadRenderTargetViewRefs(renderPassDesc);
	LoadDepthStencilViewRefs(renderPassDesc);
}

void DUOLGraphicsLibrary::D3D11RenderPass::BindRenderPass(ID3D11DeviceContext* context)
{
	context->ClearDepthStencilView(_depthStencilViewRef, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	for(auto& renderTarget : _renderTargetViewRefs)
	{
		const float color[4] = {0.2f, 0.2f, 0.2f, 1.f};

		context->ClearRenderTargetView(renderTarget, color);
	}


	context->OMSetRenderTargets(_numOfRenderTargetViewRefs, _renderTargetViewRefs.data(), _depthStencilViewRef);
}

void DUOLGraphicsLibrary::D3D11RenderPass::LoadRenderTargetViewRefs(const RenderPassDesc& renderPassDesc)
{
	for (auto& renderTarget : renderPassDesc._renderTargetView)
	{
		auto castedRenderTarget = TYPE_CAST(D3D11RenderTarget*, renderTarget);

		if (castedRenderTarget->IsColor())
		{
			_renderTargetViewRefs.emplace_back(castedRenderTarget->GetNativeRenderTarget()._renderTargetView.Get());
		}
	}

	_numOfRenderTargetViewRefs = static_cast<UINT>(_renderTargetViewRefs.size());
}

void DUOLGraphicsLibrary::D3D11RenderPass::LoadDepthStencilViewRefs(const RenderPassDesc& renderPassDesc)
{
	auto castedDepth = TYPE_CAST(D3D11RenderTarget*, renderPassDesc._depthStencilView);

	if (castedDepth->IsDepthStencil())
	{
		_depthStencilViewRef = castedDepth->GetNativeRenderTarget()._depthStencilView.Get();
	}
}
