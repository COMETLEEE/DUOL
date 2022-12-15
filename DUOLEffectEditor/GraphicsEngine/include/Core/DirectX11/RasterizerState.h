#pragma once
class ID3D11RasterizerState;

namespace MuscleGrapics
{
	class RasterizerState
	{
	public:
		RasterizerState();

		~RasterizerState();

	private:
		static ID3D11RasterizerState* _wireframeRS;

		static ID3D11RasterizerState* _solidRS;

	public:
		static void SetRasterizerState(int stateNum);

	};
}

