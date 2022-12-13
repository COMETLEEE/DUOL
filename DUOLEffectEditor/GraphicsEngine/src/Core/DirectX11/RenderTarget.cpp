#include "Core/DirectX11/RenderTarget.h"
#include <dxgi.h>
#include "Core/Pass/DeferredRenderPass.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/RenderTexture.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/ObjectIDTexture.h"


namespace MuscleGrapics
{
	RenderTexture* RenderTarget::_renderTexture[Mutil_Render_Count + 1] = {}; // �ٸ� �н����� �ʿ� �� ���� ������ static���� ������..

	ObjectIDTexture* RenderTarget::_objectIDTxture = nullptr;

	RenderTarget::RenderTarget()
	{
		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			_renderTexture[i] = new RenderTexture();

			_textureRenderTargetView[i] = nullptr;
		}

		_deferredTexture = new RenderTexture();

		_textureRenderPass = new TextureRenderPass();

		_deferredRenderPass = new DeferredRenderPass();

		_objectIDTxture = new ObjectIDTexture();

		_renderTexture[Mutil_Render_Count] = _objectIDTxture;

	}

	RenderTarget::~RenderTarget()
	{
		Release();

		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			delete _renderTexture[i];
		}

		delete _deferredTexture;

		delete _textureRenderPass;

		delete _deferredRenderPass;

		delete _objectIDTxture;
	}
	void RenderTarget::OnResize()
	{
		IDXGISwapChain* _SwapChain = DXEngine::GetInstance()->GetSwapChain();

		ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		UINT _Width = DXEngine::GetInstance()->GetWidth();

		UINT _Height = DXEngine::GetInstance()->GetHeight();

		for (int i = 0; i < Mutil_Render_Count + 1; i++)
		{
			_renderTexture[i]->Initialize(_Width, _Height);

			_textureRenderTargetView[i] = _renderTexture[i]->GetRenderTargetView();
		}
		_deferredTexture->Initialize(_Width, _Height);

		_deferredRenderTargetView = _deferredTexture->GetRenderTargetView();

		ATLTRACE("OnResize ������ ����\n");

		Release();

		ATLTRACE("OnResize ���� ������ �缳��\n");

		HR(_SwapChain->ResizeBuffers((UINT)1, _Width, _Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		ID3D11Texture2D* backBuffer;

		_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer)); //��ȯ �罽�� ����Ű�� �����͸� ��´�.

		ATLTRACE("OnResize ���� ��� �並 ����\n");

		_Device->CreateRenderTargetView(backBuffer, 0,
			&_renderTargetView); //���� ��� �並 ����

		backBuffer->Release();

		D3D11_VIEWPORT _Viwport; // ����Ʈ

		_Viwport.TopLeftX = 0;
		_Viwport.TopLeftY = 0;
		_Viwport.Width = static_cast<float>(_Width);
		_Viwport.Height = static_cast<float>(_Height);
		_Viwport.MinDepth = 0;
		_Viwport.MaxDepth = 1.0f;

		_DeviceContext->RSSetViewports(1, &_Viwport);
	}


	void RenderTarget::SetRenderTargetView(int _Num)
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();

		_DC->OMSetRenderTargets(
			Mutil_Render_Count + 1, _textureRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(_Num));
	}

	void RenderTarget::BeginRender()
	{
		ClearRenderTarget();
	}

	void RenderTarget::ExecuteRender()
	{
		/// <summary>
		/// �� �Լ��� �����ϱ����� ���۵� ������ ���� �ؽ��ĸ� �ϼ� ���� ���� �����̴�.
		/// </summary>
		CreateDeferredTexture();

		SetBackBufferRenderTarget(); // ���� �ϼ��� �ؽ��ĵ�� ����ۿ� �׸� ����

		RenderDeferredWindow(); // postprocessing

		RenderDebugWindow(); // Debug
	}
	unsigned int RenderTarget::PickObjectID(int x, int y)
	{
		ID3D11DeviceContext* dc = DXEngine::GetInstance()->Getd3dImmediateContext();
		D3D11_BOX pickPointBox;

		UINT width = DXEngine::GetInstance()->GetWidth();
		UINT height = DXEngine::GetInstance()->GetHeight();

		if (width <= x || height <= y)
			return 0;

		pickPointBox.left = x;
		pickPointBox.right = x + 1;
		pickPointBox.top = y;
		pickPointBox.bottom = y + 1;
		pickPointBox.front = 0;
		pickPointBox.back = 1;

		dc->OMSetRenderTargets(0, nullptr, nullptr);

		dc->CopySubresourceRegion(
			_objectIDTxture->GetCopyTargetTexture()
			, 0, 0, 0, 0,
			_objectIDTxture->GetRenderTargetTexture(),
			0, &pickPointBox);

		// ���� ������ Object ID..
		UINT pickID = 0;

		// Mapping SubResource Data..
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		// GPU Access Lock Texture Data..
		dc->Map(_objectIDTxture->GetCopyTargetTexture(), 0, D3D11_MAP_READ, 0, &mappedResource);

		// �ش� Pixel Copy�� �߸� �Ǿ��� ���..
		if (mappedResource.pData == nullptr) return 0;

		pickID = ((UINT*)mappedResource.pData)[0];

		// GPU Access UnLock Texture Data..
		dc->Unmap(_objectIDTxture->GetCopyTargetTexture(), 0);

		return pickID;
	}

	void RenderTarget::PopShaderResource()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
		ID3D11ShaderResourceView* shaderResourceView[Mutil_Render_Count + 1] = { nullptr };
		_DC->PSSetShaderResources(0, Mutil_Render_Count + 1, shaderResourceView);
	}

	void RenderTarget::ClearRenderTarget()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
		float _Color[4];
		_Color[0] = 0; //r
		_Color[1] = 0; //g
		_Color[2] = 0; //b
		_Color[3] = 1.0f; //a
		// ������� ������ ����ϴ�. 
		_DC->ClearRenderTargetView(_renderTargetView, _Color);
		// ���� ���۸� ����ϴ�. 


		// �ؽ�ó ���ҽ� �����ֱ�~
		for (int i = 0; i < Mutil_Render_Count + 1; i++)
			_renderTexture[i]->ClearRenderTarget();
		_deferredTexture->ClearRenderTarget();
		// ���� Ÿ�� ��� ���� ���ٽ� ���۸� ������ ���������ο� �����Ѵ�.


	}

	void RenderTarget::CreateDeferredTexture()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
		_DC->OMSetRenderTargets(1, &_deferredRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(0));

		std::vector<std::pair<ID3D11ShaderResourceView*, int>> shaderResource = {
		{_renderTexture[0]->GetSRV(),1}, // ����
		{_renderTexture[1]->GetSRV(),2}, // �븻
		{_renderTexture[2]->GetSRV(),3}, // ������
		{_renderTexture[3]->GetSRV(),4}, // �˺���
		{_renderTexture[4]->GetSRV(),5}, // mat diffuse
		{_renderTexture[5]->GetSRV(),6} // mat specular
		};


		_deferredRenderPass->Draw(shaderResource);
	}

	void RenderTarget::SetBackBufferRenderTarget()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();

		_DC->OMSetRenderTargets(1, &_renderTargetView, nullptr);
	}
	void RenderTarget::RenderDebugWindow()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(_renderTexture[i]->GetSRV(), 0);

			_textureRenderPass->SetDrawRectangle(
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count) + (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				0,
				(DXEngine::GetInstance()->GetHeight() / Mutil_Render_Count)
			);

			_textureRenderPass->Draw(renderData);
		}
		// ����Ÿ�ٺ信 �׸����� ȭ�鿡 �׸� �� ���ÿ� ���ε��� �ȵż� ����� ��. 
		PopShaderResource();
	}

	void RenderTarget::RenderDeferredWindow()
	{
		auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(_deferredTexture->GetSRV(), 0);

		_textureRenderPass->SetDrawRectangle(0, DXEngine::GetInstance()->GetWidth(), 0, DXEngine::GetInstance()->GetHeight());

		_textureRenderPass->Draw(renderData);
		///����Ʈ ���μ����� �Ϸ��� �Ѵٸ� �� �κ��� ����Ʈ ���μ��� �н��� �����ϸ� �ȴ�..!
	}

	void RenderTarget::Release()
	{
		ReleaseCOM(_renderTargetView);
	}
}
