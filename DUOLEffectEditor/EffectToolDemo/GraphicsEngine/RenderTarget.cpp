#include "pch.h"
#include "RenderTarget.h"

#include "DeferredRenderPass.h"
#include "TextureRenderPass.h"

RenderTexture* RenderTarget::m_RederTexture[Mutil_Render_Count] = {}; // �ٸ� �н����� �ʿ� �� ���� ������ static���� ������..

RenderTarget::RenderTarget()
{

	for (int i = 0; i < Mutil_Render_Count; i++)
	{
		m_RederTexture[i] = new RenderTexture();
		m_TextureRenderTargetView[i] = nullptr;

	}
	m_DeferredTexture = new RenderTexture();
	_textureRenderPass = new TextureRenderPass();
	_deferredRenderPass = new DeferredRenderPass();

}

RenderTarget::~RenderTarget()
{
	Release();
	for (int i = 0; i < Mutil_Render_Count; i++)
	{
		delete m_RederTexture[i];
	}
	delete m_DeferredTexture;
	delete _deferredRenderPass;
}
void RenderTarget::OnResize()
{
	HRESULT hr;


	IDXGISwapChain* _SwapChain = DXEngine::GetInstance()->GetSwapChain();
	ID3D11Device* _Device = DXEngine::GetInstance()->GetD3dDevice();
	ID3D11DeviceContext* _DeviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

	UINT _Width = DXEngine::GetInstance()->GetWidth();
	UINT _Height = DXEngine::GetInstance()->GetHeight();


	for (int i = 0; i < Mutil_Render_Count; i++)
	{
		m_RederTexture[i]->Initialize(_Width, _Height);
		m_TextureRenderTargetView[i] = m_RederTexture[i]->GetRenderTargetView();

		//m_DebugWindows[i]->Initialize(_Width, _Height, _Width / Mutil_Render_Count, _Height / Mutil_Render_Count);
	}
	//m_DeferredWindow->Initialize(_Width, _Height, _Width, _Height);
	m_DeferredTexture->Initialize(_Width, _Height);
	m_DeferredRenderTargetView = m_DeferredTexture->GetRenderTargetView();
	ATLTRACE("OnResize ������ ����\n");
	Release();

	ATLTRACE("OnResize ���� ������ �缳��\n");
	HR(_SwapChain->ResizeBuffers((UINT)1, _Width, _Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;

	_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)); //��ȯ �罽�� ����Ű�� �����͸� ��´�.


	ATLTRACE("OnResize ���� ��� �並 ����\n");
	_Device->CreateRenderTargetView(backBuffer, 0,
		&m_RenderTargetView); //���� ��� �並 ����

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
		Mutil_Render_Count, m_TextureRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(_Num));
}

void RenderTarget::BeginRender()
{
	ClearRenderTarget();
}

void RenderTarget::EndRender() //Endrender�� �ƴ϶� Rendering���� �ٲ�� �� ��??
{
	/// <summary>
	/// �� �Լ��� �����ϱ����� ���۵� ������ ���� �ؽ��ĸ� �ϼ� ���� ���� �����̴�.
	/// </summary>
	
	CreateDeferredTexture();

	SetBackBufferRenderTarget(); // ���� �ϼ��� �ؽ��ĵ�� ����ۿ� �׸� ����

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
	// ������� ������ ����ϴ�. 
	_DC->ClearRenderTargetView(m_RenderTargetView, _Color);
	// ���� ���۸� ����ϴ�. 


	// �ؽ�ó ���ҽ� �����ֱ�~
	for (int i = 0; i < Mutil_Render_Count; i++)
		m_RederTexture[i]->ClearRenderTarget();
	m_DeferredTexture->ClearRenderTarget();
	// ���� Ÿ�� ��� ���� ���ٽ� ���۸� ������ ���������ο� �����Ѵ�.


}

void RenderTarget::CreateDeferredTexture()
{
	ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
	_DC->OMSetRenderTargets(1, &m_DeferredRenderTargetView, DXEngine::GetInstance()->GetDepthStencil()->GetDpethStencilView(0));

	std::vector<pair<ID3D11ShaderResourceView*, int>> shaderResource = {
	{m_RederTexture[0]->GetSRV(),1}, // ����
	{m_RederTexture[1]->GetSRV(),2}, // �븻
	{m_RederTexture[2]->GetSRV(),3}, // ������
	{m_RederTexture[3]->GetSRV(),4}, // �˺���
	{m_RederTexture[4]->GetSRV(),5}, // mat diffuse
	{m_RederTexture[5]->GetSRV(),6}, // mat specular
	{m_RederTexture[6]->GetSRV(),7} // mat ambinet
	};


	_deferredRenderPass->Draw(shaderResource);
}

void RenderTarget::SetBackBufferRenderTarget()
{
	ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
	_DC->OMSetRenderTargets(1, &m_RenderTargetView, nullptr);
}
void RenderTarget::RenderDebugWindow()
{
	ID3D11DeviceContext* _DC = DXEngine::GetInstance()->Getd3dImmediateContext();
	for (int i = 0; i < Mutil_Render_Count; i++)
	{
		auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(m_RederTexture[i]->GetSRV(), 0);
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
	auto renderData = std::make_pair<ID3D11ShaderResourceView*, int>(m_DeferredTexture->GetSRV(), 0);
	_textureRenderPass->SetDrawRectangle(0, DXEngine::GetInstance()->GetWidth(),0, DXEngine::GetInstance()->GetHeight());
	_textureRenderPass->Draw(renderData);
	///����Ʈ ���μ����� �Ϸ��� �Ѵٸ� �� �κ��� ����Ʈ ���μ��� �н��� �����ϸ� �ȴ�..!
}

void RenderTarget::Release()
{
	ReleaseCOM(m_RenderTargetView)

}
