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
		// ���� ���۴� �̹��� �ڷḦ ���� �ʴ� �ؽ�ó�� ��.
		// ���� ���۴� �� �ȼ��� ���� ������ ��´�.
		ID3D11Texture2D* _depthStencilBuffer; // ���� ���ٽ� ���� , �����ϴ� ��.

		ID3D11DepthStencilView* _depthStencilView; // ���� ���ٽ� �� , �׸��� ��.

	public:
		void Clear();

		void OnResize();

		void Finalize();

		ID3D11DepthStencilView* GetDepthStencilView();

		ID3D11Texture2D* GetDepthBuffer();
	};
}
