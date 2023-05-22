#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/Depth.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/DirectX11/BlendState.h"
#include <d3d11_1.h>

#include "Core/Resource/Resource/ParticleMesh.h"

#define LAYER_COUNT 16

namespace MuscleGrapics
{
	OrderIndependentTransparency::~OrderIndependentTransparency()
	{
		Finalize();
	}

	void OrderIndependentTransparency::OnResize()
	{
		_dxEngine = DXEngine::GetInstance();

		_d3dContext = _dxEngine->Getd3dImmediateContext();

		Finalize();

		_OIT_PixelData_Over_Color_Texture = _dxEngine->GetRenderTarget()->GetRenderTexture()[static_cast<unsigned int>(MutilRenderTexture::OIT_PixelData_Over_Color)];
		_OIT_PixelData_Over_Info_Texture = _dxEngine->GetRenderTarget()->GetRenderTexture()[static_cast<unsigned int>(MutilRenderTexture::OIT_PixelData_Over_Info)];
		_OIT_PixelData_Additive_Color_Texture = _dxEngine->GetRenderTarget()->GetRenderTexture()[static_cast<unsigned int>(MutilRenderTexture::OIT_PixelData_Additive_Color)];
		_OIT_PixelData_Additive_Info_Texture = _dxEngine->GetRenderTarget()->GetRenderTexture()[static_cast<unsigned int>(MutilRenderTexture::OIT_PixelData_Additive_Info)];
	}

	void OrderIndependentTransparency::SetRTV_OITLayerCreate()
	{
		DXEngine::GetInstance()->GetRenderTarget()->SetRenderTargetView(nullptr, 4,
			_OIT_PixelData_Over_Color_Texture->GetRTV(),
			_OIT_PixelData_Over_Info_Texture->GetRTV(),
			_OIT_PixelData_Additive_Color_Texture->GetRTV(),
			_OIT_PixelData_Additive_Info_Texture->GetRTV()
		);
	}

	void OrderIndependentTransparency::UnBindingResource_UAV()
	{
		_d3dContext->ClearState(); // UAV만 파이프라인에서 언바인딩 하는 방법을 못찾았다..

		_dxEngine->GetRenderTarget()->SetViewPort();

		// todo : 나중에 찾아보자.
		//_uav_list[0] = nullptr;
		//_uav_list[1] = nullptr;

		//_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
		//	nullptr, nullptr,
		//	0, 2, _uav_list, _initCount);
	}

	// 픽셀 셰이더에서 밖에 쓸 일이 없다...!
	void OrderIndependentTransparency::BindingResource_SRV() // 레이어를 화면에 그릴 때 쓰는 함수.
	{

		auto rtv = _dxEngine->GetRenderTarget()->GetRenderTargetView();
		_dxEngine->GetRenderTarget()->SetRenderTargetView(nullptr, 1, rtv);

		_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &rtv,
			nullptr, 0, 0, nullptr, nullptr);

		ID3D11ShaderResourceView* srv[4] = {
		_OIT_PixelData_Over_Color_Texture->GetSRV(),
		_OIT_PixelData_Over_Info_Texture->GetSRV(),
		_OIT_PixelData_Additive_Color_Texture->GetSRV(),
		_OIT_PixelData_Additive_Info_Texture->GetSRV(),
		};

