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

		auto device = _dxEngine->GetD3dDevice();

		Finalize();

		std::vector<Structure::PixelNode> initVertex(DXEngine::GetInstance()->GetWidth() * DXEngine::GetInstance()->GetHeight() * LAYER_COUNT);

		UINT elenmentsCount = _dxEngine->GetWidth() * _dxEngine->GetHeight() * LAYER_COUNT;

		D3D11_BUFFER_DESC buffDesc;
		buffDesc.ByteWidth = sizeof(Structure::PixelNode) * elenmentsCount;
		buffDesc.Usage = D3D11_USAGE_DEFAULT;
		buffDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffDesc.CPUAccessFlags = 0;
		buffDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffDesc.StructureByteStride = sizeof(Structure::PixelNode);

		D3D11_SUBRESOURCE_DATA	data;
		ZeroMemory(&data, sizeof(data));

		data.pSysMem = initVertex.data();
		data.SysMemPitch = buffDesc.ByteWidth;
		data.SysMemSlicePitch = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		srvDesc.Buffer.NumElements = elenmentsCount;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN; //Must be UNKOWN when creating structured Buffer
		uavDesc.Buffer.NumElements = elenmentsCount;


		if (FAILED(device->CreateBuffer(&buffDesc, &data, &_pixelLinkBufferBuffer)))
			assert(false);
		if (FAILED(device->CreateShaderResourceView(_pixelLinkBufferBuffer, &srvDesc, &_pixelLinkBufferSRV)))
			assert(false);
		if (FAILED(device->CreateUnorderedAccessView(_pixelLinkBufferBuffer, &uavDesc, &_pixelLinkBufferUAV)))
			assert(false);

		// -------------------------------------------------------------------------------------------------------
		std::vector<unsigned int> initData(elenmentsCount);
		D3D11_BUFFER_DESC rawbufferDesc;
		rawbufferDesc.Usage = D3D11_USAGE_DEFAULT;
		rawbufferDesc.ByteWidth = elenmentsCount;
		rawbufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		rawbufferDesc.CPUAccessFlags = 0;
		rawbufferDesc.StructureByteStride = 0;
		rawbufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

		D3D11_SHADER_RESOURCE_VIEW_DESC rawsrvDesc;
		rawsrvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		rawsrvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		rawsrvDesc.BufferEx.FirstElement = 0;
		rawsrvDesc.BufferEx.NumElements = _dxEngine->GetWidth() * _dxEngine->GetHeight();
		rawsrvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;

		D3D11_UNORDERED_ACCESS_VIEW_DESC rawuavDesc;
		rawuavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		rawuavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		rawuavDesc.Buffer.FirstElement = 0;
		rawuavDesc.Buffer.NumElements = _dxEngine->GetWidth() * _dxEngine->GetHeight();
		rawuavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

		if (FAILED(device->CreateBuffer(&rawbufferDesc, nullptr, &_FirstOffsetBuffer)))
			assert(false);
		if (FAILED(device->CreateShaderResourceView(_FirstOffsetBuffer, &rawsrvDesc, &_FirstOffsetBufferSRV)))
			assert(false);
		if (FAILED(device->CreateUnorderedAccessView(_FirstOffsetBuffer, &rawuavDesc, &_FirstOffsetBufferUAV)))
			assert(false);

	}
	// 픽셀 셰이더에서 밖에 쓸 일이 없다...!
	void OrderIndependentTransparency::BindingResource_UAV() // 레이어를 만들 때 쓰는 함수.
	{
		auto depth = _dxEngine->GetDepthStencil()->GetDepthStencilView(0);

		_dxEngine->GetRenderTarget()->SetRenderTargetView(depth, 0);

		//// 숨겨진 카운트를 0으로 만들기 때문에 한번만 불러야한다.
		_uav_list[0] = _pixelLinkBufferUAV;
		_uav_list[1] = _FirstOffsetBufferUAV;

		_initCount[0] = -1;
		_initCount[1] = -1;

		ID3D11RenderTargetView* rtv = nullptr;

		_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(0,
			nullptr, _dxEngine->GetDepthStencil()->GetDepthStencilView(0),
			0, 2, _uav_list, _initCount);

		//UnBindingResource_UAV();
		// Todo : 왜 바인딩 하면 디버깅이 안되니....?
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
		ID3D11UnorderedAccessView* uav_list[2];

		uav_list[0] = nullptr;
		uav_list[1] = nullptr;

		auto rtv = _dxEngine->GetRenderTarget()->GetRenderTargetView();

		_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &rtv,
			nullptr, 0, 0, nullptr, nullptr);

		_d3dContext->PSSetShaderResources(2, 1, &_pixelLinkBufferSRV);
		_d3dContext->PSSetShaderResources(3, 1, &_FirstOffsetBufferSRV);
	}

	void OrderIndependentTransparency::Finalize()
	{
		while (!_renderQueueParticle.empty())
			_renderQueueParticle.pop();

		while (!_renderQueue3D.empty())
			_renderQueue3D.pop();


		ReleaseCOM(_pixelLinkBufferBuffer);
		ReleaseCOM(_pixelLinkBufferSRV);
		ReleaseCOM(_pixelLinkBufferUAV);

		ReleaseCOM(_FirstOffsetBuffer);
		ReleaseCOM(_FirstOffsetBufferSRV);
		ReleaseCOM(_FirstOffsetBufferUAV);
	}

	void OrderIndependentTransparency::Clear()
	{
		unsigned int clearNum = 0xffffffff;

		_d3dContext->ClearUnorderedAccessViewUint(_pixelLinkBufferUAV, &clearNum);
		_d3dContext->ClearUnorderedAccessViewUint(_FirstOffsetBufferUAV, &clearNum);

		auto depth = _dxEngine->GetDepthStencil()->GetDepthStencilView(0);

		_dxEngine->GetRenderTarget()->SetRenderTargetView(depth, 0);

		//// 숨겨진 카운트를 0으로 만들기 때문에 한번만 불러야한다.
		_uav_list[0] = _pixelLinkBufferUAV;
		_uav_list[1] = _FirstOffsetBufferUAV;

		_initCount[0] = 0;
		_initCount[1] = 0;

		ID3D11RenderTargetView* rtv = nullptr;

		_d3dContext->OMSetRenderTargetsAndUnorderedAccessViews(D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
			nullptr, nullptr,
			0, 2, _uav_list, _initCount);

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

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTexture()[static_cast<int>(MutilRenderTexture::NullTexture)]->GetRenderTargetView());

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
