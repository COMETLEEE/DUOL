#pragma once

class ID3D11DepthStencilState;

class ID3D11DepthStencilView;

namespace MuscleGrapics
{
	class Depth;
	enum class DepthStencilUsage
	{
		Normal,
		Shadow
	};
	class DepthStencil
	{
	public:
		DepthStencil();

		~DepthStencil();
	private:
		Depth* _depthStencil[2];

		ID3D11DepthStencilState* _offDepthStencilState; // ���� Off�� ���� ���ٽ� ������Ʈ

		ID3D11DepthStencilState* _onDepthStencilState; // ���� On�� ���� ���ٽ� ������Ʈ

	public:
		void OnDepthStencil(); // 0 �Ϲ� 1 ������ ����

		ID3D11DepthStencilView* GetDepthStencilView(int _Num); // 0 �Ϲ� 1 ������ ����

		Depth* GetDepth(int num);

		void OffDepthStencil();

		void Clear();

		void OnResize();

		void Finalize();
	};
}
