#include "Core/DirectX11/OrderIndependentTransparency.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/RenderTexture.h"
#include "Core/DirectX11/RenderTarget.h"
#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/Depth.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/DirectX11/BlendState.h"



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

		_dxEngine->GetResourceManager()->GetTextureRenderPass()->SetDrawRectangle(0, _dxEngine->GetWidth(), 0, _dxEngine->GetHeight());

		for (auto iter = _vdxPic.rbegin(); iter != _vdxPic.rend(); iter++)
		{
			auto renderData = std::make_pair(iter->_backSrv, 0);
			_dxEngine->GetResourceManager()->GetTextureRenderPass()->Draw(renderData);
		}

		_dxEngine->Getd3dImmediateContext()->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	// 오브젝트를 Layer별로 나누는 함수
	void OrderIndependentTransparency::Render(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle)
	{
		auto renderTarget = DXEngine::GetInstance()->GetRenderTarget();

		std::queue<std::shared_ptr<RenderingData_Particle>> temp;

		for (_drawCount = 0; _drawCount < g_layerCount; _drawCount++)
		{
			if (!temp.empty())
				renderQueueParticle.swap(temp);
			while (!renderQueueParticle.empty())
			{
				/// <summary>
				/// 파티클은 반투명하기 때문에 렌더링의 순서를 마지막으로 변경 하여야한다.
				/// </summary>
				auto& object = renderQueueParticle.front();


				for (auto& iter : object->shaderName)
				{
					const auto shader = DXEngine::GetInstance()->GetResourceManager()->GetParticleShader(iter);

					shader->Draw(*object);
				}

				if (object->_isDelete)
					DXEngine::GetInstance()->GetResourceManager()->DeleteParticleMesh(object->_objectID);

				if (_drawCount < g_layerCount - 1)
					temp.push(object);
				renderQueueParticle.pop();
			}
		}

		renderTarget->SetRenderTargetView(nullptr, 1, renderTarget->GetRenderTargetView());

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

	OrderIndependentTransparency& OrderIndependentTransparency::Get()
	{
		static OrderIndependentTransparency instacne;

		return instacne;
	}
}