		_d3dContext->PSSetShaderResources(2, 4, srv);
	}

	void OrderIndependentTransparency::Finalize()
	{
		while (!_renderQueueParticle.empty())
			_renderQueueParticle.pop();

		while (!_renderQueue3D.empty())
			_renderQueue3D.pop();


		_OIT_PixelData_Over_Color_Texture = nullptr;
		_OIT_PixelData_Over_Info_Texture = nullptr;
		_OIT_PixelData_Additive_Color_Texture = nullptr;
		_OIT_PixelData_Additive_Info_Texture = nullptr;

	}

	void OrderIndependentTransparency::Clear()
	{
		float _color[4] = { 0,0,0,0 };
		float _info[4] = { 0,0,1,1 };

		_OIT_PixelData_Over_Color_Texture->ClearRenderTarget(_color);
		_OIT_PixelData_Over_Info_Texture->ClearRenderTarget(_info);

		_OIT_PixelData_Additive_Color_Texture->ClearRenderTarget(_color);
		_OIT_PixelData_Additive_Info_Texture->ClearRenderTarget(_info);
	}

	// 완성된 Layer를 그리는 함수.
	void OrderIndependentTransparency::Draw()
	{
		BindingResource_SRV();

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		auto pass = _dxEngine->GetResourceManager()->GetResource<Pass_Texture>("OITBlendPass");

		pass->SetDrawRectangle(0, _dxEngine->GetWidth(), 0, _dxEngine->GetHeight());

		std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderingData;

		renderingData.push_back({ _dxEngine->GetRenderTarget()->GetDeferredTexture()->GetSRV(),0 });

		pass->Draw(renderingData);

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	void OrderIndependentTransparency::PostProcessing()
	{
		Renderer::BeginEvent(TEXT("ParticlePostProcessing"));

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		//// ----------------------------------- Out Line -----------------------------------------------
		const auto outlineShader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Texture>("OutLinePass");

		std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderingData;

		renderingData.push_back({ renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::OutLine)]->GetSRV(),0 });

		renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->ClearRenderTarget();

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->GetRTV());

		outlineShader->Draw(renderingData);

		renderTarget->SetRenderTargetView(nullptr, 0);

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		renderingData.clear();
		// ----------------------------------- Out Line -----------------------------------------------
		// ----------------------------------- Blur ---------------------------------------------------
		renderingData.push_back({ renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->GetSRV(),0 });

		const auto blurShader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Texture>("BlurPass");

		blurShader->Draw(renderingData);

		// ----------------------------------- Blur ----------------------------------------------------
		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(*BlendState::GetUiBlendState(), nullptr, 0xffffffff);

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTargetView());

		outlineShader->Draw(renderingData);

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		Renderer::EndEvent();
	}

	void OrderIndependentTransparency::RegistRenderingData(std::queue<std::shared_ptr<RenderingData_3D>>& renderQueue_3D)
	{
		std::swap(_renderQueue3D, renderQueue_3D);
	}

	// 오브젝트를 Layer별로 나누는 함수
	void OrderIndependentTransparency::RegistRenderingData(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle)
	{
		std::swap(_renderQueueParticle, renderQueueParticle);
	}

	void MuscleGrapics::OrderIndependentTransparency::CreateLayer()
	{
		Clear();

		Renderer::BeginEvent(TEXT("CreateOITLayer"));

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		std::queue<std::shared_ptr<RenderingData_Particle>> particleTemp;

		std::queue<std::shared_ptr<RenderingData_3D>> object3DTemp;

		std::wstring str = TEXT("OIT");

		Renderer::BeginEvent(str.c_str());

		Renderer::BeginEvent(TEXT("Particle"));

		while (!_renderQueueParticle.empty())
		{
			auto& object = _renderQueueParticle.front();

			for (auto& iter : object->shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Particle>(iter);

				shader->Draw(*object);
			}

			if (object->_isDelete)
				DXEngine::GetInstance()->GetResourceManager()->SubResource<ParticleMesh>("Particle" + object->_objectID);

			_renderQueueParticle.pop();
		}

		Renderer::EndEvent();

		Renderer::BeginEvent(TEXT("3DObject"));

		while (!_renderQueue3D.empty())
		{
			auto& object = _renderQueue3D.front();

			for (auto& iter : object->_shaderInfo._shaderName)
			{
				const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_3D>(iter);

				shader->Draw(*object);
			}

			_renderQueue3D.pop();
		}

		Renderer::EndEvent();

		Renderer::EndEvent();

		UnBindingResource_UAV();

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTargetView());

		Renderer::EndEvent();
	}

	void MuscleGrapics::OrderIndependentTransparency::MergeLayer()
	{
		Renderer::BeginEvent(TEXT("MergeOITLayer"));
		Draw();
		Renderer::EndEvent();
	}

	OrderIndependentTransparency& OrderIndependentTransparency::Get()
	{
		static OrderIndependentTransparency instacne;

		return instacne;
	}

}
