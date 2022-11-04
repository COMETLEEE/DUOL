#include "pch.h"
#include "RenderTexture.h"

RenderTexture::RenderTexture() :m_RenderTargetTexture(nullptr), m_RenderTargetView(nullptr), m_ShaderResourceView(nullptr)
{

}

RenderTexture::~RenderTexture()
{
	Release();
}

bool RenderTexture::Initialize(int _Width, int _Height)
{
	Release();

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();

	// RTT ��ũ������ �ʱ�ȭ�մϴ�.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// RTT ��ũ������ �����մϴ�.
	textureDesc.Width = _Width;
	textureDesc.Height = _Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// RTT�� �����մϴ�.
	result = device->CreateTexture2D(&textureDesc, NULL, &m_RenderTargetTexture);
	if (FAILED(result))
	{
		return false;
	}

	// ���� Ÿ�� �信 ���� ��ũ������ �����մϴ�.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	
	// ���� Ÿ�� �並 �����մϴ�.
	result = device->CreateRenderTargetView(m_RenderTargetTexture, &renderTargetViewDesc, &m_RenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// ���̴� ���ҽ� �信 ���� ��ũ������ �����մϴ�.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// ���̴� ���ҽ� �並 �����մϴ�.
	result = device->CreateShaderResourceView(m_RenderTargetTexture, &shaderResourceViewDesc, &m_ShaderResourceView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void RenderTexture::Release()
{
	ReleaseCOM(m_RenderTargetTexture);
	ReleaseCOM(m_RenderTargetView);
	ReleaseCOM(m_ShaderResourceView);
}

void RenderTexture::ClearRenderTarget()
{
	// ClearRenderTarget�Լ��� BeginScene�Լ��� ������ ����۰� �ƴ� 
	// m_renderTargetView�� Ŭ��� �̷�����ٴ� ���� �ٸ��ϴ�.
	// �� �Լ��� �� ������ RTT�� �Ͼ�� ���� ȣ���� �־�� �մϴ�.

	ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();
	float color[4];
	// ���۸� �ʱ�ȭ�� ������ �����մϴ�.
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	// Rendertarget�� �ʱ�ȭ �մϴ�~~
	deviceContext->ClearRenderTargetView(m_RenderTargetView, color);
}

ID3D11RenderTargetView* RenderTexture::GetRenderTargetView()
{
	return m_RenderTargetView;

}

ID3D11ShaderResourceView* RenderTexture::GetSRV()
{
	return m_ShaderResourceView;
}
