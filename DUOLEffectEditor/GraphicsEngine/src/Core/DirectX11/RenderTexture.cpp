#include "Core/DirectX11/RenderTexture.h"
#include <d3d11.h>
#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	RenderTexture::RenderTexture() :
		_renderTargetTexture(nullptr),
		_renderTargetView(nullptr),
		_shaderResourceView(nullptr),
		_scale(1.0f)
	{
		_device = DXEngine::GetInstance()->GetD3dDevice();
	}

	RenderTexture::~RenderTexture()
	{
		Release();
	}

	bool RenderTexture::Initialize(int _Width, int _Height)
	{
		Release();

		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// RTT ��ũ������ �ʱ�ȭ�մϴ�.
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		// RTT ��ũ������ �����մϴ�.
		textureDesc.Width = _Width * _scale;
		textureDesc.Height = _Height * _scale;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// ���� Ÿ�� �信 ���� ��ũ������ �����մϴ�.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;


		// ���̴� ���ҽ� �信 ���� ��ũ������ �����մϴ�.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		CreateTexture2D(&textureDesc);
		CreateSRV(&shaderResourceViewDesc);
		CreateRTV(&renderTargetViewDesc);

		return true;

	}

	bool RenderTexture::CreateTexture2D(D3D11_TEXTURE2D_DESC* desc)
	{
		if (_renderTargetTexture)
			assert(false);

		if (FAILED(_device->CreateTexture2D(desc, NULL, &_renderTargetTexture)))
			return false;

		return true;
	}

	bool RenderTexture::CreateSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* desc)
	{
		if (_shaderResourceView)
			assert(false);

		if (FAILED(_device->CreateShaderResourceView(_renderTargetTexture, desc, &_shaderResourceView)))
			return false;

		return true;
	}

	bool RenderTexture::CreateRTV(D3D11_RENDER_TARGET_VIEW_DESC* desc)
	{
		if (_renderTargetView)
			assert(false);

		// ���� Ÿ�� �並 �����մϴ�.
		if (FAILED(_device->CreateRenderTargetView(_renderTargetTexture, desc, &_renderTargetView)))
			return false;

		return true;
	}

	void RenderTexture::Release()
	{
		ReleaseCOM(_renderTargetTexture);
		ReleaseCOM(_renderTargetView);
		ReleaseCOM(_shaderResourceView);
	}

	void RenderTexture::ClearRenderTarget()
	{
		// ClearRenderTarget�Լ��� BeginScene�Լ��� ������ ����۰� �ƴ� 
// m_renderTargetView�� Ŭ��� �̷�����ٴ� ���� �ٸ��ϴ�.
// �� �Լ��� �� ������ RTT�� �Ͼ�� ���� ȣ���� �־�� �մϴ�.

		ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		float clearColor[4] = { 0,0,0,0 };

		// Rendertarget�� �ʱ�ȭ �մϴ�~~
		deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);
	}

	void RenderTexture::ClearRenderTarget(float clearColor[4])
	{
		// ClearRenderTarget�Լ��� BeginScene�Լ��� ������ ����۰� �ƴ� 
		// m_renderTargetView�� Ŭ��� �̷�����ٴ� ���� �ٸ��ϴ�.
		// �� �Լ��� �� ������ RTT�� �Ͼ�� ���� ȣ���� �־�� �մϴ�.

		ID3D11DeviceContext* deviceContext = DXEngine::GetInstance()->Getd3dImmediateContext();

		// Rendertarget�� �ʱ�ȭ �մϴ�~~
		deviceContext->ClearRenderTargetView(_renderTargetView, clearColor);
	}

	ID3D11RenderTargetView* RenderTexture::GetRTV()
	{
		return _renderTargetView;
	}

	ID3D11ShaderResourceView* RenderTexture::GetSRV()
	{
		return _shaderResourceView;
	}

	ID3D11Texture2D* RenderTexture::GetRenderTargetTexture()
	{
		return _renderTargetTexture;
	}

	void RenderTexture::SetScale(float scale)
	{
		_scale = scale;
	}
}
