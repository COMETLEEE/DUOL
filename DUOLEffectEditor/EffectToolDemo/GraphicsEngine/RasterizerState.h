#pragma once
namespace MuscleGrapics
{
	class RasterizerState
	{
	public:
		RasterizerState();

		~RasterizerState();

	public:
		static ID3D11RasterizerState* _wireframeRS;

		static ID3D11RasterizerState* _solidRS;

	};
}

