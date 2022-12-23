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

		ID3D11DepthStencilState* _offDepthStencilState; // 뎁스 Off를 위한 스텐실 스테이트

		ID3D11DepthStencilState* _onDepthStencilState; // 뎁스 On을 위한 스텐실 스테이트

	public:
		void OnDepthStencil(); // 0 일반 1 쉐도우 뎁스

		ID3D11DepthStencilView* GetDepthStencilView(int _Num); // 0 일반 1 쉐도우 뎁스

		Depth* GetDepth(int num);

		void OffDepthStencil();

		void Clear();

		void OnResize();

		void Finalize();
	};
}
