#include "pch.h"
#include "RenderTarget.h"

#include "DeferredRenderPass.h"
#include "TextureRenderPass.h"
namespace MuscleGrapics
{
	RenderTexture* RenderTarget::_rederTexture[Mutil_Render_Count] = {}; // 다른 패스에서 필요 할 수도 있으니 static으로 만들자..

	RenderTarget::RenderTarget()
	{
		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			_rederTexture[i] = new RenderTexture();

			_textureRenderTargetView[i] = nullptr;
		}

		_deferredTexture = new RenderTexture();

		_textureRenderPass = new TextureRenderPass();

		_deferredRenderPass = new DeferredRenderPass();
	}

	RenderTarget::~RenderTarget()
	{
		Release();

		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			delete _rederTexture[i];
		}

		delete _deferredTexture;

		delete _textureRenderPass;

		delete _deferredRenderPass;
	}
	void RenderTarget::OnResize()
	{
		IDXGISwapChain* _SwapChain = DXEngine::GetInstance()->GetSwapChain();

		ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();

		ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		UINT _Width = DXEngine::GetInstance()->GetWidth();

		UINT _Height = DXEngine::GetInstance()->GetHeight();

		for (int i = 0; i < Mutil_Render_Count; i++)
		{
			_rederTexture[i]->Initialize(_Width, _Height);

			_textureRenderTargetView[i] = _rederTexture[i]->GetRenderTargetView();
		}
		_deferredTexture->Initialize(_Width, _Height);

		_deferredRenderTargetView = _deferredTexture->GetRenderTargetView();

		ATLTRACE("OnResize 포인터 삭제\n");

		Release();

		ATLTRACE("OnResize 버퍼 사이즈 재설정\n");

		HR(_SwapChain->ResizeBuffers((UINT)1, _Width, _Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

		ID3D11Texture2D* backBuffer;

		_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
			reinterpret_cast<void**>(&backBuffer)); //교환 사슬을 가르키는 포인터를 얻는다.

		ATLTRACE("OnResize 렌더 대상 뷰를 생성\n");

		_Device->CreateRenderTargetView(backBuffer, 0,
			&_renderTargetView); //랜더 대상 뷰를 생성

		backBuffer->Release();

		D3D11_VIEWPORT _Viwport; // 뷰포트

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
			Mutil_Render_Count, _textureRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(_Num));
	}

	void RenderTarget::BeginRender()
	{
		ClearRenderTarget();
	}

	void RenderTarget::ExecuteRender()
	{
		/// <summary>
		/// 이 함수를 실행하기전에 디퍼드 조립을 위한 텍스쳐를 완성 시켜 놓은 상태이다.
		/// </summary>
		CreateDeferredTexture();

		SetBackBufferRenderTarget(); // 이제 완성된 텍스쳐들로 백버퍼에 그릴 차례

		RenderDeferredWindow(); // postprocessing

		RenderDebugWindow(); // Debug
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
		// 백버퍼의 내용을 지웁니다. 
		_DC->ClearRenderTargetView(_renderTargetView, _Color);
		// 깊이 버퍼를 지웁니다. 


		// 텍스처 리소스 지워주기~
		for (int i = 0; i < Mutil_Render_Count; i++)
			_rederTexture[i]->ClearRenderTarget();
		_deferredTexture->ClearRenderTarget();
		// 렌더 타겟 뷰와 깊이 스텐실 버퍼를 렌더링 파이프라인에 결합한다.


	}

	void RenderTarget::CreateDeferredTexture()
	{
		ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
		_DC->OMSetRenderTargets(1, &_deferredRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(0));

		std::vector<pair<ID3D11ShaderResourceView*, int>> shaderResource = {
		{_rederTexture[0]->GetSRV(),1}, // 깊이
		{_rederTexture[1]->GetSRV(),2}, // 노말
		{_rederTexture[2]->GetSRV(),3}, // 포지션
		{_rederTexture[3]->GetSRV(),4}, // 알베도
		{_rederTexture[4]->GetSRV(),5}, // mat diffuse
		{_rederTexture[5]->GetSRV(),6}, // mat specular
		{_rederTexture[6]->GetSRV(),7} // mat ambinet
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
			auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(_rederTexture[i]->GetSRV(), 0);

			_textureRenderPass->SetDrawRectangle(
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				i * (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count) + (DXEngine::GetInstance()->GetWidth() / Mutil_Render_Count),
				0,
				(DXEngine::GetInstance()->GetHeight() / Mutil_Render_Count)
			);

			_textureRenderPass->Draw(renderData);
		}
		// 렌더타겟뷰에 그릴때와 화면에 그릴 때 동시에 바인딩이 안돼서 빼줘야 함. 
		PopShaderResource();
	}

	void RenderTarget::RenderDeferredWindow()
	{
		auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(_deferredTexture->GetSRV(), 0);

		_textureRenderPass->SetDrawRectangle(0, DXEngine::GetInstance()->GetWidth(), 0, DXEngine::GetInstance()->GetHeight());

		_textureRenderPass->Draw(renderData);
		///포스트 프로세싱을 하려고 한다면 이 부분을 포스트 프로세싱 패스로 변경하면 된다..!
	}

	void RenderTarget::Release()
	{
		ReleaseCOM(_renderTargetView);
	}
}