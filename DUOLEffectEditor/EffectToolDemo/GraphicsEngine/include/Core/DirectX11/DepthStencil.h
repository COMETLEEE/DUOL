#pragma once

class ID3D11DepthStencilState;

class ID3D11DepthStencilView;

namespace MuscleGrapics
{
	class Depth;

	class DepthStencil
	{
	public:
		DepthStencil();

		~DepthStencil();
	private:
		Depth* _depthStencil[2];

		ID3D11DepthStencilState* _offDepthStencilState; // ���� Off�� ���� ���ٽ� ��
	public:
		void OnDepthStencil(int _Num); // 0 �Ϲ� 1 ������ ����

		ID3D11DepthStencilView* GetDpethStencilView(int _Num); // 0 �Ϲ� 1 ������ ����

		void OffDepthStencil();

		void Clear();

		void OnResize();

		void Finalize();
	};
}
