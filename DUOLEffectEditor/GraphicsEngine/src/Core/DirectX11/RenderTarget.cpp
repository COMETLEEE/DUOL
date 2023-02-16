#include "Core/DirectX11/RenderTarget.h"
#include <dxgi.h>
#include "Core/Pass/DeferredRenderPass.h"
#include "Core/Pass/TextureRenderPass.h"
#include "Core/DirectX11/DXEngine.h"
#include "Core/DirectX11/RenderTexture.h"

#include "Core/DirectX11/DepthStencil.h"
#include "Core/DirectX11/ObjectIDTexture.h"
#include "Core/Resource/ResourceManager.h"


namespace MuscleGrapics
{
	RenderTexture* RenderTarget::_renderTexture[Mutil_Render_Count + Additional_Texture] = {}; // �ٸ� �н����� �ʿ� �� ���� ������ static���� ������..

	ObjectIDTexture* RenderTarget::_objectIDTxture = nullptr;

	RenderTarget::RenderTarget()
	{

	}
	void RenderTarget::init()
	{
		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			_renderTexture[i] = new RenderTexture();
		}

		_deferredTexture = new RenderTexture();

		_deferredRenderPass = new DeferredRenderPass();

		_objectIDTxture = new ObjectIDTexture();

		_renderTexture[6] = _objectIDTxture;

		_renderTexture[7] = new RenderTexture();

		_renderTexture[8] = new RenderTexture();

		_renderTexture[8]->SetScale(0.5f);

		_renderTexture[9] = new RenderTexture();

		_renderTexture[9]->SetScale(0.25f);

		_renderTexture[10] = new RenderTexture();

		_renderTexture[10]->SetScale(0.125f);

		_renderTexture[11] = new RenderTexture();

		_renderTexture[11]->SetScale(0.0625f);

		_renderTexture[12] = new RenderTexture();
	}
	RenderTarget::~RenderTarget()
	{
		Release();

		for (int i = 0; i < Mutil_Render_Count + Additional_Texture; i++)
		{
			delete _renderTexture[i];
			_renderTexture[i] = nullptr;
		}

		delete _deferredTexture;

		delete _deferredRenderPass;

	}
	void RenderTarget::OnResize()
	{
		IDXGISwapChain* _SwapChain = DXEngine::GetInstance()->GetSwapChain();

		ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		UINT _Width = DXEngine::GetInstance()->GetWidth();

		UINT _Height = DXEngine::GetInstance()->GetHeight();

		for (int i = 0; i < Mutil_Render_Count + Additional_Texture; i++)
		{
			_renderTexture[i]->Initialize(_Width, _Height);

			//_textureRenderTargetView[i] = _renderTexture[i]->GetRenderTargetView();
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


	void RenderTarget::SetRenderTargetView(ID3D11DepthStencilView* _depthStencilView, int _num, ...)
	{
		ID3D11DeviceContext* dc = DXEngine::GetInstance()->Getd3dImmediateContext();

		if (_num == 0)
		{
			dc->OMSetRenderTargets(0, nullptr, _depthStencilView);
			return;
		}

		va_list VA_LIST;
		va_start(VA_LIST, _num);


		std::vector<ID3D11RenderTargetView*> textureRTVs;

		for (int i = 0; i < _num; i++)
		{
			textureRTVs.push_back(va_arg(VA_LIST, ID3D11RenderTargetView*));
		}

		dc->OMSetRenderTargetsAndUnorderedAccessViews(
			_num, textureRTVs.data(), _depthStencilView,
			_num, 0, nullptr, nullptr);

		va_end(VA_LIST);
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

		RenderDeferredWindow(); // postprocessing

#ifdef DEBUG
		RenderDebugWindow(); // Debug
#endif
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



	ID3D11RenderTargetView* RenderTarget::GetRenderTargetView()
	{
		return _renderTargetView;
	}

	RenderTexture* RenderTarget::GetDeferredTexture()
	{
		return _deferredTexture;
	}

	ID3D11RenderTargetView* RenderTarget::GetDeferredRTV()
	{
		return _deferredRenderTargetView;
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
		_Color[3] = 0; //a
		// ������� ������ ����ϴ�. 
		_DC->ClearRenderTargetView(_renderTargetView, _Color);
		// ���� ���۸� ����ϴ�. 


		// �ؽ�ó ���ҽ� �����ֱ�~
		for (int i = 0; i < Mutil_Render_Count + Additional_Texture; i++)
			_renderTexture[i]->ClearRenderTarget();
		_deferredTexture->ClearRenderTarget();
		// ���� Ÿ�� ��� ���� ���ٽ� ���۸� ������ ���������ο� �����Ѵ�.


	}

	void RenderTarget::CreateDeferredTexture()
	{

		SetRenderTargetView(DXEngine::GetInstance()->GetDepthStencil()->GetDepthStencilView(0), 1, _deferredRenderTargetView);

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


	void RenderTarget::RenderDebugWindow()
	{
		auto renderPass = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Texture>("TextureRenderPass");

		SetRenderTargetView(nullptr, 1, _renderTargetView);

		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderData;

			renderData.push_back(std::make_pair<ID3D11ShaderResourceView*, int>(_renderTexture[i]->GetSRV(), 0));

			renderPass->SetDrawRectangle(
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count) + (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				0,
				(DXEngine::GetInstance()->GetHeight() / Mutil_Render_Count)
			);

			renderPass->Draw(renderData);
		}
		// ����Ÿ�ٺ信 �׸����� ȭ�鿡 �׸� �� ���ÿ� ���ε��� �ȵż� ����� ��. 
		PopShaderResource();
	}

	void RenderTarget::RenderDeferredWindow()
	{
		//auto renderPass = DXEngine::GetInstance()->GetResourceManager()->GetResource<Pass_Texture>("TextureRenderPass");

		//SetRenderTargetView(nullptr, 1, _renderTargetView);

		//std::vector<std::pair<ID3D11ShaderResourceView*, int>> renderData;

		//renderData.push_back({ _deferredTexture->GetSRV(), 0 });

		//renderPass->SetDrawRectangle(0, DXEngine::GetInstance()->GetWidth(), 0, DXEngine::GetInstance()->GetHeight());

		//renderPass->Draw(renderData);
		/////����Ʈ ���μ����� �Ϸ��� �Ѵٸ� �� �κ��� ����Ʈ ���μ��� �н��� �����ϸ� �ȴ�..!
	}

	void RenderTarget::Release()
	{
		ReleaseCOM(_renderTargetView);
	}
}
