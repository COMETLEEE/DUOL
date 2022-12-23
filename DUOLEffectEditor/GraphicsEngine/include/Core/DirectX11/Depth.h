#pragma once

class ID3D11Texture2D;

class ID3D11DepthStencilView;

class ID3D11DepthStencilState;

namespace MuscleGrapics
{
	class Depth
	{
	public:
		Depth();

		~Depth();
	private:
		// 깊이 버퍼는 이미지 자료를 담지 않는 텍스처의 예.
		// 깊이 버퍼는 각 픽셀의 깊이 정보를 담는다.
		ID3D11Texture2D* _depthStencilBuffer; // 뎁스 스텐실 버퍼 , 저장하는 곳.

		ID3D11DepthStencilView* _depthStencilView; // 뎁스 스텐실 뷰 , 그리는 곳.

	public:
		void Clear();

		void OnResize();

		void Finalize();

		ID3D11DepthStencilView* GetDepthStencilView();

		ID3D11Texture2D* GetDepthBuffer();
	};
}
