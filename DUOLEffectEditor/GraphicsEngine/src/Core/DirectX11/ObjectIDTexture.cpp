#include "Core/DirectX11/ObjectIDTexture.h"
#include <d3d11.h>
#include "Core/DirectX11/DXEngine.h"

namespace MuscleGrapics
{
	void ObjectIDTexture::Release()
	{
		ReleaseCOM(_renderTargetTexture);
		ReleaseCOM(_renderTargetView);
		ReleaseCOM(_shaderResourceView);
		ReleaseCOM(_copyTexture);
	}
	ObjectIDTexture::~ObjectIDTexture()
	{
		Release();
	}
	bool ObjectIDTexture::Initialize(int width, int height)
	{
		Release();

		HRESULT result;
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		ID3D11Device* device = DXEngine::GetInstance()->GetD3dDevice();
		// Picking 전용 Texture 2D
	// 해당 Texture는 데이터 입력은 Shader에서 하되, 해당 값을 CPU에서 읽을수 있어야 하므로
	// Usage Option을 D3D11_USAGE_STAGING으로 설정..
		D3D11_TEXTURE2D_DESC texCopyDesc;
		ZeroMemory(&texCopyDesc, sizeof(texCopyDesc));
		texCopyDesc.Width = 1;
		texCopyDesc.Height = 1;
		texCopyDesc.MipLevels = 1;
		texCopyDesc.ArraySize = 1;
		texCopyDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texCopyDesc.SampleDesc.Count = 1;
		texCopyDesc.SampleDesc.Quality = 0;
		texCopyDesc.Usage = D3D11_USAGE_STAGING;
		texCopyDesc.BindFlags = 0;
		texCopyDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		texCopyDesc.MiscFlags = 0;

		D3D11_TEXTURE2D_DESC texDesc;
		ZeroMemory(&texDesc, sizeof(texDesc));
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		/// RenderTargetView 2D
		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
		ZeroMemory(&rtvDesc, sizeof(rtvDesc));
		rtvDesc.Format = texDesc.Format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D.MipSlice = 0;

		result = device->CreateTexture2D(&texDesc, NULL, &_renderTargetTexture);
		if (FAILED(result))
		{
			return false;
		}
		result = device->CreateTexture2D(&texCopyDesc, NULL, &_copyTexture);
		if (FAILED(result))
		{
			return false;
		}

		// RenderTarget 생성..
		// 렌더 타겟 뷰를 생성합니다.
		result = device->CreateRenderTargetView(_renderTargetTexture, &rtvDesc, &_renderTargetView);

	}
	ID3D11Texture2D* ObjectIDTexture::GetCopyTargetTexture()
	{
		return _copyTexture;
	}
}

