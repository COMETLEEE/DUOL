#pragma once
namespace MuscleGrapics
{
	class DepthStencil
	{
	public:
		DepthStencil();

		~DepthStencil();
	private:
		Depth* _depthStencil[2];

		ID3D11DepthStencilState* _offDepthStencilState; // µª½º Off¸¦ À§ÇÑ ½ºÅÙ½Ç ºä
	public:
		void OnDepthStencil(int _Num); // 0 ÀÏ¹Ý 1 ½¦µµ¿ì µª½º

		ID3D11DepthStencilView* GetDpethStencilView(int _Num); // 0 ÀÏ¹Ý 1 ½¦µµ¿ì µª½º

		void OffDepthStencil();

		void Clear();

		void OnResize();

		void Finalize();
	};
}
