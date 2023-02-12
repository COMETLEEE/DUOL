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


namespace MuscleGrapics
{
	OrderIndependentTransparency::~OrderIndependentTransparency()
	{
		Finalize();
	}

	void OrderIndependentTransparency::OnResize()
	{
		_dxEngine = DXEngine::GetInstance();

		Finalize();

		D3D11_TEXTURE2D_DESC texDesc;
		ID3D11Texture2D* tex;
		_dxEngine->GetRenderTarget()->GetRenderTexture()[0]->GetRenderTargetTexture()->GetDesc(&texDesc);
		texDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.ArraySize = 1;
		texDesc.MipLevels = 1;

		D3D11_SUBRESOURCE_DATA data;
		int nSize = 4 * texDesc.Width * texDesc.Height;
		data.pSysMem = (void*)malloc(nSize);
		ZeroMemory((void*)data.pSysMem, nSize);
		data.SysMemPitch = 4 * texDesc.Width;
		_dxEngine->GetD3dDevice()->CreateTexture2D(&texDesc, &data, &tex);
		free((void*)data.pSysMem);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		_dxEngine->GetD3dDevice()->CreateShaderResourceView(tex, &srvDesc, &_nullSRV);
		tex->Release();

		for (int i = 0; i < g_layerCount; i++)
		{
			_colorTexture[i] = new RenderTexture();

			D3D11_TEXTURE2D_DESC texDesc;
			D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc;
			ID3D11Texture2D* depthTex;
			ID3D11DepthStencilView* dsv;
			ID3D11ShaderResourceView* depthSrv;

			_dxEngine->GetDepthStencil()->GetDepth(0)->GetDepthBuffer()->GetDesc(&texDesc);

			texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			_dxEngine->GetD3dDevice()->CreateTexture2D(&texDesc, nullptr, &depthTex);

			_dxEngine->GetDepthStencil()->GetDepth(0)->GetDepthStencilView()->GetDesc(&dsvDesc);
			_dxEngine->GetD3dDevice()->CreateDepthStencilView(depthTex, &dsvDesc, &dsv);
			_dxEngine->Getd3dImmediateContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, NULL);

			depthSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			depthSrvDesc.Texture2D.MipLevels = 1;
			depthSrvDesc.Texture2D.MostDetailedMip = 0;
			_dxEngine->GetD3dDevice()->CreateShaderResourceView(depthTex, &depthSrvDesc, &depthSrv);
			depthTex->Release();



			_colorTexture[i]->Initialize(texDesc.Width, texDesc.Height);

			PictureInfo picInfo;

			picInfo._dsv = dsv;
			picInfo._depthSrv = depthSrv;
			picInfo._rtv = _colorTexture[i]->GetRenderTargetView();
			picInfo._backSrv = _colorTexture[i]->GetSRV();

			_vdxPic.push_back(picInfo);

		}

	}

	void OrderIndependentTransparency::Finalize()
	{
		ReleaseCOM(_nullSRV);

		for (int i = 0; i < g_layerCount; i++)
		{
			if (_colorTexture[i])
				delete _colorTexture[i];
			_colorTexture[i] = nullptr;
		}
		for (auto& iter : _vdxPic)
		{
			ReleaseCOM(iter._dsv);

			ReleaseCOM(iter._depthSrv);

			iter._rtv = nullptr;

			iter._backSrv = nullptr;
		}

		_vdxPic.clear();
	}

	void OrderIndependentTransparency::Clear()
	{
		for (auto& iter : _vdxPic)
		{
			_dxEngine->Getd3dImmediateContext()->ClearDepthStencilView(iter._dsv, D3D11_CLEAR_DEPTH, 1.0, NULL);
			const FLOAT colBlack[4] = { 0,0,0,0 };
			_dxEngine->Getd3dImmediateContext()->ClearRenderTargetView(iter._rtv, colBlack);
		}
	}

	// 완성된 Layer를 그리는 함수.
	void OrderIndependentTransparency::Draw()
	{
		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(*BlendState::GetSrcDestAdditiveBlendState(), nullptr, 0xffffffff);

		_dxEngine->GetRenderTarget()->SetRenderTargetView(nullptr, 1, _dxEngine->GetRenderTarget()->GetRenderTargetView());

		_dxEngine->GetResourceManager()->GetResource<Pass_Texture>("TextureRenderPass")->SetDrawRectangle(0, _dxEngine->GetWidth(), 0, _dxEngine->GetHeight());

		for (auto iter = _vdxPic.rbegin(); iter != _vdxPic.rend(); iter++)
		{
			std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderData;

			renderData.push_back(std::make_pair(iter->_backSrv, 0));

			_dxEngine->GetResourceManager()->GetResource<Pass_Texture>("TextureRenderPass")->Draw(renderData);
		}

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);

		_drawCount = 0;
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

		Clear();

		Renderer::EndEvent();
	}

	void OrderIndependentTransparency::SetRenderTargetAndDepth()
	{
		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		renderTarget->SetRenderTargetView(_vdxPic[_drawCount]._dsv, 1, _vdxPic[_drawCount]._rtv);

		DXEngine::GetInstance()->Getd3dImmediateContext()->PSSetShaderResources(2, 1, (_drawCount == 0) ? &_nullSRV : &_vdxPic[_drawCount - 1]._depthSrv);
	}

	int OrderIndependentTransparency::GetDrawCount()
	{
		return _drawCount;
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
		Renderer::BeginEvent(TEXT("CreateOITLayer"));

		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		std::queue<std::shared_ptr<RenderingData_Particle>> particleTemp;

		std::queue<std::shared_ptr<RenderingData_3D>> object3DTemp;

		for (_drawCount = 0; _drawCount < g_layerCount; _drawCount++)
		{
			std::wstring str = TEXT("OIT") + std::to_wstring(_drawCount);

			Renderer::BeginEvent(str.c_str());

			if (!particleTemp.empty())
				_renderQueueParticle.swap(particleTemp);

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

				if (_drawCount < g_layerCount - 1)
					particleTemp.push(object);

				_renderQueueParticle.pop();
			}

			Renderer::EndEvent();

			if (!object3DTemp.empty())
				_renderQueue3D.swap(object3DTemp);

			Renderer::BeginEvent(TEXT("3DObject"));

			while (!_renderQueue3D.empty())
			{
				auto& object = _renderQueue3D.front();

				for (auto& iter : object->_shaderInfo._shaderName)
				{
					const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_3D>(iter);

					shader->Draw(*object);
				}

				if (_drawCount < g_layerCount - 1)
					object3DTemp.push(object);

				_renderQueue3D.pop();
			}

			Renderer::EndEvent();

			Renderer::EndEvent();
		}
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
